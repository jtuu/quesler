#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "qst.h"

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

    bin->object_code_offset = 4652;
    bin->function_offset_table_offset = bin->object_code_offset +
                                        bin->object_code_len;
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

void free_bin(QstBin* bin) {
    free(bin->object_code);
    free(bin->function_offset_table);
    free(bin);
}
