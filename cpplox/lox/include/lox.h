#pragma once
#include "interpreter.h"
#include "runtime_error.h"
#include "token.h"
#include <string>
#include <string_view>

class Lox {
private:
    static std::unique_ptr<Interpreter> interpreter;

public:    
    static bool hadError;
    static bool hadRuntimeError;

    static void runFile(std::string filePath);
    static void runPrompt();
    static void run(std::string_view source);
    static void error(int line, std::string message);
    static void report(int line, std::string where, std::string message);
    static void error(Token token, std::string message);
    static void runtimeError(RuntimeError &error);
};