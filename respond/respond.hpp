/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:49:15 by aoumad            #+#    #+#             */
/*   Updated: 2023/05/22 18:40:32 by aoumad           ###   ########.fr       */
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
class request;

class Respond
{
    public:
        Respond();
        Respond(std::vector<server> server, int _index, bool rtn_error, request &req);
        Respond(request& req, int index_);
        ~Respond();

        void set_http_version(std::string http_version);
        void set_status_code(int status_code);
        void set_status_message(std::string status_message);
        void set_header(std::string key, std::string value);

        void set_response_body(std::string body);
        std::string get_status_line(const std::string &status_code);
        void    set_date();
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
        int         ft_check_location_index(std::vector<server> server);
        int         ft_check_server_index(std::vector<server> server);
        
        
        void print_respond();
        std::string rtn_response();

        std::string response_root(std::vector<server> server);
        std::string response_autoindex(request &r);
        std::string response_cgi(request &r);
        int         ft_parse_location(std::vector<server> server, bool flag);
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
        void        ft_handle_file(std::vector<server> server);
        int         ft_handle_autoindex(std::vector<server> servers);
        void        ft_check_cgi();
        int         ft_check_file();
        int         ft_handle_index(std::vector<server> server);
        int         ft_handle_index_2(std::vector<server> server, std::string index);
        void        ft_show_autoindex(std::vector<server> server);

        // POST RESPONSE
        std::string check_post_type();
        void        handle_post_response(std::vector<server> server);
        void        handle_form_data(std::vector<server> server);
        size_t      find_boundary(size_t pos);
        FormData    read_form_data(std::vector<server> servers, size_t pos);
        void        handle_urlencoded();
        void        create_form_data();

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
        std::string _mime_string;
        static std::string _uri;
        std::string _pur_uri;
        static bool check_location;
        bool        _rtn_error;
        bool        _file_too_large;

        bool        _is_cgi;
        bool        _is_allowed_method;
        bool        _is_redirection;
        bool        _is_index;
        bool        _last_boundary;

        void        handle_get_response(std::vector<server> servers);
        void        print_response();
        void        init_response_body(std::vector<server> server ,std::string file, std::string _root);

        request& r;
        void        create_decode_files();

        // DELETE RESPONSE
        void        handle_delete_response(std::vector<server> server);
        // ERROR RESPONSE
        void        handle_error_response(std::vector<server> server, int error_code);
        void        ft_handle_error(int error_code);

        // void        cout_respond();

};

#endif

    // void http_sever ::unchunk(int sockfd)
    // {
        
            
    //     if (header_error == 1)
    //     {
    //         request req;
    //         header_error = 0;
    //         Respond res(conf, conf_fd[sockfd]->getIndex() ,false, req);
    //         requist_data[sockfd] = res.rtn_response();
    //        // std::cout<<requist_data[sockfd]<<std::endl;
    //         read_info[sockfd] = true;
    //     }
    //     else 
    //     {
    //         if (transfer_encoding != std::string::npos && transfer_encoding < header_end)
    //         {
    //             requist_data[sockfd] = join_chunked(requist_data[sockfd], sockfd);
    //             conf_fd[sockfd]->setContent_length(requist_data[sockfd].size() - (header_end + 4));
    //         }
    //         int rtn_error;
    //         request req(requist_data[sockfd], conf_fd[sockfd]->getContent_length());
    //         rtn_error = req.parse_request();
    //         if (rtn_error == 2)
    //         {
    //             Respond res(conf, conf_fd[sockfd]->getIndex() ,false, req);
    //             requist_data[sockfd] = res.rtn_response();
    //         }
    //         else if (rtn_error == 0)
    //         {
    //             Respond   res(req, conf_fd[sockfd]->getIndex());
    //             requist_data[sockfd] =  res.response_root(conf);
    //         }
    //     }
    // }