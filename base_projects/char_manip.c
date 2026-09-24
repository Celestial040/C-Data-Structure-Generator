#include "bool.h"
#include "status.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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

uint64_t power_uint_64(uint64_t base, uint64_t power) {
    uint64_t returned_value = 1;
    if (power < 0) {
        return 0;
    }
    if (power == 0) {
        return returned_value;
    }
    if (power == 1) {
        return returned_value * base;
    }

    for (; power > 0; power--) {
        returned_value *= base;
    }

    return returned_value;
}

Status char_to_uint_64(const char *start, const size_t len, uint64_t *output) {
    size_t i;
    uint64_t temp = 0;

    if (len > 19) {
        return DIGITS_TOO_LONG;
    }

    for (i = 0; i < len ; i++) {
        temp += ((start[len-1-i] - 48) * power_uint_64(10,i));
    }

    *output = temp;
    return NO_ERROR;
}
