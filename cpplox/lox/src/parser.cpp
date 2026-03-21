#include "parser.h"
#include "lox.h"

// Constructor
Parser::Parser(std::vector<Token> &tokens) {
    this->tokens = tokens;
    current = 0;
}

Parser::ParseError::ParseError() : runtime_error{""} {}

// Utility functions
bool Parser::match(std::vector<TokenType> &types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }

    return false;
}

Token Parser::consume(TokenType type, std::string message) {
    if (check(type)) return advance();

    throw error(peek(), message);
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == EOFILE;
}

Token &Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];    
}

Parser::ParseError Parser::error(Token &token, std::string message) {
    Lox::error(token, message);
    return Parser::ParseError();
}

void Parser::synchronize() {
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
        }

        advance();
    }
}

// Parsing 
std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while(!isAtEnd()) {
        statements.push_back(declaration());
    }

    return statements;
}

std::unique_ptr<Expr> Parser::expression() {
    return comma();
}

std::unique_ptr<Stmt> Parser::declaration() {
    try {
        if (match(VAR)) return varDeclaration();

        return statement();
    }
    catch (ParseError &error) {
        synchronize();
        return NULL;
    }
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match(PRINT)) return printStatement();
    if (match(LEFT_BRACE)) return std::make_unique<Stmt>(
        Block{block()});
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::printStatement() {
    std::unique_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after the value.");
    return std::make_unique<Stmt>(Print{std::move(value)}); 
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");

    std::unique_ptr<Expr> initializer = NULL;
    if (match(EQUAL)) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<Stmt>(Var{name, std::move(initializer)});
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    std::unique_ptr<Expr> expr = expression();
    consume(SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<Stmt>(Expression{std::move(expr)});
}

std::vector<std::unique_ptr<Stmt>> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::unique_ptr<Expr> Parser::comma() {
    std::unique_ptr<Expr> expr = assignment();

    while (match(COMMA)) {
        Token op = previous();
        std::unique_ptr<Expr> right = assignment();
        expr = std::make_unique<Expr>(Binary{
            std::move(expr), op, std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::assignment() {
    std::unique_ptr<Expr> expr = conditional();

    if (match(EQUAL)) {
        Token equals = previous();
        std::unique_ptr<Expr> right = assignment();

        if (std::holds_alternative<Variable>(*expr)) {
            Token name = std::get<Variable>(*expr).name;
            return std::make_unique<Expr>(Assign{
                name, std::move(right)});
        }

        error(equals, "Inavlid assignment target.");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::conditional() {
    std::unique_ptr<Expr> expr = equality();

    while(match(QUESTION)) {
        Token op1 = previous();
        std::unique_ptr<Expr> mid = expression();

        Token op2 = consume(COLON, "Expect ':' after expression.");

        std::unique_ptr<Expr> right = conditional();

        expr = std::make_unique<Expr>(Ternary{std::move(expr), op1, 
            std::move(mid), op2, std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::equality() {
    std::unique_ptr<Expr> expr = comparison();

    std::vector<TokenType> types = {BANG_EQUAL, EQUAL_EQUAL};
    while (match(types)) {
        Token op = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<Expr>(Binary{
            std::move(expr), op, std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = term();

    std::vector<TokenType> types = {GREATER, GREATER_EQUAL, LESS, LESS_EQUAL};
    while (match(types)) {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<Expr>(Binary{
            std::move(expr), op, std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr = factor();

    std::vector<TokenType> types = {MINUS, PLUS};
    while (match(types)) {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<Expr>(Binary{
            std::move(expr), op, std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr = unary();

    std::vector<TokenType> types = {SLASH, STAR};
    while (match(types)) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<Expr>(Binary{
            std::move(expr), op, std::move(right)});
    }

    return expr;

}

std::unique_ptr<Expr> Parser::unary() {
    std::vector<TokenType> types = {BANG, MINUS};
    if (match(types)) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Expr>(Unary{op, std::move(right)});
    }

    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    if (match(FALSE)) return std::make_unique<Expr>(Literal{false});
    if (match(TRUE)) return std::make_unique<Expr>(Literal{true});
    if (match(NIL)) return std::make_unique<Expr>(Literal{std::monostate{}});

    std::vector<TokenType> numberAndString = {NUMBER, STRING}; 
    if (match(numberAndString)) {
        return std::make_unique<Expr>(Literal{previous().literal});
    }

    if (match(IDENTIFIER)) {
        return std::make_unique<Expr>(Variable{previous()});
    }

    if (match(LEFT_PAREN)) {
        std::unique_ptr<Expr> expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Expr>(Grouping{std::move(expr)});
    }

    throw error(peek(), "Expect expression.");
}