#include "environment.h"
#include "runtime_error.h"

// Constructor and Destructor
Environment::Environment() {
    this->enclosing = NULL;
}

Environment::Environment(std::shared_ptr<Environment> &enclosing) {
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

void Environment::define(std::string name, Object value) {
    values.insert({name, value});
}

Environment* Environment::ancestor(int distance) {
    Environment *environment = this;
    for (int i = 0; i < distance; i++) {
        environment = environment->enclosing.get();
    }
    return environment;
}

Object Environment::getAt(int distance, std::string name) {
    return ancestor(distance)->values[name];
}

void Environment::assignAt(int distance, Token &name, Object value) {
    ancestor(distance)->values.insert({name.lexeme, value});
}