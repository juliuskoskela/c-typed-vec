###############################################################################
#
#	Makefile
#
###############################################################################

CC			=	clang
CFLAGS 		=	-O3
WFLAGS		=	-Wall -Wextra -Werror -Wunreachable-code -Wpedantic -Wtype-limits
COMPILE		=	$(CC) $(CFLAGS) $(WFLAGS)

SRC			=	memswap.c
OBJ			=	memswap.o

all:
			@$(COMPILE) -c $(SRC) -o $(OBJ)
			@echo "compilation successful!"

clean:
			@rm -rf *.o

fclean: clean

re: fclean all

.PHONY: all test fclean clean re