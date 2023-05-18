#include "location.hpp"
#include "server.hpp"
#include <sstream>
#include <string>

std::string toLower(std::string str)
{
  for (std::string::iterator it = str.begin(); it != str.end(); ++it)
    *it = std::tolower(*it);
  return str;
}

std::string toUpper(std::string str) {
  for (std::string::iterator it = str.begin(); it != str.end(); ++it) 
    *it = std::toupper(*it);
  return str;
}

// int count_lines(const std::string& str) {
//     int count = 1;
//     for (char c : str) {
//         if (c == '\n') {
//             count++;
//         }
//     }
//     return count;
// }

bool isIpAddress(const std::string& ip) {
    int numDots = 0;
    std::string temp = "";

    for (size_t i = 0; i < ip.size(); i++)
    {
        if (ip[i] == '.') 
        {
            numDots++;
            if (numDots > 3)
                return 0;
            if (temp.size() == 0 || std::stoi(temp) > 255)
                return 0;
            temp = "";
        } 
        else if (!isdigit(ip[i])) 
            return 0;
        else 
            temp += ip[i];
    }
    if (temp.size() == 0 || std::stoi(temp) > 255 || numDots != 3) 
        return 0;
    return 1;
}

int isDomainName(const std::string& domainName)
{

    if (domainName.empty())
        return 0;
    int i = 0;
    while (domainName[i]) 
    {
        char c =  domainName[i];
        if (!std::isalnum(c) && c != '-' && c != '.') 
            return 0;
        i++;
    }

    if (domainName.front() == '-' || domainName.back() == '-')
        return 0;
    if (domainName.find('.') == std::string::npos) 
        return 0;
    return 1;
}

int search_char(std::string str, char c)
{
    int i = 0;
    int k = 0;
    while (str[i])
    {
        if(str[i] == c)
            k++;
        i++;
    }
    return (k);
}

int is_number(const std::string& str)
{
    size_t i = 0;
    int exp = 0;
    int digit = 0;
    int flag = 0;

    for (i = 0; i < str.size(); i++)
    {
        if (i == 0 && (str[i] == '+' || str[i] == '-')) {
            i++;
            continue;
        }
        if (str[i] == 'e' || str[i] == 'E') {
            if (exp || !digit) {
                return 0;
            }
            exp = 1;
            if (i + 1 < str.size() && (str[i + 1] == '+' || str[i + 1] == '-')) {
                if(str[i + 1] == '-')
                    flag = 1;
                i++;
            }
            continue;
        }
        if(!isdigit(str[i]))
            return (0);
        digit = 1;
    }
    if(i == 1 && (str[0] == '+' || str[0] == '-'))
        return (0);
    if(flag)
        return (-1);
    return (1);
}

int ft_number(std::string value, std::string line)
{
    int port;
    if (!is_number(value))
    {
        std::cerr << " ERROOR : " << line << std::endl;
        exit (1);
    }
    try
    {
        port = std::stod(value);
    }
    catch(const std::exception& e)
    {
        std::cerr << " ERROOR : " << line << std::endl;
        exit (1);
    }
    return (port);
}

void ft_error(std::string line, std::string err)
{
    std::cerr << err <<" : " << line << std::endl;
    exit (1);
}

void is_empty_value(std::string value, std::string line)
{
        if (value.empty())
            ft_error(line, "Error");
}

int ft_numbers_value(std::istringstream &iss)
{
    int i = 0;
    std::string value;
    while (iss >> value)
        i++;
    return (i);
}

int ft_non_alphabetic(std::string value)
{
    int i = 0;
    while (value[i])
    {
        if (!isalpha(value[i]))
            return (1);
        i++;
    }
    return (0);
}

void ft_check_index(std::string index_file, std::string line)
{
    int i = index_file.find_last_of(".");
    if (i == -1)
        ft_error(line, "Error");
    std::string filedot = index_file.substr(i + 1);
    if (filedot != "html" && filedot != "php" && filedot != "htm" && filedot != "css"
        && filedot != "js" && filedot != "jpg" && filedot != "jpeg" && filedot != "png" && filedot != "gif"
        && filedot != "txt" && filedot != "xml" && filedot != "json") 
            ft_error(line, "Error");
}

