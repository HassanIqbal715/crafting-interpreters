#pragma once
#include "expr.h"
#include <string>
#include <variant>
using Object = std::variant<bool, double, std::string, std::monostate>;

class Interpreter {
private:
    Object evaluate(Expr* expr);
    bool isTruthy(Object &object);
    bool isEqual(Object &a, Object &b);
    std::string stringify(Object &object);
    bool endsWith(std::string &str, std::string delimiter);
    void checkNumberOperand(Token &op, Object &right);
    void checkNumberOperands(Token &op, Object &left, Object &right);

public:
    void interpret(Expr* expression);

    Object operator()(Binary &expr);
    Object operator()(Grouping &expr);
    Object operator()(Literal &expr);
    Object operator()(Ternary &expr);
    Object operator()(Unary &expr);
};