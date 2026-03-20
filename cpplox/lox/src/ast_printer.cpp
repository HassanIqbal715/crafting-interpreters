#include "ast_printer.h"
#include <iostream>
#include <variant>

std::string parenthesize(
        std::string op, 
        std::vector<Expr*> &exprs) {
    std::string output = "(";
    output.append(op);
    for (int i = 0; i < exprs.size(); i++) {
        output.push_back(' ');
        output.append(std::visit(AstPrinter{},
            static_cast<ExprVariant&>(*exprs[i])));
    }

    output.push_back(')');
    return output;
}

std::string parenthesize(std::string op, Expr* expr) {
    std::string output = "(";
    output.append(op);
    output.push_back(' ');
    output.append(std::visit(AstPrinter{},
        static_cast<ExprVariant&>(*expr)));
    output.push_back(')');
    return output;
}

// Print visitor
std::string AstPrinter::operator()(const Binary &expr) {
    std::vector<Expr*> expressions;
    expressions.push_back(expr.left.get());
    expressions.push_back(expr.right.get());
    return parenthesize(expr.op.lexeme, expressions);
}

std::string AstPrinter::operator()(const Grouping &expr) {
    return parenthesize("group", expr.expression.get());
}

std::string AstPrinter::operator()(const Literal &expr) {
    return std::visit(LiteralToString{}, expr.value);
}

std::string AstPrinter::operator()(const Unary &expr) {
    return parenthesize(expr.op.lexeme, expr.right.get());
}