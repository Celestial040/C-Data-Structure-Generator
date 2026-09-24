#ifndef STRING_DYNAMIC_ARRAY_H
#define STRING_DYNAMIC_ARRAY_H

#include "../status.h"
#include <stddef.h>

typedef struct StringDynamicArray {
    char *start_pointer;
    size_t used;
    size_t capacity;
}StringDynamicArray;

typedef struct StringDynamicArrayPointer {
    size_t string_start;
    size_t string_length;
    Status status;
}StringDynamicArrayPointer;


Status allocate_string_dynamic_array(StringDynamicArray *string_dynamic_array, size_t size_requested);
StringDynamicArrayPointer insert_string(StringDynamicArray *string_dynamic_array, const char *string, const size_t string_len);
Status free_string_dynamic_array(StringDynamicArray *string_dynamic_array);

#endif
