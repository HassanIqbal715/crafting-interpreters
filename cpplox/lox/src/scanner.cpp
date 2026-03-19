#include "scanner.h"
#include "lox.h"

std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and",    AND},
    {"class",  CLASS},
    {"else",   ELSE},
    {"false",  FALSE},
    {"for",    FOR},
    {"fun",    FUN},
    {"if",     IF},
    {"nil",    NIL},
    {"or",     OR},
    {"print",  PRINT},
    {"return", RETURN},
    {"super",  SUPER},
    {"this",   THIS},
    {"true",   TRUE},
    {"var",    VAR},
    {"while",  WHILE}
};

Scanner::Scanner(std::string_view source) : source{source} {
    start = 0;
    current = 0;
    line = 1;
}

std::vector<std::unique_ptr<Token>> Scanner::scanTokens() {
    while(!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.push_back(std::make_unique<Token>(EOFILE, "", std::monostate{}, line));
    return std::move(tokens);
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;
        case '!':
            addToken(match('=') ? BANG_EQUAL : BANG);
            break;
        case '=':
            addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            addToken(match('=') ? LESS_EQUAL : LESS);
            break;
        case '>':
            addToken(match('=') ? GREATER_EQUAL : GREATER);
            break;
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line.
                while (peek() != '\n' && !isAtEnd()) advance();
            } 
            else {
                addToken(SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            line++;
            break;
        
        case '"': string(); break;

        default:
            if (isDigit(c)) {
                number();
            }
            else if (isAlpha(c)) {
                identifier();
            }
            else {
                Lox::error(line, "Unexpected character.");
            }
            break;
    }
}

void Scanner::identifier() {
    while(isAlphaNumeric(peek())) advance();

    std::string text = (std::string) source.substr(start, current - start);
    auto it = keywords.find(text);
    TokenType type;
    if (it != keywords.end()) {
        type = it->second;
    }
    else {
        type = IDENTIFIER;
    }
    addToken(type);
}

void Scanner::number() {
    while(isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();

        while(isDigit(peek())) advance();
    }

    addToken(NUMBER, std::stod(
        (std::string) source.substr(start, current - start)));
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        Lox::error(line, "Unterminated string.");
        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string value = (std::string) source.substr(start + 1, 
        current - start - 2);
    addToken(STRING, value);
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source.at(current) != expected) return false;

    current++;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source.at(current);
}

char Scanner::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source.at(current + 1);
}
bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

char Scanner::advance() {
    return source.at(current++);
}

void Scanner::addToken(TokenType type) {
    addToken(type, std::monostate{});
}

void Scanner::addToken(TokenType type, Object literal) {
    std::string text = (std::string) source.substr(start, current - start);
    tokens.push_back(std::make_unique<Token>(type, text, literal, line));
}