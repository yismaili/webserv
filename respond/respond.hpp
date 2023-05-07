/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:49:15 by aoumad            #+#    #+#             */
/*   Updated: 2023/05/07 13:47:01 by aoumad           ###   ########.fr       */
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
# include "additional_class.hpp"
# include "../prs_rsc/location.hpp"
# include "../CGI/cgi.hpp"

class server;
class location;

class Respond
{
    public:
        Respond();
        Respond(request& req);
        ~Respond();

        void set_http_version(std::string http_version);
        void set_status_code(int status_code);
        void set_status_message(std::string status_message);
        void set_header(std::string key, std::string value);

        void set_response_body(std::string body);
        std::string get_status_line(const std::string &status_code);
        void    set_date();
        void    set_last_modified();
        void    set_cache_control(std::string control);

        std::string get_http_version();
        int         get_status_code();
        std::string get_status_message();
        std::string get_response_body();
        std::string get_response_status(int status_code);
        std::string get_header_line(std::string key);
        std::string get_mime_type(std::string file_extension);
        int         is_path_safe(std::string requested_path);
        std::string get_document_root();
        std::string get_file_cgi();
        std::string get_path_info_founded();
        
        
        void print_respond();

        void        response_root(std::vector<server> server);
        std::string response_autoindex(request &r);
        std::string response_cgi(request &r);
        int         ft_parse_location(std::vector<server> server);
        int         exact_location(std::vector<server> server, std::string path);
        int         prefix_location(std::vector<server> server, std::string &path);
        int         dynamic_location(std::vector<server> server, std::string path);
        int         root_location(std::vector<server> server);
        int         ft_parse_url_forwarding(std::vector<server> server);
        int         ft_check_allowed_methods(std::vector<server> server);
        void        ft_check_autoindex(std::vector<server> server);
        int         ft_parse_root_path(std::vector<server> server);

        // GET RESPONSE
        void        ft_handle_cgi();
        void        ft_handle_file();
        void        ft_handle_autoindex(std::vector<server> servers);
        void        ft_check_cgi();
        int         ft_check_file();
        void        ft_handle_index(std::vector<server> servers);
        void        ft_handle_index_2();
        void        ft_show_autoindex();

        // POST RESPONSE
        std::string check_post_type();
        void        handle_post_response(std::vector<server> server);
        void        handle_form_data();
        size_t      find_boundary(size_t pos);
        FormData    read_form_data(size_t pos);
        void        handle_urlencoded();

        std::vector<FormData> _form_data;
        std::vector<Url_encoded> _url_decode;
    private:
        std::map<std::string, std::string> _headers;
        std::string _response_body;
        std::string _http_version;
        int         _status_code;
        std::string _status_message;
        std::string _document_root;
        std::string _path_found;
        std::string _rooted_path;
        std::string _upload_store_path;
        bool        _is_autoindex;
        std::string _boundary;
        std::string _upload_store;
        int         _server_index;
        int         _location_index;
        std::string _removed_path;
        std::string _cookie;
        std::string _path_info_founded;
        std::string _file_cgi;
        std::string _cache_control;

        bool        _is_cgi;
        bool        _is_allowed_method;
        bool        _is_redirection;
        bool        _is_index;

        void        handle_get_response(std::vector<server> servers);
        void        print_response();

        request& r;
        void        create_decode_files();

        // DELETE RESPONSE
        void        handle_delete_response();
        // ERROR RESPONSE
        void        handle_error_response(int error_code);
        void        ft_handle_error(int error_code);

        // DELETE RESPONSE

        // void        cout_respond();

};

#endif