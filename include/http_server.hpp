/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:57:52 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/13 00:55:33 by yismaili         ###   ########.fr       */
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
#include <sys/select.h>
#include <ctype.h>

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
            int ckeck_close(std::string &str);
        private:
            int clint;
            http::sockets sock;
            std::vector<http::sockets> socket_id;
            fd_set readmaster_fds;
            fd_set writemaster_fds;
    };
}
#endif

