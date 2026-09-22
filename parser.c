#include "parser.h"
#include "codegen.h"
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
    TOKEN_EOF,
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
    STRING_LITERAL,
    NUMERIC_LITERAL
} TypeMode;

typedef struct GeneratedFileNaming {
    size_t struct_name_start;
    size_t struct_name_length;
}GeneratedFileNaming;

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
static FileString *input_file_string;
static FileString struct_template_string;
static size_t line_scanned = 1;
static LookupStatus lookup_status = {NO_ERROR, 5};


static StringDynamicArray *string_dynamic_array;
static StringDynamicArrayPointer status_header;
static StringDynamicArrayPointer input_file_name;
static StringDynamicArrayPointer struct_template_file_name;
static StringDynamicArrayPointer targets_dir[4];
static const char *target_dir_fields_name[4] = {"status_header", "utils_dir", "header_dir", "struct_template"};


void set_string_dynamic_array(StringDynamicArray *loaded_string_dynamic_array) {
    string_dynamic_array = loaded_string_dynamic_array;
}

void set_file_string(FileString *loaded_file_string) {
    input_file_string = loaded_file_string;
}

void set_input_filename(StringDynamicArrayPointer *pointer) {
    memcpy(&input_file_name, pointer, sizeof(StringDynamicArrayPointer));
}

void reset_tail_mode(FileString *file_string) {
    tail = head;
    tail_mode = type_check(file_string->start[tail]);
    head++;
}


Token lexer_scan_generate_file() {
    const struct GenKeyMapping *match;
    Token token_scanned = TOKEN_UNKNOWN;

    while (head < input_file_string->length) {
        head_mode = type_check(input_file_string->start[head]);

        if (input_file_string->start[head] == '\n') {
            line_scanned++;
        }

        if (tail_mode == STRING_LITERAL && head_mode == ALPHANUMERIC) {
            head_mode = STRING_LITERAL;
        }

        if (head_mode != tail_mode) {

            switch (tail_mode) {
                case WHITESPACE:
                    reset_tail_mode(input_file_string);
                    continue;

                case ALPHANUMERIC:
                    match = generate_file_keyword(input_file_string->start + tail, head-tail);
                    if (match == NULL) {
                        lookup_status.status = FIELD_MATCH_NOT_FOUND;
                        return TOKEN_UNKNOWN;
                    }
                    lookup_status.fields_id = match->id;
                    return TOKEN_FIELDS;

                case SYMBOL:
                    return check_token_enumerator(input_file_string->start[tail]);


                case STRING_LITERAL:
                    if (input_file_string->start[head] == '\n') {
                        return TOKEN_UNKNOWN;
                    }
                    if (input_file_string->start[head] == '"') {
                        return TOKEN_STRING_LITERAL;
                    }
                    continue;

                case NUMERIC_LITERAL:
                    break;
            }
        }

        head++;
    }

    return TOKEN_EOF;
}

Token lexer_scan_struct_template() {
    const struct TemplateKeyMapping *match;
    Token token_scanned = TOKEN_UNKNOWN;


    while (head < struct_template_string.length) {
        head_mode = type_check(struct_template_string.start[head]);

        if (struct_template_string.start[head] == '\n') {
            line_scanned++;
        }

        if (tail_mode == STRING_LITERAL && head_mode == ALPHANUMERIC) {
            head_mode = STRING_LITERAL;
        }

        if (head_mode != tail_mode) {
            switch (tail_mode) {
                case WHITESPACE:
                    reset_tail_mode(&struct_template_string);
                    continue;

                case ALPHANUMERIC:
                    match = struct_template_keyword(struct_template_string.start + tail, head-tail);
                    if (match == NULL) {
                        lookup_status.status = FIELD_MATCH_NOT_FOUND;
                        return TOKEN_UNKNOWN;
                    }

                    lookup_status.fields_id = match->id;
                    return TOKEN_FIELDS;

                case SYMBOL:
                    return check_token_enumerator(struct_template_string.start[tail]);

                case STRING_LITERAL:
                    if (struct_template_string.start[head] == '\n') {
                        return TOKEN_UNKNOWN;
                    }
                    if (struct_template_string.start[head] == '"') {
                        return TOKEN_STRING_LITERAL;
                    }
                    continue;

                case NUMERIC_LITERAL:
                    break;
            }
        }

        head++;
    }
    if (head == struct_template_string.length) {
        token_scanned = TOKEN_EOF;
    }

    return token_scanned;
}

void get_one_line_error_print(FileString *file_string, StringDynamicArrayPointer file_name) {
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
    printf("%.*s (%s:%ld)\n",head-tail,file_string->start+tail, string_dynamic_array->start_pointer + file_name.string_start, line_scanned);
}

