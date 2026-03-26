#pragma once
#include "lox_callable.h"
#include <memory>
#include <vector>

class LoxFunction : public LoxCallable {
private:
    Function* declaration;
    std::shared_ptr<Environment> closure;
    bool isInitializer;

public:
    LoxFunction(Function *declaration, std::shared_ptr<Environment> &closure,
        bool isInitializer);
    std::shared_ptr<LoxFunction> bind(std::shared_ptr<LoxInstance> instance);
    int arity();
    Object call(Interpreter &interpreter, const std::vector<Object> &arguments);

    friend std::ostream &operator<<(std::ostream &cout, 
        const LoxFunction &input);
};