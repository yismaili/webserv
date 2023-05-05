#include "location.hpp"
std::string extract_path(std::string location_name)
{
    size_t pos = std::string("location").length();
	for (size_t i = pos; i < location_name.size(); i++)
	{
		if (!isspace(location_name[i]))
			break;
		pos++;			
	}
	int start = pos;
	size_t i;
	int flag = 0;
	for (i = pos; i < location_name.size(); i++)
	{
		if (isspace(location_name[i]))
		{
			flag = 1;
			break;
		}
		if(location_name[i] == '{')
			break;
	}
	
	int end = i;
	if(flag)
	{
		while (i < location_name.size())
		{
			if (!isspace(location_name[i]) && location_name[i] != '{')
			{
				std::cerr << "error : "<< location_name << std::endl;
				exit(1);
			}
			i++;
		}
	}
	std::string path;
	path = location_name.substr(start, end - start);
	return (path);
}

location::location(Data_config data, std::string location_name) : server(data, 0)
{
    this->location_name = extract_path(location_name);
    std::cout <<"|"<<this->location_name<<"|"<<std::endl;
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
