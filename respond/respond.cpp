/*
example of a basic respond
HTTP/1.1 200 OK\r\n
Content-Type: text/html\r\n
Content-Length: 1234\r\n
\r\n
<html><body>Hello, world!</body></html>
*/

#include "respond.hpp"
#include "../request/request.hpp"

Respond::Respond()
{
    _http_version = "HTTP/1.1";
    _response_body = "";
    _status_code = "200";
    _status_message = "OK";
}

Respond::~Respond()
{
}

void    Respond::set_http_version(std::string http_version)
{
    _http_version = http_version;
}

void    Respond::set_status_code(std::string status_code)
{
    _status_code = status_code;
}

void    Respond::set_status_message(std::string status_message)
{
    _status_message = status_message;
}

void    Respond::set_header(std::string key, std::string value)
{
    _headers[key] = value;
}

std::string Respond::get_http_version()
{
    return (_http_version);
}

std::string Respond::get_status_code()
{
    return (_status_code);
}

std::string Respond::get_status_message()
{
    return (_status_message);
}


std::string Respond::get_response_body()
{
    return (_response_body);
}


std::string Respond::get_status_line(const std::string &status_code)
{
    std::string status_line;
    std::string status_message;

    if (status_code == "200")
        status_message = "OK";
    else if (status_code == "201")
        status_message = "Created";
    else if (status_code == "202")
        status_message = "Accepted";
    else if (status_code == "204")
        status_message = "No Content";
    else if (status_code == "301")
        status_message = "Moved Permanently";
    else if (status_code == "302")
        status_message = "Found";
    else if (status_code == "304")
        status_message = "Not Modified";
    else if (status_code == "400")
        status_message = "Bad Request";
    else if (status_code == "401")
        status_message = "Unauthorized";
    else if (status_code == "403")
        status_message = "Forbidden";
    else if (status_code == "404")
        status_message = "Not Found";
    else if (status_code == "405")
        status_message = "Method Not Allowed";
    else if (status_code == "406")
        status_message = "Not Acceptable";
    else if (status_code == "408")
        status_message = "Request Timeout";
    else if (status_code == "409")
        status_message = "Conflict";
    else if (status_code == "410")
        status_message = "Gone";
    else if (status_code == "411")
        status_message = "Length Required";
    else if (status_code == "412")
        status_message = "Precondition Failed";
    else if (status_code == "413")
        status_message = "Payload Too Large";
    else if (status_code == "414")
        status_message = "URI Too Long";
    else if (status_code == "415")
        status_message = "Unsupported Media Type";
    else if (status_code == "416")
        status_message = "Range Not Satisfiable";
    else if (status_code == "417")
        status_message = "Expectation Failed";
    else if (status_code == "418")
        status_message = "I'm a teapot";
    else if (status_code == "426")
        status_message = "Upgrade Required";
    else if (status_code == "500")
        status_message = "Internal Server Error";
    else if (status_code == "501")
        status_message = "Not Implemented";
    else if (status_code == "502")
        status_message = "Bad Gateway";
    else if (status_code == "503")
        status_message = "Service Unavailable";
    else if (status_code == "504")
        status_message = "Gateway Timeout";
    else if (status_code == "505")
        status_message = "HTTP Version Not Supported";
    else if (status_code == "511")
        status_message = "Network Authentication Required";
    else
        status_message = "Unknown";
    
    status_line = _http_version + " " + status_code + " " + status_message;
    return (status_line);
}

std::string Respond::get_header_line(std::string key)
{
    std::string header_line;

    header_line = key + ": " + _headers[key] + "\r\n";
    return (header_line);
}

std::string Respond::get_response_body()
{
    return (_response_body);
}

std::string Respond::get_respond()
{
    std::string respond;
    std::string status_line;
    std::string header_line;
    std::string response_body;

    status_line = get_status_line(_status_code);
    respond += status_line;
    respond += "\r\n";

    for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
    {
        header_line = get_header_line(it->first);
        respond += header_line;
        respond += "\r\n";
    }

    response_body = get_response_body();
    respond += response_body;
    return (respond);
}

void    Respond::set_response_body(request &r)
{
    std::string response_body;

    if (r->get_method == "Get")
        _response_body = handle_get_response(r);
    else if (r->get_method == "Post")
        _response_body = handle_post_response(r);
    else if (r->get_method == "Put")
        _response_body = handle_put_response(r);
    else if (r->get_method == "Delete")
        _response_body = handle_delete_response(r);
    else // unsupported http method
        _response_body = get_error_content("501");
}

std::string Respond::handle_get_response(request &r)
{
    std:string path = r.
}