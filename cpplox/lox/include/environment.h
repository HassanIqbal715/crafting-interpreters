#pragma once
#include "token.h"
#include <string>
#include <unordered_map>
#include <variant>
using Object = std::variant<bool, double, std::string, std::monostate>;

class Environment {
private:
    Environment* enclosing;
    std::unordered_map<std::string, Object> values;

public:
    Environment();
    Environment(Environment *enclosing);
    
    Object get(Token &name);
    void assign(Token &name, Object &value);
    void define(std::string &name, Object &value);
};