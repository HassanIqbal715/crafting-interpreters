#pragma once
#include "token.h"
#include <iostream>

class RuntimeError : public std::runtime_error {
public:
    const Token token;
    RuntimeError(Token token, std::string message);
};