/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:42 by yismaili          #+#    #+#             */
/*   Updated: 2023/05/13 15:10:11 by yismaili         ###   ########.fr       */
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
            // AF stands for Address Family and PF stands for Protocol Family
            // This construct holds the information about the address family, port number, Internet address
            serv_addr.sin_family = AF_INET; // Address family IPv4 Internet protocols
            serv_addr.sin_addr.s_addr = INADDR_ANY;//inet_addr(ip_addr.c_str());
            serv_addr.sin_port = htons(port); // convert between host and network byte orders
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
            
        sockaddr_in &sockets::git_serv_addr()
        {
            return (serv_addr);
        }
        
        bool sockets::start_server() 
        {
            int optval;
            
            optval = 1;
            // Creates a TCP socket
            sockfd = socket(AF_INET, SOCK_STREAM, 0); 
            if (sockfd < 0)
                return (false);
            
            // set options for a socket
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
            {
                //SOL_SOCKET: manipulate the socket-level options
                //SO_REUSEADDR: the option name. It is used to enable reuse of local addresses.
                //&optval: a pointer to the buffer that contains the value of the option you want to set
                perror("setsockopt failed");
                return (false);
            }
            
            sock_addr_len = sizeof(serv_addr);
            // set the O_NONBLOCK flag for the socket file descriptor
            fcntl(sockfd, F_SETFL, O_NONBLOCK);
            //bind a socket with a specific address and port number
            if (bind(sockfd, (struct sockaddr *) &serv_addr, sock_addr_len) < 0) {
                //the second is a pointer to a struct sockaddr structure that contains the address
                perror("Bind System failed");
                return (false);
            }
          //  Set socket to listen
            if (listen(sockfd, SOMAXCONN) < 0){
                // The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow
                std::cout<<"Socket listen failed"<<std::endl;
                exit(1);
            }
            return true;
        }
    }
    