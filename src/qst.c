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
        size_t ret = fwrite(&(value), size, num, fd); \
        int err = ferror(fd); \
        if (err) { \
            return err; \
        } \
        write_size += ret * (size); \
    } while (false)
#define WRITE4(value) CHECKED_WRITE(value, 4, 1)

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
    bin->function_offset_table = malloc(bin->function_offset_table_len);
    memcpy(bin->function_offset_table, chunk->labels, bin->function_offset_table_len);

    bin->bin_size = (uint32_t) (BIN_MINIMUM_SIZE + bin->object_code_len + bin->function_offset_table_len);
}

void free_bin(QstBin* bin) {
    free(bin->object_code);
    free(bin->function_offset_table);
    free(bin);
}
