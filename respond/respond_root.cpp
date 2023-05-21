/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond_root.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 14:53:31 by aoumad            #+#    #+#             */
/*   Updated: 2023/05/20 15:03:32 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "respond.hpp"

std::string Respond::response_root(std::vector<server> servers)
{
    int rtn_location = 0;
    // std::cout << "method: " << r.get_method() << std::endl;
    // std::cout << "boundary: " << r.get_boundary() << std::endl;
    // std::cout << "content type: " << r.get_header("Content-Type") << std::endl;
    // std::cout << "content length: " << r.get_header("Content-Length") << std::endl;
    // std::cout << "request body: " << r.get_body() << std::endl;
    // std::cout << "r method: " << r.get_method() << std::endl;
    // std::cout << "uri: " << r.get_uri() << std::endl;
   // std::cout << "___________________-----_______---________----_____--___--__-_-_-_--_--" << std::endl;
    init_response_body(servers[_server_index].get_index(), servers[_server_index].get_root());
    // step 1 :check the location

    
    // if(_uri != "")
    // r.set_uri(_uri + r.get_uri());
    // _uri = "";
    rtn_location = ft_parse_location(servers, false);
    if (rtn_location)
    {
        if (root_location(servers) == 1)
        {
            handle_error_response(404);
            return (rtn_response()); 
        }
    }
    else if (rtn_location == 2)
        return (rtn_response());
    // step 2 : check the redirectation
    if (!ft_parse_url_forwarding(servers))
        return (rtn_response());
    // step 3 : check the validation of rooted path
    if (ft_parse_root_path(servers))
    {
        handle_error_response(_status_code);
        return (rtn_response());
    }
    // std::cout << "our new uri: " << _uri << std::endl;
    // step 4 : check the allowed methods
    if (ft_check_allowed_methods(servers))
    {
        handle_error_response(_status_code);
        return (rtn_response());
    }
    // step 5 : check the autoindex
    ft_check_autoindex(servers);
    // methods area
    if (r.get_method() == "GET")
        handle_get_response(servers);
    else if (r.get_method() == "POST")
        handle_post_response(servers);
    else if (r.get_method() == "DELETE")
        handle_delete_response();
    else // unsupported http method
        handle_error_response(405);
    // rtn response
    return (!rtn_response().empty() ? rtn_response() : "ERROR in returning response");
}

int Respond::exact_location(std::vector<server> server, std::string path)
{
        for (size_t j = 0; j < server[_server_index]._location.size(); j++)
        {
            if (server[_server_index]._location[j].location_name == path)
            {
            //     std::cout << "path :" << path << std::endl;
            //     std::cout << server[_server_index]._location[j].get_root() << std::endl;
            //    std::cout << server[_server_index]._location[j].get_index() << std::endl;
                _location_index = j;
                _path_found = server[_server_index]._location[j].location_name;
                return (0);
            }
        }
    return (1);
}

int Respond::prefix_location(std::vector<server> server, std::string &path)
{
    size_t pos;

    if (path == "/")
        return (1);

        for (size_t j = 0; j < server[_server_index]._location.size(); j++)
        {
            if (server[_server_index]._location[j].location_name == path)
            {
                _location_index = j;
                _path_found = server[_server_index]._location[j].location_name;
                return (0);
            }
            
        }
    pos = path.find_last_of('/');
    if (pos != std::string::npos)
    {
        _removed_path = path.substr(pos) + _removed_path;
        path = path.substr(0, pos);
        return (prefix_location(server, path));
    }
    return (1);
}

int Respond::dynamic_location(std::vector<server> server, std::string path)
{
    std::string::size_type pos = path.find(".");
    if (pos != std::string::npos)
    {
        std::string extension = path.substr(pos);
        if (!prefix_location(server, path))
        {
            // std::cout << path << std::endl;
            //std::cout << "server index: " << _server_index << " and location index: " << _location_index << std::endl;
            std::map<std::string, std::string> path_info = server[_server_index]._location[_location_index].get_path_info();
            for (std::map<std::string, std::string>::const_iterator it = path_info.begin();
                it != path_info.end(); ++it)
            {
                if (it->first == extension)
                {
                    if(it->first == ".php")
                        _path_info_founded = server[_server_index]._location[_location_index].get_root() + it->second;
                    else
                        _path_info_founded = it->second;
                    std::string::size_type end_pos = _removed_path.find_last_of('/');
                    if (end_pos == std::string::npos)
                        return (1);
                    _file_cgi = _removed_path.substr(end_pos);
                    _is_cgi = true;
                    return (0);
                }
            }
        }
        if (extension == ".php" || extension == ".py")
        {
            handle_error_response(403);
            return (2);
        }
    }
    return (1);
}

int Respond::root_location(std::vector<server> server)
{
    for (size_t j = 0; j < server[_server_index]._location.size(); j++)
    {
        if (server[_server_index]._location[j].location_name == "/")
        {
            _location_index = j;
            _path_found = server[_server_index]._location[j].location_name;
            // std::cout << "path found: " << _path_found << std::endl;
            // std::cout << "root: " << server[_server_index]._location[j].get_root() << std::endl;
            return (0);
        }
    }
    return (1);
}

int Respond::ft_parse_location(std::vector<server> server, bool flag)
{
    std::string path = "";
    int rtn_cgi = 0;
    if (flag == false)
        path = r.get_uri();
    else if (flag == true)
        path = _uri;
    // exact location body code
    if (exact_location(server, path) == 0)
        return (0);
    // regex location body code
    rtn_cgi = dynamic_location(server, path);
    if (rtn_cgi == 0)
        return (0);
    else if (rtn_cgi == 2)
        return (2);
    _removed_path = "";
    // prefix location body code
    if (prefix_location(server, path) == 0)
        return (0);
    return (1);
}

int Respond::ft_parse_url_forwarding(std::vector<server> server)
{
           // std::cout << "___________________-----_______---________----_____--___--__-_-_-_--_--" << std::endl;
        if (_path_found == server[_server_index]._location[_location_index].location_name)
        {
            // check for redirection ===== where redirection is make_pair
            // std::cout << "___________________-----_______---________----_____--___--__-_-_-_--_--" << std::endl;
            // std::cout << server[i]._location[j].get_redirection().first << std::endl;
            // std::cout << server[i]._location[j].get_redirection().second << std::endl;
            if (!server[_server_index]._location[_location_index].get_redirection().second.empty())
            {
                std::cout << server[_server_index]._location[_location_index].get_redirection().second << "\n";
                size_t status_code = server[_server_index]._location[_location_index].get_redirection().first;
                // search for message of the status_code
                set_status_code(status_code);
                set_status_message(get_response_status(status_code));
                set_header("Location", server[_server_index]._location[_location_index].get_redirection().second);
                set_cache_control("no cache");
                _is_redirection = true;
                return (0);
            }
        }
    return (1);
}

int Respond::ft_check_allowed_methods(std::vector<server> server)
{
    if (_path_found == server[_server_index]._location[_location_index].location_name)
    {
        // get the autoindex
        // _autoindex = server[i]._location[j].autoindex;
        // check for allowed methods
        std::vector<std::string> allowed_methods = server[_server_index]._location[_location_index].get_allow_methods();
        for (size_t k = 0; k < allowed_methods.size(); k++)
        {
            if (allowed_methods[k] == r.get_method())
            {
                _is_allowed_method = true;
                return (0);
            }
        }
    }
    set_status_code(404);
    set_status_message(get_response_status(get_status_code()));
    return (1);
}

void    Respond::ft_check_autoindex(std::vector<server> server)
{
    if (_path_found == server[_server_index]._location[_location_index].location_name)
    {
        // check for autoindex
        if (server[_server_index]._location[_location_index].get_autoindex() == true)
        {
            _is_autoindex = true;
            return ;
        }
    }
}