#ifndef DATA_CONFIG_HPP
#define DATA_CONFIG_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>
#include <map>

struct Data_config
{
    std::string data_server;
    std::map<std::string, std::string> location;
};

struct compters
{
    int c_server_name;
    int c_host;
    int c_root;
    int c_client_max_body_size;
};


#endif