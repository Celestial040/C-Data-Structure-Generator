#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "file_loader.h"
#include "parser.h"
#include "status.h"
#include "string_dynamic_array.h"

int main(int argc, char *argv[]) {
    FileString file_buffer;
    StringDynamicArray string_dynamic_array;
    Status file_lookup_status;
    Status parser_status;
    Status allocation_status;
    StringDynamicArrayPointer input_filename_pointer;


    if (argc != 2) {
        printf("Missing arguments for target file \nprogram-name {target-file} \n \nfor example: \n./program generate.txt\n");
        return 1;
    }


    file_lookup_status = read_file(argv[1], &file_buffer);
    if (file_lookup_status != NO_ERROR) {
        status_print(file_lookup_status);
        return 1;
    }

    allocation_status = allocate_string_dynamic_array(&string_dynamic_array, 1024);
    if (allocation_status != NO_ERROR) {
        status_print(allocation_status);
        return 1;
    }
    input_filename_pointer = insert_string(&string_dynamic_array,argv[1],strlen(argv[1]));
    if (input_filename_pointer.status != NO_ERROR) {
        status_print(input_filename_pointer.status);
        return 1;
    }

    set_string_dynamic_array(&string_dynamic_array);
    set_input_filename(&input_filename_pointer);
    set_file_string(&file_buffer);

    parser_status = parser_start();
    if (parser_status != NO_ERROR) {
        status_print(parser_status);
        return 1;
    }

    return 0;
}
