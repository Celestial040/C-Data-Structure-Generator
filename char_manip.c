#include "char_manip.h"
#include "status.h"

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

void decimal_to_hexadecimal(unsigned char input, char *output) {
    const unsigned char hex_char[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    unsigned char char_slot[4] = {'0','x','0','0'};
    uint8_t char_slot_count = 3;
    unsigned char digit;
    unsigned char temp = input;

    do {
        digit = temp % 16;
        temp = temp / 16;
        char_slot[char_slot_count] = hex_char[digit];
        char_slot_count--;
    }while (temp > 0);

    memcpy(output, char_slot, 4);
}

uint16_t power_uint16(uint16_t base, uint16_t power) {
    uint16_t returned_value = 1;
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

Status char_to_uint8(const char *start, const size_t len, uint8_t *output) {
    size_t i;
    uint16_t temp = 0;

    if (len > 3) {
        return DIGITS_TOO_LONG;
    }

    for (i = 0; i < len ; i++) {
        temp += ((start[len-1-i] - 48) * power_uint16(10,i));
        if (temp > 255) { return INTEGER_OVERFLOW;}
    }

    *output = temp;
    return NO_ERROR;
}
