NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror -g

SRC =	src/minishell.c \
		src/ft_split.c \
		src/ft_calloc.c \
		src/ft_strncmp.c \
		src/ft_putstr_fd.c \
		src/ft_strdup.c	\
		src/ft_strtrim.c



OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
