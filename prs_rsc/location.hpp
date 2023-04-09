#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "server.hpp"
#include "Data_config.hpp"
#include <iostream>
#include <vector>
#include <map>

class location : public server
{
private:
    // std::vector<int> _listen; /*s*/
    // std::vector<std::string> _server_name; /*s*/
    // std::vector<std::string> _index;
    // std::string _host; /*s*/
    // std::string _root;
    // int _client_max_body_size;
    // std::map<int, std::string> _error_page;
    // std::vector<std::string> _allow_methods;
    // bool _autoindex;
    // // std::vector<std::string> _access_log;
    // // std::vector<std::string> _error_log;
    // //std::vector<std::string> _meme_types;
    // // std::string _gzip;
    // // std::string _ssl_certificate;
    // // std::string _ssl_certificate_key;
    // // std::string _ssl_protocols;
    // // std::string _ssl_ciphers;
    // //std::vector<location> location
public:
    std::string location_name;
    location(Data_config data, std::string location_name);
    void fill_rest(server &s);
     ~location();
};

#endif