#pragma once
#include <string>
#include <string_view>

namespace Lox {
    static bool hadError = false;

    void runFile(std::string filePath);
    void runPrompt();
    void run(std::string_view source);
    void error(int line, std::string message);
    void report(int line, std::string where, std::string message);
}