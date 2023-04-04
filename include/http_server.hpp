/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:57:52 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/04 22:17:47 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDED_HTTP_SERVER
# define INCLUDED_HTTP_SERVER

#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<fstream>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include "tcp_server.hpp"
#include <iterator>
#include <sys/select.h>
#include <ctype.h>

namespace http{
    class http_sever{
        public:
            http_sever(std::vector<int> port_, std::string ip_add);  
            ~http_sever();
            int accept_connection(int sockfd);
            std::string build_response();
            void send_response(int newsockfd);
            void run();
            int is_server(int sock);
            void read_request(int newsockfd);
            int	ft_strncmp(const char *s1, const char *s2, size_t n);
            int check_encoding(char *requiste, char *encoding);
            std::string unchunk(const char *chunck); 
            void print_message(const std::string &message);
            void exit_withError(const std::string &errormessage);
            void closeServer(int newsockfd);
            int str_len(char *str);  
        private:
                int clint;
                http::tcp_server tcp;
                std::map<int, http::tcp_server> sock_inf;
                std::vector<http::tcp_server> other_sock;
                std::string request_mssg;
                std::string request_str;
                std::stringstream ss;
    };
}
#endif

    