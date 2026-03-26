#pragma once
#include "lox_callable.h"
#include "lox_function.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class LoxClass : public LoxCallable {
private:
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;

public:
    std::string name;

    LoxClass(std::string name, 
        std::unordered_map<std::string, std::shared_ptr<LoxFunction>> &methods);
        
    std::shared_ptr<LoxFunction> findMethod(std::string name);
    int arity();
    Object call(Interpreter &interpreter, const std::vector<Object> &arguments);

    friend std::ostream &operator<<(std::ostream &cout, 
        const LoxClass &input);
};