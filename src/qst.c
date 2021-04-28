#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "qst.h"
#include "chunk.h"

#define BIN_MINIMUM_SIZE 4652

int write_bin(FILE* fd, QstBin* bin) {
    size_t write_size = 0;

#define CHECKED_WRITE(value, size, num) \
    do { \
        size_t ret = fwrite(value, size, num, fd); \
        int err = ferror(fd); \
        if (err) { \
            return err; \
        } \
        write_size += ret * (size); \
    } while (false)
#define WRITE4(value) CHECKED_WRITE(&(value), 4, 1)

    bin->object_code_offset = BIN_MINIMUM_SIZE;
    bin->function_offset_table_offset = (uint32_t) (bin->object_code_offset +
                                                    bin->object_code_len);
    bin->xffffffff = 0xffffffff;
    bin->padding = 0;

    WRITE4(bin->object_code_offset);
    WRITE4(bin->function_offset_table_offset);
    WRITE4(bin->bin_size);
    WRITE4(bin->xffffffff);
    WRITE4(bin->quest_number);
    WRITE4(bin->language);
    
    CHECKED_WRITE(bin->quest_name, 2, 32);
    CHECKED_WRITE(bin->short_description, 2, 128);
    CHECKED_WRITE(bin->long_description, 2, 288);
    
    WRITE4(bin->padding);

    CHECKED_WRITE(bin->items_list, 4, 932);
    CHECKED_WRITE(bin->object_code, 1, bin->object_code_len);
    CHECKED_WRITE(bin->function_offset_table, 4, bin->function_offset_table_len);

    return (int) write_size;

#undef CHECKED_WRITE
#undef WRITE4
}

int read_bin(FILE* fd, QstBin* bin) {
    size_t read_size = 0;

#define CHECKED_READ(dst, size, num) \
    do { \
        size_t ret = fread(dst, size, num, fd); \
        int err = ferror(fd); \
        if (err) { \
            return err; \
        } \
        read_size += ret * (size); \
    } while (false)

    CHECKED_READ(&bin->object_code_offset, 4, 1);
    CHECKED_READ(&bin->function_offset_table_offset, 4, 1);
    CHECKED_READ(&bin->bin_size, 4, 1);
    CHECKED_READ(&bin->xffffffff, 4, 1);
    CHECKED_READ(&bin->quest_number, 4, 1);
    CHECKED_READ(&bin->language, 4, 1);
    CHECKED_READ(&bin->quest_name, 64, 1);
    CHECKED_READ(&bin->short_description, 256, 1);
    CHECKED_READ(&bin->long_description, 576, 1);
    CHECKED_READ(&bin->padding, 4, 1);
    CHECKED_READ(&bin->items_list, 3728, 1);

    bin->object_code_len = bin->function_offset_table_offset - bin->object_code_offset;
    bin->object_code = malloc(bin->object_code_len);
    CHECKED_READ(bin->object_code, bin->object_code_len, 1);

    size_t labels_size = bin->bin_size - bin->function_offset_table_offset;
    bin->function_offset_table_len = labels_size / 4;
    bin->function_offset_table = malloc(labels_size);
    CHECKED_READ(bin->function_offset_table, labels_size, 1);

    return (int) read_size;

#undef CHECKED_READ
}

static void copy_to_wide_string(uint16_t* dst, char* src, size_t len) {
    size_t n = 0;
    while (*src != '\0' && n < len) {
        dst[n * 2] = (uint16_t) *src;
        n++;
        src++;
    }
}

QstBin* create_bin(
    uint32_t quest_number,
    uint32_t language,
    char* quest_name,
    char* short_description,
    char* long_description) {

    QstBin* bin = calloc(1, sizeof(QstBin));

    bin->quest_number = quest_number;
    bin->language = language;

    copy_to_wide_string(bin->quest_name, quest_name, 32);
    copy_to_wide_string(bin->short_description, short_description, 128);
    copy_to_wide_string(bin->long_description, long_description, 288);

    return bin;
}

void chunk_to_bin(Chunk* chunk, QstBin* bin) {
    bin->object_code_len = chunk->count;
    bin->object_code = malloc(bin->object_code_len);
    memcpy(bin->object_code, chunk->code, bin->object_code_len);

    bin->function_offset_table_len = chunk->labels_count;
    bin->function_offset_table = malloc(bin->function_offset_table_len * 4);
    memcpy(bin->function_offset_table, chunk->labels, bin->function_offset_table_len * 4);

    bin->bin_size = (uint32_t) (BIN_MINIMUM_SIZE + bin->object_code_len + bin->function_offset_table_len * 4);
}

void bin_to_chunk(QstBin* bin, Chunk* chunk) {
    chunk->count = bin->object_code_len;
    chunk->code = malloc(chunk->count);
    memcpy(chunk->code, bin->object_code, chunk->count);

    chunk->labels_count = bin->function_offset_table_len;
    chunk->labels = malloc(chunk->labels_count * 4);
    memcpy(chunk->labels, bin->function_offset_table, chunk->labels_count * 4);
}

void free_bin(QstBin* bin) {
    free(bin->object_code);
    free(bin->function_offset_table);
}
