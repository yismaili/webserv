NAME = request

SRC		=	request.cpp main.cpp request_utils.cpp

CC 		= c++

<<<<<<< HEAD
FLAGS	= -std=c++98 -Wall -Wextra -Werror 
=======
FLAGS	= -std=c++98 -Wall -Wextra -Werror -g -fsanitize=address
>>>>>>> e548e9e226daa2f3cfe5d0d80e153038ea8b868c
# FLAGS	= -std=c++98 -Wall -Wextra -Werror

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