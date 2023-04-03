/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:49:15 by aoumad            #+#    #+#             */
/*   Updated: 2023/04/03 03:39:05 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPOND_HPP
# define RESPOND_HPP

# include <string>
# include <map>
# include <vector>
# include <iostream>
# include <sstream>
# include <fstream>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>
# include <errno.h>

class Respond
{
    private:
        std::map<std::string, std::string> _headers;
        std::string _response_body;
        std::string _http_version;
        std::string _status_code;
        std::string _status_message;
        
        std::string handle_get_response(request &r);
        std::string handle_post_response(request &r);
        std::string handle_put_response(request &r);
        std::string handle_delete_response(request &r);
        std::string handle_head_response(request &r);
        
        std::string get_file_content(std::string path);
        std::string get_file_content(std::string path, std::string file);
        std::string get_directory_content(std::string path);
        std::string get_error_content(std::string error_code);
        std::string get_error_content(std::string error_code, std::string error_message);
        
    public:
        Respond();
        ~Respond();

        std::string get_respond();
        std::string get_respond(std::string request, std::string body);

        void set_http_version(std::string http_version);
        void set_status_code(std::string status_code);
        void set_status_message(std::string status_message);
        void set_header(std::string key, std::string value);
        void set_response_body(std::string response_body);
        std::string get_status_line(const std::string &status_code);

        std::string get_http_version();
        std::string get_status_code();
        std::string get_status_message();
        std::string get_response_body();
        std::string get_header_line(std::string key)
        
        void print_respond();
};

#endif