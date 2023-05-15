/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:23 by yismaili          #+#    #+#             */
/*   Updated: 2023/05/15 17:24:04 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/http_server.hpp"
#include "../prs_rsc/server.hpp"

namespace http{
    
   http_sever::http_sever(std::vector<server> conf_) :sock()
   {
        for (size_t i = 0; i < conf_.size(); i++)
        {
            for (size_t j = 0; j < conf_[i]._listen.size(); j++)
            {
                socket_id.push_back(sock.init_data(conf_[i]._listen[j], conf_[i].get_host(), i));
            }
        }
        conf = conf_;
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
    
    std::vector<http::sockets>::iterator http_sever::find_conf(int sockfd) 
    {
        std::vector<http::sockets>::iterator it = socket_id.begin();
        while (it != socket_id.end())
        {
            if (it->sockfd == sockfd)
            {
                return (it);
            }
            it++;
        }
        return (socket_id.begin());
    }
    
    void http_sever::run() 
    {
        int poll_ret, new_socket, recv_ret, sent_ret;
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
                if (clients[i].revents & POLLERR)
                {
                    std::vector<pollfd>::iterator it = clients.begin() + i;
                    std::cout<<"error\n"<<std::endl;
                    close(clients[i].fd);
                    clients.erase(it);
                    i--;
                }
                else if (clients[i].revents & POLLIN)
                {
                    if (is_server(clients[i].fd))
                    {
                        // Accept incoming connection
                        new_socket = accept_connection(clients[i].fd);
                        conf_fd.insert(std::make_pair(new_socket, find_conf(clients[i].fd)));
                        std::cout << "ACCEPTING...\n";
                        // Add new socket to poll list
                        pollfd new_client_pollfd;
                        new_client_pollfd.fd = new_socket;
                        new_client_pollfd.events = POLLIN;
                        read_info.insert(std::make_pair(new_socket, false));
                        requist_data.insert(std::make_pair(new_socket, ""));
                        clients.push_back(new_client_pollfd);
                    }
                    else
                    {
                        recv_ret = recv_data(clients[i].fd);
                        if (recv_ret == -2)
                        {
                            requist_data[clients[i].fd] = build_response();
                            send_data(clients[i].fd);
                            requist_data[clients[i].fd].erase();
                            close(clients[i].fd);
                            std::vector<pollfd>::iterator it = clients.begin() + i;
                            clients.erase(it);
                            i--;
                        }
                        else if (!recv_ret)
                        {
                            
                        //     std::cout << "-------WRITEING.....-----\n";
                        //    std::cout<<"-----"<< requist_data[clients[i].fd] <<std::endl;
                        //       exit(1);
                            unchunk(clients[i].fd);
                            clients[i].events = POLLOUT;
                        }
                    }
                }
               else if (clients[i].revents & POLLOUT && read_info[clients[i].fd] == true)
                {
                   std::size_t Connection = requist_data[clients[i].fd].find("Connection: keep-alive");
                    std::vector<pollfd>::iterator it = clients.begin() + i;
                    sent_ret = send_data(clients[i].fd);
                    if (sent_ret == 1){
                        clients[i].events = POLLOUT;
                    }
                    if (sent_ret == 0)
                    {
                        clients[i].events = POLLIN;
                        if (Connection == std::string::npos)
                        {
                            close(clients[i].fd);
                        }
                        clients.erase(it);
                        read_info[clients[i].fd] = 0;
                        i--;
                    }
                    else if (sent_ret == -2)
                    {
                        close(clients[i].fd);
                        clients.erase(it);
                        i--;
                    }
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
        std::size_t content_length = requist_data[sockfd].find("Content-Length: ");
        std::size_t transfer_encoding = requist_data[sockfd].find("Transfer-Encoding: chunked\r\n");
        // if ((content_length == std::string::npos && transfer_encoding == std::string::npos ) || (content_length != std::string::npos && transfer_encoding != std::string::npos ))
        // {
        //     return (-2);
        // }
        if (content_length != std::string::npos && transfer_encoding != std::string::npos)
        {
            if (requist_data[sockfd].find("0\r\n\r\n") != std::string::npos)
                return (1);
            else
                return (0);
        }
        if (content_length == std::string::npos)
        {
            if (transfer_encoding != std::string::npos)
            {
                if (requist_data[sockfd].find("0\r\n\r\n") != std::string::npos)
                {
                    return (1);
                }
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
        conf_fd[sockfd]->content_length = 0;
        bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0)
        {
            close(sockfd);
            std::cout<<"connection was closed\n";
            return (-2);
        }
        requist_data[sockfd].append(std::string(buffer, bytes_received));
        if (requist_data[sockfd].find("\r\n\r\n") != std::string::npos)
        {
            std::size_t content_length = requist_data[sockfd].find("Content-Length: ");
             std::size_t transfer_encoding = requist_data[sockfd].find("Transfer-Encoding: chunked");
            int ret_transfer = transfer_encoding_chunked(sockfd);
            if (content_length != std::string::npos && transfer_encoding == std::string::npos)
            {
                header_end = requist_data[sockfd].find("\r\n\r\n");
                content_len = std::strtol(requist_data[sockfd].substr(requist_data[sockfd].find("Content-Length: ") + 16, 9).c_str(), nullptr, 0);
                conf_fd[sockfd]->content_length = content_len; 
                if ((content_len +  header_end + 4) <= requist_data[sockfd].size())
                {
                    // std::cout<<"2-----"<<requist_data[sockfd].size()<<std::endl;
                    //  std::cout<<"3-----"<<content_len +  header_end + 4<<std::endl;
                    read_info[sockfd] = true;
                    // exit(1);
                    return (0);
                }
                else
                {
                    return (1);
                } 
            }
           else if (ret_transfer == -2)
            {
                return (-2);
            }
            else if (ret_transfer == 1)
            {
                std::cout<<"1\n";
                return (0);
            }
            else if (ret_transfer == 0)
            {
                if (read_info[sockfd] == true)
                {
                    return (0);
                }
                return (1);
            }
        }
        return (1);
    }

    void http_sever ::unchunk(int sockfd)
    {
        std::size_t Transfer_encoding = requist_data[sockfd].find("Transfer-Encoding: chunked");
        if (Transfer_encoding != std::string::npos && Transfer_encoding < requist_data[sockfd].find("\r\n\r\n"))
        {
            requist_data[sockfd] = join_chunked(requist_data[sockfd], sockfd);
            std::size_t header_end = requist_data[sockfd].find("\r\n\r\n");
            conf_fd[sockfd]->content_length = requist_data[sockfd].size() - (header_end + 4);
            // std::cout<<"********"<<conf_fd[sockfd]->content_length<<std::endl;
            //  std::cout<<"2-----"<<requist_data[sockfd]<<std::endl;
           // exit(1);
        }
            //  std::cout<<"2-----"<<requist_data[sockfd]<<std::endl;
            //  exit(1);
             std::cout<<"********"<<conf_fd[sockfd]->content_length<<std::endl;
        request req(requist_data[sockfd], conf_fd[sockfd]->content_length);
        Respond   res(req, conf_fd[sockfd]->index);
       requist_data[sockfd] =  res.response_root(conf);
    // requist_data[sockfd] =  build_response();
       std::cout << "A F T E R      R E S P O N S E" << std::endl;

       //std::cout<<"-----"<< requist_data[sockfd] <<std::endl;     
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
        result.append(data.substr(0, header_end));
        result.append("\r\n\r\n");
        chunks = data.substr(data.find("\r\n\r\n") + 4, data.size() - 1);
        subchunk = chunks.substr(0, 9);
        sizeof_chunk =  strtol(subchunk.c_str(), NULL, 16);
        pos = 0;
        while (true)
        {
            pos = chunks.find("\r\n",  pos);
            result.append(chunks.substr(pos += 2, sizeof_chunk));
            pos += sizeof_chunk + 2;
            try
            {
            
            subchunk = chunks.substr(pos , 9);

            }
            catch(const std::exception& e)
            {
                   std::cout << "-------..abbourt...-----\n";
                exit(1);
                std::cerr << e.what() << '\n';
            }
            
            sizeof_chunk = strtol(subchunk.c_str(), NULL, 16);
            if (sizeof_chunk == 0 && requist_data[sockfd].size() )
            {
                result.append("\r\n\r\n");
                read_info[sockfd] = true;
                break;
            }   
        }
        return result;
    }
    
    std::string http_sever::generate_cookie_value(int length) 
    {
        int i;
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 1);

        std::string result(length, '\0');
        i = 0;
        while(i < length) 
        {
            result[i++] = alphanum[dis(gen)];
        }

        return "cookie =" + result;
    }
    
    std::string http_sever::build_response()
    {
        std::ostringstream response; //create the output string stream
        
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/html; charset=UTF-8\r\n";
        response <<  "Content-Length: 75\r\n";
        response << "\r\n";
        response << "<html><body><h1>Hello younes </h1>";
        response << "<h1>from HTTP server!</h4>";
        response << "</body></html>";
        
        std::string response_str = response.str();
        return (response_str);
    }
    
    int http_sever::send_data(int socket)
    {
        // Keep track of how much data has been sent to a particular socket
        static std::map<int, std::size_t> sent_data;

        //If this is the first time sending data to the socket, print the response header
        if (sent_data.find(socket) == sent_data.end())
        {
            std::cout << " Response  sended "<<std::endl;
        }
        // Send the data to the client
        std::string data_to_send = requist_data[socket].substr(sent_data[socket], 1024);
        long bytes_sent = send(socket, data_to_send.c_str(), data_to_send.size(), 0);
        // Check for errors while sending data
        if (bytes_sent == -1)
        {
            std::cout << "Error: Failed to send data to the socket\n";
            close(socket);
            sent_data[socket] = 0;
            return (-2);
        }
        else
        {
            // Update the amount of data that has been sent to the socket
            sent_data[socket] += bytes_sent;
            // If all data has been sent, erase the request information and return 0
            if (sent_data[socket] >= requist_data[socket].size())
            {
                requist_data.erase(socket);
                sent_data[socket] = 0;
                return (0);
            }
            // If there is still data to send, return 1
            else
            {
                return (1);
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
