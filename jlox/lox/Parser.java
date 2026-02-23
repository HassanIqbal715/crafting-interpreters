package lox;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import static lox.TokenType.*;

class Parser {
    private static class ParseError extends RuntimeException {}

    private final List<Token> tokens;
    private int current = 0;
    
    Parser(List<Token> tokens) {
        this.tokens = tokens;
    }

    List<Stmt> parse() {
        List<Stmt> statements = new ArrayList<>();
        while(!isAtEnd()) {
            statements.add(declaration());
        }

        return statements;
    }

    private Expr expression() {
        return comma();
    }

    private Stmt declaration() {
        try {
            if (match(VAR)) return varDeclaration();

            return statement();
        }
        catch (ParseError error) {
            synchronize();
            return null;
        }
    }

    private Stmt statement() {
        if (match(FOR)) return forStatement();
        if (match(IF)) return ifStatement();
        if (match(PRINT)) return printStatement();
        if (match(LEFT_BRACE)) return new Stmt.Block(block());
        if (match(WHILE)) return whileStatement();
        if (match(DO)) return doStatement();

        return expressionStatement();
    }

    private Stmt forStatement() {
        consume(LEFT_PAREN, "Expected '(' after for.");

        Stmt initializer;
        if (match(SEMICOLON)) {
            initializer = null;
        }
        else if (match(VAR)) {
            initializer = varDeclaration();
        }
        else {
            initializer = expressionStatement();
        }

        Expr condition = null;
        if (!check(SEMICOLON)) {
            condition = expression();
        }
        consume(SEMICOLON, "Expect ';' after loop condition.");

        Expr increment = null;
        if (!check(RIGHT_PAREN)) {
            increment = expression();
        }
        consume(RIGHT_PAREN, "Expect ')' after for clauses.");

        Stmt body = statement(); // This is the code

        if (increment != null) {
            // increment exists, put code followed by the increment statement
            // in a statement block
            body = new Stmt.Block(
            Arrays.asList(
                body,
                new Stmt.Expression(increment)));
        }

        // condition does not exits, while (true);
        if (condition == null) condition = new Expr.Literal(true);
        body = new Stmt.While(condition, body); // A while loop

        // initializer exists, first initialize, then execute the while stmt.
        // put all of that in a block statement.
        if (initializer != null) {
            body = new Stmt.Block(Arrays.asList(initializer, body));
        }

        return body;
    }

    private Stmt ifStatement() {
        consume(LEFT_PAREN, "Expect '(' after if.");
        Expr condition = expression();
        consume(RIGHT_PAREN, "Expect ')' after if condition");

        Stmt thenBranch = statement();
        Stmt elseBranch = null;

        if (match(ELSE)) {
            elseBranch = statement();
        }
        return new Stmt.If(condition, thenBranch, elseBranch);
    }

    private Stmt printStatement() {
       Expr value = expression();
       consume(SEMICOLON, "Expect ';' after the value.");
       return new Stmt.Print(value);    
    }

    private Stmt varDeclaration() {
        List<Stmt.Var> declarations = new ArrayList<>();

        do {
            Token name = consume(IDENTIFIER, "Expect variable name");
            Expr initializer = null;

            if (match(EQUAL)) {
                initializer = assignment();
            }

            declarations.add(new Stmt.Var(name, initializer));
        } 
        while(match(COMMA));

        consume(SEMICOLON, "Expect ';' after variable declaration");

        return new Stmt.CommaDeclaration(declarations);
    }

    private Stmt whileStatement() {
        consume(LEFT_PAREN, "Expect '(' after 'while'");
        Expr condition = expression();
        consume(RIGHT_PAREN, "Expect ')' after condition");
        Stmt body = statement();

        return new Stmt.While(condition, body);
    }

    private Stmt doStatement() {
        Stmt body = statement();
        
        consume(WHILE, "Expect 'while' after 'do' body");
        consume(LEFT_PAREN, "Expect '(' after 'while'");
        Expr condition = expression();
        consume(RIGHT_PAREN, "Expect ')' after condition");
        consume(SEMICOLON, "Expect ';' after 'do while'");
        
        body = new Stmt.Block(
            Arrays.asList(
                body,
                new Stmt.While(condition, body)
            )
        );

        return body;
    }

