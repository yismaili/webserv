/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:49:15 by aoumad            #+#    #+#             */
/*   Updated: 2023/04/11 18:07:47 by aoumad           ###   ########.fr       */
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
# include "../request/request.hpp"
# include "../prs_rsc/server.hpp"
class Respond
{
    private:
        std::map<std::string, std::string> _headers;
        std::string _response_body;
        std::string _http_version;
        std::string _status_code;
        std::string _status_message;
        std::string _document_root;
        size_t      _location;
        std::string _path_found;
        std::string _rooted_path;
        std::string _is_autoindex;

        bool        _is_cgi;
        bool        _is_allowed_method;
        bool        _is_redirection;
        bool        _is_index;

        std::string handle_get_response();
        std::string handle_post_response();
        std::string handle_put_response();
        std::string handle_delete_response();
        std::string handle_head_response();
        
        std::string get_file_content(std::string path);
        std::string get_file_content(std::string path, std::string file);
        std::string get_directory_content(std::string path);
        std::string get_error_content(std::string error_code);
        std::string get_error_content(std::string error_code, std::string error_message);
        
        request& r;
        server& server;
    public:
        Respond();
        ~Respond();

        std::string get_respond();
        std::string get_respond(std::string request, std::string body);

        void set_http_version(std::string http_version);
        void set_status_code(int status_code);
        void set_status_message(std::string status_message);
        void set_header(std::string key, std::string value);
        void set_response_body(request &r);
        std::string get_status_line(const std::string &status_code);

        std::string get_http_version();
        std::string get_status_code();
        std::string get_status_message();
        std::string get_response_body();
        std::string get_header_line(std::string key);
        std::string get_mime_type(std::string file_extension);
        std::string get_response_status(int status_code);
        int         is_path_safe(std::string requested_path);
        std::string get_document_root();
        
        void print_respond();

        std::string response_root(request &r);
        std::string response_autoindex(request &r);
        std::string response_cgi(request &r);
        void        ft_parse_location();
        void        ft_parse_url_forwarding();
        void        ft_check_allowed_methods();
        void        ft_check_autoindex();
        void        ft_parse_root_path();

        // GET RESPONSE
        void        ft_handle_redirection();
        void        ft_handle_cgi();
        void        ft_handle_file();
        void        ft_handle_autoindex();
        void        ft_check_cgi();
        int         ft_check_file();
        void        ft_handle_index();
        void        ft_handle_index_2();
        
        // ERROR RESPONSE
        std::string handle_error(int error_code);
        std::string handle_error(int error_code, std::string error_message);
};

#endif