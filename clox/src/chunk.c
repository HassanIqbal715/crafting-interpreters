#include <stdlib.h>

#include "chunk.h"
#include "memory_manager.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;

    initLineArray(&chunk->lines);
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    // passing type and capacity for size calculation
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeLineArray(&chunk->lines);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    // full
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
            oldCapacity, chunk->capacity);
    }

    writeLineArray(&chunk->lines, line);

    chunk->code[chunk->count] = byte;
    chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1; // return index to help locate it later.
}

int getLine(Chunk* chunk, int byteIndex) {
    if (byteIndex >= chunk->count || chunk->lines.count <= 0) return -1;

    int lineNumber = -1;
    for (int i = 0; i <= byteIndex; 
            i += chunk->lines.lines[lineNumber].count) {
        lineNumber++;
    }

    return chunk->lines.lines[lineNumber].line;
}