#ifndef quesler_value_h
#define quesler_value_h

#include "common.h"

typedef enum {
    VAL_BOOLEAN,
    VAL_DWORD,
    VAL_WORD,
    VAL_BYTE,
    VAL_FLOAT,
    VAL_NULL,
    VAL_STRING
} ValueType;

typedef struct {
    ValueType type;
    union {
        int32_t boolean;
        int32_t dword;
        int16_t word;
        uint8_t byte;
        float fp;
        uint16_t* string;
    } as;
} Value;

typedef enum {
    CONSTANT_FALSE = 0,
    CONSTANT_TRUE = 1
} ConstantValue;

#define BOOLEAN_VAL(value)   ((Value) {VAL_BOOLEAN, {.boolean = value}})
#define DWORD_VAL(value)     ((Value) {VAL_DWORD, {.dword = value}})
#define WORD_VAL(value)      ((Value) {VAL_WORD, {.word = value}})
#define BYTE_VAL(value)      ((Value) {VAL_BYTE, {.byte = value}})
#define FLOAT_VAL(value)     ((Value) {VAL_FLOAT, {.fp = value}})
#define NULL_VAL             ((Value) {VAL_NULL, {.dword = 0}})
#define STRING_VAL(value)    ((Value) {VAL_STRING, {.string = value}})

#define AS_BOOLEAN(value)    ((value).as.boolean)
#define AS_DWORD(value)      ((value).as.dword)
#define AS_WORD(value)       ((value).as.word)
#define AS_BYTE(value)       ((value).as.byte)
#define AS_FLOAT(value)      ((value).as.fp)
#define AS_STRING(value)     ((value).as.string)

#define IS_BOOLEAN(value)    ((value).type == VAL_BOOLEAN)
#define IS_DWORD(value)      ((value).type == VAL_DWORD)
#define IS_WORD(value)       ((value).type == VAL_WORD)
#define IS_BYTE(value)       ((value).type == VAL_BYTE)
#define IS_FLOAT(value)      ((value).type == VAL_FLOAT)
#define IS_NULL(value)       ((value).type == VAL_NULL)
#define IS_STRING(vale)      ((value).type == VAL_STRING)

typedef struct {
    size_t capacity;
    size_t count;
    Value* values;
} ValueArray;

void init_value_array(ValueArray* array);
void write_value_array(ValueArray* array, Value value);
void free_value_array(ValueArray* array);
void print_value(Value value);

#endif
