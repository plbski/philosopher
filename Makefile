NAME = philosopher

SOURCES = main.c

LIBFT_PASS = libft/
LIBFT = $(LIBFT_PASS)libft.a

FLAGS = -Wall -Wextra -Werror -g
CC = cc


$(NAME): $(SOURCES) $(LIBFT) $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES) -L $(LIBFT_PASS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_PASS)

all: $(NAME)

clean:
	make -C $(LIBFT_PASS) clean
	rm -f $(OBJECTS)

fclean: clean
	make -C $(LIBFT_PASS) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re