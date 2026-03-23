#include "lox_function.h"
#include "return.h"

LoxFunction::LoxFunction(Function *declaration, 
        std::shared_ptr<Environment> &closure) {
    this->declaration = declaration;
    this->closure = closure;
}

int LoxFunction::arity() {
    return declaration->params.size();
}

Object LoxFunction::call(Interpreter &interpreter, 
        const std::vector<Object> &arguments) {
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(
        closure
    );
    for (int i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }

    try {
        interpreter.executeBlock(declaration->body, environment);
    }
    catch (ReturnException &returnValue) {
        return returnValue.value;
    }

    return std::monostate{};
}

std::ostream &operator<<(std::ostream &cout, const LoxFunction &input) {
    cout << "<fn " << input.declaration->name.lexeme << ">";
    return cout;
}