#include <stdio.h>
#include "memory.h"
#include "value.h"

void init_value_array(ValueArray* array) {
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

void write_value_array(ValueArray* array, Value value) {
    if (array->capacity < array->count + 1) {
        size_t old_capacity = array->capacity;
        array->capacity = GROW_CAPACITY(old_capacity);
        array->values = GROW_ARRAY(Value, array->values,
            old_capacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void free_value_array(ValueArray* array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    init_value_array(array);
}

void print_value(Value value) {
    switch (value.type) {
        case VAL_BOOLEAN:
            printf(AS_BOOLEAN(value) ? "true" : "false");
            break;
        case VAL_DWORD:
            printf("%d", AS_DWORD(value));
            break;
        case VAL_WORD:
            printf("%hd", AS_WORD(value));
            break;
        case VAL_BYTE:
            printf("%hhd", AS_BYTE(value));
            break;
        case VAL_FLOAT:
            printf("%f", AS_FLOAT(value));
            break;
        case VAL_NULL:
            printf("null");
            break;
        case VAL_STRING: {
            uint16_t* str = AS_STRING(value);
            while (*str != 0) {
                printf("%lc", (wchar_t) *str);
                str++;
            }
            break;
        }
        default:
            printf("Unknown value");
            break;
    }
}
