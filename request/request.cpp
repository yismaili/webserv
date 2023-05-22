/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 23:05:21 by aoumad            #+#    #+#             */
/*   Updated: 2023/05/22 20:41:03 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

request::request() : _method(""), _uri(""), _version(""),
    _body(""), _port(80), _query("")
{
    return ;
}

request::request(std::string request, size_t content_len) : _method(""), _uri(""), _version(""),
    _body(""), _port(80), _query(""), _content_len(content_len)
{
    this->_init_request = request;
    add_header("Content-Length", std::to_string(_content_len));
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
        this->_content_len = src._content_len;
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

size_t  request::get_content_length() const
{
    return (this->_content_len);
}

int request::parse_request()
{
    std::string request = this->_init_request;
    // Split the request into lines
    std::vector<std::string> lines;
    std::istringstream iss(request);
    std::string line;

    if (request.empty())
        return (2);
    while (std::getline(iss, line))
    {
        if (line.empty())
            break;
        lines.push_back(line);
    }
    // Parse the request line
    std::istringstream request_line(lines[0]);
    request_line >> this->_method >> this->_uri >> this->_version;
    // i need to call a function to check if the request line content is suitable or not
    if (!ft_check_request_line(this->_method, this->_uri, this->_version))
        return (2);
    ft_find_query();
    handleSpecialCharacters(this->_uri);
    // Parse the headers
    for (std::vector<std::string>::const_iterator it = lines.begin() + 1; it != lines.end(); ++it)
    {
        std::string key = it->substr(0, it->find(':'));
        std::string value = it->substr(it->find(':') + 1);
        // Trim leading and trailing whitespaces from the value
        value.erase(0, value.find_first_not_of(" \t\r"));
        value.erase(value.find_last_not_of(" \t\r") + 1);
        if (this->_headers.find(key) == this->_headers.end())
            this->_headers[key] = value;
    }
    // function that checks if the request is POST or PUT to see if there is no content-length to return error
    if (ft_check_content_length() == false || ft_check_content_type() == false)
        return (2);
    // function that checks if the header `connexion` exists or not
    int rtn = ft_check_connexion();
    if (rtn != 1)
        return (2);

    // function that will parse the port from the host 
    // and check if the host is valid or not
    if (this->get_header("Host") == "")
        return (2);
    ft_parse_port(this->get_header("Host"));

    // Parse the request body
    std::string content_len_str = this->get_header("Content-Length");
    if (content_len_str != "")
    {
        size_t content_len = std::stoi(content_len_str);
        if (content_len > request.size())
            return (2);
        this->_body = request.substr(request.size() - content_len);
    }
    else
    {
        if (content_len_str == "" || this->_method == "POST")
                return (2);
        
    }
    return (0);
}

std::string request::get_boundary() const
{
    return (this->_boundary);
}

// change the "%20" with a space
void request::handleSpecialCharacters(std::string& uri) {
    std::string encodedChars[] = {"%20", "%21", "%22", "%23", "%24", "%25", "%26", "%27", "%28", "%29", "%2A", "%2B", "%2C",
                                    "%2D", "%2E", "%2F", "%3A", "%3B", "%3C", "%3D", "%3E", "%3F", "%40", "%5B", "%5C", "%5D",
                                    "%5E", "%5F", "%60", "%7B", "%7C", "%7D", "%7E"};

    std::string specialChars[] = {" ", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", ":", ";", "<",
                                    "=", ">", "?", "@", "[", "\\", "]", "^", "_", "`", "{", "|", "}", "~"};

    for (size_t i = 0; i < sizeof(encodedChars) / sizeof(encodedChars[0]); i++) {
        std::string::size_type n = 0;
        while ((n = uri.find(encodedChars[i], n)) != std::string::npos) {
            uri.replace(n, encodedChars[i].length(), specialChars[i]);
            n += 1;
        }
    }
}