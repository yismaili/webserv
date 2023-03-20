/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 19:21:09 by yismaili          #+#    #+#             */
/*   Updated: 2023/03/19 12:12:39 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class HttpServer {
public:
    HttpServer(int port) : sockfd(-1), port(port) {}

    bool Start() {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error opening socket" << std::endl;
            return false;
        }

        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Error binding socket" << std::endl;
            return false;
        }

        listen(sockfd, 5);

        return true;
    }

    void Run() {
        while (true) {
            struct sockaddr_in cli_addr;
            socklen_t clilen = sizeof(cli_addr);
            int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) {
                std::cerr << "Error accepting connection" << std::endl;
                continue;
            }

            std::ostringstream response;
            response << "HTTP/1.1 200 OK\r\n";
            response << "Content-Type: text/html; charset=UTF-8\r\n";
            response << "\r\n";
            response << "<html><body><h1>Hello younes from HTTP server!</h1></body></html>";

            std::string response_str = response.str();
            int n = write(newsockfd, response_str.c_str(), response_str.length());
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

int main() {
    HttpServer server(8080);
    if (!server.Start()) {
        std::cerr << "Error starting server" << std::endl;
        return 1;
    }

    server.Run();

    return 0;
}