void check_methods(std::string method, std::string line)
{
    method = toLower(method);
    if (method != "get" && method != "post" && method != "delete")
              ft_error(line, "Error this method is not allowed");

}

std::string ft_method(std::string value, std::string line, std::vector<std::string> v)
{
    value = toLower(value);
    check_methods(value, line);
    value = toUpper(value);
    std::vector<std::string>::iterator it = std::find(v.begin(), v.end(), value);
    if(it != v.end())
        ft_error(line, "error duplicate index");

    return (value);
}

int is_world(std::string str, std::string tmp)
{
    int i = 0;

    while (str[i] && !isspace(str[i]))
    {
        if(tmp[i] != str[i])
            return (0);
        i++;
    }
    if(isspace(str[i]))
        return (1);
    return(0);
}

server::server(Data_config data, bool check_location) 
     : _root("./www/html"), _client_max_body_size(1048576)
{
    std::istringstream ss(data.data_server);
    std::string line;
    int c_server_name = 0;
    int c_host = 0;
    int c_root = 0;
    int c_client_max_body_size = 0;
    int c_autoindex = 0;
    int c_allow_method = 0;
    int c_listen = 0;
    int c_index = 0;
    int c_error_page = 0;
    int c_redirection = 0;
    int c_upload = 0;
    int c_upload_store = 0;
    while (getline(ss, line))
    {
        // if (!check_location)
        //     std::cout << line << std::endl;
        if(line.empty() || line[0] == '#')
            continue;
        if (!search_char(line, '}') && !search_char(line, '{'))
        {
            
            if (line.find(';') != line.size() - 1)
            {
                std::cerr << "missing ; in " << line << std::endl;
                exit (1);
            }
            line.erase(line.size() - 1);
            line = trimString(line);
        }
        else if ((search_char(line, '}') || search_char(line, '{') ))
        {
            if (line.size() > 1)
            {
                if (!is_world(line, "server") && !is_world(line, "location"))
                    ft_error(line, "Error");
            }
            if(search_char(line, '{') > 1)
                ft_error(line, "Error");
        }
        std::string key, value;
        std::istringstream iss(line);
        iss >> key >> value;
        key = toLower(key);
        if (key == "server_name")
        {
            if(c_server_name)
                ft_error(line, "Duplicated");
            is_empty_value(value, line);
            if (ft_numbers_value(iss) || ft_non_alphabetic(value))
                ft_error(line, "Error");
            c_server_name++;
            _server_name.push_back(value);
        }
        else if (key == "host")
        {
            if(c_host)
                ft_error(line, "Duplicated");
            is_empty_value(value, line);
            if (ft_numbers_value(iss) || (!isIpAddress(value) && value != "localhost"))
                ft_error(line, "Error");
            c_host++;
            _host = value;
        }
        else if (key == "listen")
        {
            is_empty_value(value, line);
            int port = ft_number(value, line);
            if (port < 1 || port > 65535)
                ft_error(line, "Error");
            std::vector<int>::iterator it = std::find(_listen.begin(), _listen.end(), port);
            if(it != _listen.end())
                ft_error(line, "duplicate port");
            _listen.push_back(port);
            while (iss >> value)
            {
                int port = ft_number(value, line);
                if (port < 1024 || port > 65535)
                    ft_error(line, "Error");
                _listen.push_back(port);
            }
            c_listen++;
        }
        else if (key == "root")
        {   
            if(c_root)
                ft_error(line, "Duplicated");
            is_empty_value(value, line);
            if (ft_numbers_value(iss))
                ft_error(line, "Error");
            c_root++;
            _root = value;
        }
        else if (key == "client_max_body_size")
        {
            is_empty_value(value, line);
            if (ft_numbers_value(iss) || c_client_max_body_size)
                ft_error(line, "Error");
            int size = ft_number(value, line);
            if (size < 0)
                ft_error(line, "Error size is negative");
            c_client_max_body_size++;
            _client_max_body_size = size;
        }
        else if (key == "error_page")
        {
            int error_code = ft_number(value, line);
            if (error_code < 100 || error_code > 599)
                ft_error(line, "Error");
            is_empty_value(value, line);
            iss >> value;
            is_empty_value(value, line);
            if (ft_numbers_value(iss) )
                ft_error(line, "Error");
            std::map<int, std::string>::const_iterator it = _error_page.find(error_code);
             if (it != _error_page.end())
                ft_error(line, "Error");
            _error_page[error_code] = value;
            c_error_page++;
        }
        else if (key == "index")
        {
            if (c_index)
                ft_error(line, "Error duplicated");
            is_empty_value(value, line);
            //ft_check_index(value, line);
            _index = value;
            if (ft_numbers_value(iss))
                ft_error(line, "Error");
            c_index++;
        }
        else if (key == "allow_methods" && !check_location)
        {
            is_empty_value(value, line);
            value = ft_method(value, line, _allow_methods);
            check_methods(value, line);
            _allow_methods.push_back(value);
            while (iss >> value)
            {
                value = ft_method(value, line, _allow_methods);
                _allow_methods.push_back(value);
            }
            c_allow_method++;
        }
        else if (key == "autoindex")
        {
            if (c_autoindex)
                ft_error(line, "Error duplicated");
            is_empty_value(value, line);
            value = toLower(value);
            if (value == "on")
                _autoindex = true;
            else if (value == "off")
                _autoindex = false;
            else
                ft_error(line, "Error"); 
            if (ft_numbers_value(iss))
                ft_error(line, "Error");
            c_autoindex++;
        }
        else if (key == "return" && !check_location)
        {
            if (c_redirection)
                ft_error(line, "Error duplicated rederiction");
            is_empty_value(value, line);
            int status = ft_number(value, line);
            if (status < 300 || status > 400)
                ft_error(line, "Error");    
            iss >> value;
            is_empty_value(value, line);
            if (ft_numbers_value(iss) )
                ft_error(line, "Error");
            _redirection = std::make_pair(status, value);
            c_redirection++;
        }
        else if (key == "path_info" && !check_location)
        {
            is_empty_value(value, line);
           std::string cgi = toLower(value);
            if (value != ".py" && value != ".php")
                ft_error(line, "Error");
            iss >> value;
            is_empty_value(value, line);
            if (ft_numbers_value(iss) )
                ft_error(line, "Error");
            _path_info[cgi] = value;
        }
        else if (key == "upload" && !check_location)
        {
             if (c_upload)
                ft_error(line, "Error duplicated");
            is_empty_value(value, line);
            value = toLower(value);
            if (value == "on")
                _upload = true;
            else if (value == "off")
                _upload = false;
            else
                ft_error(line, "Error"); 
            if (ft_numbers_value(iss))
                ft_error(line, "Error");
            c_upload++;
        }
        else if (key == "upload_store" && !check_location)
        {
             if (c_upload_store)
                ft_error(line, "Error duplicated");
            is_empty_value(value, line);
            _upload_store = value;
            if (ft_numbers_value(iss))
                ft_error(line, "Error");
            c_upload_store++;
        }
        else if ((search_char(line, '}') || search_char(line, '{') ))
        {
            if (line.size() > 1)
            {
                if (!is_world(line, "server"))
                    ft_error(line, "Error");
            }
        }
        else
            ft_error(line, "Error");
    }

    if(!c_allow_method && !check_location)
        _allow_methods.push_back("GET");
    if (!c_listen && check_location)
        _listen.push_back(80);
    if (!c_index && check_location)
        _index = "index.html";
    if (!c_host && check_location)
        _host = "127.0.0.1";
    if(!c_error_page && check_location)
    {
        _error_page[400] = "/400.html";
        _error_page[401] = "/401.html";
        _error_page[403] = "/403.html";
        _error_page[404] = "/404.html";
        _error_page[405] = "/405.html";
        _error_page[500] = "/500.html";
    }
    
    if(data.location.size())
    {
        Data_config location_data;
        std::string location_name;
        std::ostringstream oss;
        for (std::map<std::string, std::string>::const_iterator it = data.location.begin(); it != data.location.end(); ++it) 
        {
            location_data.data_server = it->second;
            location_name = it->first;
            location *l = new location(location_data, location_name);
            l->fill_rest(*this);
            _location.push_back(*l);
            delete(l);
        }
    }
}

