/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:23 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/07 01:55:03 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/http_server.hpp"
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
namespace http{
   http_sever::http_sever(std::vector<int> port_, std::string ip_add) :tcp()
   {
        int i = 0;
        std::vector<int>::iterator it = port_.begin();
        
        while (it < port_.end())
        {
           socket_id.push_back(tcp.init_data(*it, ip_add));
            it++;
        }
    }
    
    http_sever::~http_sever()
    {
        std::map<int, http::tcp_server>::iterator it = server_info.begin();
        while (it != server_info.end())
        {
            closeServer(it->first);
            it++;
        }
    }
         
    int http_sever::accept_connection(int sockfd)
    {
        // Accepts a connection on a socket.
        int newsockfd;
        
        newsockfd = accept(sockfd, (struct sockaddr *) &tcp.serv_addr, &tcp.sock_addr_len);
        server_info.insert(std::make_pair(newsockfd, tcp));
        if (newsockfd < 0) 
        {
            exit_withError("accepting connection");
        }
        return (newsockfd);
    }
    
    std::string http_sever::build_response()
    {
        // Insert html page or ...
        std::ostringstream response; //create the output string stream
        
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/html; charset=UTF-8\r\n";
        response << "\r\n";
        response << "<html><body><h1>Hello younes </h1>";
        response << "<h1>from HTTP server!</h4>";
        response << "</body></html>";
        
        std::string response_str = response.str();
        return (response_str);
    }
    
  int http_sever::send_data(int socket)
    {
        long byte_send;
        std::string response = build_response();
        
        byte_send = send(socket, response.c_str(), response.length(), 0);
        if (byte_send < 0)
        {
            print_message("Error: sending response to client");
        }
        return (0);
    }
    
    void http_sever::run() 
    {
    // Create a set of file descriptors to monitor with select
        fd_set readmaster_fds;
        fd_set writemaster_fds;
        std::vector<http::tcp_server>::iterator it = socket_id.begin();
        
        FD_ZERO(&readmaster_fds);
        FD_ZERO(&writemaster_fds);
        while (it != socket_id.end())
        {
            http::tcp_server& sock = *it;
            FD_SET(sock.sockfd, &readmaster_fds);
            FD_SET(sock.sockfd, &writemaster_fds);
            it++;
        }
    // Main server loop
        while (true) 
        {
            // Create a copy of the master set to pass to select
            fd_set read_fds = readmaster_fds;
            fd_set wirte_fds = writemaster_fds;
            // Wait for activity on any of the monitored sockets
            int activity = select(FD_SETSIZE, &read_fds, &wirte_fds, NULL, NULL);
            if (activity < 0)
            {
                exit_withError("select");
            }
            // Check each socket for activity
            std::vector<http::tcp_server>::iterator it_ = socket_id.begin();
            while( it_ != socket_id.end()) 
            {
                if (FD_ISSET(it_->sockfd, &read_fds)) 
                {
                    if (!is_server(it_->sockfd))
                    {
                        // Accept a new connection and add the new socket to the master set
                        recv_data(clint);
                        send_data(clint);
                        close(clint);
                        FD_CLR(clint, &readmaster_fds);
                    }
                    else if (is_server(it_->sockfd)){
                        // Accept a new connection and add the new socket to the master set
                        clint = accept_connection(it_->sockfd);
                        requist_info.insert(std::make_pair(clint, ""));
                        FD_SET(clint, &read_fds);
                    }
                    // Read the client request and send a response  
                    recv_data(clint);
                    std::cout<<requist_info[clint]<<std::endl;
                    send_data(clint);
                    close(clint);
                    FD_CLR(clint, &readmaster_fds);
             }
                it_++;
            }
        }
    }
        
    int http_sever::is_server(int sock)
    {
        std::vector<http::tcp_server>::iterator it = socket_id.begin();
        
        while (it != socket_id.end())
        {
            // std::cout << it->sockfd << " != " << sock << std::endl;
            if (it->sockfd == sock)
            {
                return (1);
            }
            it++;
        }
        return (0);
    }
    
   int http_sever::recv_data(int newsockfd)
    {
        char buffer[1048576];
        int bytes_received;
        
        bytes_received = recv(newsockfd, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0)
        {
            close(newsockfd);
            exit_withError("Failed to read from socket");
        }
        requist_info[newsockfd] += std::string(buffer);
        std::size_t found = requist_info[newsockfd].find("\r\n\r\n");
        std::size_t lenOf_chunck = std::atoi(requist_info[newsockfd].substr(requist_info[newsockfd].find("Content-Length: ") + 16, 10).c_str());
        if (requist_info[newsockfd].size() < (lenOf_chunck + found)){
            read_databychunck(newsockfd);
        }
        return (0);
    }

    void	http_sever::read_databychunck(int sock)
    {
        std::string	head = requist_info[sock].substr(0, requist_info[sock].find("\r\n\r\n"));
        std::string	chunks = requist_info[sock].substr(requist_info[sock].find("\r\n\r\n"), requist_info[sock].size());
        std::string	subchunk = chunks.substr(29, 20);
        std::string	body = "";
        int			chunksize = strtol(subchunk.c_str(), NULL, 16);
        size_t		i = 0;

        while (chunksize)
        {
            i = chunks.find("\r\n", i) + 2;
            body += chunks.substr(i, chunksize);
            i += chunksize + 2;
            subchunk = chunks.substr(i, 29);
            chunksize = strtol(subchunk.c_str(), NULL, 16);
        }
        requist_info[sock] = head + "\r\n\r\n" + body + "\r\n\r\n";
    }

    int	http_sever::ft_strncmp(const char *s1, const char *s2, size_t n)
    {
        size_t	i;

        i = 0;
        while (s1[i] && i < n)
        {
            if (s1[i] != s2[i])
            {
                return ((unsigned char)s1[i] - (unsigned char)s2[i]);
            }
         i ++;
        }
        if (s2[i] == '\0' || i == n)
            return (0);
        else
            return (-s2[i]);
    }
       
    int http_sever::str_len(char *str)
    {
        int i = 0;
        while (str[i])
        {
             i++;
        }
        return (i);
    }
        
    int http_sever::check_encoding(char *requiste, char *encoding)
    {
        // std::cout<<requiste<<std::endl;
        if (!ft_strncmp(requiste, encoding, str_len(encoding)))
            return (1);
        else
            return (0);
    }
        
    std::string http_sever::unchunk(std::string chunck)
    {
        std::cout<<chunck<<std::endl;
        //std::string head = server_info[sock].substr(0, server_info[sock].find())
        return(chunck);
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
        close(tcp.sockfd);
        close(newsockfd);
        exit(1);
    }
}
