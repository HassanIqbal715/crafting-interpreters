#include "lox.h"
#include "files.h"
#include "token.h"
#include "token_type.h"
#include <filesystem>
#include <string_view>
#include <list>

void Lox::runFile(std::string path) {
    std::string_view data = Files::ReadAllBytes(path);
    run(data);
    
    if (hadError) exit(65);
}

void Lox::runPrompt() {
    while(true) {
        std::cout << "> ";
        std::string line = "";
        std::getline(std::cin, line);
        std::cout << line << std::endl;
    }
}

void Lox::run(std::string_view source) {
    // Scanner scanner = new Scanner(source);
    // list<Token> tokens = scanner.scanTokens();

    // for (Token token : tokens) {
    //     cout << token << "\n";
    // }

    Token number{TokenType::NUMBER, "23", 23.0, 1};
    Token text{TokenType::STRING, "this is cool", "this is cool", 2};
    Token nil{TokenType::NIL, "nil", std::monostate{}, 3};

    std::cout << number << std::endl;
    std::cout << text << std::endl;
    std::cout << nil << std::endl;
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " 
              << message << std::endl;
    hadError = true;
}