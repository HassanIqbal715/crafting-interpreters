#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// Enum with all the OpCodes
typedef enum {
    OP_CONSTANT,
    OP_RETURN
} OpCode;

// Chunk
// Sequence of instructions "Chunk" to represent bytecode.
typedef struct {
    int count; // total used elements.
    int capacity; // total allocated elements.
    uint8_t* code; // each instruction element is of 1 byte.
    int* lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);

#endif