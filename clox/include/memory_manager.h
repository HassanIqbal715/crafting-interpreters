#ifndef clox_memory_h
#define clox_memory_h

#include <stdlib.h>
#include <string.h>

#include "common.h"

// starting size = 16, and grows by a factor 2 for every capacity increase
#define GROW_CAPACITY(capacity) \
((capacity) < 8 ? 8 : (int)(capacity * 2))

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
(type*)reallocate(pointer, sizeof(type) * (oldCount), \
sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif