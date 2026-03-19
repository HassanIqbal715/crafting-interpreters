#pragma once
#include "token_type.h"
#include <iostream>
#include <variant>
using namespace std;

struct LiteralToString{
    string operator()(double val);
    string operator()(string val);
    string operator()(monostate val);
};

struct Token {
    const TokenType type;
    const string lexeme;
    const variant<double, string, monostate> literal;
    const int line;

    Token(TokenType type, string lexeme, 
        variant<double, string, monostate> literal, int line);
};

ostream &operator<<(ostream &cout, const Token &token);