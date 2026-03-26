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
        if (match(CLASS)) return classDeclaration();
        if (match(FUN)) return std::make_unique<Stmt>(
            std::move(*function("function")));
        if (match(VAR)) return varDeclaration();

        return statement();
    }
    catch (ParseError &error) {
        synchronize();
        return NULL;
    }
}

std::unique_ptr<Stmt> Parser::classDeclaration() {
    Token name = consume(IDENTIFIER, "Expect class name.");
    consume(LEFT_BRACE, "Expect '{' before class body.");

    std::vector<std::unique_ptr<Function>> methods;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        methods.push_back(function("method"));
    }

    consume(RIGHT_BRACE, "Expect '}' after class body.");

    return std::make_unique<Stmt>(Class{std::move(name), std::move(methods)});
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match(BREAK)) return breakStatement();
    if (match(FOR)) return forStatement();
    if (match(IF)) return ifStatement();
    if (match(PRINT)) return printStatement();
    if (match(RETURN)) return returnStatement();
    if (match(LEFT_BRACE)) return std::make_unique<Stmt>(
        Block{block()});
    if (match(WHILE)) return whileStatement();

    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::breakStatement() {
    Token name = previous();
    consume(SEMICOLON, "Expect ';' after 'break'.");

    return std::make_unique<Stmt>(Break{std::move(name)}); 
}

std::unique_ptr<Stmt> Parser::forStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");

    std::unique_ptr<Stmt> initializer = NULL;
    if (match(SEMICOLON)) {
        initializer = NULL;
    }
    else if (match(VAR)) {
        initializer = varDeclaration();
    }
    else {
        initializer = expressionStatement();
    }

    std::unique_ptr<Expr> condition = NULL;
    if (!check(SEMICOLON)) {
        condition = expression();
    }
    consume(SEMICOLON, "Expect ';' after loop condition.");

    std::unique_ptr<Expr> increment = NULL;
    if (!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");
    std::unique_ptr<Stmt> body = statement();

    if (increment != NULL) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(body));
        statements.push_back(
            std::make_unique<Stmt>(Expression{std::move(increment)})
        );

        body = std::make_unique<Stmt>(Block{std::move(statements)});
    }

    if (condition == NULL) 
        condition = std::make_unique<Expr>(Literal{true});

    body = std::make_unique<Stmt>(While{std::move(condition), std::move(body)});

    if (initializer != NULL) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<Stmt>(Block{std::move(statements)});
    }

    return body;
}

std::unique_ptr<Stmt> Parser::ifStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    std::unique_ptr<Stmt> thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = NULL;
    if (match(ELSE)) {
        elseBranch = statement();
    }

    return std::make_unique<Stmt>(If{std::move(condition), 
        std::move(thenBranch), std::move(elseBranch)});
}

std::unique_ptr<Stmt> Parser::printStatement() {
    std::unique_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after the value.");
    return std::make_unique<Stmt>(Print{std::move(value)}); 
}

std::unique_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::unique_ptr<Expr> value = NULL;
    if (!check(SEMICOLON)) {
        value = expression();
    }

    consume(SEMICOLON, "Expect ';' after return value.");
    return std::make_unique<Stmt>(Return{std::move(keyword), std::move(value)});
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");

    std::unique_ptr<Expr> initializer = NULL;
    if (match(EQUAL)) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<Stmt>(Var{std::move(name), std::move(initializer)});
}

std::unique_ptr<Stmt> Parser::whileStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after the condition.");
    std::unique_ptr<Stmt> body = statement();

    return std::make_unique<Stmt>(While{std::move(condition), std::move(body)});
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    std::unique_ptr<Expr> expr = expression();
    consume(SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<Stmt>(Expression{std::move(expr)});
}

std::unique_ptr<Function> Parser::function(std::string kind) {
    Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Can't have more than 255 parameters.");
            }

            parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
        } while (match(COMMA));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");

    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<std::unique_ptr<Stmt>> body = block();
    return std::make_unique<Function>(Function{std::move(name), 
        std::move(parameters), std::move(body)});
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
            std::move(expr), std::move(op), std::move(right)});
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
                std::move(name), std::move(right)});
        }
        else if (std::holds_alternative<Get>(*expr)) {
            Get &get = std::get<Get>(*expr);
            return std::make_unique<Expr>(Set{std::move(get.object), 
                std::move(get.name), std::move(right)});
        }

        error(equals, "Inavlid assignment target.");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::conditional() {
    std::unique_ptr<Expr> expr = logicalOr();

    while(match(QUESTION)) {
        Token op1 = previous();
        std::unique_ptr<Expr> mid = expression();

        Token op2 = consume(COLON, "Expect ':' after expression.");

        std::unique_ptr<Expr> right = conditional();

        expr = std::make_unique<Expr>(Ternary{std::move(expr), std::move(op1), 
            std::move(mid), std::move(op2), std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::logicalOr() {
    std::unique_ptr<Expr> expr = logicalAnd();

    while(match(OR)) {
        Token op = previous();
        std::unique_ptr<Expr> right = logicalAnd();
        expr = std::make_unique<Expr>(Logical{std::move(expr), std::move(op), 
            std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::logicalAnd() {
    std::unique_ptr<Expr> expr = equality();

    while(match(AND)) {
        Token op = previous();
        std::unique_ptr<Expr> right = equality();
        expr = std::make_unique<Expr>(Logical{std::move(expr), std::move(op), 
            std::move(right)});
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
            std::move(expr), std::move(op), std::move(right)});
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
            std::move(expr), std::move(op), std::move(right)});
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
            std::move(expr), std::move(op), std::move(right)});
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
            std::move(expr), std::move(op), std::move(right)});
    }

    return expr;

}

std::unique_ptr<Expr> Parser::unary() {
    std::vector<TokenType> types = {BANG, MINUS};
    if (match(types)) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Expr>(Unary{std::move(op), std::move(right)});
    }

    return call();
}

std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr> &callee) {
    std::vector<std::unique_ptr<Expr>> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Can't have more than 255 arguments.");
            }
            arguments.push_back(assignment());
        }
        while (match(COMMA));
    }

    Token paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");

    return std::make_unique<Expr>(Call{std::move(callee), std::move(paren), 
        std::move(arguments)});
}

std::unique_ptr<Expr> Parser::call() {
    std::unique_ptr<Expr> expr = primary();

    while(true) {
        if (match(LEFT_PAREN)) {
            expr = finishCall(expr);
        }
        else if (match(DOT)) {
            Token name = consume(IDENTIFIER, "Expect property name after '.'.");
            expr = std::make_unique<Expr>(Get{
                std::move(expr), std::move(name)});
        }
        else {
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    if (match(FALSE)) return std::make_unique<Expr>(Literal{false});
    if (match(TRUE)) return std::make_unique<Expr>(Literal{true});
    if (match(NIL)) return std::make_unique<Expr>(Literal{std::monostate{}});

    std::vector<TokenType> numberAndString = {NUMBER, STRING}; 
    if (match(numberAndString)) {
        return std::make_unique<Expr>(Literal{previous().literal});
    }

    if (match(THIS)) return std::make_unique<Expr>(This{previous()});

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