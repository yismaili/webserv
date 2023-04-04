NAME = webserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 
SRCS = server/main.cpp server/tcp_server.cpp server/http_server.cpp
HDR = include/httpTcpServer.hpp include/tcp_server.hpp
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