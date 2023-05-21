NAME = request

SRC		=	request.cpp main.cpp request_utils.cpp

CC 		= c++

# FLAGS	= -std=c++98 -Wall -Wextra -Werror -g -fsanitize=address
FLAGS	= -std=c++98 -Wall -Wextra -Werror

OBJS 	= $(SRC:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

all: $(NAME) $(OBJS)

%.o: %.c
		$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re