/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:23 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/15 00:11:58 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/http_server.hpp"
#include <sys/poll.h>
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

    void http_sever::run() 
    {
        int poll_ret, new_socket;
        unsigned long i = 0;
        std::vector<int> sockfd;
        // Add server socket to poll list
        std::vector<http::sockets>::iterator it = socket_id.begin();
        while (it != socket_id.end())
        {
            pollfd server_pollfd;
            server_pollfd.fd = it->sockfd;
            server_pollfd.events = POLLIN | POLLOUT;
            clients.push_back(server_pollfd);
            it++;
        }
        
        while (true)
        {
            i = 0;
            // Wait for events on any of the monitored file descriptors
            poll_ret = poll(clients.data(), clients.size(), -1);
            
            //Check for events on server socket
            while (i != clients.size())
            {
               if (clients[i].events & POLLIN)
               {
                    if (is_server(clients[i].fd))
                    {
                        // Accept incoming connection
                        std::cout<<"-cleint---"<<(clients[i].fd)<<std::endl;
                        new_socket = accept_connection(clients[i].fd);
                         std::cout<<"-++++++--"<<std::endl;
                        std::cout<<new_socket<<std::endl;
                        // Add new socket to poll list
                        pollfd new_client_pollfd;
                        new_client_pollfd.fd = new_socket;
                        new_client_pollfd.events = POLLOUT | POLLIN;
                        clients.push_back(new_client_pollfd);
                    }
                    else
                    {
                        std::cout<< "---clent----"<<clients[i].fd<<std::endl;
                        recv_data(clients[i].fd);
                        std::cout<<"----"<<std::endl;
                        
                        std::cout<<requist_info[clients[i].fd]<<std::endl;
                    }
               }
            //    if (clients[i].events & POLLOUT)
            //    {
            //         send_data(clients[i].fd);
            //    }
            //    if ()
             std::cout<<"----"<<std::endl;
                i++;
            }
            std::cout<<"--222222--"<<std::endl;
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
     int http_sever::recv_data(int newsockfd)
        {
            char buffer[1024] = {0};
            int bytes_received;

            bytes_received = recv(newsockfd, buffer, 1024, 0);
            requist_info[newsockfd] += std::string(buffer);
            // exit(1);
            // std::size_t header_end = requist_info[newsockfd].find("0\r\n\r\n");//used this string ::nops check the end!!!!!!
            // std::size_t content_len = std::strtol(requist_info[newsockfd].substr(requist_info[newsockfd].find("Content-Length: ") + 16, 9).c_str(), nullptr, 0);
            // if (requist_info[newsockfd].find("GET") != std::string::npos)
            // {
            //     return (0);
            // }
            // while (true)
            // {
                // bytes_received = recv(newsockfd, buffer, 1024, 0);
                // if (bytes_received <= 0)
                // {
                //     close(newsockfd);
                //    std::cout<<"Failed to read from socket"<<std::endl;
                //    exit(1);
                // }
                // requist_info[newsockfd] += std::string(buffer);
                // if ((content_len +  header_end) >= requist_info[newsockfd].size())
                // {
                //     read_info[sockfd] = true;
                //     //break;
                // }    
            // }
            //std::size_t Transfer_encoding = requist_info[newsockfd].find("Transfer-Encoding: chunked");
            //read_info.insert(std::make_pair(newsockfd, 0));
            // if (Transfer_encoding != std::string::npos)
            // {
            //     requist_info[newsockfd] = join_chunked(requist_info[newsockfd], newsockfd); 
            // }
            return (0);
        }

        std::string http_sever::join_chunked(const std::string& chunked_msg, int sockfd)
        {
            std::string result = "";
            std::size_t pos = 0;
            (void) sockfd;
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
                    read_info[sockfd] = true;
                    break;
                }

                // Append the chunk data to the result
                result += chunked_msg.substr(len_pos + 2, len);//use append() function
                pos = len_pos + 2 + len + 2;
            }

            return result;
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
        // if (sent_data.find(socket) == sent_data.end())
        // {
        //     std::cout << "Response Header:\n";
        //     if (requist_info[socket].size() < 1000)
        //     {
        //         std::cout << "[" << requist_info[socket] << "]\n";
        //     }
        //     else
        //     {
        //         std::cout << "[" << requist_info[socket].substr(0, 1000) << "..." << requist_info[socket].substr(requist_info[socket].size() - 10, 15) << "]\n";
        //     }
       // }

        // Send the data to the client
        std::string data_to_send = response;//requist_info[socket].substr(sent_data[socket], 1024);
        long bytes_sent = send(socket, data_to_send.c_str(), data_to_send.size(), 0); 
        //read_info.insert(std::make_pair(socket, 0));
        // Check for errors while sending data
        if (bytes_sent == -1)
        {
            std::cout << "Error: Failed to send data to the socket\n";
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
    int http_sever::accept_connection(int sockfd)
    {
        // Accepts a connection on a socket.
        std::cout<<"///////////////////////"<<std::endl;
        int sockfd_client = accept(sockfd, (struct sockaddr *) &sock.serv_addr, &sock.sock_addr_len);
        if (sockfd_client < 0) 
        {
           std::cout<<"accepting connection"<<std::endl;
           exit(1);
        }
        return (sockfd_client);
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