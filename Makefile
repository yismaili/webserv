NAME = webserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 	#-g -fsanitize=address
SRCS = server/main.cpp server/sockets.cpp server/http_server.cpp
HDR = include/http_server.hpp include/sockets.hpp
OBJS = $(SRCS:.cpp=.o)

all : $(NAME)
	
%.o: %.cpp $(HDR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all