#include "runtime_error.h"

RuntimeError::RuntimeError(Token token, std::string message) : 
    token{token}, runtime_error{message} {}