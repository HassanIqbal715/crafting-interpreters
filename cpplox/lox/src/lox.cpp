#include "lox.h"
#include "files.h"
#include "token.h"
#include "token_type.h"
#include <filesystem>
#include <string_view>
#include <list>

void Lox::runFile(string path) {
    string_view data = Files::ReadAllBytes(path);
    run(data);
    
    if (hadError) exit(65);
}

void Lox::runPrompt() {
    while(true) {
        cout << "> ";
        string line = "";
        getline(cin, line);
        cout << line << endl;
    }
}

void Lox::run(string_view source) {
    // Scanner scanner = new Scanner(source);
    // list<Token> tokens = scanner.scanTokens();

    // for (Token token : tokens) {
    //     cout << token << "\n";
    // }

    Token number{TokenType::NUMBER, "23", 23.0, 1};
    Token text{TokenType::STRING, "this is cool", "this is cool", 2};
    Token nil{TokenType::NIL, "nil", monostate{}, 3};

    cout << number << endl;
    cout << text << endl;
    cout << nil << endl;
}

void Lox::error(int line, string message) {
    report(line, "", message);
}

void Lox::report(int line, string where, string message) {
    cerr << "[line " << line << "] Error" << where << ": " + message << endl;
    hadError = true;
}