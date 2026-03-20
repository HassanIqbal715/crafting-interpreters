#pragma once
#include "expr.h"
#include <string>
#include <vector>
#include <memory>

std::string parenthesize(std::string op, 
    std::vector<Expr*> &exprs);
std::string parenthesize(std::string op, Expr* expr);

struct AstPrinter {
    std::string operator()(const Binary &expr);
    std::string operator()(const Grouping &expr);
    std::string operator()(const Literal &expr);
    std::string operator()(const Unary &expr);
};