#ifndef quesler_object_h
#define quesler_object_h

#include "common.h"

typedef enum {
    OBJ_STRING
} ObjType;

typedef struct {
    ObjType type;
} Obj;

typedef struct {
    Obj obj;
    size_t length;
    char* chars;
    uint32_t hash;
} ObjString;

ObjString* take_string(char* chars, size_t length);
ObjString* copy_string(const char* chars, size_t length);

#endif
