#ifndef quesler_vm_h
#define quesler_vm_h

#include "chunk.h"
#include "value.h"

#define NUM_REGISTERS 256
#define REGISTERS_SIZE NUM_REGISTERS * sizeof(Value)

#define STACK_SIZE 256
#define STACK_LEN STACK_SIZE / 4

#define ARG_STACK_LEN 8

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value registers[NUM_REGISTERS];
    Value stack[STACK_LEN];
    Value* stack_top;
    Value arg_stack[ARG_STACK_LEN];
    Value* arg_stack_top;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void init_vm(void);
void free_vm(void);
InterpretResult interpret_source(const char* source);
InterpretResult interpret_bytecode(Chunk* chunk);
void stack_push(Value value);
Value stack_pop(void);

#endif
