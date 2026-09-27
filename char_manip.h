#ifndef CHAR_MANIP_H
#define CHAR_MANIP_H

#include "status.h"
#include <stddef.h>
#include <stdint.h>

typedef struct String {
    char *buffer;
    size_t length;
} String ;

bool is_uppercase_alphabet(const char target);
bool is_lowercase_alphabet(const char target);
bool is_alphabet(const char target);
bool is_numeric(const char target);
bool is_alphabet_numeric(const char target);
bool is_whitespace(const char target);
bool is_it_numeric_literal(const char target);
Status char_to_uint_64(const char *start, const size_t len, uint64_t *output);

#endif
