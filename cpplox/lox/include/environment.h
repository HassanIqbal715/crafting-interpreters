#pragma once
#include "token.h"
#include "object.h"
#include <string>
#include <unordered_map>
#include <variant>

class Environment {
private:
    std::shared_ptr<Environment> enclosing;
    std::unordered_map<std::string, Object> values;

public:
    Environment();
    Environment(std::shared_ptr<Environment> &enclosing);
    
    Object get(Token &name);
    void assign(Token &name, Object &value);
    void define(std::string name, Object value);
    Environment* ancestor(int distance);
    Object getAt(int distance, std::string name);
    void assignAt(int distance, Token &name, Object value);
};