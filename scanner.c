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


Status start_scanning_file(const char *filename) {
    Status operation_status;

    FileString file_string;
    StringDynamicArray string_dynamic_array;
    IndexLocationDynamicArray index_location_dynamic_array;

    const char *target_word = "awas";
    uint32_t target_word_length = strlen(target_word);

    const char *replacement_word = "aduh";
    uint32_t replacement_word_length = strlen(replacement_word);

    uint32_t start = 0;
    uint32_t end = 0;
    uint32_t source_index = 0;
    uint32_t dest_index = 0;
    uint32_t index_location_count = 0;
    uint32_t step_forward = 0;

    operation_status = load_file_to_memory(filename, &file_string);
    if (operation_status != NO_ERROR) {
        return operation_status;
    }

    operation_status = allocate_index_location_dynamic_array(&index_location_dynamic_array, 1024);
    if (operation_status != NO_ERROR) {
        return operation_status;
    }

    linear_string_matching(&file_string, &index_location_dynamic_array, target_word, target_word_length);

    operation_status = allocate_string_dynamic_array(&string_dynamic_array, 1024 + replacement_word_length * index_location_dynamic_array.count);
    if (operation_status != NO_ERROR) {
        return operation_status;
    }

    start = 0;
    end = index_location_dynamic_array.array[index_location_count].start;

    step_forward = end-start;

    memcpy(string_dynamic_array.start_pointer+dest_index, file_string.start+source_index, step_forward);

    dest_index += step_forward;
    source_index += step_forward;

    string_dynamic_array.used += step_forward;

    memcpy(string_dynamic_array.start_pointer+dest_index, replacement_word, replacement_word_length);

    dest_index += replacement_word_length;
    source_index += index_location_dynamic_array.array[index_location_count].end - index_location_dynamic_array.array[index_location_count].start + 1;
    string_dynamic_array.used += replacement_word_length;

    index_location_count++;


    for (; index_location_count < 3; index_location_count++) {
        start = index_location_dynamic_array.array[index_location_count-1].end;
        end = index_location_dynamic_array.array[index_location_count].start;

        step_forward = end-start-1;

        memcpy(string_dynamic_array.start_pointer+dest_index, file_string.start+source_index, step_forward);

        dest_index += step_forward;
        source_index += step_forward;
        string_dynamic_array.used += step_forward;

        memcpy(string_dynamic_array.start_pointer+dest_index, replacement_word, replacement_word_length);

        dest_index += replacement_word_length;
        source_index += index_location_dynamic_array.array[index_location_count].end - index_location_dynamic_array.array[index_location_count].start + 1;
        string_dynamic_array.used += replacement_word_length;
    }

    step_forward = file_string.length - 1 - index_location_dynamic_array.array[index_location_count].end;

    memcpy(string_dynamic_array.start_pointer+dest_index, file_string.start+source_index, step_forward);
    string_dynamic_array.used += step_forward;

    string_dynamic_array.start_pointer[string_dynamic_array.used] = '\0';
    string_dynamic_array.used++;

    printf("%s\n",string_dynamic_array.start_pointer);

    return NO_ERROR;
}
