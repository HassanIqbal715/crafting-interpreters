#pragma once
#include "token.h"
#include <string>
#include <vector>

class Scanner {
private:
    const std::string source;
    const std::vector<Token> tokens;

public:
    Scanner(std::string source);
};