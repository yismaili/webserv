#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "cgi.hpp"

#define PORT 80
int main(int ac, char **av)
{
    if(ac != 3)
    {
        std::cout << "error\n";
        return (1);
    }
    
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char *hello = "Hello from server";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        //write(new_socket , hello , strlen ));
        //printf("------------------Hello message sent-------------------\n");
       // request r(buffer);
        //std::cout << "query :::::::::::::::::::::::::::::::::::::::::::::: " <<r.get_query() << "\n";
        std::string content = run_cgi(av[1], av[2]);
        std::string header = "HTTP/1.1 200 OK\n";
        header = header + content ;
        std::cout << content;
        send(new_socket, header.c_str(), content.length(), 0);
        // write(new_socket, content.c_str(), content.size());
        close(new_socket);
    }

    return 0;
}