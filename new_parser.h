#ifndef NEW_PARSER_H
#define NEW_PARSER_H

#include "status.h"
#include <stddef.h>

typedef struct GeneratedFileNaming {
    size_t struct_name_start;
    size_t struct_name_length;
}GeneratedFileNaming;

Status new_parser_start(const char *filename);

#endif
