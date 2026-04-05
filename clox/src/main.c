#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    // 4 - 3 * -2 without OP_SUBTRACT
    writeConstant(&chunk, 4, 1);
    writeConstant(&chunk, 3, 1);
    writeConstant(&chunk, 2, 1);
    writeChunk(&chunk, OP_NEGATE, 1);
    writeChunk(&chunk, OP_MULTIPLY, 1);
    writeChunk(&chunk, OP_NEGATE, 1);
    writeChunk(&chunk, OP_ADD, 1);

    writeChunk(&chunk, OP_RETURN, 2);

    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);
    DESTROY_MEMORY();
    return 0;
}