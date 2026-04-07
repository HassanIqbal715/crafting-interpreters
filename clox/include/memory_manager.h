#ifndef clox_memory_h
#define clox_memory_h

#include <stdlib.h>
#include <string.h>

#include "common.h"

typedef struct FreeHeader {
    size_t size; // size of this free block
    void* address; // starting address of this free block
    struct FreeHeader* next; // next free block
} FreeHeader;

typedef struct {
    void* memory;
    size_t size;
} Memory;

typedef struct {
    size_t size;
} BlockMetaData;

void initMemory(Memory* memory);
void writeMemory(Memory* memory, size_t size);
void freeMemory(Memory* memory);
void* assignMem(Memory* memory, FreeHeader** head, size_t size);
void* reassignMem(Memory* memory, FreeHeader** head, void* pointer, 
    size_t oldSize, size_t newSize);
void freeMem(Memory* memory, FreeHeader** head, void* address);

void initFreeList(Memory* memory, FreeHeader** head);

// starting size = 16, and grows by a factor 1.5 for every capacity increase
#define GROW_CAPACITY(capacity) \
((capacity) < 16 ? 16 : (int)(capacity * 1.5))

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
(type*)reallocate(pointer, sizeof(type) * (oldCount), \
sizeof(type) * (newCount), 0)

#define FREE_ARRAY(type, pointer, oldCount) \
reallocate(pointer, sizeof(type) * (oldCount), 0, false)

#define MALLOC(size) \
reallocate(NULL, 0, size, false)

#define FREE(pointer) \
reallocate(pointer, 0, 0, false)

#define DESTROY_MEMORY() \
reallocate(NULL, 0, 0, true)

#define MEM_SIZE 1024 * sizeof(uint8_t) * 128

void* reallocate(void* pointer, size_t oldSize, size_t newSize, bool destroy);

#endif