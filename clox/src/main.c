#include "common.h"
#include "chunk.h"
#include "debug.h"
#include <stdio.h>

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 6.7);
    
    for (int i = 0; i < 3; i++) {
        writeConstant(&chunk, i*2, i%100);
    }

    disassembleChunk(&chunk, "test chunk");

    // freeChunk(&chunk);

    reallocate(NULL, 0, 0, true); // destroy memory
    return 0;
}