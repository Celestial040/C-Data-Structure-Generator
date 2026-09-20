#include "parser.h"
#include "bool.h"
#include "file_loader.h"
#include "status.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "struct_template_keyword.h"
#include "generate_file_keyword.h"
#include "char_manip.h"
#include "string_dynamic_array.h"

typedef enum Token {
    TOKEN_FIELDS,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_ASSIGNMENT,
    TOKEN_DOUBLE_QUOTE,
    TOKEN_OPEN_BRACKET,
    TOKEN_CLOSE_BRACKET,
    TOKEN_OPEN_CURLY_BRACKET,
    TOKEN_CLOSE_CURLY_BRACKET,
    TOKEN_STRING_LITERAL,
    TOKEN_UNKNOWN
}Token;

typedef enum GenerateFields {
    STATUS_HEADER,
    UTILS_DIR,
    HEADER_DIR,
    STRUCT_TEMPLATE
}GenerateFields;

typedef struct LookupStatus {
    Status status;
    GenerateFields fields_id;
}LookupStatus ;

typedef enum TypeMode {
    WHITESPACE,
    ALPHANUMERIC,
    SYMBOL,
    STRING_LITERAL
} TypeMode;

TypeMode type_check(const char target) {
    if (is_alphabet_numeric(target) || target == 95 ||target == 46 ||target == 47) {
        return ALPHANUMERIC;
    } else if (is_whitespace(target)) {
        return WHITESPACE;
    } else {
        return SYMBOL;
    }
}

Token check_token_enumerator(const char target) {
    switch (target) {
        case '=':
            return TOKEN_ASSIGNMENT;

        case ';':
            return TOKEN_SEMICOLON;

        case '"':
            return TOKEN_DOUBLE_QUOTE;

        case '(':
            return TOKEN_OPEN_BRACKET;

        case ')':
            return TOKEN_CLOSE_BRACKET;

        case '{':
            return TOKEN_OPEN_CURLY_BRACKET;

        case '}':
            return TOKEN_CLOSE_CURLY_BRACKET;

        case ',':
            return TOKEN_COMMA;

        default:
            return TOKEN_UNKNOWN;
    }
}


static size_t head,tail = 0;
static TypeMode head_mode, tail_mode = WHITESPACE;
static FileString *file_string = NULL;
static size_t line_scanned = 1;
static LookupStatus lookup_status = {NO_ERROR, 5};


static StringDynamicArray *string_dynamic_array;
static StringDynamicArrayPointer status_header;
static StringDynamicArrayPointer input_file_name;
static StringDynamicArrayPointer targets_dir[4];
static const char *target_dir_fields_name[4] = {"status_header", "utils_dir", "header_dir", "struct_template"};


void set_string_dynamic_array(StringDynamicArray *loaded_string_dynamic_array) {
    string_dynamic_array = loaded_string_dynamic_array;
}

void set_file_string(FileString *loaded_file_string) {
    file_string = loaded_file_string;
}

void set_input_filename(StringDynamicArrayPointer *pointer) {
    memcpy(&input_file_name, pointer, sizeof(StringDynamicArrayPointer));
}

void reset_tail_mode() {
    tail = head;
    tail_mode = type_check(file_string->start[tail]);
    head++;
}


Token lexer_linear_scan() {
    const struct KeyMapping *match;
    Token token_scanned = TOKEN_UNKNOWN;

    while (head < file_string->length) {
        head_mode = type_check(file_string->start[head]);

        if (file_string->start[head] == '\n') {
            line_scanned++;
        }

        if (tail_mode == STRING_LITERAL && file_string->start[head] != '"' && file_string->start[head] != ';' &&file_string->start[head] != '\n') {
            head_mode = STRING_LITERAL;
        }

        if (head_mode != tail_mode) {

            switch (tail_mode) {
                case WHITESPACE:
                    reset_tail_mode();
                    continue;

                case ALPHANUMERIC:
                    match = lookup_key(file_string->start + tail, head-tail);
                    if (match == NULL) {
                        lookup_status.status = FIELD_MATCH_NOT_FOUND;
                        return TOKEN_UNKNOWN;
                    }
                    lookup_status.fields_id = match->id;
                    return TOKEN_FIELDS;

                case SYMBOL:
                    return check_token_enumerator(file_string->start[tail]);


                case STRING_LITERAL:
                    if (file_string->start[head] == ';' ||file_string->start[head] == '\n') {
                        return TOKEN_UNKNOWN;
                    }
                    if (file_string->start[head] == '"') {
                        return TOKEN_STRING_LITERAL;
                    }
                    continue;
            }
        }

        head++;
    }
}

