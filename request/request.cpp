/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 23:05:21 by aoumad            #+#    #+#             */
/*   Updated: 2023/03/27 22:22:36 by aoumad           ###   ########.fr       */
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

    // Parse the headers
    for (std::vector<std::string>::const_iterator it = lines.begin() + 1; it != lines.end(); ++it)
    {
        std::string key = it->substr(0, it->find(':'));
        std::string value = it->substr(it->find(':') + 1);
        // Trim leading and trailing whitespaces from the value
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        this->_headers[key] = value;
    }

    // Parse the request body
    std::string content_len_str = this->get_header("Content-Length");
    if (!content_len_str.empty())
    {
        size_t content_len = std::stoi(content_len_str);
        this->_body = lines.back().substr(0, content_len);
        std::string transfer_encoding = this->get_header("Transfer-Encoding");
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
                type_tmp.erase(0, type_tmp.find_first_not_of(" \t"));
                type_tmp.erase(type_tmp.find_first_not_of(" \t") + 1);
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
}

/*
example of the request message:
POST /index.html HTTP/1.1
Host: example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*\/*;q=0.8
Accept-Language: en-US,en;q=0.8
Content-Length: 11
Connection: keep-alive

This is the request body.
*/

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
    
}