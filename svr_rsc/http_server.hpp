/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:57:52 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/04 00:14:43 by yismaili         ###   ########.fr       */
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
#include <ctype.h>

namespace http{
    class http_sever{
        public:
            http_sever(std::vector<int> port_, std::string ip_add) :tcp(){
                int i = 0;
                std::vector<int>::iterator it = port_.begin();
                
                while (it < port_.end()){
                        other_sock.push_back(tcp.init_data(*it, ip_add));
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
                if (byte_send < 0){
                    print_message("Error: sending response to client");
                }
            }
    
            void run() {
            // Create a set of file descriptors to monitor with select
                fd_set readmaster_fds;
                fd_set writemaster_fds;
                std::vector<http::tcpServer>::iterator it = other_sock.begin();
        
                FD_ZERO(&readmaster_fds);
                FD_ZERO(&writemaster_fds);
                while (it != other_sock.end()){
                    http::tcpServer& sock = *it;
                    FD_SET(sock.sockfd, &readmaster_fds);
                    FD_SET(sock.sockfd, &writemaster_fds);
                    it++;
                }
            // Main server loop
                while (true) {
                    // Create a copy of the master set to pass to select
                    fd_set read_fds = readmaster_fds;
                    fd_set wirte_fds = writemaster_fds;
                    // Wait for activity on any of the monitored sockets
                    int activity = select(FD_SETSIZE, &read_fds, &wirte_fds, NULL, NULL);
                    if (activity < 0) {
                        exit_withError("select");
                    }
                    // Check each socket for activity
                    std::vector<http::tcpServer>::iterator it_ = other_sock.begin();
                    while( it_ != other_sock.end()) {
                        if (FD_ISSET(it_->sockfd, &read_fds)) {
                            if (!is_server(it_->sockfd)){
                                // Accept a new connection and add the new socket to the master set
                                read_request(clint);
                                send_response(clint);
                                close(clint);
                                FD_CLR(clint, &readmaster_fds);
                            }
                            else if (is_server(it_->sockfd)){
                                // Accept a new connection and add the new socket to the master set
                                clint = accept_connection(it_->sockfd);
                                FD_SET(clint, &read_fds);
                            }
                            // Read the client request and send a response  
                            read_request(clint);
                            send_response(clint);
                            //close(clint);
                            FD_CLR(clint, &readmaster_fds);
                        }
                        it_++;
                    }
                }
                }
                
                int is_server(int sock){
                    std::vector<http::tcpServer>::iterator it = other_sock.begin();
                    while (it != other_sock.end()){
                        // std::cout << it->sockfd << " != " << sock << std::endl;
                        if (it->sockfd == sock){
                            return (1);
                        }
                        it++;
                    }
                    return (0);
                }
                
                void read_request(int newsockfd){
                    // Read incoming request data
                    char buffer[1024] = {0};
                    int bytes_received = recv(newsockfd, buffer, 1024, 0);
                    if (bytes_received == -1) {
                        close(newsockfd);
                        exit_withError("Failed to read from socket");
                    }
                     //std::cout<<buffer<<std::endl; 
                    char encoding[] = "POST";
                    if (check_encoding(buffer, encoding)){
                        unchunk(buffer);
                    }
                }
                
               int	ft_strncmp(const char *s1, const char *s2, size_t n)
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
                
                int str_len(char *str){
                    int i = 0;
                    while (str[i]){
                        i++;
                    }
                    return (i);
                }
                
                int check_encoding(char *requiste, char *encoding){
                   // std::cout<<requiste<<std::endl;
                    if (!ft_strncmp(requiste, encoding, str_len(encoding))){
                        return (1);
                    }
                    else{
                        return (0);
                    }
                }
                
                std::string unchunk(const char *chunck) {
                    std::cout<<chunck<<std::endl;
                    return(chunck);
                }
                
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
                int clint;
                http::tcpServer tcp;
                std::map<int, http::tcpServer> sock_inf;
                std::vector<http::tcpServer> other_sock;
                std::string request_mssg;
                std::string request_str;
                 std::stringstream ss;
            
        };
}
#endif

    