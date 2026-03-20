#pragma once
#include "expr.h"
#include <string>
#include <vector>
#include <memory>

std::string parenthesize(std::string op, 
    const std::vector<const Expr*> &exprs);

struct AstPrinter {
    std::string operator()(const Binary &expr);
    std::string operator()(const Grouping &expr);
    std::string operator()(const Literal &expr);
    std::string operator()(const Unary &expr);
};