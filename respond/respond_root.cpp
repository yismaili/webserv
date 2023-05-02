/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond_root.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 14:53:31 by aoumad            #+#    #+#             */
/*   Updated: 2023/04/30 15:29:13 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "respond.hpp"

void    Respond::response_root()
{
    // step 1 :check the location
    if (ft_parse_location())
    {
        handle_error_response(_status_code);
        return ;
    }

    // step 2 : check the redirectation
    if (ft_parse_url_forwarding())
    {
        handle_error_response(_status_code);
        return ;
    } 

    // step 3 : check the validation of rooted path
    if (ft_parse_root_path())
    {
        handle_error_response(_status_code);
        return ;
    }
    
    // step 4 : check the allowed methods
    if (ft_check_allowed_methods())
    {
        handle_error_response(_status_code);
        return ;
    }

    // step 5 : check the autoindex
    ft_check_autoindex();

    // handle redirection in case it's true
    if (_is_redirection == true)
        ft_handle_redirection();

    // methods area
    if (r.get_method() == "Get")
        handle_get_response();
    else if (r.get_method() == "Post")
        handle_post_response();
    else if (r.get_method() == "Delete")
        handle_delete_response();
    else // unsupported http method
        handle_error_response(405);
    
    // set the response
    print_response();
}

int Respond::ft_parse_location()
{
    // exact location body code
    std::string path = r.get_uri();
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server[i]._location.size(); j++)
        {
            if (server[i]._location[j].location_name == path)
            {
                _path_found = server[i]._location[j].location_name;
                return (0);
            }
        }
    }

    // prefix location body code
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server[i]._location.size(); j++)
        {
            if (path.find(server[i]._location[j].location_name) == 0)
            {
                _path_found = server[i]._location[j].location_name;
                return (0);
            }
        }
    }

    // regex location body code
    std::string::size_type pos = path.find(".");
    if (pos != std::string::npos)
    {
        std::string extension = ".*" + path.substr(pos);
        for (int i = 0; i < server.size(); i++)
        {
            for (int j = 0; j < server[i]._location.size(); j++)
            {
                if (server[i]._location[j].location_name == extension)
                {
                    _path_found = server[i]._location[j].location_name;
                    _is_cgi = true;
                    return (0);
                }
            }
        }
    }

    // root location
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server._location.size(); j++)
        {
            if (server[i]._location[j].location_name == "/")
            {
                _path_found = server[i]._location[j].location_name;
                return (0);
            }
        }
    }
    
    set_status_code(404);
    return (1);
}

int Respond::ft_parse_url_forwarding()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0;  j < server._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                // check for redirection ===== where redirection is make_pair
                if (!server[i]._location[j].redirection.first.first.empty() &&
                    !server[i]._location[j].redirection.first.second.empty())
                {
                    int status_code = server[i]._location[j].redirection.second;
                    // search for message of the status_code
                    std::string message = get_response_status(status_code);
                    set_status_code(status_code);
                    set_status_message(message);
                    set_header("Location", server[i]._location[j].redirection.first);
                    _is_redirection = true;
                    return (0);
                }
            }
            else
            {
                set_status_code(200);
                set_status_message("OK");
                return (1);
            }
        }
    }
}

int Respond::ft_check_allowed_methods()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; server._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                // get the autoindex
                // _autoindex = server[i]._location[j].autoindex;
                // check for allowed methods
                std::vector<std::string> allowed_methods = server[i]._location[j].allowed_methods;
                for (int k = 0; k < allowed_methods.size(); k++)
                {
                    if (allowed_methods[k] == r.get_method())
                    {
                        _is_allowed_method = true;
                        return (0);
                    }
                }
            }
        }
    }

    set_status_code(405);
    return (1);
}

void    Respond::ft_check_autoindex()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; server._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                // check for autoindex
                if (server[i]._location[j].autoindex == true)
                {
                    _is_autoindex = true;
                    return ;
                }
            }
        }
    }
}