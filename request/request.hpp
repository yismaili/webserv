/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 18:00:51 by aoumad            #+#    #+#             */
/*   Updated: 2023/03/26 23:05:07 by aoumad           ###   ########.fr       */
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

class request
{
    private:
        std::string _method; // stores http method of the request (e.g "GET", "POST"..etc)
        std::string _uri; // stores the URI of the request  (e.g "/index.html")
        std::string _version; // stores the http version of the request (e.g "HTTP/1.1")
        std::map<std::string, std::string> _headers; // stores the HTTP headers of the request. Each header is stored as a key-value as the value.
        std::string _body; // stores the body of the request

    public:
        request();
        request(const request &src);
        ~request();
        
        request &operator=(const request &src);
        
        std::string get_method() const;
        std::string get_uri() const;
        std::string get_version() const;
        std::string get_body() const;

        void set_method(std::string method);
        void set_uri(std::string uri);
        void set_version(std::string version);
        void set_body(std::string body);
        
        void add_header(std::string key, std::string value);
        std::string get_header(std::string key) const;
        std::map<std::string, std::string> get_headers() const;
        
        void parse_request(std::string request);
};

#endif