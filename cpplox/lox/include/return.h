#pragma once
#include "object.h"
#include <iostream>

class ReturnException : public std::exception {
public:
    Object value;
    
    ReturnException(Object value);
};