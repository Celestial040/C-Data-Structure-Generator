#include "file_loader.h"
#include "status.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "lookup.h"
#include "char_manip.h"
#include "string_dynamic_array.h"


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
    if (target == 34) {
        return STRING_LITERAL;
    }
    else if (is_alphabet_numeric(target) || target == 95) {
        return ALPHANUMERIC;
    } else if (is_whitespace(target)) {
        return WHITESPACE;
    } else {
        return SYMBOL;
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



void lexer_linear_scan() {
    const struct KeyMapping *match;

    while (head < file_string->length) {
        head_mode = type_check(file_string->start[head]);

        if (file_string->start[head] == '\n') {
            line_scanned++;
        }

        if (head_mode != tail_mode) {

            switch (tail_mode) {
                case WHITESPACE:
                    tail = head;
                    tail_mode = type_check(file_string->start[tail]);
                    head++;
                    continue;

                case ALPHANUMERIC:
                    match = lookup_key(file_string->start + tail, head-tail);
                    if (match == NULL) {
                        lookup_status.status = FIELD_MATCH_NOT_FOUND;
                        printf("Field {%.*s} match up not found at ", head-tail,file_string->start+tail);
                        tail = head;
                        tail_mode = type_check(file_string->start[tail]);
                        head++;
                        return;
                    }
                    lookup_status.fields_id = match->id;
                    tail = head;
                    tail_mode = type_check(file_string->start[tail]);
                    head++;
                    break;;

                case SYMBOL:
                    break;

                case STRING_LITERAL:
                    break;

            }
        }

        head++;
    }
}

void reset_tail_mode() {
    tail = head;
    tail_mode = type_check(file_string->start[tail]);
    head++;
}

Status move_head_forward_until(const char target) {
    while (head < file_string->length) {
        if (file_string->start[head] == target) {
            return ITEM_FOUND;
        }
        head++;
    }
    return ITEM_FOUND;
}

Status move_tail_backward_until(const char target) {
    while (tail > 0) {
        if (file_string->start[tail] == target) {
            return ITEM_FOUND;
        }
        tail--;
    }
    return ITEM_FOUND;
}

void get_one_line(){
    move_head_forward_until('\n');
    move_tail_backward_until('\n');
    if (tail > 0) {
        tail++;
    };
}


void no_match_error_print() {
    get_one_line();
    printf("%.*s (%s:%ld)\n",head-tail,file_string->start+tail, string_dynamic_array->start_pointer + input_file_name.string_start, line_scanned);
}

Status parser_start() {

    lexer_linear_scan();
    if (lookup_status.status == FIELD_MATCH_NOT_FOUND) {
        no_match_error_print();
        return lookup_status.status;
    }

    return NO_ERROR;
}
