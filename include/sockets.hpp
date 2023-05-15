/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:29:15 by yismaili          #+#    #+#             */
/*   Updated: 2023/05/15 22:03:55 by yismaili         ###   ########.fr       */
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
#include <netdb.h>
#include <map>
#include <fcntl.h>
#include "../prs_rsc/server.hpp"

namespace http{
    class sockets{
        public: 
            sockets(); 
            ~sockets();
            sockets &init_data(int port_, std::string ip_add,int index_);

            int git_sockfd()const;
            unsigned int &get_sock_addr_len();
            sockaddr_in &git_serv_addr();
            bool start_server();
        public:
            int sockfd;
            int port;
            struct addrinfo hints;
            struct addrinfo *result, *rp;
            unsigned int sock_addr_len;
            std::string ip_addr;
            int index;
            std::size_t content_length;
    };
}
#endif