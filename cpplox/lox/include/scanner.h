#pragma once
#include "token.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
using Object = std::variant<double, std::string, std::monostate>;

class Scanner {
private:
    static std::unordered_map<std::string, TokenType> keywords;
    const std::string_view source;
    std::vector<std::unique_ptr<Token>> tokens;
    int start;
    int current;
    int line;

    void scanToken();
    void identifier();
    void multilineComment();
    void number();
    void string();
    bool match(char expected);
    char peek();
    char peekNext();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    bool isDigit(char c);
    bool isAtEnd();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, Object literal);

public:
    Scanner(std::string_view source);

    std::vector<std::unique_ptr<Token>> scanTokens();
};