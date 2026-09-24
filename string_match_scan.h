#ifndef STRING_MATCH_SCAN_H
#define STRING_MATCH_SCAN_H

#include "dynamic_array/index_location_dynamic_array.h"
#include "file_loader.h"
#include "status.h"
#include <stdint.h>

Status linear_string_matching(FileString *file_string, IndexLocationDynamicArray *index_store, const char *target_match, const uint32_t target_length);
#endif
