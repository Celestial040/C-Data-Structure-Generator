CC := gcc
CFLAGS := -std=c89 -Wpedantic -Wall -Wextra -Werror -O3
TEST_CFLAGS := -std=c89 -Wpedantic -Werror -O0 -g
C_FILES := base_projects/main.c base_projects/new_parser.c base_projects/new_lexer.c base_projects/char_manip.c \
           base_projects/file_loader.c base_projects/status.c base_projects/string_dynamic_array.c

SCANNER_FILES := main.c status.c file_loader.c char_manip.c scanner.c dynamic_array/string_dynamic_array.c dynamic_array/index_location_dynamic_array.c string_match_scan.c

all:
	$(CC) $(CFLAGS) $(C_FILES) -o output/program
test:
	$(CC) -std=c89 -Wpedantic -Werror -O0 -g $(C_FILES) -o output/program

gperf_gen:
	gperf -L ANSI-C -t -c -N generate_file_keyword -H generate_file_hash -W generate_file_wordlist generate_file_keyword.gperf > generate_file_keyword.h
	gperf -L ANSI-C -t -c -N struct_template_keyword -H struct_template_hash -W struct_template_wordlist struct_template_keyword.gperf > struct_template_keyword.h

scanner:
	$(CC) $(CFLAGS) $(SCANNER_FILES) -o output/scanner
