###############################################################################
#
#	Makefile
#
###############################################################################

NAME		=	libvec.a
UTESTS		=	utests
CC			=	clang
CFLAGS 		=	-O3
WFLAGS		=	-Wall -Wextra -Werror -Wunreachable-code -Wpedantic -Wtype-limits
COMPILE		=	$(CC) $(CFLAGS) $(WFLAGS)

UTESTS_SRC	=	test.c
SRC			=	vbuf.c
OBJ			=	vbuf.o

all:
			@$(COMPILE) -c $(SRC) -o $(OBJ)
			@ar -rcs $(NAME) $(OBJ) memswap/memswap.o
			@echo "compilation successful!"

test: re
			@$(COMPILE) $(UTESTS_SRC) $(NAME) -o $(UTESTS)
			@./utests

clean:
			make fclean -C memswap
			@rm -rf *.o *.d utests *.out

fclean: clean
			@rm -rf *.a;

re: fclean all

.PHONY: all test fclean clean re