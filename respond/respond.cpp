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
    // _document_root = "./www";
    _location = 0;
    _path_found = "";
    is_cgi = false;
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

    if (r->get_method() == "Get")
        _response_body = handle_get_response(r);
    else if (r->get_method() == "Post")
        _response_body = handle_post_response(r);
    else if (r->get_method() == "Put")
        _response_body = handle_put_response(r);
    else if (r->get_method() == "Delete")
        _response_body = handle_delete_response(r);
    else // unsupported http method
        _response_body = get_error_content("501");
}

std::string Respond::handle_get_response(request &r)
{
    std::string requested_path = r.get_uri();
    if (!is_path_safe(requested_path))
        return (get_error_content("400")); // or 403
    
    // contrust the full file path by appending the reequested path to the document root directory
    std::string full_path = get_document_root() + requested_path;
    // check if the file exists
    std::ifstream file_stream(full_path);
    if (!file_stream.is_open())
        return (get_error_content("404"));
    
    // read the file content into a string
    std::stringstream buffer;
    buffer << file_stream.rdbuf();
    std::string content_file = buffer.str();

    // generate HTTP response
    std::string response_body = content_file;
    if (!r.get_query().empty())
    {
        // if there is a query string, search the content file for the query term
        std::string query_term = r.get_query().substr(2); // remove "q="
        std::string::size_type pos = content_file.find(query_term);
        if (pos != std::string::npos)
        {
            // if the query term is found, return the content file from the query term to the end of the file
            response_body = content_file.substr(pos);
        }
        else
        {
            // if the query term is not found, return an empty string
            response_body = "Query term not found: " + query_term;
        }
    }
    else
    {
        // if there is no query string, return the entire content file
        response_body = content_file;
    }
    /*
        std::string status_line = "HTTP/1.1 200 OK\r\n";
    std::string content_type_header = "Content-Type: text/plain\r\n"; // Change this based on the file type
    std::string content_length_header = "Content-Length: " + std::to_string(response_body.length()) + "\r\n";
    std::string connection_header = "Connection: close\r\n";
    std::string response_headers = content_type_header + content_length_header + connection_header + "\r\n";
    */
    return (response_body);
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

std::string Respond::handle_post_response(request &r)
{

}

std::string Respond::response_root(request &r)
{
    // step 1 :check the location
    ft_parse_location(r);
    // step 2 : check the redirectation
    ft_parse_redirection();

}

void  Respond::ft_parse_location(request &r)
{
    // parse location based on the confige file
    // 1: exact location
    // 2: prefix location
    // 3: regex location
    // 4: default location

    // exact location body code
    std::string path = r.get_uri();
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server._location.size(); j++)
        {
            if (server[i]._location[j].location_name == path)
            {
                _path_found = server[i]._location[j].location_name;
            }
        }
    }

    // prefix location body code
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server._location.size(); j++)
        {
            if (path.find(server[i]._location.location_name) == 0)
            {
                _path_found = server[i]._location[j].location_name;
            }
        }
    }

    // regex location body code
    std::string::size_type pos = path.find(".");
    if (pos != std::string::npos)
    {
        std::string extension = "*" + path.substr(pos);
        for (int i = 0; i < server.size(); i++)
        {
            for (int j = 0; j < server._location.size(); j++)
            {
                if (server[i]._location[j].location_name == extension)
                {
                    _path_found = server[i]._location[j].location_name;
                    is_cgi = true;
                }
            }
        }
    }

    // root location
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server._location.size(); j++)
        {
            if (server[i]._location[j].location_name == "/")
            {
                _path_found = server[i]._location[j].location_name;
            }
        }
    }
}

/*
    // step 1 : check the location
    // step 2 : check the root
    // step 3 : check the index
    // step 4 : check the autoindex
    // step 5 : check the error_page
    // step 6 : check the limit_except
    // step 7 : check the cgi
    // step 8 : check the return
*/

void    Respond::ft_parse_redirection()
{

}