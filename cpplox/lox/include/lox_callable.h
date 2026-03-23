#pragma once
#include "interpreter.h"
#include "object.h"
#include <vector>

class LoxCallable {
public:
    virtual int arity() = 0;
    virtual Object call(Interpreter &interpreter, const std::vector<Object> &arguments) = 0;
};