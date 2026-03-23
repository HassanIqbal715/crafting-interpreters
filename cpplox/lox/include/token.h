#pragma once
#include "object.h"
#include "token_type.h"
#include <iostream>
#include <variant>

struct Token {
    TokenType type;
    std::string lexeme;
    Object literal;
    int line;

    Token(TokenType type, std::string lexeme, Object literal, int line);
};

std::ostream &operator<<(std::ostream &cout, const Token &token);