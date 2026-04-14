#ifndef clox_line_h
#define clox_line_h

#include "common.h"
#include "memory_manager.h"

// Struct with line info
typedef struct {
    int line; // line number
    int count; // number of instructions on this line
} Line;

typedef struct {
    int capacity;
    int count;
    Line* lines;
} LineArray;

void initLineArray(LineArray* array);
void writeLineArray(LineArray* array, int line);
void freeLineArray(LineArray* array);

#endif