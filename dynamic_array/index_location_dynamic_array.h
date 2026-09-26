#ifndef TEMPLATE_DYNAMIC_ARRAY_H
#define TEMPLATE_DYNAMIC_ARRAY_H

#include "../status.h"
#include <stddef.h>

typedef struct IndexLocationDynamicArray {
    uint32_t *array;
    size_t count;
    size_t capacity;
} IndexLocationDynamicArray;

typedef struct IndexLocationDynamicArrayPointer {
    Status status;
    size_t index;
} IndexLocationDynamicArrayPointer;

Status allocate_index_location_dynamic_array(IndexLocationDynamicArray *array, size_t size_requested);
Status reallocate_index_location_dynamic_array(IndexLocationDynamicArray *array, size_t size_requested);
Status insert_item_index_location_dynamic_array(IndexLocationDynamicArray *array, uint32_t item);
IndexLocationDynamicArrayPointer insert_item_return_id_index_location_dynamic_array(IndexLocationDynamicArray *array, uint32_t item);
Status get_index_index_location_dynamic_array(IndexLocationDynamicArray *array, size_t index, uint32_t *output);
Status free_index_location_dynamic_array(IndexLocationDynamicArray *array);


#endif
