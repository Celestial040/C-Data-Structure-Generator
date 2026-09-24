#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "status.h"
#include <stdint.h>

typedef struct FileString {
    char *start;
    uint32_t length;
} FileString;

Status load_file_to_memory(const char *filename, FileString *output);

#endif
