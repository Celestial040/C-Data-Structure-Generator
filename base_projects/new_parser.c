#include "bool.h"
#include "char_manip.h"
#include "file_loader.h"
#include "new_lexer.h"
#include "status.h"
#include "string_dynamic_array.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

Status new_parser_start(const char *input_file_path) {
    Status status;

    StringDynamicArray string_dynamic_array;
    FileString input_file;

    TokenPointer received_token;
    size_t line_count = 1;


    status = load_file_to_memory(input_file_path, &input_file);
    if (status !=  NO_ERROR) { return status; }

    status = allocate_string_dynamic_array(&string_dynamic_array, 1024);
    if (status !=  NO_ERROR) { return status; }

    received_token = lexer_scan(&input_file, &line_count);

    printf("token : %d\n", received_token.token);

    if (received_token.token != TOKEN_IDENTIFIER) {

        printf("Identifier expected at: \n");
        printf("(%s:%ld) \n", input_file_path, line_count);

    }




    return status;
}
