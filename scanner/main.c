#include "status.h"
#include "scanner.h"
#include "status.h"
#include <stdio.h>

int main(int argc, char *argv[]){
    Status status = NO_ERROR;

    if (argc != 2) {
        printf("Wrong arguments");
        return 1;
    }

    status = start_scanning_file(argv[1]);
    if (status != NO_ERROR) {
        status_print(status);
    }

    return NO_ERROR;
}
