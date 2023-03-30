#include "server.hpp"
#include <sstream>
#include <string>


std::string toUpper(std::string str) {
  for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
    *it = std::toupper(*it);
  }
  return str;
}

server::server(Data_config data)
{
     //std::cout << data.data_server;
    std::istringstream ss(data.data_server);
    std::string line;
    while (getline(ss, line))
    {
            if(line.empty() || line[0] == '#')
                continue;
            std::string key, value;
            std::istringstream iss(line) ;
            iss >> key >> value;
            key = toUpper(key);
            if (key == "server_name") {
            _server_name.push_back(value);
        } else if (key == "host") {
            _host = value;
        } else if (key == "listen") {
            _listen.push_back(std::stoi(value));
        } else if (key == "root") {
            _root = value;
        } else if (key == "client_max_body_size") {
            _client_max_body_size = std::stoi(value);
        }
    }
}

server::~server()
{
}