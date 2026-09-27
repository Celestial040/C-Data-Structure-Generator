#include "../status.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "index_location_dynamic_array.h"

Status allocate_index_location_dynamic_array(IndexLocationDynamicArray *array, size_t size_requested) {
    uint32_t *temp_array = (uint32_t *) malloc(sizeof(uint32_t) *  size_requested);
    if (temp_array == NULL) {
        return ALLOCATION_ERROR;
    }

    array->capacity = size_requested;
    array->count = 0;
    array->array = temp_array;

    return NO_ERROR;
}

Status reallocate_index_location_dynamic_array(IndexLocationDynamicArray *array, size_t size_requested) {
    uint32_t *temp_array = (uint32_t *) realloc(array->array,sizeof(uint32_t) *  size_requested);
    if (temp_array == NULL) {
        return ALLOCATION_ERROR;
    }

    array->capacity = size_requested;
    array->array = temp_array;

    return NO_ERROR;
}

Status insert_item_index_location_dynamic_array(IndexLocationDynamicArray *array, uint32_t item) {
    if (array->count + 1 >= array->capacity) {
        Status reallocation_status = reallocate_index_location_dynamic_array(array, array->capacity * 2);
        if (reallocation_status != NO_ERROR) {
            return reallocation_status;
        }
    }

    array->array[array->count] = item;
    array->count++;

    return NO_ERROR;
}

IndexLocationDynamicArrayPointer insert_item_return_id_index_location_dynamic_array(IndexLocationDynamicArray *array, uint32_t item) {
    IndexLocationDynamicArrayPointer returned_index = {NO_ERROR , 0};
    if (array->count + 1 >= array->capacity) {
        Status reallocation_status = reallocate_index_location_dynamic_array(array, array->capacity * 2);
        if (reallocation_status != NO_ERROR) {
            returned_index.status = reallocation_status;
            return returned_index;
        }
    }

    returned_index.index = array->count;
    array->array[array->count] = item;
    array->count++;

    return returned_index;
}

Status get_index_index_location_dynamic_array(IndexLocationDynamicArray *array, size_t index, uint32_t *output) {
    if (index >= array->count) {
        return INDEX_OUT_OF_BOUND;
    }

    *output = array->array[index];
    return NO_ERROR;
}

Status free_index_location_dynamic_array(IndexLocationDynamicArray *array) {
    if (array == NULL || array->array == NULL) {
        return NULL_POINTER;
    }

    free(array->array);
    array->array = NULL;
    array->count = 0;
    array->capacity = 0;
    return NO_ERROR;
}
