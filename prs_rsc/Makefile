SRC = main.cpp server.cpp location.cpp
CC = c++
FLAGS = 
RM = rm  -rf
OBJ= $(SRC:.cpp=.o)
NAME = WebServer

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o : %.cpp 
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re : fclean all

.PHONY:all clean fclean re