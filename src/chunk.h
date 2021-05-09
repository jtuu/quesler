#ifndef quesler_chunk_h
#define quesler_chunk_h

#include "common.h"
#include "value.h"
#include "string.h"

typedef struct {
    size_t count;
    size_t capacity;
    uint8_t* code;
    size_t* lines;
    ValueArray constants;
    int32_t* labels;
    size_t labels_count;
    size_t labels_capacity;
} Chunk;

void init_chunk(Chunk* chunk);
void write_chunk(Chunk* chunk, size_t add_size, uint8_t* bytes, size_t line);
void free_chunk(Chunk* chunk);
size_t add_constant(Chunk* chunk, Value value);
void set_chunk_label_offset(Chunk* chunk, size_t label, int32_t offset);

#define WRITE_CHUNK(chunk, type, value, line) \
    do { \
        type temp = value; \
        char buf[sizeof(type)]; \
        memcpy(buf, &(temp), sizeof(type)); \
        write_chunk(chunk, sizeof(type), (uint8_t*) buf, line); \
    } while (false)

#endif
