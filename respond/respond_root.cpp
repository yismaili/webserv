/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond_root.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 14:53:31 by aoumad            #+#    #+#             */
/*   Updated: 2023/05/08 18:03:36 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "respond.hpp"

std::string Respond::response_root(std::vector<server> servers)
{
    // step 1 :check the location
    if (ft_parse_location(servers))
    {
        // considering the location of the root directory is indeed the final step in handling requests when no specific location matches the requested URI.
        // if (root_location(servers) == 0)
        if (root_location(servers) == 1)
            handle_error_response(404);
            return (rtn_response());
            // direct it to the GET response and see if i can read the file concatenated with the root using `stat`
    }
    // step 2 : check the redirectation
    if (!ft_parse_url_forwarding(servers))
        return (rtn_response());
    // step 3 : check the validation of rooted path
    if (ft_parse_root_path(servers))
    {
        handle_error_response(_status_code);
        return (rtn_response());
    }

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
    std::cout << path << std::endl;
    std::string::size_type pos = path.find(".");
    std::cout << pos << std::endl;
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
    }
    return (1);
}

// int Respond::dynamic_location(std::vector<server> server, std::string path)
// {
//     std::string::size_type pos = path.find(".");
//     if (pos != std::string::npos)
//     {
//         // std::string extension = "*" + path.substr(pos);
//         for (size_t i = 0; i < server.size(); i++)
//         {
//             for (size_t j = 0; j < server[i]._location.size(); j++)
//             {
//                 if (server[i]._location[j].location_name == path)
//                 {
                    
//                 }
//                 for (std::map<std::string, std::string>::iterator it = server[i]._location[j]._path_info.begin(); it != server[i]._location[j]._path_info.end(); it++)
//                 {
//                     if (it->first == extension)
//                     {
//                         _server_index = i;
//                         // _path_found = it->second;
//                         _is_cgi = true;
//                         prefix_location(server, path);
//                         return (0);
//                     }
//                 }
//             }
//         }
//     }
//     return (1);
// }

int Respond::root_location(std::vector<server> server)
{
    for (size_t i = 0; i < server.size(); i++)
    {
        for (size_t j = 0; j < server[i]._location.size(); j++)
        {
            if (server[i]._location[j].location_name == "/")
            {
                _server_index = i;
                _location_index = j;
                _path_found = server[i]._location[j].location_name;
                return (0);
            }
        }
    }
    return (1);
}

int Respond::ft_parse_location(std::vector<server> server)
{
    std::string path = r.get_uri();
    // exact location body code
    if (exact_location(server, path) == 0)
        return (0);

    // regex location body code
    if (dynamic_location(server, path) == 0)
        return (0);

    // prefix location body code
    if (prefix_location(server, path) == 0)
        return (0);

    return (1);
}

int Respond::ft_parse_url_forwarding(std::vector<server> server)
{
    for (size_t j = 0;  j < server[_server_index]._location.size(); j++)
    {
        if (_path_found == server[_server_index]._location[j].location_name)
        {
            // check for redirection ===== where redirection is make_pair
            // std::cout << "___________________-----_______---________----_____--___--__-_-_-_--_--" << std::endl;
            // std::cout << server[i]._location[j].get_redirection().first << std::endl;
            // std::cout << server[i]._location[j].get_redirection().second << std::endl;
            // std::cout << "___________________-----_______---________----_____--___--__-_-_-_--_--" << std::endl;
            if (!server[_server_index]._location[j].get_redirection().second.empty())
            {
                size_t status_code = server[_server_index]._location[j].get_redirection().first;
                // search for message of the status_code
                std::string message = get_response_status(status_code);
                set_status_code(status_code);
                set_status_message(get_response_status(get_status_code()));
                set_header("Location", server[_server_index]._location[j].get_redirection().second);
                _is_redirection = true;
                return (0);
            }
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
    for (size_t i = 0; i < server.size(); i++)
    {
        for (size_t j = 0; server[i]._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                // check for autoindex
                if (server[i]._location[j].get_autoindex() == true)
                {
                    _is_autoindex = true;
                    return ;
                }
            }
        }
    }
}