#ifndef SERVER_HPP
#define SERVER_HPP

#include "Data_config.hpp"
#include <iostream>
#include <vector>
#include <utility>
#include <map>

class location;
class server
{
protected:
    std::string _index;
    std::string _root; 
    int _client_max_body_size; /*l*/
    std::map<int, std::string> _error_page;
    std::vector<std::string> _allow_methods;
    std::pair <int , std::string> _redirection;
    std::string _upload_store;
    bool _autoindex;
    bool _upload;
    // std::vector<std::string> _access_log;
    // std::vector<std::string> _error_log;
    //std::vector<std::string> _meme_types;
    // std::string _gzip;
    // std::string _ssl_certificate;
    // std::string _ssl_certificate_key;
    // std::string _ssl_protocols;
    // std::string _ssl_ciphers;
    std::map<std::string ,std::string> _path_info; 
public:
    std::string _host; /*s*/
    std::vector<std::string> _server_name; /*s*/
    std::vector<int> _listen; /*s*/
    std::vector<location> _location;
    server(Data_config data, bool check_location);
    server& operator=(const server& o);
    server(const server& o);
    server();
    ~server();
    void display_sever();
    std::vector<int> &get_listen();
    std::vector<std::string> get_server_name() const;
    std::string get_host() const;
    int get_client_max_body_size() const;
    std::string get_index() const;
    std::string get_root() const;
    std::map<int, std::string> get_error_page() const;
    std::vector<std::string> get_allow_methods() const;
    bool get_autoindex () const;
    bool get_upload() const;
    std::pair <int , std::string> get_redirection() const;
    std::string get_upload_store() const;
    std::map<std::string ,std::string> get_path_info() const;

};

int is_world(std::string word, std::string str);
int search_char(std::string str, char c);
std::string trimString(const std::string& str);
int skip_spaces(std::string str);
std::vector<server> ft_fill_servers(char **av);
std::vector<int> get_all_ports(std::vector<server> servers);

#endif