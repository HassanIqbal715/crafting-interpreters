#pragma once
#include "lox_callable.h"
#include <functional>
#include <iostream>

class NativeFunction : public LoxCallable {
private:
    int arityCount;
    std::function<Object(Interpreter&, const std::vector<Object>&)> function;

public:
    NativeFunction(int arity, std::function<Object(
        Interpreter&, const std::vector<Object>&)> func);
    
    int arity();
    Object call(Interpreter& interpreter, const std::vector<Object>& arguments);

    friend std::ostream &operator<<(std::ostream &cout, 
        const NativeFunction &input);
};