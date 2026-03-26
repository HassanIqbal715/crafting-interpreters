#include "lox_instance.h"
#include "runtime_error.h"

LoxInstance::LoxInstance(LoxClass *klass) {
    this->klass = klass;
}

Object LoxInstance::get(Token &name, std::shared_ptr<LoxInstance> &self) {
    auto it = fields.find(name.lexeme);
    if (it != fields.end()) {
        return it->second;
    }

    std::shared_ptr<LoxFunction> method = klass->findMethod(name.lexeme);
    if (method != NULL) return method->bind(self);

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token &name, Object value) {
    fields[name.lexeme] = value;
}

std::ostream &operator<<(std::ostream &cout, const LoxInstance &input) {
    cout << input.klass->name << " instance";
    return cout;
}