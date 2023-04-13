/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:29:15 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/13 00:31:49 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDED_SOCKETS_HPP
# define INCLUDED_SOCKETS_HPP

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

namespace http{
    class sockets{
        public: 
            sockets(); 
            ~sockets();
            sockets &init_data(int port_, std::string ip_add);
            int git_sockfd()const;
            unsigned int &get_sock_addr_len();
            sockaddr_in &git_serv_addr();
            bool start_server();
            std::string join_chunked(const std::string& chunked_msg, int sockfd);
            int recv_data(int newsockfd);
            std::string build_response();
            int send_data(int socket);
        public:
            int sockfd;
            int port;
            struct sockaddr_in serv_addr;
            unsigned int sock_addr_len;
            std::string ip_addr;
            std::map<int, std::string> requist_info;
            std::map<int, bool> read_info;
            std::map<int, bool> write_info;
    };
}
#endif