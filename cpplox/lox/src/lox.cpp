#include "lox.h"
#include "files.h"
#include "token.h"
#include "token_type.h"
#include "scanner.h"
#include <filesystem>
#include <string_view>
#include <vector>

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
        run(line);
    }
}

void Lox::run(std::string_view source) {
    Scanner scanner(source);
    std::vector<std::unique_ptr<Token>> tokens = scanner.scanTokens();

    for (int i = 0; i < tokens.size(); i++) {
        std::cout << *(tokens[i].get()) << std::endl;
    }
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " 
              << message << std::endl;
    hadError = true;
}