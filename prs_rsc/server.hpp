#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>

class server
{
private:
    std::vector<int> _listen;
    std::vector<std::string> _server_name;
    std::vector<std::string> _index;
    std::string _host;
    std::string _root;
    std::string _client_max_body_size;
    std::vector<std::string> _error_page;
    std::vector<std::string> _allow_methods;
    std::string _autoindex;
    // std::vector<std::string> _access_log;
    // std::vector<std::string> _error_log;
    std::vector<std::string> _meme_types;
    // std::string _gzip;
    // std::string _ssl_certificate;
    // std::string _ssl_certificate_key;
    // std::string _ssl_protocols;
    // std::string _ssl_ciphers;
    //std::vector<location> location
public:
    server();
    ~server();
};

// server::server(/* args */)
// {
// }

// server::~server()
// {
// }


#endif