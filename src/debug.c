#include <stdio.h>
#include "debug.h"
#include "value.h"
#include "opcodes.h"

void disassemble_chunk(Chunk* chunk, const char* name) {
    printf("== %s==\n", name);

    for (size_t offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
}

#define READ2(buf) ((buf)[1] << 8 | (buf)[0])
#define READ4(buf) ((buf)[3] << 24 | (buf)[2] << 16 | \
                    (buf)[1] << 8 | (buf)[0])

size_t disassemble_instruction(Chunk* chunk, size_t offset) {
    printf("%04ld ", offset);

    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4ld ", chunk->lines[offset]);
    }

    size_t opcode_size = 1;
    size_t args_size = 0;

    uint16_t opcode = chunk->code[offset];
    
    if (opcode == EXTENDED_OPCODE_TABLE1 ||
            opcode == EXTENDED_OPCODE_TABLE2) {
        opcode_size = 2;
        opcode = (uint16_t) READ2(&chunk->code[offset]);
    }

    const OpcodeDefinition* opcode_def = get_opcode_definition(opcode);

    if (opcode_def == NULL) {
        fprintf(stderr, "Disassembler: Unknown opcode 0x%hx at offset 0x%08lx\n", opcode, offset);
        printf("unknown%hx", opcode);
        goto done;
    }

    printf("%s", opcode_def->name);

    uint8_t* arg = &chunk->code[offset + opcode_size];

    for (size_t i = 0; i < OPCODE_MAX_ARITY; i++) {
        ParameterType param_type = opcode_def->parameters[i];

        if (param_type == T_NONE || param_type == T_ARGS) {
            break;
        } else if (param_type == T_IMED || param_type == T_PUSH) {
            continue;
        } else {
            if (i < 2) {
                printf(" ");
            } else {
                printf(", ");
            }

            switch (param_type) {
                case T_REG:
                    printf("R%hhu", *arg);
                    break;
                case T_DWORD:
                    printf("%d", READ4(arg));
                    break;
                case T_STR: {
                    uint16_t* str_start = (uint16_t*) arg;
                    uint16_t* str_end = (uint16_t*) arg;
                    
                    while (*str_end != 0) {
                        printf("%lc", (wchar_t) *str_end);
                        str_end++;
                    }

                    size_t str_size = (size_t) (str_end - str_start + 1) * 2;
                    args_size += str_size;
                    arg += str_size;
                    break;
                }
                case T_FUNC: {
                    size_t label = (size_t) READ2(arg);
                    int32_t label_offset = chunk->labels[label];
                    printf("%lu (%d)", label, label_offset);
                    break;
                }
                default:
                    fprintf(stderr, "Disassembler: Unknown parameter type %u\n", param_type);
                    goto done;
            }
        }

        if (!is_variable_sized(param_type)) {
            size_t param_size = get_parameter_size(param_type);
            args_size += param_size;
            arg += param_size;
        }
    }

done:
    printf("\n");
    return offset + opcode_size + args_size;
}
