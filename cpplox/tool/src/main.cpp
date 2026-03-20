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
        "Binary : Expr left, Token op, Expr right",
        "Grouping : Expr expression",
        "Literal : Object value",
        "Unary : Token op, Expr right",
    });
}