    private Stmt expressionStatement() {
        Expr expr = expression();
        consume(SEMICOLON, "Expect ';' after the value.");
        return new Stmt.Expression(expr);
    }

    private List<Stmt> block() {
        List<Stmt> statements = new ArrayList<>();

        while (!check(RIGHT_BRACE) && !isAtEnd()) {
            statements.add(declaration());
        }

        consume(RIGHT_BRACE, "Expect '}' after block.");
        return statements;
    }

    // comma -> equality ("," equality)*;
    private Expr comma() {
        Expr expr = assignment();

        while (match(COMMA)) {
            Token operator = previous();
            Expr right = assignment();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private Expr assignment() {
        Expr expr = ternary();

        if (match(EQUAL)) {
        Token equals = previous();
        Expr value = assignment();

        if (expr instanceof Expr.Variable) {
            Token name = ((Expr.Variable)expr).name;
            return new Expr.Assign(name, value);
        }

        error(equals, "Invalid assignment target.");
        }

        return expr;
    }

    private Expr ternary() {
        Expr expr = or();

        while (match(QUESTION)) {
            Token operator1 = previous();
            Expr mid = expression();

            Token operator2;
            if (match(COLON)) {
                operator2 = previous();
            }
            else {
                throw error(peek(), "Expect colon ':''.");
            }

            Expr right = ternary();
            
            expr = new Expr.Ternary(expr, operator1, mid, operator2, right);
        }

        return expr;
    }

    private Expr or() {
        Expr expr = and();

        while (match(OR)) {
            Token operator = previous();
            Expr right = and();
            expr = new Expr.Logical(expr, operator, right);
        }

        return expr;
    }

    private Expr and() {
        Expr expr = equality();
        
        while (match(AND)) {
            Token operator = previous();
            Expr right = equality();
            expr = new Expr.Logical(expr, operator, right);
        }

        return expr;
    }

    private Expr equality() {
        Expr expr = comparison();

        while (match(BANG_EQUAL, EQUAL_EQUAL)) {
            Token operator = previous();
            Expr right = comparison();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private Expr comparison() {
        Expr expr = term();

        while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
            Token operator = previous();
            Expr right = term();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private Expr term() {
        Expr expr = factor();

        while (match(MINUS, PLUS)) {
            Token operator = previous();
            Expr right = factor();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private Expr factor() {
        Expr expr;
        expr = unary();
        while (match(STAR, SLASH, MODULO)) {
            Token operator = previous();
            Expr right = unary();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private Expr unary() {
        if (match(BANG, MINUS, PLUS_PLUS, MINUS_MINUS)) {
            Token operator = previous();
            Expr right = unary();
            return new Expr.Unary(operator, right);
        }
        return postfix();
    }

    private Expr postfix() {
        Expr expr = primary();

        if (match(PLUS_PLUS, MINUS_MINUS)) {
            Token operator = previous();
            
            expr = new Expr.Postfix(expr, operator);
        }

        return expr;
    }

    private Expr primary() {
        if (match(FALSE)) return new Expr.Literal(false);
        if (match(TRUE)) return new Expr.Literal(true);
        if (match(NIL)) return new Expr.Literal(null);

        if (match(NUMBER, STRING)) {
            return new Expr.Literal(previous().literal);
        }

        if (match(IDENTIFIER)) {
            return new Expr.Variable(previous());
        }

        if (match(LEFT_PAREN)) {
            Expr expr = expression();
            consume(RIGHT_PAREN, "Expect ')' after expression.");
            return new Expr.Grouping(expr);
        }

        throw error(peek(), "Expect expression.");
    }

    private boolean match(TokenType ...types) {
        for (TokenType type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }

        return false;
    }

    private Token consume(TokenType type, String message) {
        if (check(type)) return advance();

        throw error(peek(), message);
    }
    
    private boolean check(TokenType type) {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    private Token advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    private boolean isAtEnd() {
        return peek().type == TokenType.EOF;
    }

    private Token peek() {
        return tokens.get(current);
    }

    private Token previous() {
        return tokens.get(current - 1);
    }
    
    private ParseError error(Token token, String message) {
        Lox.error(token, message);
        return new ParseError();
    } 

    private void synchronize() {
        advance();

        while (!isAtEnd()) {
        if (previous().type == SEMICOLON) return;
        
        switch (peek().type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
            case DO:
            return;
            default:
            return;
        }

            // advance();
        }
    }
}
