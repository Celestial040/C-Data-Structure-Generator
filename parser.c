#include "parser.h"
#include "bool.h"
#include "file_loader.h"
#include "status.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "lookup.h"
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
static StringDynamicArrayPointer utils_dir;
static StringDynamicArrayPointer header_dir;
static StringDynamicArrayPointer struct_template;
static StringDynamicArrayPointer input_file_name;


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




void get_one_line(){
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
}


void get_one_line_error_print() {
    get_one_line();
    printf("%.*s (%s:%ld)\n",head-tail,file_string->start+tail, string_dynamic_array->start_pointer + input_file_name.string_start, line_scanned);
}

Status parser_start() {
    size_t temp_head = 0;
    Status scan_status = ITEM_FOUND;
    Token current_token = TOKEN_UNKNOWN;

    current_token = lexer_linear_scan();

    if (current_token != TOKEN_FIELDS) {
        printf("Field {%.*s} match up not found at:\n", head-tail,file_string->start+tail);
        get_one_line_error_print();
        return lookup_status.status;
    }

    if (lookup_status.fields_id != STATUS_HEADER) {
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
        printf("\nIt should be: \n status_header = \"{file_path}\"; \n");
        return MISMATCH_EXPECTATION;
    }

    reset_tail_mode();
    current_token = lexer_linear_scan();

    if (current_token != TOKEN_DOUBLE_QUOTE) {
        printf("Missing double quote at:\n");
        get_one_line_error_print();
        printf("\nIt should be: \n status_header = \"{file_path}\"; \n");
        return MISMATCH_EXPECTATION;
    }

    reset_tail_mode();
    tail_mode = STRING_LITERAL;
    current_token = lexer_linear_scan();

    if (current_token != TOKEN_STRING_LITERAL) {
        printf("Expected string literal at:\n");
        get_one_line_error_print();
        printf("\nIt should be: \n status_header = \"{file_path}\"; \n");
        return MISMATCH_EXPECTATION;
    }

    reset_tail_mode();
    current_token = lexer_linear_scan();

    if (current_token != TOKEN_SEMICOLON) {
        printf("Missing semicolon at:\n");
        get_one_line_error_print();
        printf("\nIt should be: \n status_header = \"{file_path}\"; \n");
        return MISMATCH_EXPECTATION;
    }



    return NO_ERROR;
}

Status parser_start_old() {
    size_t temp_head = 0;
    Status scan_status = ITEM_FOUND;

    lexer_linear_scan();

    if (lookup_status.status == FIELD_MATCH_NOT_FOUND) {
        printf("Field {%.*s} match up not found at:\n", head-tail,file_string->start+tail);
        get_one_line_error_print();
        return lookup_status.status;
    }

    if (lookup_status.fields_id != STATUS_HEADER) {
        printf("Wrong fields order {%.*s} at:\n",  head-tail,file_string->start+tail);
        get_one_line_error_print();
        printf("\nThe order should be:\n status_header, utils_dir, header_dir, then struct_template");
        return MISMATCH_EXPECTATION;
    }

    reset_tail_mode();

    temp_head = head;


    while (head < file_string->length) {
        if (file_string->start[head] == '=') {
            scan_status = ITEM_FOUND;
            break;
        }
        if (type_check(file_string->start[head]) == ALPHANUMERIC ) {
            scan_status = ITEM_NOT_FOUND;
            break;
        }
        head++;
    }

    if (scan_status == ITEM_NOT_FOUND) {
        head = temp_head;
        printf("Missing assignment at:\n");
        get_one_line_error_print();
        printf("\nIt should be: \n status_header = \"{file_path}\"; \n");
    }

    reset_tail_mode();

    temp_head = head;


    while (head < file_string->length) {
        if (file_string->start[head] == '"') {
            scan_status = ITEM_FOUND;
            break;
        }
        if (type_check(file_string->start[head]) == ALPHANUMERIC ) {
            scan_status = ITEM_NOT_FOUND;
            break;
        }
        head++;
    }

    if (scan_status == ITEM_NOT_FOUND) {
        head = temp_head;
        printf("Missing double quote at:\n");
        get_one_line_error_print();
        printf("\nIt should be: \n status_header = \"{file_path}\"; \n");
    }

    reset_tail_mode();
    head++;

    while (head < file_string->length) {
        if (file_string->start[head] == '"') {
            scan_status = ITEM_FOUND;
            break;
        }
        if (type_check(file_string->start[head]) != ALPHANUMERIC || file_string->start[head] == '\n' || file_string->start[head] == ';') {
            scan_status = ITEM_NOT_FOUND;
            break;
        }
        head++;
    }

    while (head > tail) {
        if (file_string->start[head] == '"') {
            scan_status = ITEM_FOUND;
            break;
        }
        head--;
    }

    if (scan_status == ITEM_NOT_FOUND) {
        head = temp_head;
        printf("Missing double quote at:\n");
        get_one_line_error_print();
        printf("\nIt should be: \n status_header = \"{file_path}\"; \n");
    }

    temp_head = head;

    while (head < file_string->length) {
        if (file_string->start[head] == ';') {
            scan_status = ITEM_FOUND;
            break;
        }
        if (type_check(file_string->start[head]) == ALPHANUMERIC || file_string->start[head] == '\n') {
            scan_status = ITEM_NOT_FOUND;
            break;
        }
        head++;
    }

    if (scan_status == ITEM_NOT_FOUND) {
        head = temp_head;
        printf("Missing semicolon at the end\n");
        get_one_line_error_print();
        printf("\nIt should be: \n status_header = \"{file_path}\"; \n");
    }

    header_dir = insert_string(string_dynamic_array, file_string->start + tail, head-tail);
    printf("%s \n", string_dynamic_array->start_pointer + header_dir.string_start);


    return NO_ERROR;
}
