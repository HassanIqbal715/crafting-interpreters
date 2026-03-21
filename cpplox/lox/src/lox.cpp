#include "lox.h"
#include "ast_printer.h"
#include "expr.h"
#include "files.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "token_type.h"
#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>

std::unique_ptr<Interpreter> Lox::interpreter = std::make_unique<Interpreter>();
bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;

void Lox::runFile(std::string path) {
    std::string data = Files::ReadAllBytes(path);
    run(data);
    if (hadError) exit(65);
    if (hadRuntimeError) exit(70);
}

void Lox::runPrompt() {
    while(true) {
        std::cout << "> ";
        std::string line = "";
        if (!std::getline(std::cin, line)) {
            std::cout << '\n';
            return;
        }
        run(line);
    }
}

void Lox::run(std::string_view source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();

    if (hadError) return;

    interpreter.get()->interpret(statements);
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " 
              << message << std::endl;
    hadError = true;
}

void Lox::error(Token &token, std::string message) {
    if (token.type == EOFILE) {
        report(token.line, " at end", message);
    }
    else {
        report(token.line, " at '" + token.lexeme + "' ", message);
    }
}

void Lox::runtimeError(RuntimeError &error) {
    std::cout << error.what() << "\n[line " << error.token.line << "]\n";
    hadRuntimeError = true;
}