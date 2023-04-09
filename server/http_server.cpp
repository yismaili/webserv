/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:23 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/09 15:04:16 by yismaili         ###   ########.fr       */
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
        // Get the response to be sent to the client
        std::string response = build_response();

        // Keep track of how much data has been sent to a particular socket
        static std::map<int, size_t> sent_data;

        // If this is the first time sending data to the socket, print the response header
        if (sent_data.find(socket) == sent_data.end())
        {
            std::cout << "Response Header:\n";
            if (requist_info[socket].size() < 1000)
            {
                std::cout << "[" << requist_info[socket] << "]\n";
            }
            else
            {
                std::cout << "[" << requist_info[socket].substr(0, 1000) << "..." << requist_info[socket].substr(requist_info[socket].size() - 10, 15) << "]\n";
            }
        }

        // Send the data to the client
        std::string data_to_send = response;//requist_info[socket].substr(sent_data[socket], 65536);
        long bytes_sent = ::send(socket, data_to_send.c_str(), data_to_send.size(), 0);

        // Check for errors while sending data
        if (bytes_sent == -1)
        {
            std::cerr << "Error: Failed to send data to the socket\n";
            close(socket);
            sent_data[socket] = 0;
            return -1;
        }
        else
        {
            // Update the amount of data that has been sent to the socket
            sent_data[socket] += bytes_sent;

            // If all data has been sent, erase the request information and return 0
            if (sent_data[socket] >= requist_info[socket].size())
            {
                requist_info.erase(socket);
                sent_data[socket] = 0;
                return 0;
            }
            // If there is still data to send, return 1
            else
            {
                return 1;
            }
        }
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
        char buffer[65536];
        int bytes_received;
        bytes_received = recv(newsockfd, buffer, 65535, 0);
        if (bytes_received <= 0)
        {
            close(newsockfd);
            exit_withError("Failed to read from socket");
        }
        requist_info[newsockfd] += std::string(buffer);
        std::size_t found = requist_info[newsockfd].find("\r\n\r\n");
        std::size_t lenOf_chunck = std::atoi(requist_info[newsockfd].substr(requist_info[newsockfd].find("Content-Length: ") + 16, 10).c_str());
        if (requist_info[newsockfd].size() < (lenOf_chunck + found))
        {
            requist_info[newsockfd] = join_chunked(requist_info[newsockfd]);
        }
        return (0);
    }

    std::string http_sever::join_chunked(const std::string& chunked_msg)
    {
        std::string result = "";
        std::size_t pos = 0;

        // Find the end of the headers
        std::size_t header_end = chunked_msg.find("\r\n\r\n");
        if (header_end == std::string::npos) {
            // No headers found, return an empty string
            return "";
        }

        // Append the headers to the result
        result += chunked_msg.substr(0, header_end);

        // Find the start of the first chunk
        pos = header_end + 4;

        while (true) {
            // Find the next chunk size
            std::size_t len_pos = chunked_msg.find("\r\n", pos);
            std::string len_str = chunked_msg.substr(pos, len_pos - pos);
            long len = strtol(len_str.c_str(), nullptr, 16);
            // If the length is 0, we're done
            if (len == 0) {
                break;
            }

            // Append the chunk data to the result
            result += chunked_msg.substr(len_pos + 2, len);
            pos = len_pos + 2 + len + 2;
        }

        return result;
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
