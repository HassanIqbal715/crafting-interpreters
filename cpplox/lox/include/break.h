#pragma once
#include <iostream>
class BreakException : public std::exception {
public:
    BreakException();
};