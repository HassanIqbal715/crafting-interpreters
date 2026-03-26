#pragma once
#include "expr.h"
#include "interpreter.h"
#include "stmt.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

class Resolver {
private:    
    enum FunctionType {
        FUNCTION_NONE,
        FUNCTION,
        INITIALIZER,
        METHOD
    };

    enum ClassType {
        CLASS_NONE,
        CLASS
    };

    Interpreter *interpreter;
    std::vector<std::unique_ptr<std::unordered_map<std::string, bool>>> scopes;
    FunctionType currentFunction;
    ClassType currentClass;

    void beginScope();
    void endScope();
    void declare(Token &name);
    void define(Token &name);
    template<class T>
    void resolveLocal(T *expr, Token &name);
    void resolveFunction(Function &function, FunctionType type);

public:
    Resolver(Interpreter *interpreter);

    void resolve(std::vector<std::unique_ptr<Stmt>> &statements);
    void resolve(std::unique_ptr<Stmt> &stmt);
    void resolve(std::unique_ptr<Expr> &expr);

    // statements
    void operator()(Block &stmt);
    void operator()(Break &stmt);
    void operator()(Class &stmt);
    void operator()(Expression &stmt);
    void operator()(Function &stmt);
    void operator()(If &stmt);
    void operator()(Print &stmt);
    void operator()(Return &stmt);
    void operator()(Var &stmt);
    void operator()(While &stmt);

    // expressions
    void operator()(Assign &expr);
    void operator()(Binary &expr);
    void operator()(Call &expr);
    void operator()(Get &expr);
    void operator()(Grouping &expr);
    void operator()(Literal &expr);
    void operator()(Logical &expr);
    void operator()(Set &expr);
    void operator()(Ternary &expr);
    void operator()(This &expr);
    void operator()(Unary &expr);
    void operator()(Variable &expr);
};