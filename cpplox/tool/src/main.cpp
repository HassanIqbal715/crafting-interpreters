#include "generate_ast.h"
#include "utils.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: .generate_ast <output directory>\n";
        return 64;
    }
    string outputDir = argv[1];

    GenerateAst::defineAst(outputDir, "expr", {
        "Assign     : Token name, unique_ptr<Expr> value",
        "Binary     : unique_ptr<Expr> left, Token op, unique_ptr<Expr> right",
        "Call       : unique_ptr<Expr> callee, Token paren, vector<unique_ptr<Expr>> arguments",
        "Get        : unique_ptr<Expr> object, Token name",
        "Grouping   : unique_ptr<Expr> expression",
        "Literal    : Object value",
        "Logical    : unique_ptr<Expr> left, Token op, unique_ptr<Expr> right",
        "Set        : unique_ptr<Expr> object, Token name, unique_ptr<Expr> value",
        "Ternary    : unique_ptr<Expr> left, Token op1, unique_ptr<Expr> mid, Token op2, unique_ptr<Expr> right",
        "This       : Token keyword",
        "Unary      : Token op, unique_ptr<Expr> right",
        "Variable   : Token name"
    });

    GenerateAst::defineAst(outputDir, "stmt", {
        "Block      : vector<unique_ptr<Stmt>> statements",
        "Break      : Token name",
        "Class      : Token name, vector<unique_ptr<Function>> methods",
        "Expression : unique_ptr<Expr> expression",
        "Function   : Token name, vector<Token> params, vector<unique_ptr<Stmt>> body",
        "If         : unique_ptr<Expr> condition, unique_ptr<Stmt> thenBranch, unique_ptr<Stmt> elseBranch",
        "Print      : unique_ptr<Expr> expression",
        "Return     : Token keyword, unique_ptr<Expr> value",
        "Var        : Token name, unique_ptr<Expr> initializer",
        "While      : unique_ptr<Expr> condition, unique_ptr<Stmt> body"
    }, true);
}