Status parse_generate_file() {
    Token current_token = TOKEN_UNKNOWN;
    size_t path_pointer, path_length;
    size_t i;
    Status file_opening_status;

    for (i = 0; i<4; i++) {
        current_token = lexer_scan_generate_file();

        if (current_token != TOKEN_FIELDS) {
            printf("Field {%.*s} match up not found at:\n", head-tail,input_file_string->start+tail);
            get_one_line_error_print(input_file_string,input_file_name);
            return lookup_status.status;
        }


        if (lookup_status.fields_id != i) {
            printf("Wrong fields order {%.*s} at:\n",  head-tail,input_file_string->start+tail);
            get_one_line_error_print(input_file_string,input_file_name);
            printf("\nThe order should be:\n status_header, utils_dir, header_dir, then struct_template");
            return MISMATCH_EXPECTATION;
        }

        reset_tail_mode(input_file_string);
        current_token = lexer_scan_generate_file();

        if (current_token != TOKEN_ASSIGNMENT) {
            printf("Missing assignment at:\n");
            get_one_line_error_print(input_file_string,input_file_name);
            printf("\nIt should be: \n %s = \"{file_path}\" \n",target_dir_fields_name[i]);
            return MISMATCH_EXPECTATION;
        }

        reset_tail_mode(input_file_string);
        current_token = lexer_scan_generate_file();

        if (current_token != TOKEN_DOUBLE_QUOTE) {
            printf("Missing double quote at:\n");
            get_one_line_error_print(input_file_string,input_file_name);
            printf("\nIt should be: \n %s = \"{file_path}\" \n",target_dir_fields_name[i]);
            return MISMATCH_EXPECTATION;
        }

        reset_tail_mode(input_file_string);
        tail_mode = STRING_LITERAL;
        current_token = lexer_scan_generate_file();

        path_pointer = tail;
        path_length = head-tail;

        if (current_token != TOKEN_STRING_LITERAL) {
            printf("Expected string literal at:\n");
            get_one_line_error_print(input_file_string,input_file_name);
            printf("\nIt should be: \n %s = \"{file_path}\" \n",target_dir_fields_name[i]);
            return MISMATCH_EXPECTATION;
        }

        targets_dir[i] = insert_string(string_dynamic_array, input_file_string->start + path_pointer, path_length);

        if (targets_dir[i].status != NO_ERROR) {
            return targets_dir[i].status;
        }

        reset_tail_mode(input_file_string);
        reset_tail_mode(input_file_string);
    }

    file_opening_status = read_file(string_dynamic_array->start_pointer + targets_dir[3].string_start, &struct_template_string);
    if (file_opening_status != NO_ERROR) {
        printf("Error opening %s due to:\n", target_dir_fields_name[3]);
        return file_opening_status;
    }

    for (i = targets_dir[3].string_length; i > 0; i--) {
        if (string_dynamic_array->start_pointer[targets_dir[3].string_start + i-1] == '/' ) {
            struct_template_file_name.string_start = targets_dir[3].string_start + i;
            struct_template_file_name.string_length = targets_dir[3].string_length - i;
            break;
        }
    }

    /* printf("%.*s \n",(int)struct_template_file_name.string_length,string_dynamic_array->start_pointer+struct_template_file_name.string_start); */

    return NO_ERROR;
}

Status parse_struct_template() {
    Token current_token = TOKEN_UNKNOWN;
    size_t path_pointer, path_length;
    uint8_t i;

    head = 0;
    tail = 0;
    head_mode = WHITESPACE;
    tail_mode = WHITESPACE;
    line_scanned = 1;
    lookup_status.status = NO_ERROR;
    lookup_status.fields_id = 5;

    current_token = lexer_scan_struct_template();

    if (current_token != TOKEN_OPEN_CURLY_BRACKET) {
        printf("Expected open curly bracket at:\n");
        get_one_line_error_print(&struct_template_string,struct_template_file_name);
        return MISMATCH_EXPECTATION;
    }

    reset_tail_mode(&struct_template_string);
    current_token = lexer_scan_struct_template();

    if (current_token != TOKEN_FIELDS) {
        printf("Field {%.*s} match up not found at:\n", head-tail,input_file_string->start+tail);
        get_one_line_error_print(input_file_string,input_file_name);
        return lookup_status.status;
    }

    if (lookup_status.fields_id != 0) {
        printf("Wrong fields order {%.*s} at:\n",  head-tail,input_file_string->start+tail);
        get_one_line_error_print(input_file_string,input_file_name);
        printf("\nThe order should be:\n status_header, utils_dir, header_dir, then struct_template");
        return MISMATCH_EXPECTATION;
    }

    reset_tail_mode(&struct_template_string);
    current_token = lexer_scan_struct_template();


    if (current_token != TOKEN_ASSIGNMENT) {
        printf("Missing assignment at:\n");
        get_one_line_error_print(input_file_string,input_file_name);
        printf("\nIt should be: \n %s = \"{file_path}\" \n",target_dir_fields_name[i]);
        return MISMATCH_EXPECTATION;
    }



    return NO_ERROR;
}

Status parser_start() {
    Status parse_result;

    parse_result = parse_generate_file();



    parse_result = parse_struct_template();

    return parse_result;
}