void get_one_line_error_print() {
    while (head < file_string->length) {
        if (file_string->start[head] == '\n') {
            break;
        }
        head++;
    }
    while (tail > 0) {
        if (file_string->start[tail] == '\n') {
            break;
        }
        tail--;
    }

    if (tail > 0) {
        tail++;
    };
    printf("%.*s (%s:%ld)\n",head-tail,file_string->start+tail, string_dynamic_array->start_pointer + input_file_name.string_start, line_scanned);
}


Status parser_start() {
    Token current_token = TOKEN_UNKNOWN;
    size_t path_pointer, path_length;
    uint8_t i;
    Status file_opening_status;

    for (i = 0; i<4; i++) {
        current_token = lexer_linear_scan();

        if (current_token != TOKEN_FIELDS) {
            printf("Field {%.*s} match up not found at:\n", head-tail,file_string->start+tail);
            get_one_line_error_print();
            return lookup_status.status;
        }


        if (lookup_status.fields_id != (GenerateFields) i) {
            printf("Wrong fields order {%.*s} at:\n",  head-tail,file_string->start+tail);
            get_one_line_error_print();
            printf("\nThe order should be:\n status_header, utils_dir, header_dir, then struct_template");
            return MISMATCH_EXPECTATION;
        }

        reset_tail_mode();
        current_token = lexer_linear_scan();


        if (current_token != TOKEN_ASSIGNMENT) {
            printf("Missing assignment at:\n");
            get_one_line_error_print();
            printf("\nIt should be: \n %s = \"{file_path}\"; \n",target_dir_fields_name[i]);
            return MISMATCH_EXPECTATION;
        }

        reset_tail_mode();
        current_token = lexer_linear_scan();

        if (current_token != TOKEN_DOUBLE_QUOTE) {
            printf("Missing double quote at:\n");
            get_one_line_error_print();
            printf("\nIt should be: \n %s = \"{file_path}\"; \n",target_dir_fields_name[i]);
            return MISMATCH_EXPECTATION;
        }

        reset_tail_mode();
        tail_mode = STRING_LITERAL;
        current_token = lexer_linear_scan();

        path_pointer = tail;
        path_length = head-tail;

        if (current_token != TOKEN_STRING_LITERAL) {
            printf("Expected string literal at:\n");
            get_one_line_error_print();
            printf("\nIt should be: \n %s = \"{file_path}\"; \n",target_dir_fields_name[i]);
            return MISMATCH_EXPECTATION;
        }

        reset_tail_mode();
        reset_tail_mode();
        current_token = lexer_linear_scan();

        if (current_token != TOKEN_SEMICOLON) {
            printf("Missing semicolon at:\n");
            get_one_line_error_print();
            printf("\nIt should be: \n %s = \"{file_path}\"; \n",target_dir_fields_name[i]);
            return MISMATCH_EXPECTATION;
        }

        targets_dir[i] = insert_string(string_dynamic_array, file_string->start + path_pointer, path_length);

        if (targets_dir[i].status != NO_ERROR) {
            return targets_dir[i].status;
        }

        reset_tail_mode();
    }

    file_opening_status = read_file(string_dynamic_array->start_pointer + targets_dir[3].string_start, file_string);
    if (file_opening_status != NO_ERROR) {
        printf("Error opening %s due to:\n", target_dir_fields_name[3]);
        return file_opening_status;
    }




    return NO_ERROR;
}
