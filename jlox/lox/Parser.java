package lox;

import java.util.ArrayList;
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
        if (match(PRINT)) return printStatement();

        return expressionStatement();
    }

    private Stmt printStatement() {
       Expr value = expression();
       consume(SEMICOLON, "Expect ';' after the value.");
       return new Stmt.Print(value);    
    }

    private Stmt varDeclaration() {
        Token name = consume(IDENTIFIER, "Expect variable name");

        Expr initializer = null;
        
        if (match(EQUAL)) {
            initializer = expression();
        }

        consume(SEMICOLON, "Expect ';' after variable declaration");
        return new Stmt.Var(name, initializer);
    }

    private Stmt expressionStatement() {
        Expr expr = expression();
        consume(SEMICOLON, "Expect ';' after the value.");
        return new Stmt.Expression(expr);
    }

    // comma -> equality ("," equality)*;
    private Expr comma() {
        // if (match(COMMA)) {
        //     error(previous(), "Expect left expression.");
        // }

        Expr expr = ternary();

        while (match(COMMA)) {
            Token operator = previous();
            Expr right = ternary();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private Expr ternary() {
        Expr expr = equality();

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

    private Expr equality() {
        // if (match(BANG_EQUAL, EQUAL_EQUAL)) {
        //     error(previous(), "Expect left expression.");
        // }

        Expr expr = comparison();

        while (match(BANG_EQUAL, EQUAL_EQUAL)) {
            Token operator = previous();
            Expr right = comparison();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private Expr comparison() {
        // if (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        //     error(previous(), "Expect left expression.");
        // }

        Expr expr = term();

        while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
            Token operator = previous();
            Expr right = term();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private Expr term() {
        // if (match(MINUS, PLUS)) {
        //     error(previous(), "Expect left expression.");
        // }

        Expr expr = factor();

        while (match(MINUS, PLUS)) {
            Token operator = previous();
            Expr right = factor();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private Expr factor() {
        // if (match(STAR, SLASH)) {
        //     error(previous(), "Expect left expression.");
        // }

        Expr expr;
        expr = unary();
        while (match(STAR, SLASH)) {
            Token operator = previous();
            Expr right = unary();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private Expr unary() {
        if (match(BANG, MINUS)) {
            Token operator = previous();
            Expr right = unary();
            return new Expr.Unary(operator, right);
        }
        return primary();
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
            return;
            default:
            return;
        }

            // advance();
        }
    }
}
