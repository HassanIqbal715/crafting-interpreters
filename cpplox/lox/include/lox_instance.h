#pragma once
#include "lox_class.h"
#include <unordered_map>

class LoxInstance {
private:
    LoxClass *klass;
    std::unordered_map<std::string, Object> fields;

public:
    LoxInstance(LoxClass *klass);

    Object get(Token &name, std::shared_ptr<LoxInstance> &self);
    void set(Token &name, Object value);

    friend std::ostream &operator<<(std::ostream &cout, 
        const LoxInstance &input);
};