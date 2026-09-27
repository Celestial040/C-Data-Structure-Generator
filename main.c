
#include "decimal_to_hex.h"
int main() {
    char output[4];
    decimal_to_hexadecimal('\n', output);
    printf("%.*s \n", 4,output);
    return 0;
}
