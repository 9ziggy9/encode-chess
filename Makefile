CFLAGS=-Wall -Werror -Wconversion -pedantic -std=c11

# COLOR ALIASES
RED=\033[31m
GREEN=\033[32m
YELLOW=\033[33m
BLUE=\033[34m
MAGENTA=\033[35m
CYAN=\033[36m
RESET=\033[0m

# Colored output function
define print_in_color
	@printf "$1"
	@printf "$2"
	@printf "\033[0m"
endef

make_run_clean: main run clean

run:
	$(call print_in_color, $(GREEN), \nRUNNING:\n)
	@./main.out
	$(call print_in_color, $(YELLOW), \nHEXDUMP:\n)
	@xxd ./out.txt
	$(call print_in_color, $(YELLOW), \nSIZE IN BYTES:\t)
	@stat --format="%s" out.txt

leak_test: main
	valgrind --leak-check=full ./main.out

main: main.c
	$(call print_in_color, $(BLUE), \nCOMPILING:\n)
	cc -o ./main.out $(CFLAGS) main.c

clean:
	$(call print_in_color, $(RED), \nREMOVING OLD EXE\n)
	@rm -rf ./main.out
