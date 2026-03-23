#pragma once
#include <string>
#include <variant>
#include <memory>

class LoxCallable;

struct ObjectToString{
    std::string operator()(bool val);
    std::string operator()(double val);
    std::string operator()(std::shared_ptr<LoxCallable> val);
    std::string operator()(std::string val);
    std::string operator()(std::monostate val);
};

using Object = std::variant<
    bool,
    double,
    std::shared_ptr<LoxCallable>,
    std::string,
    std::monostate
>;