#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);
    int constant = addConstant(&chunk, 6.7);
    writeChunk(&chunk, OP_CONSTANT, 67); // expects a constant after this
    writeChunk(&chunk, constant, 67);

    writeChunk(&chunk, OP_RETURN, 67);

    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    return 0;
}