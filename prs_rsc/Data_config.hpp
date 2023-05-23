#ifndef DATA_CONFIG_HPP
#define DATA_CONFIG_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>
#include <map>
#include "server.hpp"


struct Data_config
{
    std::string data_server;
    std::map<std::string, std::string> location;
};

#endif