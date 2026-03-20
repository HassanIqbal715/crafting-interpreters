#include "lox.h"
#include "files.h"
#include "token.h"
#include "token_type.h"
#include "scanner.h"
#include "ast_printer.h"
#include "expr.h"
#include <filesystem>
#include <string_view>
#include <vector>

void Lox::runFile(std::string path) {
    std::string data = Files::ReadAllBytes(path);
    run(data);
    if (hadError) exit(65);
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

    for (int i = 0; i < tokens.size(); i++) {
        std::cout << tokens[i] << std::endl;
    }

    // Expr expression = Binary{
    //     std::make_unique<Expr>(Unary{
    //         Token(MINUS, "-", std::monostate{}, 1), 
    //         std::make_unique<Expr>(Literal{123.0})
    //     }),
    //     Token(STAR, "*", std::monostate{}, 1),
    //     std::make_unique<Expr>(Grouping{
    //         std::make_unique<Expr>(Literal{45.67})
    //     })
    // };

    // AstPrinter printer;
    // std::cout << std::visit(printer, expression) << std::endl;
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " 
              << message << std::endl;
    hadError = true;
}