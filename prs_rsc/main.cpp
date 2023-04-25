#include "location.hpp"

std::vector<server> servers;
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

// std::vector<server> get_data()
// {

// }



int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Error: could not open config file.\n";
        return (1);
    }

    std::vector<server> servers;
    std::string config_file;
    std::ifstream file;
    std::string line;
    //std::string key;
    std::string map;
    Data_config data;
    std::vector<Data_config> v;
    int c = 0;
    size_t i = 0, j = 0;
    int flag = 0;
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
            c  += search_char(line, '{');
            c -= search_char(line, '}');
            i = skip_spaces(line);

             if (is_world(&line[i], "server"))
                 j = 1;
            if (is_world(&line[i], "location"))
            {
                if(!j)
                {
                    std::cerr << "error something outside of server\n";
                    return (1);
                }
                line = trimString(line);
                std::string key = line + '\n';
                while (!file.eof())
                {
                    std::getline(file, line);
                    c  += search_char(line, '{');
                    map += line + '\n';
                    if (search_char (line, '}'))
                    {
                        c -= search_char(line, '}'); 
                        std::map<std::string, std::string>::const_iterator it = data.location.find(key);
                        if (it != data.location.end())
                        {
                            std::cerr << "Error dupplicate location\n";
                            return (1);
                        }
                        data.location.insert(std::make_pair(key, map));
                        map.clear();
                        break;
                    }
                }
            }
            else if (!j)
            {
                i = skip_spaces(line);
                if (i != line.size())
                {
                    std::cerr << "error something outside of server\n";
                    return (1);
                }
            }
            else if (i != line.size())
            {
                line = trimString(line);
                data.data_server += line + "\n";
            }
            if (!c && j)
            {
                j = 0;
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
        return (1);
    }
    if (c)
    {
        std::cerr << "Error : not closed" << std::endl;
        return (1);
    }
    std::cout << v.size() << "\n";

    for (size_t i = 0; i < v.size(); i++)
    {
        server *s = new server(v[i], 1);
        servers.push_back(*s);
        delete (s);
    }

   for (size_t i = 0; i < servers.size(); i++)
   {
        servers[i].display_sever();
        std::cout << "locations :::::::::::::::::::::::::::::::::::::::::::: \n";
        for (size_t j = 0; j < servers[i]._location.size(); j++)
        {
            std::cout << "location : " << servers[i]._location[j].location_name << std::endl;
            servers[i]._location[j].display_sever();
        }
        std::cout << "++++++++++++++++++++++\n";
   }
}