#include "interpreter.h"
#include "lox.h"
#include "runtime_error.h"
#include <cmath>
#define EPSILON 1e-9

// Constructor and Functions
void Interpreter::interpret(Expr* expression) {
    try {
        Object value = evaluate(expression);
        std::cout << stringify(value) << std::endl;
    }
    catch (RuntimeError &error) {
        Lox::runtimeError(error);
    }
}

// Interpreter class
Object Interpreter::evaluate(Expr* expr) {
    return std::visit(*this, static_cast<ExprVariant&>(*expr));
}

bool Interpreter::isTruthy(Object &object) {
    if (std::holds_alternative<std::monostate>(object)) return false;
    if (std::holds_alternative<bool>(object)) return std::get<bool>(object);
    return true;
}

bool Interpreter::isEqual(Object &a, Object &b) {
    if (std::holds_alternative<std::monostate>(a) && 
        std::holds_alternative<std::monostate>(b)) 
        return true;
    if (std::holds_alternative<std::monostate>(a)) return false;

    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
        return fabs(std::get<double>(a) - std::get<double>(b)) < EPSILON;
    
    if (std::holds_alternative<std::string>(a) && 
        std::holds_alternative<std::string>(b))
        return std::get<std::string>(a).compare(std::get<std::string>(b)) == 0;
    
    return false;
}

std::string Interpreter::stringify(Object &object) {
    if (std::holds_alternative<std::monostate>(object)) return "nil";

    if (std::holds_alternative<double>(object)) {
        std::string text = std::to_string(std::get<double>(object));
        if (endsWith(text, ".000000")) {
            text = text.substr(0, text.length() - 7);
        }
        return text;
    }

    if (std::holds_alternative<bool>(object)) {
        bool value = std::get<bool>(object);
        if (value == true) {
            return "true";
        }
        else {
            return "false";
        }
    }

    return std::get<std::string>(object);
}

bool Interpreter::endsWith(std::string &str, std::string delimiter) {
    if (str.length() < delimiter.length()) return false;
    std::string endingString = str.substr(str.length() - delimiter.length(),  
        delimiter.length());
    return endingString.compare(delimiter) == 0;
}

void Interpreter::checkNumberOperand(Token &op, Object &right) {
    if (std::holds_alternative<double>(right)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token &op, Object &left, Object &right) {
    if (std::holds_alternative<double>(left) && 
        std::holds_alternative<double>(right)) 
        return;
    throw RuntimeError(op, "Operands must be numbers.");
}

// Interpreter visitor
Object Interpreter::operator()(Binary &expr) {
    Object left = evaluate(expr.left.get());
    Object right = evaluate(expr.right.get());

    switch(expr.op.type) {
        case BANG_EQUAL: return !isEqual(left, right);
        case COMMA: 
            return right;
        case EQUAL_EQUAL: return isEqual(left, right);
        case GREATER:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) > std::get<double>(right);
        case GREATER_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return (std::get<double>(left) > std::get<double>(right)) || 
                isEqual(left, right);
        case LESS:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) < std::get<double>(right);
        case LESS_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return (std::get<double>(left) < std::get<double>(right)) ||
                isEqual(left, right);
        case MINUS:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) - std::get<double>(right);
        case PLUS:
            if (std::holds_alternative<double>(left) && 
                    std::holds_alternative<double>(right)) {
                return std::get<double>(left) + std::get<double>(right);
            }

            if (std::holds_alternative<std::string>(left) &&
                    std::holds_alternative<std::string>(right)) {
                return std::get<std::string>(left) + 
                    std::get<std::string>(right); 
            }

            if (std::holds_alternative<std::string>(left) && 
                    std::holds_alternative<double>(right)) {
                return std::get<std::string>(left) + 
                    std::to_string(std::get<double>(right));
            }

            if (std::holds_alternative<double>(left) &&
                    std::holds_alternative<std::string>(right)) {
                return std::to_string(std::get<double>(left)) + 
                    std::get<std::string>(right);
            }
            
            throw RuntimeError(expr.op, 
                "Operands must be numbers or strings");
        case SLASH:
            checkNumberOperands(expr.op, left, right);

            if (fabs(std::get<double>(right)) < EPSILON)
                throw RuntimeError(expr.op, "Cannot divide by 0.");

            return std::get<double>(left) / std::get<double>(right);
        case STAR:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) * std::get<double>(right);
    }

    return std::monostate{};
}

Object Interpreter::operator()(Literal &expr) {
    return expr.value;
}

Object Interpreter::operator()(Grouping &expr) {
    return evaluate(expr.expression.get());
}

Object Interpreter::operator()(Ternary &expr) {
    Object right = evaluate(expr.right.get());
    Object mid = evaluate(expr.mid.get());
    Object left = evaluate(expr.left.get());

    if (isTruthy(left)) return mid;
    else return right;
}

Object Interpreter::operator()(Unary &expr) {
    Object right = evaluate(expr.right.get());

    switch(expr.op.type) {
        case BANG:
            return !isTruthy(right);
        case MINUS:
            return -(std::get<double>(right));
    }

    return std::monostate{};
}