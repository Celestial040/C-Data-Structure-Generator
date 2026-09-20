#include "bool.h"
#include <stdint.h>

static const char *numeric_literal_chars = "0123456789abcdeflpuxABCDEFLPUX.+-\0";
static const uint8_t numeric_literal_chars_len = 33;

bool is_uppercase_alphabet(const char target) {
    return target >= 65 && target <= 90;
}

bool is_lowercase_alphabet(const char target) {
    return target >= 97 && target <= 122;
}

bool is_alphabet(const char target) {
    return is_uppercase_alphabet(target) || is_lowercase_alphabet(target);
}

bool is_numeric(const char target) {
    return target >= 48 && target <= 57;
}

bool is_alphabet_numeric(const char target) {
    return is_alphabet(target) || is_numeric(target);
}

bool is_whitespace(const char target) {
    return target == ' ' || (target >= '\t' && target <= '\r');
}

bool is_it_numeric_literal(const char target) {
    uint8_t i;
    for (i = 0; i < numeric_literal_chars_len; i++) {
        if (target == numeric_literal_chars[i]) {
            return true;
        }
    }
    return false;
}
