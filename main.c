
#include "char_manip.h"
#include "status.h"
#include <stdint.h>
#include <stdio.h>
int main() {
    char output[4];
    Status status = NO_ERROR;
    uint8_t result = 0;

    decimal_to_hexadecimal('\n', output);
    printf("%.*s \n", 4,output);

    status = char_to_uint8("1111", 4, &result);
    if (status != NO_ERROR) {
        status_print(status);
        return 1;
    }
    printf("%d \n",result);
    return 0;
}
