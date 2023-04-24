/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:23 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/24 16:29:10 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/http_server.hpp"

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
        int poll_ret, new_socket, recv_ret;
        unsigned long i = 0;
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
                if (clients[i].revents & POLLIN)
                {
                    if (is_server(clients[i].fd))
                    {
                        // Accept incoming connection
                        new_socket = accept_connection(clients[i].fd);
                        std::cout << "ACCEPTING...\n";
                        // Add new socket to poll list
                        pollfd new_client_pollfd;
                        new_client_pollfd.fd = new_socket;
                        new_client_pollfd.events = POLLOUT | POLLIN;
                        read_info.insert(std::make_pair(new_socket, false));
                        tmp_requist.insert(std::make_pair(new_socket, ""));
                        clients.push_back(new_client_pollfd);
                    }
                    else
                    {
                        recv_ret = recv_data(clients[i].fd);
                        if (!recv_ret)
                        {
                            unchunk(clients[i].fd);
                        }
                    }
                }
                if (clients[i].revents & POLLOUT && read_info[clients[i].fd] == true)
                {
                    std::cout<<requist[clients[i].fd]<<std::endl;
                    std::vector<pollfd>::iterator it = clients.begin() + i;
                    send_data(clients[i].fd);
                    clients[i].events = POLLIN;
                    clients.erase(it);
                    i--;
                }
                if (clients[i].revents & POLLERR)
                {
                    std::cout<<"error\n"<<std::endl;
                    exit(1);
                }
                i++;
            }
        }   
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
    
    int http_sever ::transfer_encoding_chunked(int sockfd)
    {
       
        if (tmp_requist[sockfd].find("Content-Length: ") == std::string::npos)
        {
            if (tmp_requist[sockfd].find("Transfer-Encoding: chunked") != std::string::npos)
            {
                if (tmp_requist[sockfd].find("0\r\n\r\n") != std::string::npos)
                    return (1);
                else
                    return (0);
            }
            else
            {
                read_info[sockfd] = true;
                return (0);           
            }
        }
        return (2);
    }

    int http_sever::recv_data(int sockfd)
    {
        char buffer[1024] = {0};
        int bytes_received;
        std::size_t header_end = 0;
        std::size_t content_len = 0;
        static  std::size_t cont_ = 0;
            
        bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0)
        {
            close(sockfd);
            std::cout<<"connection was closed\n";
            return (-2);
        }
        tmp_requist[sockfd].append(std::string(buffer, bytes_received));
        if (tmp_requist[sockfd].find("\r\n\r\n") != std::string::npos)
        {
            if (transfer_encoding_chunked(sockfd) == 1)
            {
                return (0);
            }
            else if (transfer_encoding_chunked(sockfd) == 0)
            {
                return (1);
            }
            else if (transfer_encoding_chunked(sockfd) == 2)
            {
                cont_+= bytes_received;
                header_end = tmp_requist[sockfd].find("\r\n\r\n");
                content_len = std::strtol(tmp_requist[sockfd].substr(tmp_requist[sockfd].find("Content-Length: ") + 16, 9).c_str(), nullptr, 0);
                if ((content_len +  header_end + 4) <= tmp_requist[sockfd].size())
                {
                    read_info[sockfd] = true;
                    return (0);
                }
                else
                {
                    return (1);
                } 
            }
        }
        return (1);
    }

    void http_sever ::unchunk(int sockfd)
    {
        std::size_t Transfer_encoding = tmp_requist[sockfd].find("Transfer-Encoding: chunked");
        if (Transfer_encoding != std::string::npos && Transfer_encoding < tmp_requist[sockfd].find("\r\n\r\n"))
        {
            requist[sockfd] = join_chunked(tmp_requist[sockfd], sockfd);
           return ;
        }
        requist[sockfd] = tmp_requist[sockfd];
    }
    
    std::string http_sever::join_chunked(const std::string &data, int sockfd) 
    {
        std::string result = "";
        std::size_t sizeof_chunk;
        std::string	body = "";
        std::string	chunks = ""; 
        std::string	subchunk = "";
        std::size_t header_end;
        std::size_t  pos; 
        
        // Find the end of the headers
        header_end = data.find("\r\n\r\n");
        // Append the headers to the result
        result = data.substr(0, header_end);
        result += "\r\n\r\n";
        chunks = data.substr(data.find("\r\n\r\n") + 4, data.size() - 1);
        subchunk = chunks.substr(0, 20);
        sizeof_chunk =  strtol(subchunk.c_str(), NULL, 16);
        pos = 0;
        while (true)
        {
            pos = chunks.find("\r\n",  pos);
            result += chunks.substr(pos += 2, sizeof_chunk);
            pos += sizeof_chunk + 2;
            subchunk = chunks.substr(pos, 20);
            sizeof_chunk = strtol(subchunk.c_str(), NULL, 16);
            if (sizeof_chunk == 0)
            {
                result += "\r\n\r\n";
                read_info[sockfd] = true;
                break;
            }
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

        //If this is the first time sending data to the socket, print the response header
        if (sent_data.find(socket) == sent_data.end())
        {
            if (tmp_requist[socket].size() < 1024)
            {
                std::cout << " Response  sended "<<std::endl;
            }
            else
            {
                std::cout << " Response is sending "<<std::endl;
            }
       }

        // Send the data to the client
        std::string data_to_send = response;//tmp_requist[socket].substr(sent_data[socket], 1024);
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
            if (sent_data[socket] >= tmp_requist[socket].size())
            {
                tmp_requist.erase(socket);
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
        int sockfd_client = accept(sockfd, (struct sockaddr *) NULL, NULL);
        if (sockfd_client < 0) 
        {
           std::cout<<"accepting connection"<<std::endl;
           exit(1);
        }
        return (sockfd_client);
    }
}
