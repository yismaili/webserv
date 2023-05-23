/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:41:23 by yismaili          #+#    #+#             */
/*   Updated: 2023/05/23 20:57:32 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/http_server.hpp"

namespace http{
    
   http_sever::http_sever(std::vector<server> conf_) :sock()
   {
        for (size_t i = 0; i < conf_.size(); i++)
        {
            for (size_t j = 0; j < conf_[i]._listen.size(); j++)
            {
                socket_id.push_back(sock.init_data(conf_[i]._listen[j], conf_[i].get_host(), conf_[i].get_server_name(), i));
                port.push_back(conf_[i]._listen[j]);
                host.push_back(conf_[i].get_host());
                std::vector<std::string>::iterator it = conf_[i]._server_name.begin();
                while (it != conf_[i]._server_name.end())
                {
                    servers_names.push_back(*it);
                    it++;
                }
            }
        }
        conf = conf_;   
        flag = false;
    }
    
    http_sever::~http_sever()
    {
        std::vector<http::sockets>::iterator it = socket_id.begin();
        while (it != socket_id.end())
        {
            closeServer(it->getSockfd());
            it++;
        }
    }
    
    std::vector<http::sockets>::iterator http_sever::find_conf(int sockfd) 
    {
        std::vector<http::sockets>::iterator it = socket_id.begin();
        while (it != socket_id.end())
        {
            if (it->getSockfd() == sockfd)
            {
                return (it);
            }
            it++;
        }
        return (socket_id.begin());
    }
    
    unsigned int http_sever::getTime(void)
    {
        struct timeval current_time;
        gettimeofday(&current_time, NULL);
        return (current_time.tv_sec * 1000 + current_time.tv_usec / 1000);
    }
        
    int http_sever::ifport_dup(int port_)
    {
        std::vector<int>::iterator it;
        int check = 0;
        for (it = port.begin(); it != port.end(); ++it) 
        {
            if (port_ == (*it))
            {
                check++;
            }
        }
        if (check != 1 && check != 0)
        {
            return (1);
        }
        return (0);
    }
    
    int http_sever::ifserver_dup(std::string server_name)
    {
        std::vector<std::string>::iterator it;
        int check = 0;
        it = servers_names.begin();
        for (it = servers_names.begin(); it != servers_names.end(); ++it) 
        {
            if (server_name == (*it))
            {
                check++;
            }
        }
        if (check != 1 && check != 0)
        {
            return (1);
        }
        return (0);
    }
    
    int http_sever::ifhost_dup(std::string host_)
    {
        std::vector<std::string>::iterator it;
        int check = 0;
        
        for (it = host.begin(); it != host.end(); ++it) 
        {
            if (host_ == (*it))
            {
                check++;
            }
        }
        if (check != 1 && check != 0)
        {
           return (1);
        }
        return (0);
    }
    
    void http_sever::run() 
    {
        int poll_ret, new_socket, recv_ret, sent_ret;
        unsigned long i = 0;
        std::signal(SIGPIPE, SIG_IGN);
        // Add server socket to poll list
        std::vector<http::sockets>::iterator it = socket_id.begin();
        while (it != socket_id.end())
        {
            pollfd server_pollfd;
            server_pollfd.fd = it->getSockfd();
            server_pollfd.events = POLLIN | POLLOUT;
            clients.push_back(server_pollfd);
            it++;
        }
        
        while (true)
        {
            i = 0;
            // Wait for events on any of the monitored file descriptors
            poll_ret = poll(&clients[0], clients.size(), 0);
            //Check for events on server socket
            while (i < clients.size())
            {
                if (!is_server(clients[i].fd) && requist_data[clients[i].fd].size() > 0 && conf_fd[clients[i].fd]->data_issending == 0)
                {
                    if (getTime() - conf_fd[clients[i].fd]->getTime_out() >= 10000)
                    {
                        header_error = 1;
                        conf_fd[clients[i].fd]->data_issending = 1;
                        unchunk(clients[i].fd);
                        clients[i].events = POLLOUT;
                    }
                }
                if (clients[i].revents & POLLERR)
                {
                    std::vector<pollfd>::iterator it = clients.begin() + i;
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
                        fcntl(new_socket, F_SETFL, O_NONBLOCK);
                        conf_fd.insert(std::make_pair(new_socket, find_conf(clients[i].fd)));
                        std::cout <<"\n\033[32mCONNECTION TO ["<<conf_fd[new_socket]->getPort()<<"] "<<"ACCEPTED...\033[0m\n";
                        // Add new socket to poll list
                        pollfd new_client_pollfd;
                        new_client_pollfd.fd = new_socket;
                        new_client_pollfd.events = POLLIN | POLLOUT;
                        read_info.insert(std::make_pair(new_socket, false));
                        requist_data.insert(std::make_pair(new_socket, ""));
                        clients.push_back(new_client_pollfd);
                    }
                    else
                    {   
                        header_error = 0;

                        recv_ret = recv_data(clients[i].fd);
                        if (recv_ret == -2)
                        {
                            header_error = 1;
                            unchunk(clients[i].fd);
                            clients[i].events = POLLOUT;
                        }
                        else if (!recv_ret)
                        {
                            unchunk(clients[i].fd);
                            clients[i].events = POLLOUT;
                        }
                        else if (recv_ret == -3)
                        {
                            std::map<int, std::string>::iterator it_ = requist_data.find(clients[i].fd);
                            requist_data.erase(it_);
                            close(clients[i].fd);
                            std::vector<pollfd>::iterator it = clients.begin() + i;
                            clients.erase(it);
                            i--;
                        }
                    }
                }
               else if (clients[i].revents & POLLOUT && read_info[clients[i].fd] == true)
                {
                    std::vector<pollfd>::iterator it = clients.begin() + i;
                    std::map<int, bool>::iterator it_read = read_info.find(clients[i].fd);
                    sent_ret = send_data(clients[i].fd);
                    if (sent_ret == 0)
                    { 
                        usleep(1000);
                        close(clients[i].fd);
                        read_info.erase(it_read);
                        clients.erase(it);   
                        i--;
                    }
                    else if (sent_ret == -2)
                    {
                        close(clients[i].fd);
                        clients.erase(it);
                        read_info.erase(it_read);
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
            if (it->getSockfd() == sock)
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
        close(sock.getSockfd());
        close(newsockfd);
    }
    int http_sever ::parse_header(std::string header, int sockfd)
    {
        content_length = header.find("Content-Length: ");
        transfer_encoding = header.find("Transfer-Encoding: chunked");
        transfer_encoding_gzip = header.find("Transfer-Encoding: gzip, chunked");
        post_method = header.find("POST");
        content_len = std::strtol(header.substr(content_length + 16, 9).c_str(), nullptr, 0);
        conf_fd[sockfd]->setContent_length(content_len);
        std::string  subchunk = header.substr(header_end + 4, 9);
        int sizeof_chunk = std::strtol(subchunk.c_str(), NULL, 16);
        if ((content_length == std::string::npos && transfer_encoding == std::string::npos && post_method !=  std::string::npos) 
            || (sizeof_chunk == 0 && content_length == std::string::npos && post_method !=  std::string::npos))
        {
            return (-2);
        }
        return (0);
    }
    
    int http_sever ::transfer_encoding_chunked(int sockfd)
    {
        if (((content_length == std::string::npos && transfer_encoding == std::string::npos ) 
        || (content_length != std::string::npos && transfer_encoding != std::string::npos )) && post_method != std::string::npos)
        {
            return (-2);
        }
        if (content_length == std::string::npos)
        {
            if (transfer_encoding != std::string::npos || transfer_encoding_gzip != std::string::npos)
            {
                body_end = requist_data[sockfd].find("0\r\n\r\n");
                if (body_end != std::string::npos)
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
        char buffer[1000204] = {0};
        int bytes_received;
        int ret_parce;
        int ret_transfer;

        header_end = 0;
        content_len = 0;
        conf_fd[sockfd]->setContent_length(0);
        conf_fd[sockfd]->setTime_out(getTime());
        conf_fd[sockfd]->data_issending = 0;
        bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0)
        {
            return (-3);
        }
        requist_data[sockfd].append(std::string(buffer, bytes_received));
        header_end = requist_data[sockfd].find("\r\n\r\n");
        if (header_end != std::string::npos)
        {
            ret_parce = parse_header(requist_data[sockfd], sockfd);
            if (ret_parce == -2 )
            {
                return (-2);
            }
            ret_transfer = transfer_encoding_chunked(sockfd);
            if (ret_transfer == 2)
            {
                if ((content_len +  header_end + 4) <= requist_data[sockfd].size())
                {
                    read_info[sockfd] = true;
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
    
    void print(std::string str)
    {
        for(std::string::iterator it = str.begin(); it != str.end(); it++)
        {
            if (*it == '\r')
                std::cout << "\\r";
            else if (*it == '\n')
                std::cout << "\\n" << std::endl;
            else
                std::cout  << *it;
        }
    }
    
    void http_sever ::setIndexOfserver(int sockfd)
    {
        std::vector<std::string>::iterator it = conf_fd[sockfd]->server_name.begin();
        while (it != conf_fd[sockfd]->server_name.end())
        {
            if (ifhost_dup(conf_fd[sockfd]->ip_addr) && ifport_dup(conf_fd[sockfd]->getPort()) && !ifserver_dup(*it))
            {
              
                int host_index = requist_data[sockfd].find("Host") + 6;
                int host_end = requist_data[sockfd].find("\r\n", host_index);
                std::string cleint_host = requist_data[sockfd].substr(0, host_end);
                host_index = cleint_host.find("Host") + 6;
                cleint_host = cleint_host.substr(host_index, cleint_host.size());
                for (size_t i = 0; i < conf.size(); i++)
                {
                    for (size_t j = 0; j < conf[i]._server_name.size(); j++)
                    {
                        if (!std::strncmp(cleint_host.c_str(),  conf[i]._server_name[j].c_str(), strlen( conf[i]._server_name[j].c_str())))
                        {
                            conf_fd[sockfd]->setIndex(i);
                            flag = true;
                        }
                    }
                }
            }
            it++;
        }
    }
    
    void http_sever ::unchunk(int sockfd)
     {
        if (flag == true)
        {
            conf_fd[sockfd]->setIndex(conf_fd[sockfd]->getIndex_tmp());
        }
        setIndexOfserver(sockfd);
        if (header_error == 1)
        {
            request req;
            header_error = 0;
            Respond res(conf, conf_fd[sockfd]->getIndex() ,false, req);
            requist_data[sockfd] = res.rtn_response();
            read_info[sockfd] = true;
        }
        else 
        {
            if (transfer_encoding != std::string::npos && transfer_encoding < header_end)
            {
                requist_data[sockfd] = join_chunked(requist_data[sockfd], sockfd);
                conf_fd[sockfd]->setContent_length(requist_data[sockfd].size() - (header_end + 4));
            }
            int rtn_error;
            request req(requist_data[sockfd], conf_fd[sockfd]->getContent_length());
            rtn_error = req.parse_request();
            if (rtn_error == 2)
            {
                Respond res(conf, conf_fd[sockfd]->getIndex() ,false, req);
                requist_data[sockfd] = res.rtn_response();
            }
            else if (rtn_error == 0)
            {
                Respond   res(req, conf_fd[sockfd]->getIndex());
                requist_data[sockfd] =  res.response_root(conf);
            }
        }
    }
    
    std::string http_sever::join_chunked(const std::string &data, int sockfd) 
    {
        std::string result = "";
        std::size_t sizeof_chunk;
        std::string	body = "";
        std::string	chunks = ""; 
        std::string	subchunk = "";
        std::size_t  pos; 

        result.append(data.substr(0, header_end));
        result.append("\r\n\r\n");
        chunks = data.substr(header_end + 4, data.size() - 1);
        subchunk = chunks.substr(0, 9);
        sizeof_chunk =  std::strtol(subchunk.c_str(), NULL, 16);
        pos = 0;
        while (true)
        {
            pos = chunks.find("\r\n",  pos);
            result.append(chunks.substr(pos += 2, sizeof_chunk));
            pos += sizeof_chunk + 2;
            subchunk = chunks.substr(pos , 9);
            sizeof_chunk = std::strtol(subchunk.c_str(), NULL, 16);
            if (sizeof_chunk == 0)
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
        static std::map<int, std::size_t> sent_data;
        std::string data_to_send;
        long bytes_sent;
        static int check = 0;

        data_to_send = requist_data[socket].substr(sent_data[socket], 1024);
        bytes_sent = send(socket, data_to_send.c_str(), data_to_send.size(), 0);
        // Check for errors while sending data
        if (bytes_sent == -1)
        {
            sent_data[socket] = 0;
            std::map<int, std::string>::iterator it = requist_data.find(socket);
            requist_data.erase(it);
            conf_fd.erase(socket);
            return (-2);
        }
        else
        {
            // Update the amount of data that has been sent to the socket
            conf_fd[socket]->data_issending = 1;
            sent_data[socket] += bytes_sent;
            // If all data has been sent, erase the request information and return 0
            if (sent_data[socket] >= requist_data[socket].size())
            {
                sent_data[socket] = 0;
                std::cout << "\n\033[33mRESPONSE SENDED TO [" << conf_fd[socket]->getPort() << "]\033[0m" << std::endl;
                std::map<int, std::string>::iterator it = requist_data.find(socket);
                requist_data.erase(it);
                conf_fd.erase(socket);
                return (0);
            }
            else
            {
                if (check == 0)
                    std::cout << "\n\033[33mRESPONSE SENDING TO [" << conf_fd[socket]->getPort() << "]...\033[0m" << std::endl;
                // If there is still data to send, return 1
                check = 1;
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
           std::cout << "\033[31mError: accepting connection\033[0m\n";
        }
        return (sockfd_client);
    }
}
