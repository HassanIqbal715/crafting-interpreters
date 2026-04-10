#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "vm.h"

VM vm;

static void resetStack() {
    vm.stack.stackTop = vm.stack.stack;
}

void initStack(Stack* stack) {
    stack->count = 0;
    stack->capacity = 0;
    stack->stack = NULL;
    resetStack();
}

void writeStack(Stack* stack, Value value) {
    if (stack->capacity < stack->count + 1) {
        int oldCapacity = stack->capacity;
        stack->capacity = GROW_CAPACITY(oldCapacity);
        stack->stack = GROW_ARRAY(Value, stack->stack, oldCapacity, 
            stack->capacity);
        stack->stackTop = &stack->stack[stack->count];
    }
    *stack->stackTop = value;
    stack->stackTop++;
    stack->count++;
}

void freeStack(Stack* stack) {
    FREE_ARRAY(Value, stack->stack, stack->capacity);
    initStack(stack);
}

void initVM() {
    Stack stack;
    initStack(&stack);
}

void freeVM() {
    freeStack(&vm.stack);
}

void push(Value value) {
    writeStack(&vm.stack, value);
}

Value pop() {
    vm.stack.stackTop--;
    vm.stack.count--;
    return *vm.stack.stackTop;
}

Value* peek() {
    return vm.stack.stackTop - 1;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()]);
#define READ_CONSTANT_LONG() (vm.chunk->constants.values[ \
    ((uint32_t) (READ_BYTE() << 16)) | \
    ((uint32_t) (READ_BYTE() << 8)) | \
    ((uint32_t) READ_BYTE())])
#define BINARY_OP(op) \
do { \
double b = pop(); \
Value* stackPeek = peek(); \
*stackPeek = (*stackPeek op b); \
} while(false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf(" ");
    for (Value* slot = vm.stack.stack; slot < vm.stack.stackTop; slot++) {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk,
                           (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = READ_CONSTANT_LONG();
                push(constant);
                break;
            }
            case OP_ADD:      BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE:   BINARY_OP(/); break;
            case OP_NEGATE: {
                Value* stackPeek = peek();
                *stackPeek = -(*stackPeek); break;
            }
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
#undef BINARY_OP
}

InterpretResult interpret(const char* source) {
    compile(source);
    return INTERPRET_OK;
}