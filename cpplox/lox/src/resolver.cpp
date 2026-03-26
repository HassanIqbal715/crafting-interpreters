#include "resolver.h"
#include "lox.h"

// constructor and public methods

Resolver::Resolver(Interpreter* interpreter) {
    this->interpreter = interpreter;
    currentFunction = FUNCTION_NONE;
    currentClass = CLASS_NONE;
}

void Resolver::resolve(std::vector<std::unique_ptr<Stmt>> &statements) {
    for (auto &statement : statements) {
        resolve(statement);
    }
}

void Resolver::resolve(std::unique_ptr<Stmt> &stmt) {
    std::visit(*this, static_cast<StmtVariant&>(*stmt.get()));
}

void Resolver::resolve(std::unique_ptr<Expr> &expr) {
    std::visit(*this, static_cast<ExprVariant&>(*expr.get()));
}

// private methods

void Resolver::beginScope() {
    scopes.push_back(std::make_unique<unordered_map<std::string, bool>>());
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(Token &name) {
    if (scopes.empty()) return;

    std::unordered_map<std::string, bool> *scope = scopes[
        scopes.size() - 1].get();
    
    if (scope->count(name.lexeme) > 0) {
        Lox::error(name, "Already a variable with this name in this scope.");
    }

    scope->insert({name.lexeme, false});
}

void Resolver::define(Token &name) {
    if (scopes.empty()) return;
    (*scopes[scopes.size() - 1].get())[name.lexeme] = true;
}

template <class T>
void Resolver::resolveLocal(T *expr, Token &name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes[i]->count(name.lexeme) > 0) {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::resolveFunction(Function &function, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (Token param : function.params) {
        declare(param);
        define(param);
    }
    resolve(function.body);
    endScope();
    currentFunction = enclosingFunction;
}

// vistor

// statements

void Resolver::operator()(Block &stmt) {
    beginScope();
    resolve(stmt.statements);
    endScope();
}

void Resolver::operator()(Break &stmt) {}

void Resolver::operator()(Class &stmt) {
    ClassType enclosingClass = currentClass;
    currentClass = CLASS;

    declare(stmt.name);
    define(stmt.name);

    beginScope();
    scopes[scopes.size() - 1]->insert({"this", true});

    for (auto &method : stmt.methods) {
        FunctionType declaration = METHOD;
        if (method->name.lexeme.compare("init") == 0) {
            declaration = INITIALIZER;
        }
        resolveFunction(*method, declaration);
    }

    endScope();

    currentClass = enclosingClass;
}

void Resolver::operator()(Expression &stmt) {
    resolve(stmt.expression);
}

void Resolver::operator()(Function &stmt) {
    declare(stmt.name);
    define(stmt.name);

    resolveFunction(stmt, FUNCTION);
}

void Resolver::operator()(If &stmt) {
    resolve(stmt.condition);
    resolve(stmt.thenBranch);
    if (stmt.elseBranch != NULL) resolve(stmt.elseBranch);
}

void Resolver::operator()(Print &stmt) {
    resolve(stmt.expression);
}

void Resolver::operator()(Return &stmt) {
    if (currentFunction == FUNCTION_NONE) {
        Lox::error(stmt.keyword, "Can't return from top-level code.");
    }

    if (stmt.value != NULL) {
        if (currentFunction == INITIALIZER) {
            Lox::error(stmt.keyword, "Can't return a value from initializer.");
        }
        resolve(stmt.value);
    }
}

void Resolver::operator()(Var &stmt) {
    declare(stmt.name);
    if (stmt.initializer != NULL) {
        resolve(stmt.initializer);
    }
    define(stmt.name);
}

void Resolver::operator()(While &stmt) {
    resolve(stmt.condition);
    resolve(stmt.body);
}

// expressions

void Resolver::operator()(Assign &expr) {
    resolve(expr.value);
    resolveLocal(&expr, expr.name);
}

void Resolver::operator()(Binary &expr) {
    resolve(expr.left);
    resolve(expr.right);
}

void Resolver::operator()(Call &expr) {
    resolve(expr.callee);

    for (std::unique_ptr<Expr> &argument : expr.arguments) {
        resolve(argument);
    }
}

void Resolver::operator()(Get &expr) {
    resolve(expr.object);
}

void Resolver::operator()(Grouping &expr) {
    resolve(expr.expression);
}

void Resolver::operator()(Literal &expr) {}

void Resolver::operator()(Logical &expr) {
    resolve(expr.left);
    resolve(expr.right);
}

void Resolver::operator()(Set &expr) {
    resolve(expr.value);
    resolve(expr.object);
}

void Resolver::operator()(Ternary &expr) {
    resolve(expr.left);
    resolve(expr.mid);
    resolve(expr.right);
}

void Resolver::operator()(This &expr) {
    if (currentClass == CLASS_NONE) {
        Lox::error(expr.keyword, "Can't use 'this' outside of a class.");
        return;
    }

    resolveLocal(&expr, expr.keyword);
}

void Resolver::operator()(Unary &expr) {
    resolve(expr.right);
}

void Resolver::operator()(Variable &expr) {
    if (!scopes.empty()) {
        auto& currScope = scopes[scopes.size() - 1];
        auto it = currScope->find(expr.name.lexeme);
        if (it != currScope->end() && it->second == false) {
            Lox::error(expr.name, 
                "Can't read local variable in its own initializer");
        }
    }

    resolveLocal(&expr, expr.name);
}