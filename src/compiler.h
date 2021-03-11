#ifndef quesler_compiler_h
#define quesler_compiler_h

#include "vm.h"

bool compile(const char* source, Chunk* chunk);

extern size_t entry_point_label;

#endif
