#include <stdlib.h>
#include "chunk.h"
#include "memory.h"

void init_chunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    init_value_array(&chunk->constants);
}

void write_chunk(Chunk* chunk, size_t add_size, uint8_t* bytes, size_t line) {
    if (chunk->capacity < chunk->count + add_size) {
        size_t old_capacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old_capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
            old_capacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(size_t, chunk->lines,
            old_capacity, chunk->capacity);
    }

    for (size_t i = 0; i < add_size; i++) {
        chunk->lines[chunk->count] = line;
        chunk->code[chunk->count] = bytes[i];
        chunk->count++;
    }
}

void free_chunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(size_t, chunk->lines, chunk->capacity);
    free_value_array(&chunk->constants);
    init_chunk(chunk);
}

size_t add_constant(Chunk* chunk, Value value) {
    write_value_array(&chunk->constants, value);
    return chunk->constants.count - 1;
}
