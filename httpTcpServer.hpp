/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpTcpServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:57:52 by yismaili          #+#    #+#             */
/*   Updated: 2023/03/22 15:11:51 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDED_HTTP_TCP_SERVER
# define INCLUDED_HTTP_TCP_SERVER

#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

namespace http{
    class tcpServer{
        public:
         tcpServer(int port) : sockfd(-1), port(port) {}
         ~tcpServer(){}
           bool Start() {
        
// Socket System Call

        //Creates a socket and returns a Socket Descriptor (like file descriptor) which is an integer value
        sockfd = socket(AF_INET, SOCK_STREAM, 0); 
        
        //domain: specifies the communication domain, such as AF_INET for IPv4 or AF_INET6 for IPv6.
        //type: specifies the type of socket, such as SOCK_STREAM for a TCP socket or SOCK_DGRAM for a UDP socket.
        /*protocol: specifies the protocol to be used with the socket, such as IPPROTO_TCP for TCP or IPPROTO_UDP for UDP. 
        This argument is usually set to 0,which allows the operating system to choose the 
        appropriate protocol based on the socket type and domain.*/
        
        if (sockfd < 0) {
            std::cerr << "Error opening socket" << std::endl;
            return false;
        }  
        // AF stands for Address Family and PF stands for Protocol Family
       // This construct holds the information about the address family, port number, Internet address
        serv_addr.sin_family = AF_INET; // Address family // IPv4 Internet protocols    
        serv_addr.sin_addr.s_addr = INADDR_ANY;  // Internet address
        serv_addr.sin_port = htons(port); // Port number // Network to Host Shor

// Bind System Call
        //associate a socket with a specific address and port number
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            //the second is a pointer to a struct sockaddr structure that contains the address
            std::cerr << "Error binding socket" << std::endl;
            return false;
        }
//Listen() System Call
        //prepares a connection-oriented server to accept client connections.
        listen(sockfd, 5);
        //The second parameter specifies the number of requests that the system queues before it executes the accept()
        return true;
    }

     void Run() {
    while (true) {
        //structure that is used to store the information about the client's address
        struct sockaddr_in cli_addr;
        //represent the length of a socket address
        socklen_t clilen = sizeof(cli_addr);
        // Accepts a connection on a socket.
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        // Read incoming request data
        char buffer[1024];
        ssize_t bytes_read = read(newsockfd, buffer, sizeof(buffer));
        if (bytes_read == -1) {
            std::cerr << "Failed to read from socket" << std::endl;
            close(newsockfd);
            continue;
        }else{
            int i = 0;
            while (buffer[i]){
                 write(1, &buffer[i++], 1);
            }
            
        }

        // Parse incoming request data
        std::string request_str(buffer, bytes_read);
        // parse request_str to extract requested resource and any parameters or headers

        // Insert html page
        std::ostringstream response;
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/html; charset=UTF-8\r\n";
        response << "\r\n";
        response << "<html><body><h1>Hello younes </h1>";
        response << "<h1>from HTTP server!</h4>";
        response << "</body></html>";
        std::string response_str = response.str();

        // Send response to client
        int n = send(newsockfd, response_str.c_str(), response_str.length(), 0);
        if (n < 0) {
            std::cerr << "Error writing to socket" << std::endl;
        }

        close(newsockfd);
    }
    }
        private:
            int sockfd;
            int port;
            struct sockaddr_in serv_addr;
    };
}

#endif