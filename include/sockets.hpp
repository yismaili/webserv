/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:29:15 by yismaili          #+#    #+#             */
/*   Updated: 2023/05/02 17:24:04 by yismaili         ###   ########.fr       */
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
#include "../prs_rsc/server.hpp"

namespace http{
    class sockets{
        public: 
            sockets(); 
            ~sockets();
            sockets &init_data(int port_, std::string ip_add, std::vector<server> conf_);
            int git_sockfd()const;
            unsigned int &get_sock_addr_len();
            sockaddr_in &git_serv_addr();
            bool start_server();
        public:
            int sockfd;
            int port;
            struct sockaddr_in serv_addr;
            unsigned int sock_addr_len;
            std::string ip_addr;
            std::vector<server> conf;
            
    };
}
#endif