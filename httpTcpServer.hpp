/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpTcpServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:57:52 by yismaili          #+#    #+#             */
/*   Updated: 2023/03/22 22:53:53 by yismaili         ###   ########.fr       */
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
#include<fstream>
#include <arpa/inet.h>

#define num_of_req 20;
namespace http{
    class tcpServer{
        public:
         tcpServer(int port_, std::string ip_add) : sockfd(-1), port(port_),  newsockfd(),sock_addr_len(0), ip_addr(ip_add){
            // AF stands for Address Family and PF stands for Protocol Family
            // This construct holds the information about the address family, port number, Internet address
            serv_addr.sin_family = AF_INET; // Address family // IPv4 Internet protocols    
            serv_addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());  // Internet "address inet_addr(ip_addr.c_str());"
            serv_addr.sin_port = htons(port); // Port number // Network to Host Shor
            if(start_server() == false){
                print_message("Failed to start server ");
            }
         }
         ~tcpServer(){
            closeServer();
         }
        
        bool start_server() {
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
    // Bind System Call
            //associate a socket with a specific address and port number
            if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
                //the second is a pointer to a struct sockaddr structure that contains the address
                return (false);
            }
            return true;
    }
    
    void accept_connection(){
        // Accepts a connection on a socket.
       newsockfd = accept(sockfd, (struct sockaddr *) &serv_addr, &sock_addr_len);
        if (newsockfd < 0) {
            exit_withError(" accepting connection");
        }
    }
    
    std::string build_response(){
         // Insert html page or ...
            std::ostringstream response; //create the output string stream
            response << "HTTP/1.1 200 OK\r\n";
            response << "Content-Type: text/html; charset=UTF-8\r\n";
            response << "\r\n";
            response << "<html><body><h1>Hello younes </h1>";
            response << "<h1>from HTTP server!</h4>";
            response << "</body></html>";
            // std::ifstream file("index.html"); // open the HTML file
            // if (file) { // check if the file is open
            //     response << file.rdbuf(); // copy the file contents to the string stream
            //     file.close(); // close the file
            // } else {
            //     std::cerr << "Failed to open file." << std::endl;
            //     return (response.str()); // exit the program with an error status
            // }
            // print the contents of the string stream to the console
            // std::cout << response.str() << std::endl;
            std::string response_str = response.str();
            return (response_str);
    }
    
    void send_response(){
        long byte_send;
         std::string response = build_response();
        byte_send = send(newsockfd, response.c_str(), response.length(), 0);
        if (byte_send > 0){
            print_message("Server Response sent to client");
        }else{
            print_message("Error: sending response to client");
        }
    }
    
    void run() {
     //Listen() System Call
        //prepares a connection-oriented server to accept client connections.
        if (listen(sockfd, 5) < 0){
        //The second parameter specifies the number of requests that the system queues before it executes the accept()
            exit_withError("Socket listen failed");
        }
        print_message(" Listening on adress ... ");
        while (true) {
            print_message("Waiting for a new connection ...");
            accept_connection();
            read_request();
            send_response();
            close(newsockfd);
        }
    }
    
    void read_request(){
        // Read incoming request data
        char buffer[1024];
        int  bytes_received = read(newsockfd, buffer, sizeof(buffer));
        if (bytes_received == -1) {
            close(newsockfd);
            exit_withError("Failed to read from socket");
        }
        // Parse incoming request data
        std::string request_str(buffer, bytes_received);
        std::cout<<request_str<<std::endl;
    }
    
    void print_message(const std::string &message){
            std::cout << message << std::endl;
    }
        
    void exit_withError(const std::string &errormessage){
        print_message("ERROR: " + errormessage);
        exit(1);
    }

    void closeServer(){
        close(sockfd);
        close(newsockfd);
        exit(1);
    }
    
    private:
        int sockfd;
        int port;
        int newsockfd;
        struct sockaddr_in serv_addr;
        std::string serv_message;
        unsigned int sock_addr_len;
        std::string ip_addr;   
    };
}

#endif