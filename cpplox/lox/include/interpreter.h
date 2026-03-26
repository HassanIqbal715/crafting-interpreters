#pragma once
#include "object.h"
#include "environment.h"
#include "expr.h"
#include "stmt.h"
#include <string>
#include <variant>
#include <vector>

class Interpreter {
private:
    std::shared_ptr<Environment> environment;
    std::unordered_map<void*, int> locals;

    Object evaluate(Expr *expr);
    void execute(Stmt *stmt);
    bool isTruthy(Object &object);
    bool isEqual(Object &a, Object &b);
    std::string stringify(Object &object);
    bool endsWith(std::string &str, std::string delimiter);
    void checkNumberOperand(Token &op, Object &right);
    void checkNumberOperands(Token &op, Object &left, Object &right);
    template<class T> Object lookUpVariable(Token &name, T *expr);

public:
    std::shared_ptr<Environment> globals;

    Interpreter();
    void interpret(std::vector<std::unique_ptr<Stmt>> &statements);
    void executeBlock(std::vector<std::unique_ptr<Stmt>> &statements, 
        std::shared_ptr<Environment> &environment);
    void resolve(void *expr, int depth);

    void operator()(Block &stmt);
    void operator()(Break &stmt);
    void operator()(Expression &stmt);
    void operator()(Function &stmt);
    void operator()(If &stmt);
    void operator()(Print &stmt);
    void operator()(Return &stmt);
    void operator()(Var &stmt);
    void operator()(While &stmt);
    Object operator()(Assign &expr);
    Object operator()(Binary &expr);
    Object operator()(Call &expr);
    Object operator()(Grouping &expr);
    Object operator()(Literal &expr);
    Object operator()(Logical &expr);
    Object operator()(Ternary &expr);
    Object operator()(Unary &expr);
    Object operator()(Variable &expr);
};