#include "string_dynamic_array.h"
#include "../status.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Status allocate_string_dynamic_array(StringDynamicArray *string_dynamic_array, size_t size_requested) {
    char *allocated_string_dynamic_array = (char *) malloc(sizeof(char) * size_requested);
    if (allocated_string_dynamic_array == NULL) {
        return ALLOCATION_ERROR;
    }
    string_dynamic_array->capacity = size_requested;
    string_dynamic_array->start_pointer = allocated_string_dynamic_array;
    string_dynamic_array->used = 0;

    return NO_ERROR;
}

Status reallocate_string_dynamic_array(StringDynamicArray *string_dynamic_array, size_t size_requested) {
    char *temp = realloc(string_dynamic_array->start_pointer, sizeof(char) * size_requested);
    if (temp == NULL) {
        return ALLOCATION_ERROR;
    }
    string_dynamic_array->start_pointer = temp;
    string_dynamic_array->capacity = size_requested;
    return NO_ERROR;
}

StringDynamicArrayPointer insert_string(StringDynamicArray *string_dynamic_array, const char *string, const size_t string_len) {

    StringDynamicArrayPointer return_pointer = {NO_ERROR, 0, 0};
    Status reallocation_status;
    size_t requested_size;

    if (string_len == 0 || string == NULL || string[0] == '\0') {
        return_pointer.status = STRING_EMPTY;
        return return_pointer;
    }

    if (string_dynamic_array->used + string_len + 1 >= string_dynamic_array->capacity) {
        requested_size = string_dynamic_array->capacity * 2;
        if (string_len + 1 > requested_size) {
            requested_size = string_len * 2;
        }
        reallocation_status = reallocate_string_dynamic_array(string_dynamic_array, requested_size);
        if (reallocation_status != NO_ERROR) {
            return_pointer.status = reallocation_status;
            return return_pointer;
        }
    }

    return_pointer.string_start = string_dynamic_array->used;
    return_pointer.string_length = string_len;
    strncpy(string_dynamic_array->start_pointer + string_dynamic_array->used, string, string_len);
    string_dynamic_array->start_pointer[string_dynamic_array->used + string_len] = '\0';
    string_dynamic_array->used += (string_len + 1);

    return return_pointer;
}

Status free_string_dynamic_array(StringDynamicArray *string_dynamic_array) {
    if (string_dynamic_array->start_pointer == NULL) {
        return NULL_POINTER;
    }

    free(string_dynamic_array->start_pointer);
    string_dynamic_array->start_pointer = NULL;
    string_dynamic_array->capacity = 0;
    string_dynamic_array->used = 0;

    return NO_ERROR;
}
