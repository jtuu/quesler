#ifndef quesler_chunk_h
#define quesler_chunk_h

#include "common.h"
#include "value.h"

typedef struct {
    size_t count;
    size_t capacity;
    uint8_t* code;
    size_t* lines;
    ValueArray constants;
} Chunk;

void init_chunk(Chunk* chunk);
void write_chunk(Chunk* chunk, size_t add_size, uint8_t* bytes, size_t line);
void free_chunk(Chunk* chunk);
size_t add_constant(Chunk* chunk, Value value);

#define WRITE_CHUNK(chunk, type, value, line) \
    do { \
        type temp = value; \
        write_chunk(chunk, sizeof(type), (uint8_t*) &(temp), line); \
    } while (false)

#endif
