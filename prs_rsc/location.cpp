#include "location.hpp"

location::location(Data_config data, std::string location_name) : server(data)
{
    this->location_name = location_name;

}

location::~location()
{
}

void location::fill_rest(server &s)
{
    if (_index.empty())
        _index = s.get_index();
    if (_root.empty())
        _root = s.get_root();
    if(_error_page.empty())
        _error_page = s.get_error_page();
    if (_allow_methods.empty())
        _allow_methods = s.get_allow_methods();
    if (!_autoindex)
        _autoindex = s.get_autoindex();
}