void server::display_sever()
{ 
    std::cout << "listen : "; 
    for (size_t i = 0; i < _listen.size(); i++) 
        std::cout << _listen[i] << " ";
    std::cout << std::endl;
    std::cout << "Server name : "; 
    for (size_t i = 0; i < _server_name.size(); i++) 
        std::cout << _server_name[i] << " ";
    std::cout << std::endl;
    std::cout << "Index : " << _index ;
    // for (size_t i = 0; i < _index.size(); i++) 
    //     std::cout << _index[i] << " ";
    std::cout << std::endl;
    std::cout << "hostname : " << _host << std::endl;
    std::cout << "root : " << _root << std::endl;
    std::cout << "client max body size : " << _client_max_body_size << std::endl;
    std::cout << "error pages : \n"; 
    for(std::map<int, std::string>::const_iterator it = _error_page.begin(); it != _error_page.end(); ++it)
        std::cout << it->first << "  "<< it->second<< "\n";
    std::cout << "allow methods : "; 
    for (size_t i = 0; i < _allow_methods.size(); i++) 
        std::cout << _allow_methods[i] << " ";
    std::cout << std::endl;
    std::cout << "autoindex : ";
    if (_autoindex)
        std::cout << "on\n";
    else
         std::cout << "of\n";
    std::cout << "rederiction : " << _redirection.first << ", " << _redirection.second << '\n';
    std::cout << "upload_store :"<< _upload_store << "\n";
    std::cout << "path_info : \n";
    for(std::map<std::string, std::string>::const_iterator it = _path_info.begin(); it != _path_info.end(); ++it)
        std::cout << it->first << "  "<< it->second<< "\n";
}

