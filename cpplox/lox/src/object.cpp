#include "object.h"

// The visitor struct
std::string ObjectToString::operator()(bool val) {
    return (val == true) ? "true" : "false";
}

std::string ObjectToString::operator()(double val) {
    return std::to_string(val);
}

std::string ObjectToString::operator()(std::shared_ptr<LoxCallable> val) {
    return "callable";
}

std::string ObjectToString::operator()(std::shared_ptr<LoxInstance> val) {
    return "instance";
}

std::string ObjectToString::operator()(std::string val) {
    return val;
}

std::string ObjectToString::operator()(std::monostate val) {
    return "nil";
}