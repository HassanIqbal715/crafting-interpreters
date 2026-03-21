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
        "Grouping   : unique_ptr<Expr> expression",
        "Literal    : Object value",
        "Ternary    : unique_ptr<Expr> left, Token op1, unique_ptr<Expr> mid, Token op2, unique_ptr<Expr> right",
        "Unary      : Token op, unique_ptr<Expr> right",
        "Variable   : Token name"
    });

    GenerateAst::defineAst(outputDir, "stmt", {
        "Block      : vector<unique_ptr<Stmt>> statements",
        "Expression : unique_ptr<Expr> expression",
        "Print      : unique_ptr<Expr> expression",
        "Var        : Token name, unique_ptr<Expr> initializer"
    }, true);
}