std::string server::get_index() const
{
    return (_index);
}

std::string server::get_root() const
{
    return (_root);
}

std::map<int, std::string> server::get_error_page() const
{
    return (_error_page);
}

std::vector<std::string> server::get_allow_methods() const
{
    return (_allow_methods);
}

bool server::get_autoindex () const
{
    return (_autoindex);
}

std::vector<int> &server::get_listen() 
{
    return(_listen);
}

std::vector<std::string> server::get_server_name() const
{
    return(_server_name);
}

std::string server::get_host() const
{
    return(_host);
}

int server::get_client_max_body_size() const
{
    return(_client_max_body_size);
}

std::pair <int , std::string> server::get_redirection() const
{
    return (_redirection);
}

std::string server::get_upload_store() const
{
    return(_upload_store);
}

std::map<std::string ,std::string> server::get_path_info() const
{
    return (_path_info);
}


server& server::operator=(const server& o)
{
    if(this != &o)
    {
        this->_server_name = o._server_name;
        this->_index = o._index;
        this->_host = o._host;
        this->_root = o._root;
        this->_client_max_body_size = o._client_max_body_size;
        this->_error_page = o._error_page;
        this->_allow_methods = o._allow_methods;
        this->_redirection = o._redirection;
        this->_path_info = o._path_info;
        this->_upload_store = o._upload_store;
        this->_autoindex = o._autoindex;
        this->_upload = o._upload;
        this->_listen = o._listen;
        this->_location = o._location;
    }
    return (*this);
}

bool server::get_upload() const
{
    return(_upload);
}

server::server(const server& o)
{
    *this = o;
}

server::~server()
{

}

server::server()
{

}