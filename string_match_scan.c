#include "base_projects/status.h"
#include "dynamic_array/index_location_dynamic_array.h"
#include "file_loader.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
typedef uint8_t ScanState;
enum {
    SCANNER_TEXT,
    SCANNER_MATCH
};

Status linear_string_matching(FileString *file_string, IndexLocationDynamicArray *index_store, const char *target_match, const uint32_t target_length) {

    uint32_t target_index = 0;
    uint32_t index = 0;

    uint32_t start = 0;

    ScanState state = SCANNER_TEXT;

    char current_item;
    char target_char;

    IndexLocation index_location = {0,0};

    Status insertion_status = NO_ERROR;

    for (; index < file_string->length; index++) {
        current_item = file_string->start[index];
        target_char = target_match[target_index];

        switch (state) {
            case SCANNER_TEXT:
                if (current_item == target_char) {
                    if (target_length == 1) {
                        index_location.start = index;
                        index_location.end = index;
                        insertion_status = insert_item_index_location_dynamic_array(index_store, index_location);
                        continue;
                    }
                   state = SCANNER_MATCH;
                   start = index;
                   target_index++;
                   continue;
                }
                continue;
            case SCANNER_MATCH:
                if (current_item != target_char) {
                    target_index = 0;
                    target_char = target_match[target_index];
                    if (current_item != target_char) {
                        state = SCANNER_TEXT;
                        continue;
                    }
                    start = index;
                    target_index++;
                    continue;
                }

                if (target_index == target_length-1) {
                    target_index = 0;
                    index_location.start = start;
                    index_location.end = index;
                    insertion_status = insert_item_index_location_dynamic_array(index_store, index_location);
                    state = SCANNER_TEXT;
                    continue;
                }

                target_index++;
                continue;

            default:
                continue;
        }


        if (insertion_status != NO_ERROR) {
            return insertion_status;
        }
    }


    return NO_ERROR;
}
