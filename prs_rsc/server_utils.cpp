#include "location.hpp"
#include "../include/http_server.hpp"

int skip_spaces(std::string str)
{
    int i = 0;
    while(str[i])
    {
        if(isspace(str[i]))
            i++;
        else
            return (i);
    }
    return (i);
}


std::string trimString(const std::string& str)
{
  std::string trimmedStr = str;

  std::string::iterator it = trimmedStr.begin();
  while (it != trimmedStr.end() && std::isspace(*it)) 
    ++it;
  trimmedStr.erase(trimmedStr.begin(), it);
  it = trimmedStr.end();
  while (it != trimmedStr.begin() && std::isspace(*(it - 1)))
    --it;
  trimmedStr.erase(it, trimmedStr.end());
  return trimmedStr;
}

std::vector<server> ft_fill_servers(char **av)
{
    std::vector<server> servers;
    std::string config_file;
    std::ifstream file;
    std::string line;
    std::string map;
    std::string previousLine;
    Data_config data;
    std::vector<Data_config> v;
    int c = 0;
    size_t i = 0, j = 0;
    int flag = 0;
    int kws = 0;
    config_file = av[1];
    file.open(config_file);
    if(file.is_open())
    {
        while (!file.eof())
        {
            std::getline(file, line);
            line = trimString(line);
            if(line.empty() || line[0] == '#')
                continue;
            c += search_char(line, '{');
            if (!is_world(&line[i], "location"))
                kws += search_char(line, '{');
                c -= search_char(line, '}');
             if (is_world(&line[i], "server"))
             {
                j = 1;
                if (!search_char(line, '{'))
                {
                    while (!file.eof())
                    {
                        std::getline(file, line);
                        line = trimString(line);
                        if(line.empty() || line[0] == '#')
                            continue;
                        else
                            break ;
                    }
                    if (!search_char(line, '{'))
                    {
                        std::cerr << "error something outside of server\n";
                        exit (1);
                    }
                    c  += search_char(line, '{');
                    kws += search_char(line, '{');
                }
             }
            if (kws >  1)
            {
                std::cerr << "error \n";
                exit (1);
            }
            if (is_world(&line[i], "location"))
            {
                if(!j)
                {
                    std::cerr << "error something outside of server\n";
                    exit (1);
                }
                kws = 0;
                kws = search_char(line, '{');             
                std::string key = line + '\n';
                while (!file.eof())
                {
                    std::getline(file, line);
                    line = trimString(line);
                    if(line.empty() || line[0] == '#')
                        continue;
                    kws +=  search_char(line, '{');
                    if (kws >  1)
                    {
                        std::cerr << "error \n";
                        exit (1);
                    }
                    c  += search_char(line, '{');
                    map += line + '\n';
                    if (search_char (line, '}'))
                    {
                        c -= search_char(line, '}'); 
                        data.location.insert(std::make_pair(key, map));
                        map.clear();
                        break;
                    }
                }
            }
            else if (!j)
            {
                if (!line.empty())
                {
                    std::cerr << "error something outside of server\n";
                    exit (1);
                }
            }
            else if (!line.empty())
                data.data_server += line + "\n";
            if (!c && j)
            {
                j = 0;
                kws = 0;
                v.push_back(data);
                data.data_server.clear();
                data.location.clear();
            }
            flag++;
        }        
    }
    else
    {
        std::cerr << "No such file\n";
        exit (1);
    }
    if (c)
    {
        std::cerr << "Error : not closed" << std::endl;
        exit (1);
    }
    
    for (size_t i = 0; i < v.size(); i++)
    {
        c = 0;
        server *s = new server(v[i], 1);
        for (size_t i = 0; i < s->_location.size(); i++)
        {
            if (s->_location[i].location_name == "/")
            {
                c = 1;
                break ;
            }
        }
        if (c != 1)
        {
            std::cerr << "Error : missing location root\n";
            delete (s);
            exit (1);
        }
        servers.push_back(*s);
        delete (s);
    }
    return (servers);
}

std::vector<int> get_all_ports(std::vector<server> servers)
{
    std::vector<int> all_ports;
    for (size_t i = 0; i < servers.size(); i++)
    {
        for (size_t j = 0; j < servers[i]._listen.size(); j++)
            all_ports.push_back(servers[i]._listen[j]);
    }
    return (all_ports);
}