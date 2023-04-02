#include "location.hpp"


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



int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Error: could not open config file.\n";
        return (1);
    }

    std::vector<server> server;
    std::string config_file;
    std::ifstream file;
    std::string line;
    //std::string key;
    std::string map;
    Data_config data;
    std::vector<Data_config> v;
    int c = 0;
    int i = 0, j = 0;
    config_file = av[1];
    file.open(config_file);
    if(file.is_open())
    {
        while (!file.eof())
        {
             std::getline(file, line);
            c  += search_char(line, '{');
            c -= search_char(line, '}');
            i = skip_spaces(line);

             if (is_world(&line[i], "server"))
                 j = 1;
            if (is_world(&line[i], "location"))
            {
                std::string key = line + '\n';
                while (!file.eof())
                {
                    std::getline(file, line);
                    line = trimString(line);
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
            else if ((!j && i != line.size()))
            {
                std::cerr << "error something outside of server\n";
                return (1);
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
    for (int i = 0; i < v.size(); i++) 
        server.push_back(v[i]);
        // std::cout << "++++++++++++++++++++++\n";
        // std::cout << v[i].data_server << " ";
        // puts("------------------------------");
        // for(auto it = v[i].location.begin(); it != v[i].location.end(); ++it)
        // {
        //     std::cout << it->first << it->second<< "\n";
        // }
   // }
   for (int i = 0; i < server.size(); i++)
   {
        server[i].display_sever();
        std::cout << "locations :::::::::::::::::::::::::::::::::::::::::::: \n";
        for (int j = 0; j < server[i]._location.size(); j++)
        {
            std::cout << "location : " << server[i]._location[j].location_name << std::endl;
            server[i]._location[j].display_sever();
        }
        std::cout << "++++++++++++++++++++++\n";
   }
}