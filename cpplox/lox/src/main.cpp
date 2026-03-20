#include "lox.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc > 2) {
        cout << "Usage: .lox [script]";
        return 64;
    }
    else if (argc == 2) {
        Lox::runFile((string) argv[1]);
    }
    else {
        Lox::runPrompt();
    }

    return 0;
}