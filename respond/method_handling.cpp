/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_handling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:52:50 by aoumad            #+#    #+#             */
/*   Updated: 2023/04/11 18:30:41 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "respond.hpp"

std::string Respond::handle_get_response()
{
    // step 1: checking if it's a redirection
    if (_is_redirection == true)
        ft_handle_redirection();
    
    // step 2: check if it's a CGI or not (like if `index` of the configuration file has .py or .php...etc)
    if (_is_cgi == true)
        ft_handle_cgi();
    
    // step 3: check if it's a file or not
    if (ft_check_file() == true)
        ft_handle_file();

    // step 4 : check the index in the configuration file and render it
    if (_is_index == true)
        ft_handle_index();
    
    // step 5: check if the autoindex if on or off
    ft_handle_autoindex();
    
    // ft_handle_error(404);
    
    return ("");
}

std::string Respond::handle_post_response()
{
    if (check_post_type() == "x-www-form-urlencoded" && _is_cgi == true)
    {
        // need to call cgi and execute it
        cout_response();
    }


}

std::string Respond::check_post_type()
{
    if(r.get_header("Content-Type").find("multipart") != std::string::npos)
        return ("form-data");
    else if(r.get_header("Content-Type").find("application/x-www-form-urlencoded") != std::string::npos)
        return ("x-www-form-urlencoded");
    else
        return ("regular");
}

void    Respond::ft_handle_delete_response()
{
    std::string content_type;
    
    content_type = get_content_type(_path_found);
    if (ft_check_file() == true)
    {
        if (std::remove(_rooted_path.c_str()) == 0)
        {
            _status_code = "200";
            _status_message = get_response_status(_status_code);
            set_response_body("File deleted successfully");
            set_header("Content-Type", content_type);
            set_header("Content-Length", std::to_string(_response_body.length()));
            set_header("Connection", "keep-alive");
        }
        else
        {
            _status_code = "500";
            _status_message = get_response_status(_status_code);
            set_response_body("Error deleting file");
            set_header("Content-Type", content_type);
            set_header("Content-Length", std::to_string(_response_body.length()));
            set_header("Connection", "keep-alive");
        }
    }
    else
    {
        _status_code = "404";
        _status_message = "Not Found";
        set_response_body("File not found");
        set_header("Content-Type", content_type);
        set_header("Content-Length", std::to_string(_response_body.length()));
        set_header("Connection", "keep-alive");
    }
}