CC := gcc
CFLAGS := -std=c89 -Wpedantic -Wall -Wextra -Werror -O3
C_FILES := main.c new_parser.c new_lexer.c char_manip.c file_loader.c status.c string_dynamic_array.c

all:
	$(CC) $(CFLAGS) $(C_FILES) -o output/program
test:
	$(CC) -std=c89 -Wpedantic -Werror -O0 -g $(C_FILES) -o output/program
gperf_gen:
	gperf -L ANSI-C -t -c -N generate_file_keyword -H generate_file_hash -W generate_file_wordlist generate_file_keyword.gperf > generate_file_keyword.h
	gperf -L ANSI-C -t -c -N struct_template_keyword -H struct_template_hash -W struct_template_wordlist struct_template_keyword.gperf > struct_template_keyword.h
