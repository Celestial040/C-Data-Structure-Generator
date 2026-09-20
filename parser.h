#ifndef PARSER_H
#define PARSER_H

#include "file_loader.h"
#include "status.h"
#include "string_dynamic_array.h"

void set_string_dynamic_array(StringDynamicArray *loaded_string_dynamic_array);
void set_file_string(FileString *loaded_file_string);
void set_input_filename(StringDynamicArrayPointer *pointer);
Status parser_start();

#endif
