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
    _is_cgi = false;
    _is_allowed_method = false;
    _rooted_path = "";
    _is_autoindex = "";
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
    ft_parse_url_forwarding();

    // step 3 : check the validation of rooted path
    ft_parse_root_path();


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
        for (int j = 0; j < server[i]._location.size(); j++)
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
        for (int j = 0; j < server[i]._location.size(); j++)
        {
            if (path.find(server[i]._location[j].location_name) == 0)
            {
                _path_found = server[i]._location[j].location_name;
            }
        }
    }

    // regex location body code
    std::string::size_type pos = path.find(".");
    if (pos != std::string::npos)
    {
        std::string extension = ".*" + path.substr(pos);
        for (int i = 0; i < server.size(); i++)
        {
            for (int j = 0; j < server[i]._location.size(); j++)
            {
                if (server[i]._location[j].location_name == extension)
                {
                    _path_found = server[i]._location[j].location_name;
                    _is_cgi = true;
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

void    Respond::ft_parse_url_forwarding()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0;  j < server._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                // check for redirection ===== where redirection is make_pair
                if (!server[i]._location[j].redirection.first.first.empty() &&
                    !server[i]._location[j].redirection.first.second.empty())
                {
                    int status_code = server[i]._location[j].redirection.second;
                    // search for message of the status_code
                    std::string message = get_response_status(status_code);
                    set_status_code(status_code);
                    set_status_message(message);
                    set_header("Location", server[i]._location[j].redirection.first);
                    _is_redirection = true;
                    return ;
                }
            }
            else
            {
                set_status_code(200);
                set_status_message("OK");
                return ;
            }
        }
    }
}

void    Respond::ft_check_allowed_methods()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; server._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                // get the autoindex
                _autoindex = server[i]._location[j].autoindex;
                // check for allowed methods
                std::vector<std::string> allowed_methods = server[i]._location[j].allowed_methods;
                for (int k = 0; k < allowed_methods.size(); k++)
                {
                    if (allowed_methods[k] == r.get_method())
                    {
                        _is_allowed_method = true;
                        return ;
                    }
                }
                set_status_code(405);
                set_status_message("Method Not Allowed");
                return ;
            }
        }
    }
}

void    Respond::ft_check_autoindex()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; server._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                // check for autoindex
                if (server[i]._location[j].autoindex == true)
                {
                    _is_autoindex = true;
                    return ;
                }
            }
        }
    }
}

void    Respond::ft_parse_root_path()
{
    struct stat file_stats;
    _rooted_path = server.get_root() + _path_found;

    if (!stat(_rooted_path.c_str(), &file_stats))
        return;
    set_status_code(403);
    set_status_message("Forbidden");
    return ; // i need to call error function instead
}