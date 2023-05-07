NAME = webserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 
SRCS =  prs_rsc/main.cpp prs_rsc/server.cpp prs_rsc/location.cpp prs_rsc/server_utils.cpp \
		request/request.cpp request/request_utils.cpp respond/respond.cpp respond/respond_root.cpp respond/pairs_def.cpp respond/method_utils.cpp \
		respond/method_handling.cpp CGI/cgi.cpp
HDR = include/http_server.hpp include/sockets.hpp  respond/additional_class.hpp respond/respond.hpp \
		CGI/cgi.hpp
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