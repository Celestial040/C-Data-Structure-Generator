CC := gcc
CFLAGS := -std=c89 -Wpedantic -Wall -Wextra -Werror -O3
C_FILES := main.c parser.c char_manip.c file_loader.c status.c string_dynamic_array.c

all:
	$(CC) $(CFLAGS) $(C_FILES) -o output/program
test:
	$(CC) -std=c89 -Wpedantic -Werror -O0 -g $(C_FILES) -o output/program
