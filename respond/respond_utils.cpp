/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 02:14:39 by aoumad            #+#    #+#             */
/*   Updated: 2023/04/11 18:42:00 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "respond.hpp"

void    Respond::ft_handle_redirection()
{
    std::string response;
    std::string body;

    body = "<html><head><title>301 Moved Permanently</title></head><body><h1>Moved Permanently</h1><p>The document has moved <a href=\"" + this->_uri + "\">here</a>.</p></body></html>";
    response = r.get_version() + " 301 Moved Permanently\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
    response += "Connection: keep-alive\r\n";
    response += "Location: " + r.get_uri() + "\r\n";
    response += "\r\n";
    response += body;
    return (response);
}

void    Respond::ft_handle_cgi()
{
}

int    ft_check_file()
{
    struct stat st;
    if (stat(server.get_root().c_str(), &st) == 0)
    {
        if (S_ISREG(st.st_mode)) // This macro returns non-zero if the file is a regular file.
            return (true);
    }
    else
        return (false);
}

void    Respond::ft_handle_file()
{
    std::ifstream file;
    if (_rooted_path != "")
    {
        file.open(_rooted_path);
        if (file.is_open())
        {
            _response_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _status_code = "200";
            _status_message = "OK";
            _headers["Content-Type"] = "text/html";
            _headers["Content-Length"] = std::to_string(_response_body.length());
            _headers["Connection"] = "keep-alive";
        }
        else
        {
            _status_code = "404";
            _status_message = "Not Found";
        }
    }
}
/*
Here's how it works:
• std::istreambuf_iterator<char>(file) constructs an input iterator that reads successive characters from the std::basic_streambuf object associated with the file object. This iterator is used to read the contents of the file.
• std::istreambuf_iterator<char>() constructs a default-constructed end-of-stream iterator. This iterator is used to indicate the end of the input sequence.
• The two iterators are passed as arguments to the std::string constructor, which constructs a new std::string object initialized with the characters read from the input sequence.
In other words, the std::string constructor reads the entire contents of the file into a std::string object. The resulting std::string object contains all the characters from the file, including any whitespace characters and newline characters.
*/

void    Respond::ft_handle_index()
{
    std::string index;
    
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server[i]._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                if (!server[i]._location[j].get_index())
                {
                    if (!server[i].get_index())
                    {
                        // show forbidden result
                    }
                    else
                    {
                        index = server[i].get_index();
                        _rooted_path = server[i].get_root() + _path_found + index;
                        ft_handle_index_2();
                    }
                }
                else
                {
                    index = server[i]._location[j].get_index();
                    _rooted_path = server[i].get_root() + _path_found + index;
                    ft_handle_index_2();
                }
            }
        }
    }
}

void    Respond::ft_handle_index_2()
{
}

void    Respond::ft_handle_autoindex()
{
        for (int j = 0; j < server[i]._location.size(); j++)
    {
        if (_path_found == server[i]._location[j].location_name)
        {
            if (!server[i]._location[j].get_autoindex())
            {
                if (!server[i].get_autoindex())
                {
                    // show forbidden result
                    ft_handle_error(403);
                }
                else
                {
                    // show autoindex
                }
            }
            else
            {
                // show autoindex-
            }
        }
    }
}