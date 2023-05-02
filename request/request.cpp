/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 23:05:21 by aoumad            #+#    #+#             */
/*   Updated: 2023/04/30 18:28:34 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

typedef void (request::*encoding_handler)(std::string &body);

encoding_handler    handlers[] = 
{
    &request::handle_chunked_encoding,
    &request::handle_compress_encoding,
    &request::handle_deflate_encoding,
    &request::handle_gzip_encoding
};

const std::string  supported_encodings[] = 
{
    "chunked",
    "compress",
    "deflate",
    "gzip"
};

request::request()
{
    return ;
}

request::request(std::string request)
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
    ft_parse_language_charset();

    // Parse the request body
    std::string content_len_str = this->get_header("Content-Length");
    if (!content_len_str.empty())
    {
        size_t content_len = std::stoi(content_len_str);
        this->_body = lines.back().substr(0, content_len);
        std::string transfer_encoding = this->get_header("Accept-Encoding");
        if (!transfer_encoding.empty())
        {
            // using pointers to member functions to call the functions handlers
            std::vector<std::string> encoding_types;
            size_t startPos = 0;
            size_t endPos = transfer_encoding.find(',');
            while (endPos != std::string::npos)
            {
                encoding_types.push_back(transfer_encoding.substr(startPos, endPos - startPos));
                startPos = endPos + 1;
                endPos = transfer_encoding.find(',', startPos);
            }
            encoding_types.push_back(transfer_encoding.substr(startPos, endPos - startPos));
            for (std::vector<std::string>::const_iterator it = encoding_types.begin(); it != encoding_types.end(); ++it)
            {
                std::string type_tmp = *it;
                // Trim leading and trailing whitespaces from the value
                // type_tmp.erase(0, type_tmp.find_first_not_of(" \t"));
                // type_tmp.erase(type_tmp.find_first_not_of(" \t") + 1);
                bool supported = false;
                for (size_t i = 0; i < sizeof(handlers) / sizeof(handlers[0]); ++i)
                {
                    if (type_tmp == supported_encodings[i])
                    {
                        (this->*handlers[i])(this->_body);
                        supported = true;
                        break;
                    }
                }
                if (!supported)
                    std::cerr << "Unsupported encoding type: " << type_tmp << std::endl;
            }
            
        }
    }
    else
    {
        if (this->_headers.find("Content-Length") != this->_headers.end() || this->_method == "POST" || this->_method == "PUT"
            || this->_headers.find("Content-Type") != this->_headers.end())
            {
                std::cerr << "Body request is missing" << std::endl;
                exit(1);
            }
        
    }
}


void    request::handle_chunked_encoding(std::string &body)
{
    std::stringstream ss(body);
    std::string line;
    std::stringstream decoded;
    while (std::getline(ss, line))
    {
        std::stringstream size_ss(line);
        int chunk_size;
        size_ss >> std::hex >> chunk_size;
        if (chunk_size == 0) // end of chunks
            break;
        std::string chunk(chunk_size, ' ');
        ss.read(&chunk[0], chunk_size);
        decoded << chunk;
    }
    body = decoded.str();
}

void    request::handle_gzip_encoding(std::string &body)
{
    (void)body;
    std::cerr << "Unsupport gzip encoding" << std::endl;
}

void    request::handle_compress_encoding(std::string &body)
{
    (void)body;
    std::cerr << "Unsupport compress encoding" << std::endl;
}

void    request::handle_deflate_encoding(std::string &body)
{
    (void)body;
    std::cerr << "Unsupport deflate encoding" << std::endl;
}