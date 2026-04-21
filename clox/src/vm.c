#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "object.h"
#include "memory_manager.h"
#include "vm.h"

VM vm;

static void resetStack() {
    vm.stack.stackTop = vm.stack.stack;
}
 static void runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = getLine(vm.chunk, instruction);
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack();
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
    vm.objects = NULL;
    initTable(&vm.strings);
    initTable(&vm.globals);
}

void freeVM() {
    freeTable(&vm.globals);
    freeTable(&vm.strings);
    freeStack(&vm.stack);
    freeObjects();
}

void push(Value value) {
    writeStack(&vm.stack, value);
}

Value pop() {
    vm.stack.stackTop--;
    vm.stack.count--;
    return *vm.stack.stackTop;
}

Value popn(int n) {
    vm.stack.stackTop -= n;
    vm.stack.count -= n;
    return *vm.stack.stackTop;
}

Value* peekPtr() {
    return vm.stack.stackTop - 1;
}

static Value peek(int distance) {
    return vm.stack.stackTop[-1 - distance];
}

static bool isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate() {
    ObjString* b = AS_STRING(pop());
    ObjString* a = AS_STRING(pop());

    int length = a->length + b->length;
    char* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString* result = takeString(chars, length);
    push(OBJ_VAL(result));
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_BYTE_LONG() ( \
    ((uint32_t) (READ_BYTE() << 16)) | \
    ((uint32_t) (READ_BYTE() << 8)) | \
    ((uint32_t) READ_BYTE()))
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG() (vm.chunk->constants.values[ \
    ((uint32_t) (READ_BYTE() << 16)) | \
    ((uint32_t) (READ_BYTE() << 8)) | \
    ((uint32_t) READ_BYTE())])
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define READ_STRING_LONG() AS_STRING(READ_CONSTANT_LONG())
#define BINARY_OP(valueType, op) \
    do { \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
            runtimeError("Operands must be numbers."); \
            return INTERPRET_RUNTIME_ERROR; \
        } \
        double b = AS_NUMBER(pop()); \
        Value* stackPeek = peekPtr(); \
        *stackPeek = (valueType(AS_NUMBER(*stackPeek) op b)); \
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
            case OP_NIL: push(NIL_VAL); break;
            case OP_TRUE: push(BOOL_VAL(true)); break;
            case OP_FALSE: push(BOOL_VAL(false)); break;
            case OP_POP: pop(); break;
            case OP_POPN: popn((int)READ_BYTE()); break;
            case OP_POPN_LONG: popn((int)READ_BYTE_LONG()); break;
            case OP_GET_LOCAL: {
                uint8_t slot = READ_BYTE();
                push(vm.stack.stack[slot]);
                break;
            }
            case OP_GET_LOCAL_LONG: {
                uint32_t slot = READ_BYTE_LONG();
                push(vm.stack.stack[slot]);
                break;
            }
            case OP_SET_LOCAL: {
                uint8_t slot = READ_BYTE();
                vm.stack.stack[slot] = peek(0);
                break;
            }
            case OP_SET_LOCAL_LONG: {
                uint32_t slot = READ_BYTE_LONG();
                vm.stack.stack[slot] = peek(0);
                break;
            }
            case OP_GET_GLOBAL: {
                ObjString* name = READ_STRING();
                Value value;
                if (!tableGet(&vm.globals, name, &value)) {
                    runtimeError("Undefined variable '%s'.", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(value);
                break;
            }
            case OP_GET_GLOBAL_LONG: {
                ObjString* name = READ_STRING_LONG();
                Value value;
                if (!tableGet(&vm.globals, name, &value)) {
                    runtimeError("Undefined variable '%s'.", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(value);
                break;
            }
            case OP_DEFINE_GLOBAL: {
                ObjString* name = READ_STRING();
                tableSet(&vm.globals, name, peek(0));
                pop();
                break;
            }
            case OP_DEFINE_GLOBAL_LONG: {
                ObjString* name = READ_STRING_LONG();
                tableSet(&vm.globals, name, peek(0));
                pop();
                break;
            }
            case OP_SET_GLOBAL: {
                ObjString* name = READ_STRING();
                if (tableSet(&vm.globals, name, peek(0))) { // does not exist
                    tableDelete(&vm.globals, name);
                    runtimeError("Undefined variable '%s'.", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_SET_GLOBAL_LONG: {
                ObjString* name = READ_STRING_LONG();
                if (tableSet(&vm.globals, name, peek(0))) { // does not exist
                    tableDelete(&vm.globals, name);
                    runtimeError("Undefined variable '%s'.", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_EQUAL: {
                Value b = pop();
                Value* stackPeek = peekPtr();
                *stackPeek = BOOL_VAL(valuesEqual(*stackPeek, b));
                break;
            }
            case OP_GREATER:  BINARY_OP(BOOL_VAL, >); break;
            case OP_LESS:     BINARY_OP(BOOL_VAL, <); break;
            case OP_ADD: {
                if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                    concatenate();
                }
                else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                    BINARY_OP(NUMBER_VAL, +);
                }
                else {
                    runtimeError(
                        "Operands must be two numbers or two strings.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;
            case OP_NOT:
                Value* stackPeek = peekPtr();
                *stackPeek = BOOL_VAL(isFalsey(*stackPeek)); 
                break;
            case OP_NEGATE: {
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                Value* stackPeek = peekPtr();
                stackPeek->as.number = -AS_NUMBER(*stackPeek); 
                break;
            }
            case OP_PRINT: {
                printValue(pop());
                printf("\n");
                break;
            }
            case OP_RETURN: {
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
#undef READ_STRING
#undef READ_STRING_LONG
#undef BINARY_OP
}

InterpretResult interpret(const char* source) {
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    freeChunk(&chunk);
    return result;
}