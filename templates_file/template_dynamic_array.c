#include "status.h"
#include <stddef.h>
#include <stdlib.h>
#include "template_dynamic_array.h"

Status allocate_template_dynamic_array(TemplateDynamicArray *array, size_t size_requested) {
    Template *temp_array = (Template *) malloc(sizeof(Template) *  size_requested);
    if (temp_array == NULL) {
        return ALLOCATION_ERROR;
    }

    array->capacity = size_requested;
    array->count = 0;
    array->array = temp_array;

    return NO_ERROR;
}

Status reallocate_template_dynamic_array(TemplateDynamicArray *array, size_t size_requested) {
    Template *temp_array = (Template *) realloc(array->array,sizeof(Template) *  size_requested);
    if (temp_array == NULL) {
        return ALLOCATION_ERROR;
    }

    array->capacity = size_requested;
    array->array = temp_array;

    return NO_ERROR;
}

Status insert_item_template_dynamic_array(TemplateDynamicArray *array, Template item) {
    if (array->count + 1 >= array->capacity) {
        Status reallocation_status = reallocate_template_dynamic_array(array, array->capacity * 2);
        if (reallocation_status != NO_ERROR) {
            return reallocation_status;
        }
    }

    array->array[array->count] = item;
    array->count++;

    return NO_ERROR;
}

TemplateDynamicArrayPointer insert_item_return_id_template_dynamic_array(TemplateDynamicArray *array, Template item) {
    TemplateDynamicArrayPointer returned_index = {NO_ERROR , 0};
    if (array->count + 1 >= array->capacity) {
        Status reallocation_status = reallocate_template_dynamic_array(array, array->capacity * 2);
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

Status get_index_template_dynamic_array(TemplateDynamicArray *array, size_t index, Template *output) {
    if (index >= array->count) {
        return INDEX_OUT_OF_BOUND;
    }

    *output = array->array[index];
    return NO_ERROR;
}

Status free_template_dynamic_array(TemplateDynamicArray *array) {
    if (array == NULL || array->array == NULL) {
        return NULL_POINTER;
    }

    free(array->array);
    array->array = NULL;
    array->count = 0;
    array->capacity = 0;
}
