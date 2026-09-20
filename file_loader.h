#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "status.h"
#include <stddef.h>

typedef struct FileString {
    char *start;
    size_t length;
} FileString;

Status read_file(const char *filename, FileString *output);

#endif
