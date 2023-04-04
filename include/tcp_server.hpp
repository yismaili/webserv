/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_server.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:29:15 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/04 22:16:35 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDED_TCP_SERVER
# define INCLUDED_TCP_SERVER

#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<fstream>
#include <arpa/inet.h>

namespace http{
    class tcp_server{
        public: 
            tcp_server(); 
            ~tcp_server();
            tcp_server &init_data(int port_, std::string ip_add);
            int git_sockfd()const;
            unsigned int &get_sock_addr_len();
            sockaddr_in &git_serv_addr();
            bool start_server();
        public:
            int sockfd;
            int port;
            struct sockaddr_in serv_addr;
            std::string serv_message;
            unsigned int sock_addr_len;
            std::string ip_addr;   
    };
}
#endif