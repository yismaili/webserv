/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 23:05:21 by aoumad            #+#    #+#             */
/*   Updated: 2023/05/08 16:17:03 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

request::request() : _method(""), _uri(""), _version(""),
    _body(""), _port(80), _query("")
{
    return ;
}

request::request(std::string request) : _method(""), _uri(""), _version(""),
    _body(""), _port(80), _query("")
{
    this->parse_request(request);
    return ;
}

request::request(const request &src)
{
    *this = src;
    return ;
}

request::~request()
{
    return ;
}

request &request::operator=(const request &src)
{
    if (this != &src)
    {
        this->_method = src._method;
        this->_uri = src._uri;
        this->_version = src._version;
        this->_headers = src._headers;
        this->_body = src._body;
        this->_port = src._port;
        this->_query = src._query;
    }
    return (*this);
}

std::string request::get_method() const
{
    return (this->_method);
}

std::string request::get_uri() const
{
    return (this->_uri);
}

std::string request::get_version() const
{
    return (this->_version);
}

std::string request::get_body() const
{
    return (this->_body);
}

void request::set_method(std::string method)
{
    this->_method = method;
}

void request::set_uri(std::string uri)
{
    this->_uri = uri;
}

void request::set_version(std::string version)
{
    this->_version = version;
}

void request::set_body(std::string body)
{
    this->_body = body;
}

void request::add_header(std::string key, std::string value)
{
    this->_headers[key] = value;
}

std::string request::get_header(std::string key) const
{
    std::map<std::string, std::string>::const_iterator it = this->_headers.find(key);
    if (it != this->_headers.end())
        return (it->second);
    return ("");
}

std::map<std::string, std::string> request::get_headers() const
{
    return (this->_headers);
}

void request::parse_request(std::string request)
{
    // Split the request into lines
    std::vector<std::string> lines;
    std::istringstream iss(request);
    std::string line;

    while (std::getline(iss, line))
    {
        if (line.empty())
            break;
        lines.push_back(line);
    }
    // Parse the request line
    // std::cout << lines[0] << std::endl;
    std::istringstream request_line(lines[0]);
    request_line >> this->_method >> this->_uri >> this->_version;
    // i need to call a function to check if the request line content is suitable or not
    if (!ft_check_request_line(this->_method, this->_uri, this->_version))
    {
        std::cerr << "Invalid request line" << std::endl;
        exit(1);
    }
    ft_find_query();
    // Parse the headers
    for (std::vector<std::string>::const_iterator it = lines.begin() + 1; it != lines.end(); ++it)
    {
        std::string key = it->substr(0, it->find(':'));
        std::string value = it->substr(it->find(':') + 1);
        // Trim leading and trailing whitespaces from the value
        value.erase(0, value.find_first_not_of(" \t\r"));
        value.erase(value.find_last_not_of(" \t\r") + 1);
        this->_headers[key] = value;
    }

    // function that checks if the request is POST or PUT to see if there is no content-length to return error
    if (ft_check_content_length() == false || ft_check_content_type() == false)
    {
        std::cerr << "Invalid Content-Length or Content-Type" << std::endl;
        exit(1);
    }
    // function that checks if the header `connexion` exists or not
    int rtn = ft_check_connexion();
    if (rtn != 1)
    {
        if (rtn == 2)
            std::cerr << "Invalid Connexion header" << std::endl;
        else
            std::cerr << "Missing Connexion header" << std::endl;
        exit(1);
    }

    // function that will parse the port from the host 
    // and check if the host is valid or not
    if (this->get_header("Host") == "")
    {
        std::cerr << "Invalid Host header" << std::endl;
        exit(1);
    }
    ft_parse_port(this->get_header("Host"));
    // ft_parse_language_charset();

    // Parse the request body
    std::string content_len_str = this->get_header("Content-Length");
    if (content_len_str != "")
    {
        size_t content_len = std::stoi(content_len_str);
        this->_body = lines.back().substr(0, content_len);
    }
    else
    {
        // std::cout << "____----_-_-_-_--_-_____-_-_-_-____-_-_-_-_-------" << std::endl;
        if (content_len_str == "" || this->_method == "POST")
            {
                set_body("");
                return ;
            }
        
    }
}

void    request::print_request()
{
    std::cout << "Method: " << this->_method << std::endl;
    std::cout << "URI: " << this->_uri << std::endl;
    std::cout << "Version: " << this->_version << std::endl;
    std::cout << "Headers:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
        std::cout << it->first << ": " << it->second << std::endl;
    std::cout << "Body: " << this->_body << std::endl;
}