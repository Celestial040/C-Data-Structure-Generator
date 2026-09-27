#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
