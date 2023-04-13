/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:42 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/13 00:32:36 by yismaili         ###   ########.fr       */
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
    
    sockets &sockets::init_data(int port_, std::string ip_add)
    {
        sockfd =  -1;
        port = port_;
        sock_addr_len = 0;
        ip_addr = ip_add;
        // AF stands for Address Family and PF stands for Protocol Family
        // This construct holds the information about the address family, port number, Internet address
        serv_addr.sin_family = AF_INET; // Address family // IPv4 Internet protocols    !!!get add info
        serv_addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());  // Internet "address inet_addr(ip_addr.c_str());"
        serv_addr.sin_port = htons(port); // Port number // Network to Host Shor
        if(start_server() == false){
            std::cout<<"Failed to start server "<<std::endl;
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
            
        sockaddr_in &sockets::git_serv_addr(){
            return (serv_addr);
        }
        bool sockets::start_server() 
        {
            // Socket System Call
            //Creates a socket and returns a Socket Descriptor (like file descriptor) which is an integer value
            sockfd = socket(AF_INET, SOCK_STREAM, 0); 
            //domain: specifies the communication domain, such as AF_INET for IPv4 or AF_INET6 for IPv6.
            //type: specifies the type of socket, such as SOCK_STREAM for a TCP socket or SOCK_DGRAM for a UDP socket.
            /*protocol: specifies the protocol to be used with the socket, such as IPPROTO_TCP for TCP or IPPROTO_UDP for UDP. 
            This argument is usually set to 0,which allows the operating system to choose the 
            appropriate protocol based on the socket type and domain.*/
            if (sockfd < 0) {
                return (false);
            }
            int optval = 1;
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
                //SOL_SOCKET: specifies the level at which the option is defined. In this case, it is the socket level.
                //SO_REUSEADDR: the option name. It is used to enable reuse of local addresses.
                //&optval: a pointer to the buffer that contains the value of the option you want to set
                perror("setsockopt failed");
                exit(EXIT_FAILURE);
            }
            sock_addr_len = sizeof(serv_addr);
            //fcntl(sockfd, F_SETFL, O_NONBLOCK);
            // Bind System Call
            //associate a socket with a specific address and port number
            if (bind(sockfd, (struct sockaddr *) &serv_addr, sock_addr_len) < 0) {
                //the second is a pointer to a struct sockaddr structure that contains the address
                return (false);
            }
            if (listen(sockfd, SOMAXCONN) < 0){
            //The second parameter specifies the number of requests that the system queues before it executes the accept()
                std::cout<<"Socket listen failed"<<std::endl;
                exit(1);
            }
            // std::cout<<" Listening on adress ... "<<std::endl;
            return true;
        }
         int sockets::recv_data(int newsockfd)
        {
            char buffer[1024] = {0};
            int bytes_received;

            bytes_received = recv(newsockfd, buffer, 1024, 0);
            requist_info[newsockfd] += std::string(buffer);
            std::size_t header_end = requist_info[newsockfd].find("0\r\n\r\n");//used this string ::nops check the end!!!!!!
            std::size_t content_len = std::strtol(requist_info[newsockfd].substr(requist_info[newsockfd].find("Content-Length: ") + 16, 9).c_str(), nullptr, 0);
            if (requist_info[newsockfd].find("GET") != std::string::npos)
            {
                return (0);
            }
        std::size_t check = 0;
            while (true)
            {
                bytes_received = recv(newsockfd, buffer, 1024, 0);
                if (bytes_received <= 0)
                {
                    close(newsockfd);
                   std::cout<<"Failed to read from socket"<<std::endl;
                   exit(1);
                }
                requist_info[newsockfd] += std::string(buffer);
                check += 1024;
                if ((content_len +  header_end) <= check){
                    read_info[sockfd] = true;
                    break;
                }    
            }
            std::size_t Transfer_encoding = requist_info[newsockfd].find("Transfer-Encoding: chunked");
           // read_info.insert(std::make_pair(newsockfd, 0));
            if (Transfer_encoding != std::string::npos)
            {
                requist_info[newsockfd] = join_chunked(requist_info[newsockfd], newsockfd); 
            }
            return (0);
        }

        std::string sockets::join_chunked(const std::string& chunked_msg, int sockfd)
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
        
    std::string sockets::build_response()
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
    
    int sockets::send_data(int socket)
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
}