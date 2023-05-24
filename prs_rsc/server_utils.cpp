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

  size_t pos = 0;
  while (pos < trimmedStr.size() && std::isspace(trimmedStr[pos]))
    ++pos;
  trimmedStr.erase(0, pos);

  size_t end = trimmedStr.size();
  while (end > 0 && std::isspace(trimmedStr[end - 1]))
    --end;
  trimmedStr.erase(end);

  return trimmedStr;
}


std::vector<server> ft_fill_servers(char **av, int ac)
{
    std::vector<server> servers;
    std::vector<Data_config> v;
    Data_config data;
    std::string config_file;
    std::ifstream file;
    std::string line;
    std::string map;
    int c = 0;
    int flag = 0;
    int kws = 0;
    size_t i = 0, is_server = 0;
    if (ac == 2)
        config_file = av[1];
    else
        config_file = "./config/server.conf";
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
                is_server = 1;
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
                if(!is_server)
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
                        std::map<std::string, std::string>::const_iterator it = data.location.find(key);
                        if (it != data.location.end())
                        {
                            std::cerr << "Error dupplicate location\n";
                            exit (1);
                        }
                        data.location.insert(std::make_pair(key, map));
                        map.clear();
                        break;
                    }
                }
            }
            else if (!is_server)
            {
                if (!line.empty())
                {
                    std::cerr << "erraor something outside of server\n";
                    exit (1);
                }
            }
            else if (!line.empty())
                data.data_server += line + "\n";
            if (!c && is_server)
            {
                is_server = 0;
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
    if(v.size() == 0)
    {
        std::cerr << "Error : config file empty" << std::endl;
        exit (1);
    }
    for (size_t i = 0; i < v.size(); i++)
    {
        server s = server(v[i], 1);
        servers.push_back(s);
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