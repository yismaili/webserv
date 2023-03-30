/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:57:52 by yismaili          #+#    #+#             */
/*   Updated: 2023/03/30 00:47:26 by yismaili         ###   ########.fr       */
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
#include <map>
#include <vector>
#include "tcpServer.hpp"
#include <iterator>
 #include <sys/select.h>

#define num_of_req 20;
namespace http{
    class http_sever{
        public:
         http_sever(std::vector<int> port_, std::string ip_add) :tcp(){
            int i = 0;
            std::vector<int>::iterator it = port_.begin();
           while (it < port_.end()){
                other_sock.push_back(tcp.init_data(*it, ip_add));
                std::cout<<other_sock.begin()->sockfd<<std::endl;
                it++;
            } 
         }
         
         ~http_sever(){
           std::map<int, http::tcpServer>::iterator it = sock_inf.begin();
           while (it != sock_inf.end()){
                closeServer(it->first);
                it++;
           }
         }
         
    int accept_connection(int sockfd){
        // Accepts a connection on a socket.
        int newsockfd;
        newsockfd = accept(sockfd, (struct sockaddr *) &tcp.serv_addr, &tcp.sock_addr_len);
        sock_inf.insert(std::make_pair(newsockfd, tcp));
        if (newsockfd < 0) {
            exit_withError(" accepting connection");
        }
        return (newsockfd);
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
            std::string response_str = response.str();
            return (response_str);
    }
    
    void send_response(int newsockfd){
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
    // Create a set of file descriptors to monitor with select
        fd_set master_fds;
        FD_ZERO(&master_fds);
        std::vector<http::tcpServer>::iterator it = other_sock.begin();
        while (it != other_sock.end()){
            http::tcpServer& sock = *it;
            FD_SET(sock.sockfd, &master_fds);
            it++;
        }
    // Main server loop
        while (true) {
            // Create a copy of the master set to pass to select
            fd_set read_fds = master_fds;
            
            // Wait for activity on any of the monitored sockets
            int activity = select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);
            if (activity < 0) {
                exit_withError("select");
            }
            // Check each socket for activity
            std::vector<http::tcpServer>::iterator it_ = other_sock.begin();
            while( it_ != other_sock.end()) {
                if (FD_ISSET(it_->sockfd, &read_fds)) {
                        // Accept a new connection and add the new socket to the master set
                        int clint = accept_connection(it_->sockfd);
                        std::map<int, http::tcpServer>::iterator it_map = sock_inf.find(it_->sockfd);
                std::cout<<"-----"<<it_map->first<<"-----"<<std::endl;
                         
                        FD_SET(clint, &read_fds);
                        // Read the client request and send a response  
                        send_response(clint);
                        // // read_request(it_->sockfd);
                        close(it_->sockfd);
                        FD_CLR(it_->sockfd, &master_fds);
                }
                it_++;
            }
        }
}


    void read_request(int newsockfd){
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
    
        //     void read_request(int sockfd) {
        //             http::tcpServer sock;
        //             char buffer[256];
        //             int n = read(sockfd, buffer, 255);
        //             if (n < 0) {
        //                 exit_withError(" reading from socket");
        //             }
        //             if (n == 0) {
        //                 close(sockfd);
        //                 sock_inf.erase(sockfd);
        //                 return;
        //             }
        //             buffer[n] = '\0';
        //             std::string request(buffer);
        //             print_message("Received request from client:");
        //             std::cout << request << std::endl;

        //             // check if request is a GET request
        //             std::istringstream iss(request);
        //             std::string method;
        //             std::string path;
        //             std::string version;
        //             iss >> method >> path >> version;
        //             if (method != "GET") {
        //                 print_message("Error: unsupported HTTP method");
        //                 return;
        //             }

        //             // check if path is a valid file path
        //             path = path.substr(1); // remove leading slash
        //             if (path == "") {
        //                 path = "index.html"; // default file
        //             }
        //             std::ifstream file(path);
        //             if (!file.good()) {
        //                 print_message("Error: invalid file path");
        //                 return;
        //             }

        //             // send response to client
        //             send_response(sockfd);
        // }

            void print_message(const std::string &message){
                    std::cout << message << std::endl;
            }
                
            void exit_withError(const std::string &errormessage){
                print_message("ERROR: " + errormessage);
                exit(1);
            }

            void closeServer(int newsockfd){
                close(tcp.sockfd);
                close(newsockfd);
                exit(1);
            }
            
            private:
                http::tcpServer tcp;
        std::map<int, http::tcpServer> sock_inf;
        std::vector<http::tcpServer> other_sock;
        
    };
}

#endif

    