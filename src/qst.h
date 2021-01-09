#ifndef quesler_qst_h
#define quesler_qst_h

#include <stdio.h>
#include "common.h"

typedef struct {
    uint32_t object_code_offset;
    uint32_t function_offset_table_offset;
    uint32_t bin_size;
    uint32_t xffffffff;
    uint32_t quest_number;
    uint32_t language;
    uint16_t quest_name[32];
    uint16_t short_description[128];
    uint16_t long_description[288];
    uint32_t padding;
    uint32_t items_list[932];
    unsigned int object_code_len;
    uint8_t *object_code;
    unsigned int function_offset_table_len;
    int32_t *function_offset_table;
} QstBin;

int write_bin(FILE* fd, QstBin* bin);
void free_bin(QstBin* bin);

#endif
