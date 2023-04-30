NAME = webserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
SRCS = server/sockets.cpp server/http_server.cpp prs_rsc/main.cpp prs_rsc/server.cpp prs_rsc/location.cpp prs_rsc/server_utils.cpp \
		request/request.cpp request/request_utils.cpp
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