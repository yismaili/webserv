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

Respond::Respond(request& req, int index_) : r(req)
{
    _http_version = "HTTP/1.1";
    _status_code = 200;
    _status_message = "OK";
    _path_found = "";
    _is_cgi = false;
    _is_allowed_method = false;
    _rooted_path = "";
    _is_autoindex = false;
    _is_redirection = false;
    _is_index = false;
    _boundary = r.get_boundary();
    // _boundary = r.get_header("Content-Type").substr(r.get_header("Content-Type").find("boundary=") + 9);
    _upload_store = "";
    _server_index = index_;
    _location_index = 0;
    _removed_path = "";
    _path_info_founded = "";
    _file_cgi = "";
    _cache_control = "";
    _last_boundary = false;
    _mime_string = "";
    _pur_uri = r.get_uri();
}

Respond::Respond(bool rtn_error, request &req) : _rtn_error(rtn_error), r(req)
{
    _http_version = "HTTP/1.1";
    if (_rtn_error == false)
    {
        std::string msg = "400";
        // i need to set response wuth bad request
        set_status_code(400);
        set_status_message(get_response_status(get_status_code()));
        set_header("Content-Type", "text/html");
        set_header("Connection", "keep-alive");
        set_date();
        _response_body = "<html><head><title>" + msg + " " + _status_message + "</title></head><body><h1>" + msg + " " + _status_message + "</p></body></html>";
        set_header("Content-Length", std::to_string(_response_body.length()));
    }
    return ;
}

Respond::~Respond()
{
}

std::string Respond::_uri = "";
bool Respond::check_location = false;

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

void    Respond::set_date()
{
    std::time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);
    std::strftime(buffer, 80, "%a, %d %b %Y %X %Z", timeinfo);
    _headers["Date"] = buffer;
}

void    Respond::set_last_modified()
{
    struct stat file_stats;
    char buffer[80];

    stat(_rooted_path.c_str(), &file_stats);
    std::strftime(buffer, 80, "%a, %d %b %Y %X %Z", localtime(&file_stats.st_mtime));
    _headers["Last-Modified"] = buffer;
}

std::string Respond::get_http_version()
{
    return (_http_version);
}

int Respond::get_status_code()
{
    return (_status_code);
}

std::string Respond::get_status_message()
{
    return (_status_message);
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
    // if (check_location == false)
    _rooted_path = server[_server_index]._location[_location_index].get_root() + _removed_path;
    // if (check_location == true)
        // _rooted_path = server[_server_index]._location[_location_index].get_root() + _uri;
    // std::cout << "rooted path: " << _rooted_path << std::endl;
    if (!stat(_rooted_path.c_str(), &file_stats))
    {
        _file_cgi = _rooted_path;
        return (0);
    }
    set_status_code(403);
    set_status_message(get_response_status(get_status_code()));
    return (1);
}

void    Respond::set_response_body(std::string body)
{
    _response_body = body;
}

std::string Respond::get_path_info_founded()
{
    return (_path_info_founded);
}

std::string Respond::get_file_cgi()
{
    return (_file_cgi);
}

void    Respond::set_cache_control(std::string cache)
{
    _cache_control = cache;
}

std::string Respond::rtn_response()
{
    std::string response;

    response = _http_version + " " + std::to_string(_status_code) + " " + _status_message + "\r\n";
    for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
        response += it->first + ": " + it->second + "\r\n";
    response += "\r\n\r\n";
    response += _response_body;
    return (response);
}

void    Respond::init_response_body(std::string file, std::string _root)
{
    std::ifstream file_;
    std::string line;

    std::string f;
    f = _root + "/" + file;
    file_.open(f);
    if (file_.is_open())
    {
        while (getline(file_, line))
            _response_body += line + "\n";
        file_.close();
    }
    else
        std::cout << "Unable to open file" << std::endl;
}