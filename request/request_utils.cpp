/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 21:18:13 by aoumad            #+#    #+#             */
/*   Updated: 2023/03/30 02:32:27 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

int ft_check_request_line(std::string method, std::string uri, std::string version)
{
    if (method != "GET" && method != "POST" && method != "HEAD" && method != "PUT")
    {
        if (method == "DELETE" || method == "OPTIONS" || method == "TRACE")
            std::cerr << "Method not implemented" << std::endl;
        return (0);
    }
    if (uri[0] != '/')
        return (0);
    if (version != "HTTP/1.1")
        return (0);
    return (1);
}

void    request::ft_parse_port(std::string host)
{
    std::string port;
    std::string::size_type pos;
    
    pos = host.find(":");
    if (pos != std::string::npos)
    {
        port = host.substr(pos + 1);
        this->_port = atoi(port.c_str());
        // this->_host = host.substr(0, pos);
    }
    else
        this->_port = 80;
}

int request::ft_check_content_length()
{
    if (this->_method == "POST" || this->_method == "PUT")
    {
        if (this->_headers.find("Content-Length") == this->_headers.end())
            return (0);
    }
    return (1);
}

int request::ft_check_content_type()
{
    if (this->_method == "POST" || this->_method == "PUT")
    {
        if (this->_headers.find("Content-Type") == this->_headers.end())
            return (0);
    }
    return (1);
}

int request::ft_check_connexion()
{
    if (this->_headers.find("Connection") != this->_headers.end())
    {
        if (this->_headers["Connection"] == "close")
            return (0);
    }
    
    std::string connection_value = this->get_header("Connection");
    if (connection_value != "close" || connection_value != "Keep-Alive")
        return (2);
    
    return (1);
}

void    request::ft_find_query()
{
    std::string::size_type pos;

    pos = this->_uri.find("?");
    if (pos != std::string::npos)
    {
        this->_query = this->_uri.substr(pos + 1);
        // this->_uri = this->_uri.substr(0, pos);
    }
}

void request::ft_parse_language_charset()
{
    std::string accept_language = this->_headers["Accept-Language"];
    std::string accept_charset = this->_headers["Accept-Charset"];

    // Parse Accept-Language
    size_t j = 0;
    while (j < accept_language.size())
    {
        // Find key
        size_t k = accept_language.find_first_of(',', j);
        std::string key = accept_language.substr(j, k - j);

        // Find value
        j = k + 1;
        k = accept_language.find_first_of(';', j);
        if (k == std::string::npos) k = accept_language.size();
        std::string value = accept_language.substr(j, k - j);

        // Find q value
        j = k + 1;
        k = accept_language.find_first_of(',', j);
        if (k == std::string::npos) k = accept_language.size();
        std::string qvalue = accept_language.substr(j, k - j);
        if (qvalue.find("q=") != std::string::npos)
            qvalue.erase(0, 2); // Remove "q="
        else
            qvalue = "1.0"; // Default q value

        // Add to map
        this->_language[key] = std::make_pair(value, qvalue);
        j = k + 1;
    }

    // Parse Accept-Charset
    // for example Accept-Charset: iso-8859-5, unicode-1-1;q=0.8
    // ==> after parsing {
    // "iso-8859-5": "",
    // "unicode-1-1": "0.8"
    // }
    size_t l = 0;
    while (l < accept_charset.size())
    {
        // Find key
        size_t m = accept_charset.find_first_of(',', l);
        std::string key = accept_charset.substr(l, m - l);

        // Find q value
        l = m + 1;
        m = accept_charset.find_first_of(',', l);
        if (m == std::string::npos) m = accept_charset.size();
        std::string qvalue = accept_charset.substr(l, m - l);
        if (qvalue.find("q=") != std::string::npos)
            qvalue.erase(0, 2); // Remove "q="
        else
            qvalue = "1.0"; // Default q value

        // Add to map
        this->_charset[key] = qvalue;
        l = m + 1;
    }
}