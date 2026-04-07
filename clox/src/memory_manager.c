#include <stdio.h>
#include <unistd.h>

#include "memory_manager.h"

void initMemory(Memory* memory) {
    memory->memory = NULL;
    memory->size = 0;
}

void writeMemory(Memory* memory, size_t size) {
    if (memory == NULL) {
        memory = malloc(sizeof(Memory));
    }
    free(memory->memory);

    memory->memory = malloc(size);
    memory->size = size;
}

void freeMemory(Memory* memory) {
    if (memory != NULL) {
        if (memory->memory != NULL)
            free(memory->memory);
    }
}

static void* shiftMem(Memory* memory, FreeHeader** prev, FreeHeader** curr, 
    size_t size) {
    void* newAddress = (*curr)->address + size;
    if (size < (*curr)->size) {
        (*curr)->address = newAddress;
        (*curr)->size -= size;
        return newAddress;
    }
    else if (size == (*curr)->size) {
        (*curr)->size = 0;
        (*curr)->address = NULL;
        if ((*prev) != NULL) {
            (*prev)->next = (*curr)->next;
            (*curr) = (*prev);
        }
        return newAddress;
    }
    else {
        return NULL;
    }
}

void* assignMem(Memory* memory, FreeHeader** head, size_t size) {
    FreeHeader* prev = NULL;
    FreeHeader* current = *head;
    
    FreeHeader* bestFitPrev = NULL;
    FreeHeader* bestFit = NULL;
    size_t minSize = -1; // assign max value to it
    
    size_t totalSizeNeeded = size + sizeof(BlockMetaData);

    if (totalSizeNeeded < sizeof(FreeHeader)) { // minimum size
        totalSizeNeeded = sizeof(FreeHeader);
    }

    while(current != NULL) {
        if (current->size >= totalSizeNeeded && current->size < minSize) {
            bestFit = current;
            bestFitPrev = prev;
            minSize = current->size;
        }
        prev = current;
        current = current->next;
    }

    if (bestFit == NULL) {
        return NULL;
    }

    void* assignedAddress = bestFit->address;

    if (bestFitPrev == NULL) {
        shiftMem(memory, &bestFitPrev, head, totalSizeNeeded);
    }
    else {
        shiftMem(memory, &bestFitPrev, &bestFit, 
                                   totalSizeNeeded);
    }

    if (assignedAddress == NULL) {
        return NULL;
    }

    BlockMetaData* metaData = (BlockMetaData*) assignedAddress;
    metaData->size = totalSizeNeeded;
    
    return (void*)(metaData + 1);
}

void* reassignMem(Memory* memory, FreeHeader** head, void* pointer, 
    size_t oldSize, size_t newSize) {
    void* newMem = assignMem(memory, head, newSize);
    if (newMem == NULL) return NULL;

    memcpy(newMem, pointer, oldSize > newSize ? oldSize : newSize);
    freeMem(memory, head, pointer);

    return newMem;
}

static void coalesceMem(FreeHeader** list, FreeHeader** freedNode) {
    FreeHeader* currentNode = *list;
    while (currentNode != NULL) {
        if ((char*)currentNode < (char*)(*freedNode)) {
            if ((char*)currentNode + currentNode->size == (char*)(*freedNode)) {
                currentNode->size += (*freedNode)->size;
                (*freedNode) = currentNode;
            }
        }
        else if ((char*)(*freedNode) + (*freedNode)->size 
            == (char*)currentNode) {
            (*freedNode)->size += currentNode->size;
            (*freedNode)->next = currentNode->next;
        }
        currentNode = currentNode->next;
    }
}

void freeMem(Memory* memory, FreeHeader** head, void* address) {
    if (address == NULL) return;
    BlockMetaData* metaData = (BlockMetaData*) address - 1; // get the Meta data
    FreeHeader* newFreeNode = (FreeHeader*)metaData; // cast it into FreeHeader
    newFreeNode->address = (void*)(newFreeNode + 1);
    newFreeNode->size = metaData->size;
    newFreeNode->next = *head;

    coalesceMem(head, &newFreeNode);

    *head = newFreeNode;
}

void initFreeList(Memory* memory, FreeHeader** head) {
    if (memory == NULL) return;
    if (memory->memory == NULL) return;

    (*head)->address = memory->memory + sizeof(FreeHeader);
    if (memory->size < sizeof(FreeHeader)) {
        fprintf(stderr, "Not enough memory to initialize free list\n");
        exit(74);
    }
    (*head)->size = memory->size - sizeof(FreeHeader);
    (*head)->next = NULL;
}

void* reallocate(void* pointer, size_t oldSize, size_t newSize, bool destroy) {
    static Memory mem;
    static FreeHeader* head = NULL;
    static bool isInitialized = false;

    if (isInitialized == false) {
        initMemory(&mem);
        writeMemory(&mem, MEM_SIZE);

        head = (FreeHeader*)mem.memory;

        initFreeList(&mem, &head);
        isInitialized = true;
    }
    else if (destroy == true) {
        printf("head size: %ld\n", head->size);
        freeMemory(&mem);
        return NULL;
    }

    if (newSize == 0) {
        freeMem(&mem, &head, pointer);
        return NULL;
    }
    
    void* result = NULL;
    if (oldSize == 0 && newSize != 0) {
        result = assignMem(&mem, &head, newSize);
        if (result == NULL) exit(1);
        return result;
    }

    result = reassignMem(&mem, &head, pointer, oldSize, newSize);
    if (result == NULL) exit(1);
    return result;
}