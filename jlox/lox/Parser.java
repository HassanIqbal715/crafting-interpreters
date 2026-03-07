package lox;

import java.util.ArrayList;
import java.util.List;
import static lox.TokenType.*;

class Parser {
    private static class ParseError extends RuntimeException {}
    String currentFile = "";
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
            if (match(CLASS)) return classDeclaration();
            if (match(FUN)) return function("function");
            if (match(VAR)) return varDeclaration();

            return statement();
        }
        catch (ParseError error) {
            synchronize();
            return null;
        }
    }

    private Stmt classDeclaration() {
        Token name = consume(IDENTIFIER, "Expect class name");
        consume(LEFT_BRACE, "Expect '{' before class body");

        List<Stmt.Function> methods = new ArrayList<>();
        while (!check(RIGHT_BRACE) && !isAtEnd()) {
            methods.add(function("method"));
        }

        consume(RIGHT_BRACE, "Expect '}' after class body");

        return new Stmt.Class(name, methods);
    }

    private Stmt statement() {
        if (match(FOR)) return forStatement();
        if (match(IF)) return ifStatement();
        if (match(PRINT)) return printStatement();
        if (match(RETURN)) return returnStatement();
        if (match(LEFT_BRACE)) return new Stmt.Block(block());
        if (match(WHILE)) return whileStatement();
        if (match(DO)) return doStatement();
        if (match(BREAK)) return breakStatement();
        if (match(CONTINUE)) return continueStatement();
        if (match(SWITCH)) return switchStatement();
        if (match(IMPORT)) return importStatement();
        
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

        if (condition == null) {
            condition = new Expr.Literal(true);
        }
        
        return new Stmt.For(initializer, condition, increment, body);
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

    private Stmt importStatement() {
        Token keyword = previous();
        Expr value = expression();
        consume(SEMICOLON, "Expect ';' after import value.");
        return new Stmt.Import(keyword, value);
    }

    private Stmt printStatement() {
       Expr value = expression();
       consume(SEMICOLON, "Expect ';' after the value.");
       return new Stmt.Print(value);    
    }

    private Stmt returnStatement() {
        Token keyword = previous();
        Expr value = null;
        if (!check(SEMICOLON)) {
            value = expression();
        }
        consume(SEMICOLON, "Expect ';' after return value.");
        return new Stmt.Return(keyword, value);
    }

    private Stmt varDeclaration() {
        List<Stmt> declarations = new ArrayList<>();

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

        return new Stmt.Do(condition, body);
    }

    private Stmt breakStatement() {
        Token keyword = previous();
        consume(SEMICOLON, "Expect ';' after break");
        return new Stmt.Break(keyword);
    }

    private Stmt continueStatement() {
        Token keyword = previous();
        consume(SEMICOLON, "Expect ';' after continue");
        return new Stmt.Continue(keyword);
    }

    private Stmt switchStatement() {
        consume(LEFT_PAREN, "Expect '(' after swtich");
        Expr switchValue = expression();
        consume(RIGHT_PAREN, "Expect ')' after switch value");
        consume(LEFT_BRACE, "Expect '{' after switch");
        
        List<SwitchCase> cases = new ArrayList<>();
        SwitchCase defaultCase = null;

        while (!check(RIGHT_BRACE) && !isAtEnd()) {
            if (match(CASE)) {
                cases.add(caseStatement());
            }
            else if (match(DEFAULT)) {
                if (defaultCase == null) {
                    defaultCase = defaultStatement();
                    cases.add(defaultCase);
                }
                else {
                    throw error(peek(), "Expect one 'default' inside switch", 
                        currentFile);
                }
            }
            else {
                throw error(peek(), "Expect 'case' or 'default' inside switch", 
                    currentFile);
            }
        }

        consume(RIGHT_BRACE, "Expect '}' after switch");
        return new Stmt.Switch(switchValue, cases, defaultCase);
    }

    private SwitchCase caseStatement() {
        Expr value = expression();
        consume(COLON, "Expect ':' after case");

        List<Stmt> statements = new ArrayList<>();
        while (!check(CASE) && !check(DEFAULT) && !check(RIGHT_BRACE)) {
            statements.add(statement());
        }
        return new SwitchCase(value, statements);
    }

    private SwitchCase defaultStatement() {
        consume(COLON, "Expect ':' after default");
        
        List<Stmt> statements = new ArrayList<>();
        while (!check(CASE) && !check(DEFAULT) && !check(RIGHT_BRACE)) {
            statements.add(statement());
        }
        return new SwitchCase(null, statements);
    }

    private Stmt expressionStatement() {
        Expr expr = expression();
        consume(SEMICOLON, "Expect ';' after the value.");
        return new Stmt.Expression(expr);
    }

    private Stmt.Function function(String kind) {
        Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
        consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
        List<Token> parameters = new ArrayList<>();
        
        if (!check(RIGHT_PAREN)) {
            do {
                if (parameters.size() >= 255) {
                    error(peek(), "Can't have more than 255 parameters.", 
                        currentFile);
                }
                parameters.add(
                    consume(IDENTIFIER, "Expect parameter name.")
                );
            } while (match(COMMA));
        }
        consume(RIGHT_PAREN, "Expect ')' after parameters.");

        consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
        List<Stmt> body = block();
        return new Stmt.Function(name, parameters, body);
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
            else if (expr instanceof Expr.Array) {
                Expr.Array array = (Expr.Array) expr;

                Token name = array.name;
                List<Expr> indices = array.indices;

                return new Expr.AssignArray(name, indices, value);
            }

            error(equals, "Invalid assignment target.", currentFile);
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
                throw error(peek(), "Expect colon ':''.", currentFile);
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
        Expr expr = call();

        if (match(PLUS_PLUS, MINUS_MINUS)) {
            Token operator = previous();
            
            expr = new Expr.Postfix(expr, operator);
        }

        return expr;
    }

    private Expr finishCall(Expr callee) {
        List<Expr> arguments = new ArrayList<>();
        if (!check(RIGHT_PAREN)) {
            do {
                if (arguments.size() >= 255) {
                    error(peek(), "Can't have more than 255 arguments.", 
                        currentFile);
                }
                arguments.add(assignment());
            } while (match(COMMA));
        }

        Token paren = consume(RIGHT_PAREN,
                            "Expect ')' after arguments.");

        return new Expr.Call(callee, paren, arguments);
    }

    private Expr call() {
        Expr expr = primary();

        while (true) {
            if (match(LEFT_PAREN)) {
                expr = finishCall(expr);
            }
            else if (match(DOT)) {
                Token name = consume(IDENTIFIER, 
                        "Expect property name after '.'");
                expr = new Expr.Get(expr, name);
            }
            else {
                break;
            }
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
            Token name = previous();
            if (match(LEFT_SQUARE)) {
                List<Expr> indices = new ArrayList<>();
                do {
                    indices.add(expression());
                    consume(RIGHT_SQUARE, "Expect ']' after expression");
                } while (match(LEFT_SQUARE));
                return new Expr.Array(name, indices);
            }
            return new Expr.Variable(name);
        }

        if (match(LEFT_PAREN)) {
            Expr expr = expression();
            consume(RIGHT_PAREN, "Expect ')' after expression.");
            return new Expr.Grouping(expr);
        }

        if (match(LEFT_BRACE)) {
            List<Expr> elements = new ArrayList<>();
            if (!check(RIGHT_BRACE)) {
                do {
                    elements.add(assignment());
                } while (match(COMMA));
            }
            consume(RIGHT_BRACE, "Expect '}' after elements");
            return new Expr.Elements(elements);
        }

        throw error(peek(), "Expect expression.", currentFile);
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

        throw error(peek(), message, currentFile);
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
    
    private ParseError error(Token token, String message, String fileName) {
        Lox.error(token, message, fileName);
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
            case BREAK:
            case CONTINUE:
            case SWITCH:
            case CASE:
            case DEFAULT:
            return;
            default:
            return;
        }

            // advance();
        }
    }
}
