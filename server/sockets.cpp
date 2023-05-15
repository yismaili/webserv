/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:42 by yismaili          #+#    #+#             */
/*   Updated: 2023/05/15 16:45:48 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/sockets.hpp"
#include <fcntl.h>
namespace http{
    
        sockets::sockets(/* args */)
        {
        }
        
        sockets::~sockets()
        {
        }
         //Assign a port to socket
        sockets &sockets::init_data(int port_, std::string ip_add, int index_)
        {
            sockfd =  -1;
            port = port_;
            sock_addr_len = 0;
            ip_addr = ip_add;
            index = index_;
            
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = 0;
            hints.ai_protocol = 0;

            std::string port_str = std::to_string(port);
            int s = getaddrinfo(ip_add.c_str(), port_str.c_str(), &hints, &result);
            if (s != 0) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            if(start_server() == false){
                exit(1);
            }
            return (*this);
        }
            
        int sockets::git_sockfd()const
        {
            return (sockfd);
        }
            
        unsigned int &sockets::get_sock_addr_len()
        {
            return (sock_addr_len);
        }
        
        bool sockets::start_server() 
        {
            int optval;
            
            optval = 1;
            // Creates a TCP socket
           for (rp = result; rp != NULL; rp = rp->ai_next) 
           {
                sockfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
                if (sockfd < 0)
                    return (false);
           }
            
            // set options for a socket
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
            {
                //SOL_SOCKET: manipulate the socket-level options
                //SO_REUSEADDR: the option name. It is used to enable reuse of local addresses.
                //&optval: a pointer to the buffer that contains the value of the option you want to set
                perror("setsockopt failed");
                return (false);
            }
            // sock_addr_len = sizeof(hints);
            // set the O_NONBLOCK flag for the socket file descriptor
            fcntl(sockfd, F_SETFL, O_NONBLOCK);
            //bind a socket with a specific address and port number
            // bind a socket with a specific address and port number
            if (bind(sockfd, result->ai_addr, result->ai_addrlen) < 0) {
                perror("Bind System failed");
                return false;
            }
          //  Set socket to listen
            if (listen(sockfd, SOMAXCONN) < 0){
                // The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow
                std::cout<<"Socket listen failed"<<std::endl;
                exit(1);
            }
            freeaddrinfo(result);           /* No longer needed */  
            return true;
        }
    }
    