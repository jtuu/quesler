#include <stdio.h>
#include <stdlib.h>
#include "common.h"

size_t read_file_ex(const char* path, uint8_t** dst_buf, size_t dst_buf_size, size_t dst_buf_offset, bool binary) {
    FILE* fd = fopen(path, binary ? "rb" : "r");
    if (fd == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return 0;
    }

    uint8_t* cursor = *dst_buf + dst_buf_offset;
    size_t read_target_size = 0x1000;
    size_t buf_size = dst_buf_size;
    size_t read_size = 0;

    do {
        buf_size += read_target_size;
        *dst_buf = realloc(*dst_buf, buf_size);
        cursor = *dst_buf + dst_buf_offset + buf_size - dst_buf_size - read_target_size;
        read_size = fread(cursor, 1, read_target_size, fd);
    } while (read_size == read_target_size);

    return buf_size - dst_buf_size - read_target_size + read_size;
}

size_t read_file(const char* path, uint8_t** dst_buf, bool binary) {
    return read_file_ex(path, dst_buf, 0, 0, binary);
}

size_t write_file(const char* path, uint8_t* src_buf, size_t src_buf_size, bool binary) {
    FILE* fd = fopen(path, binary ? "wb" : "w");
    if (fd == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return 0;
    }

    uint8_t* cursor = src_buf;
    size_t write_block_size = 0x1000;
    size_t write_size = 0;

    if (src_buf_size < write_block_size) {
        write_block_size = src_buf_size;
    }

    do {
        cursor = src_buf + write_size;
        write_size = fwrite(cursor, write_block_size, 1, fd) * write_block_size;
    } while (write_size == write_block_size && (size_t) (cursor - src_buf) < src_buf_size);

    return (size_t) (cursor - src_buf);
}
