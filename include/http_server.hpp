/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:57:52 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/20 01:37:27 by yismaili         ###   ########.fr       */
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
#include "sockets.hpp"
#include <iterator>
#include <ctype.h>
#include <poll.h>
#include <sys/poll.h>
#define BUFFER_SIZE 1024
namespace http{
    class http_sever{
        public:
            http_sever(std::vector<int> port_, std::string ip_add);  
            ~http_sever();
            int accept_connection(int sockfd);
            std::string build_response();
            void run();
            int is_server(int sock);
            void print_message(const std::string &message);
            void exit_withError(const std::string &errormessage);
            void closeServer(int newsockfd);
            std::string join_chunked(const std::string &data);
            int recv_data(int newsockfd);
            int send_data(int socket);
            void unchunk(int sockfd);
            int transfer_encoding_chunked(int sockfd);
        public:
            int clint;
            http::sockets sock;
            std::vector<http::sockets> socket_id;
            std::vector<pollfd> clients;
            std::map<int, std::string> requist_info;
            std::map<int, bool> read_info;
            std::map<int, bool> write_info;
    };
}
#endif

