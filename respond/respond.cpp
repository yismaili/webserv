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
    _status_code = 200;
    _status_message = "OK";
    // _document_root = "./www";
    _path_found = "";
    _is_cgi = false;
    _is_allowed_method = false;
    _rooted_path = "";
    _is_autoindex = false;
    _is_redirection = false;
    _is_index = false;
    _boundary = r.get_header("Content-Type").substr(r.get_header("Content-Type").find("boundary=") + 9);
    _upload_store = "";
}

Respond::~Respond()
{
}

void    Respond::set_http_version(std::string http_version)
{
    _http_version = http_version;
}

void    Respond::set_status_code(int status_code)
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

int         Respond::is_path_safe(std::string requested_path)
{
    /*
    if a user sends a request with the requested path ../../etc/passwd
    it could potentially be used to access */
    if (requested_path.find("..") != std::string::npos)
        return (0);
    return (1);
}

std::string Respond::get_document_root()
{
    return (_document_root);
}

int Respond::ft_parse_root_path(std::vector<server> server)
{
    struct stat file_stats;
    _rooted_path = server.get_root() + _path_found;

    if (!stat(_rooted_path.c_str(), &file_stats))
        return (0);

    set_status_code(403);
    return (1);
}