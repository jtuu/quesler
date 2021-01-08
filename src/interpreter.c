#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void repl(void) {
    char* line = NULL;
    size_t buf_size = 0;

    while (true) {
        long line_len = getline(&line, &buf_size, stdin);

        if (line_len < 0) {
            printf("\n");
            break;
        }

        interpret_source(line);
    }
}

static void read_file(const char* path, uint8_t** file_buf, bool binary) {
    FILE* fd = fopen(path, binary ? "rb" : "r");
    if (fd == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return;
    }

    size_t read_target_size = 0x1000;
    size_t buf_size = read_target_size;
    size_t read_size = 0;

    do {
        *file_buf = realloc(*file_buf, buf_size);
        read_size = fread(*file_buf, read_target_size, 1, fd);
        buf_size += read_target_size;
    } while (read_size == read_target_size);
}

static InterpretResult run_source_file(const char* path) {
    uint8_t* file_buf = NULL;
    read_file(path, &file_buf, false);
    InterpretResult result = interpret_source((char*) file_buf);
    free(file_buf);

    return result;
}

static InterpretResult run_bytecode_file(const char* path) {
    uint8_t* file_buf = NULL;
    read_file(path, &file_buf, true);

    Chunk chunk;
    init_chunk(&chunk);
    chunk.code = file_buf;

    InterpretResult result = interpret_bytecode(&chunk);
    free(file_buf);

    return result;
}

int main(int argc, const char* argv[]) {
    init_vm();

    int ret = 0;

    if (argc == 1) {
        repl();
    } else {
        bool bytecode_input = false;

        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--bytecode") == 0) {
                bytecode_input = true;
                break;
            }
        }

        const char* input_file = argv[argc - 1];

        InterpretResult result;
        if (bytecode_input) {
            result = run_bytecode_file(input_file);
        } else {
            result = run_source_file(input_file);
        }

        switch (result) {
            case INTERPRET_RUNTIME_ERROR:
                fprintf(stderr, "Runtime error\n");
                __attribute__((fallthrough));
            case INTERPRET_COMPILE_ERROR:
                fprintf(stderr, "Compile error\n");
                ret = 1;
                break;
            default:
                break;
        }
    }

    free_vm();
    return ret;
}
