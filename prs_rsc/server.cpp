#include "server.hpp"
#include <sstream>
#include <string>





std::string toLower(std::string str) {
  for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
    *it = std::tolower(*it);
  }
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

int search_char(std::string str, char c)
{
    int i = 0;
    while (str[i])
    {
        if(str[i] == c)
            return (1);
        i++;
    }
    return (0);
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

int ft_port(std::string value, std::string line)
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
    return (port)
}

server::server(Data_config data)
{
    std::istringstream ss(data.data_server);
    std::string line;
    while (getline(ss, line))
    {
        if(line.empty() || line[0] == '#')
            continue;
        if (!search_char(line, '}') && !search_char(line, '{'))
        {
            if (line[line.size() - 1] != ';')
            {
                std::cerr << "missing ; in " << line << std::endl;
                exit (1);
            }
            line.erase(line.size() - 1)
        }
        std::string key, value;
        std::istringstream iss(line) ;
        iss >> key >> value;
        //std::cout << key << "|" << "\n";
        key = toLower(key);
        if (key == "server_name")
            _server_name.push_back(value);
        else if (key == "host")
            _host = value;
        else if (key == "listen")
        {
            if (value.empty())
            {
                std::cerr << " ERROOR : " << line << std::endl;
                exit (1);
            }
            int port = ft_port(value, line);
            _listen.push_back(port);
            while (iss >> value)
            {
                int port = ft_port(value, line);
                _listen.push_back(port);
            }
        }
        else if (key == "root")
            _root = value;
        else if (key == "client_max_body_size")
            _client_max_body_size = std::stoi(value);
        else if (key == "error_page")
        {
            int error_code = std::stoi(value);
            iss >> value;
            _error_page[error_code] = value;
        }
        else if (key == "meme_types")
        {

        }
        else if (key == "index")
        {
            _index.push_back(value);
            while (iss >> value)
                _index.push_back(value);
        }
        else if (key == "allow_methods")
        {
            _allow_methods.push_back(value);
            while (iss >> value)
                _allow_methods.push_back(value);
        }
        else if (key == "autoindex")
        {
            if (value == "on")
                _autoindex = true;
            else if (value == "of")
                _autoindex = false;
        }
        else if (!search_char(line, '}') && !search_char(line, '{'))
        {
            std::cerr << " ERROOR : " << line << std::endl;
            exit (1);
        }
    }
}

server::~server()
{
}