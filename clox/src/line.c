#include "line.h"

void initLineArray(LineArray* array) {
    array->capacity = 0;
    array->count = 0;
    array->lines = NULL;
}

void writeLineArray(LineArray* array, int line) {
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->lines = GROW_ARRAY(Line, array->lines,
            oldCapacity, array->capacity);
    }

    /**
     * Store 2 elements. One for line number and other for total instructions
     */
    if (array->lines[array->count - 1].line != line) {
        array->lines[array->count].line = line;
        // tells that there is 1 instruction on the line
        array->lines[array->count].count = 1;
        array->count++;
    }
    else {
        // tells that there is another instruction on an existing line
        array->lines[array->count - 1].count++;
    }
}

void freeLineArray(LineArray* array) {
    FREE_ARRAY(Line, array->lines, array->capacity);
    initLineArray(array);
}