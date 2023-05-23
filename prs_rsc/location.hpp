#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "server.hpp"
#include "Data_config.hpp"
#include <iostream>
#include <vector>
#include <map>

class location : public server
{
public:
    std::string location_name;
    location(Data_config data, std::string location_name);
    void fill_rest(server &s);
     ~location();
};

#endif