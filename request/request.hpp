/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 18:00:51 by aoumad            #+#    #+#             */
/*   Updated: 2023/05/10 19:01:41 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <map>
# include <algorithm>
# include <iterator>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/socket.h>
# include <zlib.h>


class request
{
    private:
        std::string _method; // stores http method of the request (e.g "GET", "POST"..etc)
        std::string _uri; // stores the URI of the request  (e.g "/index.html")
        std::string _version; // stores the http version of the request (e.g "HTTP/1.1")
        std::map<std::string, std::string> _headers; // stores the HTTP headers of the request. Each header is stored as a key-value as the value.
        std::string _body; // stores the body of the request
        std::map<std::string, std::pair<std::string, std::string> > _language;
        std::map<std::string, std::string> _charset;
        int         _port;
        std::string _query;
        std::string _boundary;

    public:
        request();
        request(std::string request);
        request(const request &src);
        ~request();
        
        request &operator=(const request &src);
        
        std::string get_method() const;
        std::string get_uri() const;
        std::string get_version() const;
        std::string get_body() const;
        std::string get_query() const;
        std::string get_boundary() const;
        // int     ft_get_port() const;

        void set_method(std::string method);
        void set_uri(std::string uri);
        void set_version(std::string version);
        void set_body(std::string body);
        
        void add_header(std::string key, std::string value);
        std::string get_header(std::string key) const;
        std::map<std::string, std::string> get_headers() const;
        void parse_request(std::string request);

        typedef void (request::*encoding_handler)(std::string &body);

        void    handle_chunked_encoding(std::string &body);
        void    handle_compress_encoding(std::string &body);
        void    handle_deflate_encoding(std::string &body);
        void    handle_gzip_encoding(std::string &body);
        void    ft_parse_port(std::string host);
        int     ft_check_content_length();
        int     ft_check_connexion();
        int     ft_check_content_type();
        void    ft_find_query();
        void    ft_parse_language_charset();
        void    print_request();
        void    init_parse();
};

int     ft_check_request_line(std::string method, std::string uri, std::string version);

#endif