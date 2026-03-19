#pragma once
#include "token_type.h"
#include <iostream>
#include <variant>
using Object = std::variant<double, std::string, std::monostate>;

struct LiteralToString{
    std::string operator()(double val);
    std::string operator()(std::string val);
    std::string operator()(std::monostate val);
};

struct Token {
    const TokenType type;
    const std::string lexeme;
    const Object literal;
    const int line;

    Token(TokenType type, std::string lexeme, Object literal, int line);
};

std::ostream &operator<<(std::ostream &cout, const Token &token);