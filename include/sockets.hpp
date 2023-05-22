/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:29:15 by yismaili          #+#    #+#             */
/*   Updated: 2023/05/22 18:37:36 by yismaili         ###   ########.fr       */
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
#include <sys/time.h>

namespace http{
    class sockets{
        public: 
            sockets();
            ~sockets();
            sockets &init_data(int port_, std::string ip_add, std::string server_name_, int index_);
            int const &getSockfd()const;
            unsigned int getSock_addr_len()const;
            sockaddr_in &getServ_addr()const;
            bool start_server();
            std::size_t const &getContent_length() const;
            int const &getIndex() const;
            int const &getPort() const;
            void setContent_length(int const &content);
            unsigned int const &getTime_out() const;
            void    setTime_out(unsigned int time);
            void setIndex(int const index_);
            int data_issending;
            
        private:
            struct addrinfo hints; //provides information about a network address, such as the host name, port number, and address family.
            struct addrinfo *result, *rp;
            unsigned int    sock_addr_len;
            std::string     ip_addr;
            int             index;
            std::size_t     content_length;
            int             sockfd;
            int             port;
            unsigned int    time_out;
            int           header_error;
            std::string  server_name;
    };
}
#endif