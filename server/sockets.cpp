/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:42 by yismaili          #+#    #+#             */
/*   Updated: 2023/05/22 13:40:58 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/sockets.hpp"

namespace http{
    
        sockets::sockets()
        {
            sockfd =  0;
            port = 0;
            sock_addr_len = 0;
            ip_addr = "";
            index = 0;
            content_length = 0;
            time_out = 0;
            header_error = 0;
            data_issending = 0;
        }
        
        sockets::~sockets()
        {
        }

        sockets &sockets::init_data(int port_, std::string ip_add, std::string server_name_, int index_)
        {
            std::string port_str;
            int         ret_getadd;

            sockfd =  -1;
            port = port_;
            ip_addr = ip_add;
            index = index_;
            server_name = server_name_;
            memset(&hints, 0, sizeof(hints)); // initializes the struct addrinfo variable hints with zeros
            hints.ai_family = AF_UNSPEC; // the address family IPv4 and IPv6 addresses for the given hostname
            hints.ai_socktype = SOCK_STREAM; // specifies the socket type specifies the socket type
            port_str = std::to_string(port);
            ret_getadd = getaddrinfo(server_name.c_str(), port_str.c_str(), &hints, &result);
            if (ret_getadd != 0) {
                std::cout << "\033[31mGetaddrinfo error\033[0m\n";
                exit(EXIT_FAILURE);
            }
            
            if(start_server() == false){
                exit(EXIT_FAILURE);
            }
            return (*this);
        }
          
        int const &sockets::getSockfd()const
        {
            return (sockfd);
        }
        
        int const &sockets::getIndex() const
        {
            return (index);
        }
        
        unsigned int sockets::getSock_addr_len()const
        {
            return (sock_addr_len);
        }
        
        std::size_t const &sockets::getContent_length() const
        {
            return (content_length);
        }
        
        int const &sockets::getPort() const{
            return (port);
        }
        
        void sockets::setContent_length(int const &len)
        {
            this->content_length = len;
        }
        
        bool sockets::start_server() 
        {
            int optval;
            
            optval = 1;
            // Creates a TCP socket
            sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if (sockfd < 0)
            {
                std::cout << "\033[31mCreate sockopt failed\033[0m\n";
                return (false);
            }
            // set options for a socket
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
            {
                //SOL_SOCKET: manipulate the socket-level options
                //SO_REUSEADDR: the option name. It is used to enable reuse of local addresses.
                //&optval: a pointer to the buffer that contains the value of the option you want to set
                std::cout << "\033[31mSet sockopt failed\033[0m\n";
                return (false);
            }
            // set the O_NONBLOCK flag for the socket file descriptor
            fcntl(sockfd, F_SETFL, O_NONBLOCK);

            //bind a socket with a specific address and port number
            if (bind(sockfd, result->ai_addr, result->ai_addrlen) == 0)
            {
                std::cout << "\n\033[32mLISTENING ON ["<<port<<"]...\033[0m\n";
            }
            else
            {
                std::cout << "\nwebsevr: conflicting server name  on 0.0.0.0:["<<port<<"], ignored\n";
            }
          //  Set socket to listen
            if (listen(sockfd, SOMAXCONN) < 0){
                // The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow
                std::cout << "\033[31mSocket listen failed\033[0m\n";
            }
            // std::cout << "\n\033[32mLISTENING ON ["<<port<<"]...\033[0m\n";
            freeaddrinfo(result);
            return true;
        }
        
        unsigned int const &sockets::getTime_out() const
        {
            return (time_out);
        }
        
        void sockets::setTime_out(unsigned int time)
        {
            this->time_out = time;
        }
    }
    