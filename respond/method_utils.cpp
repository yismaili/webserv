/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 02:14:39 by aoumad            #+#    #+#             */
/*   Updated: 2023/05/15 21:04:10 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "respond.hpp"

int    Respond::ft_check_file()
{
    struct stat st;
    if (stat(_rooted_path.c_str(), &st) == 0)
    {
        if (S_ISREG(st.st_mode)) // This macro returns non-zero if the file is a regular file.
            return (1);
    }
    return (0);
}

void Respond::ft_handle_file()
{
    std::string::size_type mime_index = _rooted_path.find_last_of('.');
    if (mime_index != std::string::npos)
    {
        _mime_string = _rooted_path.substr(mime_index + 1);
    }
    std::ifstream file;
    if (!strcmp(_rooted_path.c_str(), ""))
    {
        handle_error_response(404);
        return;
    }
    file.open(_rooted_path.c_str());
    if (file.is_open())
    {
        _response_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        // file.close();
        set_status_code(200);
        set_status_message(get_response_status(200));
        set_header("Content-Type", get_mime_type(_mime_string));
        std::cout << "mime _type: " << get_mime_type(_mime_string) << std::endl;
         _headers["Content-Length"] = std::to_string(_response_body.length());
         std::cout << std::to_string(_response_body.length()) << std::endl;
        _headers["Connection"] = "keep-alive";
        set_date();
        set_cache_control("cache");
    }
    else
        handle_error_response(403);
}
/*
Here's how it works:
• std::istreambuf_iterator<char>(file) constructs an input iterator that reads successive characters from the std::basic_streambuf object associated with the file object. This iterator is used to read the contents of the file.
• std::istreambuf_iterator<char>() constructs a default-constructed end-of-stream iterator. This iterator is used to indicate the end of the input sequence.
• The two iterators are passed as arguments to the std::string constructor, which constructs a new std::string object initialized with the characters read from the input sequence.
In other words, the std::string constructor reads the entire contents of the file into a std::string object. The resulting std::string object contains all the characters from the file, including any whitespace characters and newline characters.
*/

int Respond::ft_handle_index(std::vector<server> server)
{
    std::string index;
    if (server[_server_index]._location[_location_index].get_index().empty())
    {
        if (server[_server_index].get_index().empty())
        {
            handle_error_response(403);
            return (1);
        }
        else
        {
            index = server[_server_index]._location[_location_index].get_index();
            _rooted_path = server[_server_index]._location[_location_index].get_root() + _removed_path + index;
            if (ft_handle_index_2(index))
                return (1);
        }
    }
    else
    {
        index = server[_server_index]._location[_location_index].get_index();

        std::string::size_type _mime_index= index.find_last_of('.');
        if (_mime_index != std::string::npos)
        {
            _mime_string = index.substr(_mime_index + 1);
        }
        std::string file = server[_server_index]._location[_location_index].get_root() + "/" + index;
        _rooted_path = server[_location_index].get_root() + _removed_path + index;
        if (ft_handle_index_2(file))
            return (1);
    }
    return (0);
}

int Respond::ft_handle_index_2(std::string index)
{
    std::ifstream file;
    if (index != "")
    {
        file.open(index.c_str());
        if (file.is_open())
        {
            _response_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            set_status_code(200);
            set_status_message(get_response_status(200));
            set_header("Content-Type", get_mime_type(_mime_string));
           // std::cout << "mime _type in index: " << get_mime_type(_mime_string) << std::endl;
            _headers["Content-Length"] = std::to_string(_response_body.length());
            _headers["Connection"] = "keep-alive";
            set_date();
            set_cache_control("no cache");
            return (0);
        }
        else
        {
    //std::cout << "____WWWWW_--------______" << std::endl;
            handle_error_response(404);
            return (1);
        }
    }
    else
    {
        handle_error_response(404);
        return (1);
    }
    return (0);
}

int     Respond::ft_handle_autoindex(std::vector<server> server)
{
    for (size_t i = 0; i < server.size(); i++)
    {
        for (size_t j = 0; j < server[i]._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                if (!server[i]._location[j].get_autoindex())
                {
                    if (!server[i].get_autoindex())
                        return (1);
                    else
                    {
                        ft_show_autoindex();
                        return (0);
                    }
                }
                else
                {
                    ft_show_autoindex();
                    return (0);
                }
            }
        }
    }
    return (1);
}

void    Respond::ft_handle_error(int error_code)
{
    _status_code = error_code;
    _response_body = "<html><head><title " + std::to_string(error_code) + " Forbidden</title></head><body><h1>Forbidden</h1><p>You don't have permission to access " + r.get_uri() + " on this server.</p></body></html>";
    _status_message = get_response_status(_status_code);
    _headers["Content-Type"] = "text/html";
    _headers["Content-Length"] = _response_body.length();
    _headers["Connection"] = "keep-alive";
    set_date();
    set_cache_control("no cache");
}

void    Respond::ft_show_autoindex()
{
    std::string index_html = "<!DOCTYPE html>\n<html>\n<head>\n";
    index_html += "<meta charset=\"UTF-8\">\n";
    index_html += "<title>Index of " + _path_found + "</title>\n";
    index_html += "</head>\n<body style=\"text-align:center;\">\n";
    index_html += "<h1>Index of " + _path_found + "</h1>\n";
    index_html += "<table align=\"center\" style=\"border-collapse: collapse;\">\n";
    index_html += "<thead><tr><th>Name</th><th>Size</th></tr></thead>\n";
    index_html += "<tbody>\n";

    DIR *dir = opendir(_path_found.c_str());
    
    if (dir == NULL)
    {
        handle_error_response(403);
        return ;
    }
    
    struct dirent *entry;
    struct stat file_stat;
    std::string file_name;
    std::string file_size;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            file_name = std::string(entry->d_name);
            std::string file_path = _path_found + "/" + file_name;
            
            if (stat(file_path.c_str(), &file_stat) < 0)
            {
                handle_error_response(403);
                continue ;
            }
            
            file_size = std::to_string(file_stat.st_size);
            index_html += "<tr>";
            index_html += "<td><a href=\"" + file_name + "\">" + file_name + "</a></td>";
            index_html += "<td>" + file_size + "</td>";
            index_html += "</tr>\n";
        }
    }
    closedir(dir);
    index_html += "</tbody>\n</table>\n</body>\n</html>\n";
    _response_body = index_html;
}

void    Respond::handle_error_response(int error_code)
{
    set_status_code(error_code);
    set_status_message(get_response_status(error_code));
    set_header("Content-Type", "text/html");
    set_header("Connection", "keep-alive");
    set_date();
    set_last_modified();
    _response_body = "<html><head><title>" + std::to_string(error_code) + " " + _status_message + "</title></head><body><h1>" + std::to_string(error_code) + " " + _status_message + "</h1><p>You don't have permission to access " + r.get_uri() + " on this server.</p></body></html>";
    set_header("Content-Length", std::to_string(_response_body.length()));

    print_response();
}

void    Respond::print_response()
{
    // std::cout << "HTTP/1.1 " << get_status_code() << " " << get_status_message() << "\r\n";
    // for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
    //     std::cout << it->first << ": " << it->second << "\r\n";
    std::cout << "\r\n";
//    if (get_response_body() != "")
//         std::cout << "response body: " << _response_body << std::endl;;
}