#ifndef quesler_debug_h
#define quesler_debug_h

#include "chunk.h"

void disassemble_chunk(Chunk* chunk, const char* name, bool line_info);
size_t disassemble_instruction(Chunk* chunk, size_t offset, bool line_info);

#endif
