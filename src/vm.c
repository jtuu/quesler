#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "debug.h"
#include "vm.h"
#include "compiler.h"
#include "opcodes.h"
#include "memory.h"

VM vm;

#define READ1() (*vm.ip++)
#define READ2() \
    (vm.ip += 2, (vm.ip[-1] << 8 | vm.ip[-2]))
#define READ4() \
    (vm.ip += 4, (vm.ip[-1] << 24 | vm.ip[-2] << 16 | \
                  vm.ip[-3] << 8  | vm.ip[-4]))
#define READF() \
    (vm.ip += 4, *(float*) &vm.ip[-4])

static void reset_registers(void) {
    for (size_t i = 0; i < NUM_REGISTERS; i++) {
        vm.registers[i] = DWORD_VAL(0);
    }
}

static void reset_arg_stack(void) {
    vm.arg_stack_top = vm.arg_stack;
}

static void reset_stack(void) {
    vm.stack_top = vm.stack;
}

void init_vm() {
    reset_stack();
    reset_arg_stack();
    reset_registers();
}

void free_vm() {}

void stack_push(Value value) {
    *vm.stack_top = value;
    vm.stack_top++;
}

Value stack_pop() {
    if (--vm.stack_top < vm.stack) {
        fprintf(stderr, "VM: Stack underflow\n");
        vm.stack_top = vm.stack;
    }

    return *vm.stack_top;
}

static void arg_stack_push(Value value) {
    *vm.arg_stack_top = value;
    vm.arg_stack_top++;
}

static void take_stack_args(Value* dst) {
    for (size_t i = 0; i < ARG_STACK_LEN; i++) {
        vm.arg_stack_top--;
        *dst = *vm.arg_stack_top;
        dst++;
    }

    reset_arg_stack();
}

static uint16_t* process_string_tags(uint16_t* str) {
    typedef enum {
        TAG_UNKNOWN,
        TAG_REGISTER
    } TagType;

    uint16_t* cur_pos = str;

    uint16_t* tag_start = NULL;
    uint16_t* tag_value_start = NULL;
    TagType tag_type = TAG_UNKNOWN;

    struct {
        uint16_t* str;
        size_t len;
        size_t capacity;
    } result = {.str = NULL, .len = 0, .capacity = 0};

    while (true) {
        uint16_t chr = *cur_pos;

        if (chr == 0) {
            break;
        }

        switch ((char) *cur_pos) {
            case '<':
                tag_start = cur_pos;
                break;
            case '>':
                switch (tag_type) {
                    case TAG_REGISTER: {
                        uint8_t reg = 0;
                        do {
                            reg = (uint8_t) (reg * 10 + (*tag_value_start - '0'));
                        } while (++tag_value_start < cur_pos);

                        int32_t val = AS_DWORD(vm.registers[reg]);
                        uint16_t val_str[10];
                        size_t val_str_len = 0;

                        do {
                            val_str[10 - val_str_len - 1] = (uint16_t) ('0' + val % 10);
                            val_str_len++;
                            val /= 10;
                        } while (val > 0);

                        while (result.capacity < result.len + val_str_len + 1) {
                            size_t old_capacity = result.capacity;
                            result.capacity = GROW_CAPACITY(old_capacity);
                            result.str = GROW_ARRAY(uint16_t, result.str,
                                old_capacity, result.capacity);
                        }

                        memcpy((char*) &result.str[result.len], (char*) &val_str[10 - val_str_len],
                            val_str_len * 2);
                        result.len += val_str_len;
                        
                        break;
                    }
                    default:
                        fprintf(stderr, "VM: Unknown string tag\n");
                        break;
                }

                tag_start = NULL;
                tag_value_start = NULL;
                tag_type = TAG_UNKNOWN;
                break;
            default:
                if (tag_start == NULL) {
                    if (result.capacity < result.len + 2) {
                        size_t old_capacity = result.capacity;
                        result.capacity = GROW_CAPACITY(old_capacity);
                        result.str = GROW_ARRAY(uint16_t, result.str,
                            old_capacity, result.capacity);
                    }

                    result.str[result.len] = *cur_pos;
                    result.len++;
                } else if (cur_pos - tag_start == 1) {

                    switch ((char) *cur_pos) {
                        case 'R': __attribute__((fallthrough));
                        case 'r':
                            tag_type = TAG_REGISTER;
                            tag_value_start = cur_pos + 1;
                            break;
                        default:
                            fprintf(stderr, "VM: Unknown string tag\n");
                            break;
                    }
                }
                break;
        }

        cur_pos++;
    }

    result.str[result.len] = 0;

    return result.str;
}

