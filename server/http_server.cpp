/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:23 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/13 17:54:14 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/http_server.hpp"
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
namespace http{
   http_sever::http_sever(std::vector<int> port_, std::string ip_add) :sock()
   {
        std::vector<int>::iterator it = port_.begin();
        while (it < port_.end())
        {
           socket_id.push_back(sock.init_data(*it, ip_add));
            it++;
        }
    }
    
    http_sever::~http_sever()
    {
        std::vector<http::sockets>::iterator it = socket_id.begin();
        while (it != socket_id.end())
        {
            closeServer(it->sockfd);
            it++;
        }
    }
         
    int http_sever::accept_connection(int sockfd)
    {
        // Accepts a connection on a socket.
        int sockfd_client = accept(sockfd, (struct sockaddr *) &sock.serv_addr, &sock.sock_addr_len);
        if (sockfd_client < 0) 
        {
            exit_withError("accepting connection");
        }
        return (sockfd_client);
    }
    
    void http_sever::run() {
        // Create a set of file descriptors to monitor with select
        FD_ZERO(&readmaster_fds);
        FD_ZERO(&writemaster_fds);
        std::vector<http::sockets>::iterator it_skfd = socket_id.begin();
        while (it_skfd != socket_id.end()) {
            http::sockets& sock = *it_skfd;
            FD_SET(sock.sockfd, &readmaster_fds);
            FD_SET(sock.sockfd, &writemaster_fds);
            it_skfd++;
        }
        // Main server loop
        while (true) {
            // Create a copy of the master set to pass to select
            fd_set read_fds = readmaster_fds;
            fd_set write_fds = writemaster_fds;
            // Wait for activity on any of the monitored sockets
            int activity = select(FD_SETSIZE, &read_fds, &write_fds, NULL, NULL);
            if (activity < 0) 
            {
                exit_withError("select");
            }
            std::vector<http::sockets>::iterator it_accpt = socket_id.begin();
            // Check each socket for activity
            while (it_accpt != socket_id.end())
            {
                if (FD_ISSET(it_accpt->sockfd, &read_fds)) 
                {
                    if (is_server(it_accpt->sockfd))
                    {
                        // Accept a new connection and add the new socket to the master set
                        clint = accept_connection(it_accpt->sockfd);
                        it_accpt->requist_info.insert(std::make_pair(clint, ""));
                        FD_SET(clint, &read_fds);
                    }
                }
                it_accpt++;
            }   
            std::vector<http::sockets>::iterator it_recv = socket_id.begin();
            // Check each socket for activity
            
            while (it_recv != socket_id.end())
            {
                   // Read the client request and send a response 
                    it_recv->recv_data(clint);
                       std::cout << it_recv->requist_info[clint] << std::endl;
                    if (it_recv->read_info[clint] == true) 
                    { 
                        // Add the client socket to the set of sockets to write to
                        it_recv->write_info[clint] = true;
                        FD_SET(clint, &write_fds);
                        FD_CLR(clint, &readmaster_fds);
                        break;
                    }
            }
            std::vector<http::sockets>::iterator it_send = socket_id.begin();
            // Check each socket for activity
            while (it_send != socket_id.end())
            {
                   // Read the client request and send a response 
                if (FD_ISSET(clint, &write_fds)) {
                    if (it_send->write_info[clint] == true) {
                        it_send->send_data(clint);
                        close(clint);
                        FD_CLR(clint, &writemaster_fds);
                    }
                }
            }
        }
    }

    int http_sever::ckeck_close(std::string &requist)
    {
        std::string endOfstr("0\r\n\r\n");
        std::size_t size_end = endOfstr.size();
        std::size_t size_requist = requist.size();
        while (size_end > 0)
        {
            size_end--;
            size_requist--;
            if (size_requist < 0 || requist[size_requist] != endOfstr[size_end])
                return (1);
        }
        return (0);
    }
    
    int http_sever::is_server(int sock)
    {
        std::vector<http::sockets>::iterator it = socket_id.begin();
        while (it != socket_id.end())
        {
            if (it->sockfd == sock)
            {
                return (1);
            }
            it++;
        }
        return (0);
    }
        
    void http_sever::print_message(const std::string &message)
    {
        std::cout << message << std::endl;
    }
        
    void http_sever::exit_withError(const std::string &errormessage)
    {
        print_message("ERROR: " + errormessage);
        exit(1);
    }

    void http_sever::closeServer(int newsockfd)
    {
        close(sock.sockfd);
        close(newsockfd);
        exit(1);
    }
}


/*  HTTP/1.1 200 OK
Content-Type: text/plain
Transfer-Encoding: chunked
7\r\n
Mozilla\r\n
11\r\n
Developer Network\r\n
0\r\n
\r\n 

 */