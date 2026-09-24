#ifndef TEMPLATE_DYNAMIC_ARRAY_H
#define TEMPLATE_DYNAMIC_ARRAY_H

#include "../status.h"
#include <stddef.h>

typedef struct IndexLocation {
    uint32_t start;
    uint32_t end;
} IndexLocation;

typedef struct IndexLocationDynamicArray {
    IndexLocation *array;
    size_t count;
    size_t capacity;
} IndexLocationDynamicArray;

typedef struct IndexLocationDynamicArrayPointer {
    Status status;
    size_t index;
} IndexLocationDynamicArrayPointer;

Status allocate_index_location_dynamic_array(IndexLocationDynamicArray *array, size_t size_requested);
Status reallocate_index_location_dynamic_array(IndexLocationDynamicArray *array, size_t size_requested);
Status insert_item_index_location_dynamic_array(IndexLocationDynamicArray *array, IndexLocation item);
IndexLocationDynamicArrayPointer insert_item_return_id_index_location_dynamic_array(IndexLocationDynamicArray *array, IndexLocation item);
Status get_index_index_location_dynamic_array(IndexLocationDynamicArray *array, size_t index, IndexLocation *output);
Status free_index_location_dynamic_array(IndexLocationDynamicArray *array);


#endif
