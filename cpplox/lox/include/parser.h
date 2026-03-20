#pragma once
#include "token.h"
#include "token_type.h"
#include "expr.h"
#include <vector>

class Parser {
private:
    class ParseError : public std::runtime_error {
    public:
        ParseError();
    };

    std::vector<Token> tokens;
    int current;

    // Parsing
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> comma();
    std::unique_ptr<Expr> conditional();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    // Utilities
    bool match(std::vector<TokenType> &types);
    bool match(TokenType type);
    Token consume(TokenType type, std::string message);
    bool check(TokenType &type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    ParseError error(Token token, std::string message);
    void synchronize();

public:
    Parser(std::vector<Token> &tokens);
    std::unique_ptr<Expr> parse();
};