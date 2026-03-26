#include "lox_class.h"
#include "lox_instance.h"

LoxClass::LoxClass(std::string name, 
        std::unordered_map<std::string, 
            std::shared_ptr<LoxFunction>> &methods) {
    this->name = name;
    this->methods = methods;
}

std::shared_ptr<LoxFunction> LoxClass::findMethod(std::string name) {
    auto it = methods.find(name);
    if (it != methods.end()) {
        return it->second;
    }

    return NULL;
}

int LoxClass::arity() {
    std::shared_ptr<LoxFunction> initializer = findMethod("init");
    if (initializer == NULL) return 0;
    return initializer->arity();
}

Object LoxClass::call(Interpreter &interpreter, 
        const std::vector<Object> &arguments) {
    std::shared_ptr<LoxInstance> instance = std::make_shared<LoxInstance>(this);
    std::shared_ptr<LoxFunction> initializer = findMethod("init");
    if (initializer != NULL) {
        initializer->bind(instance)->call(interpreter, arguments);
    }

    return instance;
}

std::ostream &operator<<(std::ostream &cout, const LoxClass &input) {
    cout << input.name;
    return cout;
}