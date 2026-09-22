#include "new_lexer.h"
#include "char_manip.h"
#include "file_loader.h"
#include <stddef.h>
#include <stdio.h>

TypeMode type_check(const char target) {
    if (is_alphabet_numeric(target)) {
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

TokenPointer lexer_scan(FileString *file_string, size_t *line_count) {
    static size_t trail = 0;
    static TypeMode prev_mode, current_mode = WHITESPACE;
    static size_t index = 0;
    static TokenPointer tokenpointer = {0,0,TOKEN_UNKNOWN};

    while (index < file_string->length) {
        printf("%ld, %ld\n", trail, index);
        current_mode = type_check(file_string->start[index]);

        if (file_string->start[index] == '\n') { *line_count += 1; }
        if (prev_mode == ALPHANUMERIC && file_string->start[index] == '_') { current_mode = ALPHANUMERIC; }
        if (prev_mode == STRING_LITERAL && file_string->start[index] != '"' && file_string->start[index] != '\n') { current_mode = STRING_LITERAL; }
        if (prev_mode == NUMERIC_LITERAL && is_it_numeric_literal(file_string->start[index])) { current_mode = NUMERIC_LITERAL; }

        if (current_mode != prev_mode) {

            if (current_mode == ALPHANUMERIC && is_numeric(file_string->start[index])) {
                current_mode = NUMERIC_LITERAL;
            }

            switch (prev_mode) {

                case WHITESPACE:

                    if (file_string->start[index] == '"') {
                        current_mode = STRING_LITERAL;
                        index++;
                    }

                    trail = index;
                    prev_mode = current_mode;
                    index++;
                    continue;

                case ALPHANUMERIC:
                    tokenpointer.token = TOKEN_IDENTIFIER;
                    tokenpointer.start = trail;
                    tokenpointer.len = index - trail;

                    if (file_string->start[index] == '"') {
                        current_mode = STRING_LITERAL;
                        index++;
                    }

                    trail = index;
                    prev_mode = current_mode;
                    index++;

                    return tokenpointer;

                case SYMBOL:
                    tokenpointer.token = check_token_enumerator(file_string->start[trail]);
                    tokenpointer.start = trail;
                    tokenpointer.len = index - trail;

                    if (file_string->start[index] == '"') {
                        current_mode = STRING_LITERAL;
                        index++;
                    }

                    trail = index;
                    prev_mode = current_mode;
                    index++;

                    return tokenpointer;

                case STRING_LITERAL:
                    if (file_string->start[index] == '"') {
                        tokenpointer.token = TOKEN_STRING_LITERAL;
                        tokenpointer.start = trail;
                        tokenpointer.len = index - trail;

                        if (file_string->start[index] == '"') {
                            current_mode = STRING_LITERAL;
                            index++;
                        }

                        trail = index;
                        prev_mode = current_mode;
                        index++;

                        return tokenpointer;
                    }

                    if (file_string->start[index] == '\n') {
                        tokenpointer.token = TOKEN_UNKNOWN;
                        tokenpointer.start = trail;
                        tokenpointer.len = index - trail;

                        if (file_string->start[index] == '"') {
                            current_mode = STRING_LITERAL;
                            index++;
                        }

                        trail = index;
                        prev_mode = current_mode;
                        index++;

                        return tokenpointer;
                    }
                    continue;

                case NUMERIC_LITERAL:
                    tokenpointer.token = TOKEN_NUMERIC_LITERAL;
                    tokenpointer.start = trail;
                    tokenpointer.len = index - trail;

                    if (file_string->start[index] == '"') {
                        current_mode = STRING_LITERAL;
                        index++;
                    }

                    trail = index;
                    prev_mode = current_mode;
                    index++;

                    return tokenpointer;

            }
        }


        index++;
    }


}
