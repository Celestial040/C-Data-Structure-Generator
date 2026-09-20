#ifndef STATUS_H
#define STATUS_H

typedef enum Status {
    NO_ERROR,
    ALLOCATION_ERROR,
    FILE_NOT_FOUND,
    NULL_POINTER,
    STRING_EMPTY,
    FIELD_MATCH_NOT_FOUND,
    ITEM_NOT_FOUND,
    ITEM_FOUND,
    READ_ERROR
} Status;


void status_print(Status status);

#endif
