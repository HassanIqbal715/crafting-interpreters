#include "environment.h"
#include "runtime_error.h"

// Constructor and Destructor
Environment::Environment() {
    this->enclosing = NULL;
}

Environment::Environment(Environment *enclosing) {
    this->enclosing = enclosing;
}

// Methods
Object Environment::get(Token &name) {
    auto it = values.find(name.lexeme);
    if (it != values.end()) {
        if (std::holds_alternative<std::monostate>(it->second)) {
            throw RuntimeError(name, 
                "Uninitialized variable '" + name.lexeme + "'.");
        }
        return it->second;
    }

    if (enclosing != NULL) return enclosing->get(name);

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(Token &name, Object &value) {
    auto it = values.find(name.lexeme);
    if (it != values.end()) {
        it->second = value;
        return;
    }

    if (enclosing != NULL) {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::define(std::string &name, Object &value) {
    values.insert({name, value});
}