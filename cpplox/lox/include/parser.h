#pragma once
#include "expr.h"
#include "stmt.h"
#include "token.h"
#include "token_type.h"
#include <vector>

class Parser {
private:
    class ParseError : public std::runtime_error {
    public:
        ParseError();
    };

    // Attributes
    std::vector<Token> tokens;
    int current;

    // Parsing
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> classDeclaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> breakStatement();
    std::unique_ptr<Stmt> forStatement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> returnStatement();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> whileStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Function> function(std::string kind);
    std::vector<std::unique_ptr<Stmt>> block();
    std::unique_ptr<Expr> comma();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> conditional();
    std::unique_ptr<Expr> logicalOr();
    std::unique_ptr<Expr> logicalAnd();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> finishCall(std::unique_ptr<Expr> &callee);
    std::unique_ptr<Expr> call();
    std::unique_ptr<Expr> primary();

    // Utilities
    bool match(std::vector<TokenType> &types);
    bool match(TokenType type);
    Token consume(TokenType type, std::string message);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token &peek();
    Token previous();
    ParseError error(Token &token, std::string message);
    void synchronize();

public:
    Parser(std::vector<Token> &tokens);
    std::vector<std::unique_ptr<Stmt>> parse();
};