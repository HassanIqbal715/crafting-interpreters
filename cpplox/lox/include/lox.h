#pragma once
#include <iostream>
#include <string_view>
using namespace std;

namespace Lox {
    static bool hadError = false;

    void runFile(string filePath);
    void runPrompt();
    void run(string_view source);
    void error(int line, string message);
    void report(int line, string where, string message);
}