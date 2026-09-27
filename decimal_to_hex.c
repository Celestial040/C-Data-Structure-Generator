#include "decimal_to_hex.h"

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
