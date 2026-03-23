#include "native_function.h"

NativeFunction::NativeFunction(int arity, 
    std::function<Object(Interpreter&, const std::vector<Object>&)> func) 
    : arityCount(arity), function(std::move(func)) {
}

int NativeFunction::arity() {
    return arityCount;
}

Object NativeFunction::call(Interpreter& interpreter, 
        const std::vector<Object> &arguments) {
    return function(interpreter, arguments);
}

std::ostream &operator<<(std::ostream &cout, const NativeFunction &input) {
    cout << "<native fn>";
    return cout;
}