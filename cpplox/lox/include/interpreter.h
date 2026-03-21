#pragma once
#include "environment.h"
#include "expr.h"
#include "stmt.h"
#include <string>
#include <variant>
#include <vector>
using Object = std::variant<bool, double, std::string, std::monostate>;

class Interpreter {
private:
    std::unique_ptr<Environment> environment;

    Object evaluate(Expr* expr);
    void execute(Stmt* stmt);
    void executeBlock(std::vector<std::unique_ptr<Stmt>> &statements, 
        std::unique_ptr<Environment> &environment);
    bool isTruthy(Object &object);
    bool isEqual(Object &a, Object &b);
    std::string stringify(Object &object);
    bool endsWith(std::string &str, std::string delimiter);
    void checkNumberOperand(Token &op, Object &right);
    void checkNumberOperands(Token &op, Object &left, Object &right);

public:
    Interpreter();
    void interpret(std::vector<std::unique_ptr<Stmt>> &statements);

    void operator()(Block &stmt);
    void operator()(Expression &stmt);
    void operator()(Print &stmt);
    void operator()(Var &stmt);
    Object operator()(Assign &expr);
    Object operator()(Binary &expr);
    Object operator()(Grouping &expr);
    Object operator()(Literal &expr);
    Object operator()(Ternary &expr);
    Object operator()(Unary &expr);
    Object operator()(Variable &expr);
};