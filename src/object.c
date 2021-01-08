#include <string.h>
#include "object.h"
#include "memory.h"

#define ALLOCATE_OBJ(type, object_type) \
    (type*) allocate_object(sizeof(type), object_type)

static Obj* allocate_object(size_t size, ObjType type) {
    Obj* object = reallocate(NULL, 0, size);
    object->type = type;
    return object;
}

static uint32_t hash_string(const char* key, size_t length) {
    uint32_t hash = 2166136261u;

    for (size_t i = 0; i < length; i++) {
        hash ^= (unsigned char) key[i];
        hash *= 16777619;
    }

    return hash;
}

static ObjString* allocate_string(char* chars, size_t length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    return string;
}

ObjString* take_string(char* chars, size_t length) {
    uint32_t hash = hash_string(chars, length);
    return allocate_string(chars, length, hash);
}

ObjString* copy_string(const char* chars, size_t length) {
    uint32_t hash = hash_string(chars, length);

    char* heap_chars = ALLOCATE(char, length + 1);
    memcpy(heap_chars, chars, length);
    heap_chars[length] = '\0';

    return allocate_string(heap_chars, length, hash);
}
