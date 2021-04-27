#ifndef quesler_common_h
#define quesler_common_h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define UINT8_COUNT (UINT8_MAX + 1)

#define UNUSED(x) ((void) x)

size_t read_file(const char* path, uint8_t** dst_buf, bool binary);

size_t read_file_ex(const char* path, uint8_t** dst_buf, size_t dst_buf_size, size_t dst_buf_offset, bool binary);

size_t write_file(const char* path, uint8_t* src_buf, size_t src_buf_size, bool binary);

#endif
