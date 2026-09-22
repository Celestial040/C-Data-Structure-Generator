#include <stddef.h>
#include <stdio.h>
#include "new_parser.h"
#include "status.h"


int main(int argc, char *argv[]) {
    Status operation_status = NO_ERROR;

    if (argc != 2) {
        printf("Missing arguments for target file \nprogram-name {target-file} \n \nfor example: \n./program generate.txt\n");
        return 1;
    }

    operation_status = new_parser_start(argv[1]);

    if (operation_status != NO_ERROR) {
        status_print(operation_status);
        return 1;
    }

    return 0;
}
