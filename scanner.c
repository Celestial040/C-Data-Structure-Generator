#include "scanner.h"
#include "base_projects/status.h"
#include "dynamic_array/index_location_dynamic_array.h"
#include "dynamic_array/string_dynamic_array.h"
#include "file_loader.h"
#include "string_match_scan.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void copy_and_replace(FileString *string_src, StringDynamicArray *string_dest, IndexLocationDynamicArray *index_storage,
                      const size_t target_word_length, const char *replacement_word, const size_t replacement_word_length) {

    uint32_t index_location_count = 0;
    uint32_t source_index = 0;
    uint32_t dest_index = 0;
    uint32_t step_forward = 0;

    for (index_location_count = 0; index_location_count < index_storage->count; index_location_count++) {
        step_forward = index_storage->array[index_location_count] - source_index;
        memcpy(string_dest->start_pointer+dest_index, string_src->start+source_index, step_forward);

        dest_index += step_forward;
        source_index += step_forward;

        memcpy(string_dest->start_pointer+dest_index, replacement_word, replacement_word_length);

        dest_index += replacement_word_length;
        source_index += target_word_length;
        string_dest->used += replacement_word_length;
    }
}

Status start_scanning_file(const char *filename) {
    Status operation_status;

    const char *target_word = "awas";
    const char *replacement_word = "aduh";

    uint32_t target_word_length = strlen(target_word);
    uint32_t replacement_word_length = strlen(replacement_word);

    FileString file_string;
    StringDynamicArray string_dynamic_array;
    IndexLocationDynamicArray index_location_dynamic_array;

    operation_status = load_file_to_memory(filename, &file_string);
    if (operation_status != NO_ERROR) { return operation_status; }

    operation_status = allocate_index_location_dynamic_array(&index_location_dynamic_array, 1024);
    if (operation_status != NO_ERROR) { return operation_status; }

    linear_string_matching(&file_string, &index_location_dynamic_array, target_word, target_word_length);

    operation_status = allocate_string_dynamic_array(&string_dynamic_array, file_string.length + replacement_word_length * index_location_dynamic_array.count);
    if (operation_status != NO_ERROR) { return operation_status; }

    copy_and_replace(&file_string, &string_dynamic_array, &index_location_dynamic_array,
                     target_word_length, replacement_word, replacement_word_length);

    printf("%s\n",string_dynamic_array.start_pointer);


    return NO_ERROR;
}
