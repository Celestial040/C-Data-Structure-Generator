#ifndef TEMPLATE_DYNAMIC_ARRAY_H
#define TEMPLATE_DYNAMIC_ARRAY_H

#include "status.h"
#include <stddef.h>
typedef char Template;

typedef struct TemplateDynamicArray {
    Template *array;
    size_t count;
    size_t capacity;
} TemplateDynamicArray;

typedef struct TemplateDynamicArrayPointer {
    Status status;
    size_t index;
} TemplateDynamicArrayPointer;

#endif