static bool compare(uint16_t opcode, int32_t val1, int32_t val2) {
    switch (opcode) {
        case OP_JMP_EQ: __attribute__ ((fallthrough));
        case OP_JMPI_EQ:
            return val1 == val2;
        case OP_JMP_NEQ: __attribute__ ((fallthrough));
        case OP_JMPI_NEQ:
            return val1 != val2;
        case OP_JMP_GT: __attribute__ ((fallthrough));
        case OP_JMPI_GT:
            return val1 > val2;
        case OP_JMP_LT: __attribute__ ((fallthrough));
        case OP_JMPI_LT:
            return val1 < val2;
        case OP_JMP_GTE: __attribute__ ((fallthrough));
        case OP_JMPI_GTE:
            return val1 >= val2;
        case OP_JMP_LTE: __attribute__ ((fallthrough));
        case OP_JMPI_LTE:
            return val1 <= val2;
        default:
            fprintf(stderr, "VM: Invalid comparison opcode\n");
            break;
    }

    return false;
}

static void do_jump(size_t label) {
    int32_t offset = vm.chunk->labels[label];

    if (offset < 0) {
        fprintf(stderr, "VM: Invalid jump target\n");
        return;
    }

    vm.ip = vm.chunk->code + offset;
}

static InterpretResult run(void) {
#define ARITHMETIC_OP_RI(op) \
    do { \
        uint8_t reg = READ1(); \
        int32_t val1 = AS_DWORD(vm.registers[reg]); \
        int32_t val2 = READ4(); \
        vm.registers[reg] = DWORD_VAL(val1 op val2); \
    } while (false)
#define ARITHMETIC_OP_RR(op) \
    do { \
        uint8_t reg1 = READ1(); \
        uint8_t reg2 = READ1(); \
        int32_t val1 = AS_DWORD(vm.registers[reg1]); \
        int32_t val2 = AS_DWORD(vm.registers[reg2]); \
        vm.registers[reg1] = DWORD_VAL(val1 op val2); \
    } while (false)
#define ARITHMETIC_OP_RF(op) \
    do { \
        uint8_t reg1 = READ1(); \
        int8_t reg2 = READ1(); \
        float val1 = *(float*) &vm.registers[reg1]; \
        float val2 = *(float*) &vm.registers[reg2]; \
        vm.registers[reg1] = FLOAT_VAL(val1 op val2); \
    } while (false)

    while (true) {
#ifdef DEBUG
        printf("          ");
        for (Value* slot = vm.stack; slot < vm.stack_top; slot++) {
            printf("[ ");
            print_value(*slot);
            printf(" ]");
        }
        printf("\n");

        disassemble_instruction(vm.chunk, (size_t) (vm.ip - vm.chunk->code));
#endif
        uint16_t opcode = READ1();
        
        if (opcode == EXTENDED_OPCODE_TABLE1 ||
                opcode == EXTENDED_OPCODE_TABLE2) {
            opcode = (uint16_t) (opcode << 8 | READ1());
        }

        const OpcodeDefinition* opcode_def = get_opcode_definition(opcode);

        bool has_stack_args = opcode_def->parameters[0] == T_ARGS;

        Value stack_args[ARG_STACK_LEN];
        if (has_stack_args) {
            take_stack_args(stack_args);
        }

        switch (opcode) {
            case OP_RET: 
                return INTERPRET_OK;
            case OP_ARG_PUSHS: {
                    uint16_t* str_start = (uint16_t*) vm.ip;
                    uint16_t* str_end = (uint16_t*)  vm.ip;

                    while (*str_end != 0) {
                        str_end++;
                    }

                    vm.ip += (size_t) (str_end - str_start + 1) * 2;

                    arg_stack_push(STRING_VAL(str_start));
                }
                break;
            case OP_LETI: {
                uint8_t reg = READ1();
                int32_t val = READ4();
                vm.registers[reg] = DWORD_VAL(val);
                break;
            }
            case OP_REV: {
                uint8_t reg = READ1();
                if (vm.registers[reg].as.dword == 0) {
                    vm.registers[reg].as.dword = 1;
                } else {
                    vm.registers[reg].as.dword = 0;
                }
                break;
            }
            case OP_JMP: {
                size_t label = (size_t) READ2();
                int32_t offset = vm.chunk->labels[label];

                if (offset < 0) {
                    fprintf(stderr, "VM: Invalid jump target\n");
                } else {
                    vm.ip = vm.chunk->code + offset;
                }
                break;
            }
            case OP_JMP_EQ:  __attribute__ ((fallthrough));
            case OP_JMP_NEQ: __attribute__ ((fallthrough));
            case OP_JMP_GT:  __attribute__ ((fallthrough));
            case OP_JMP_LT:  __attribute__ ((fallthrough));
            case OP_JMP_GTE: __attribute__ ((fallthrough));
            case OP_JMP_LTE: {
                uint8_t reg1 = READ1();
                uint8_t reg2 = READ1();
                int32_t val1 = AS_DWORD(vm.registers[reg1]);
                int32_t val2 = AS_DWORD(vm.registers[reg2]);
                size_t label = (size_t) READ2();

                if (compare(opcode, val1, val2)) {
                    do_jump(label);
                }
                break;
            }
            case OP_JMPI_EQ:  __attribute__ ((fallthrough));
            case OP_JMPI_NEQ: __attribute__ ((fallthrough));
            case OP_JMPI_GT:  __attribute__ ((fallthrough));
            case OP_JMPI_LT:  __attribute__ ((fallthrough));
            case OP_JMPI_GTE: __attribute__ ((fallthrough));
            case OP_JMPI_LTE: {
                uint8_t reg = READ1();
                int32_t val1 = AS_DWORD(vm.registers[reg]);
                int32_t val2 = READ4();
                size_t label = (size_t) READ2();

                if (compare(opcode, val1, val2)) {
                    do_jump(label);
                }
                break;
            }
            case OP_ADDI:
                ARITHMETIC_OP_RI(+);
                break;
            case OP_SUBI:
                ARITHMETIC_OP_RI(-);
                break;
            case OP_MULI: 
                ARITHMETIC_OP_RI(*);
                break;
            case OP_DIVI:
                ARITHMETIC_OP_RI(/);
                break;
            case OP_ADD:
                ARITHMETIC_OP_RR(+);
                break;
            case OP_SUB:
                ARITHMETIC_OP_RR(-);
                break;
            case OP_MUL: 
                ARITHMETIC_OP_RR(*);
                break;
            case OP_DIV:
                ARITHMETIC_OP_RR(/);
                break;
            case OP_STACK_PUSH: {
                uint8_t reg = READ1();
                stack_push(vm.registers[reg]);
                break;
            }
            case OP_STACK_POP: {
                uint8_t reg = READ1();
                vm.registers[reg] = stack_pop();
                break;
            }
            case OP_WINDOW_MSG: {
                uint16_t* processed = process_string_tags(AS_STRING(stack_args[0]));
                print_value(STRING_VAL(processed));
                printf("\n");
                free(processed);
                break;
            }
            case OP_WINEND:
                break;
        }

        if (has_stack_args) {
            reset_arg_stack();
        }
    }
}

InterpretResult interpret_bytecode(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    return result;
}

InterpretResult interpret_source(const char* source) {
    Chunk chunk;
    init_chunk(&chunk);

    if (!compile(source, &chunk)) {
        free_chunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    InterpretResult result = interpret_bytecode(&chunk);
    free_chunk(&chunk);
    return result;
}

#undef READ1
#undef READ2
#undef READ4
