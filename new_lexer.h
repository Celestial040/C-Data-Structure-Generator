#ifndef NEW_LEXER_H
#define NEW_LEXER_H

#include "file_loader.h"
#include "status.h"
#include <stddef.h>
#include <stdint.h>

#include <stddef.h>

typedef uint8_t Token;
enum {
    TOKEN_IDENTIFIER,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_ASSIGNMENT,
    TOKEN_DOUBLE_QUOTE,
    TOKEN_OPEN_BRACKET,
    TOKEN_CLOSE_BRACKET,
    TOKEN_OPEN_CURLY_BRACKET,
    TOKEN_CLOSE_CURLY_BRACKET,
    TOKEN_STRING_LITERAL,
    TOKEN_NUMERIC_LITERAL,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};


typedef uint8_t GenerateFields;
enum {
    STATUS_HEADER,
    UTILS_DIR,
    HEADER_DIR,
    STRUCT_TEMPLATE
};

typedef struct LookupStatus {
    Status status;
    GenerateFields fields_id;
}LookupStatus ;

typedef uint8_t TypeMode;
enum {
    WHITESPACE,
    ALPHANUMERIC,
    SYMBOL,
    STRING_LITERAL,
    NUMERIC_LITERAL
};

typedef struct TokenPointer {
    size_t start;
    size_t len;
    Token token;
} TokenPointer;

TokenPointer lexer_scan(FileString *file_string, size_t *line_count)

#endif
