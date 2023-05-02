/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_handling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:52:50 by aoumad            #+#    #+#             */
/*   Updated: 2023/04/30 18:26:33 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "respond.hpp"

void    Respond::handle_get_response()
{
    
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

}

void    Respond::handle_post_response()
{
    if (check_post_type() == "application/x-www-form-urlencoded" && _is_cgi == true)
    {
        if (ft_check_cgi())
            ft_handle_cgi();
        else
        {
            if (stat())
            // need to create a file that has `Key` as it's name and the content of it as `value`
            std::istream    file;
            
        }
    }
    if (check_post_type() == "multipart/form-data")
    {
        handle_form_data();
    }

}

void    Respond::handle_form_data()
{
    struct stat st;
    if (get_upload_store() == false || server.get_upload() == "off")
        return ;
    _rooted_path.append(_upload_store);
    // if (stat(_rooted_path.c_str(), &st != 0))
    // {

    // }

    // Find the first boundary
    size_t  pos = r.get_body().find(_boundary);
    if (pos == std::string::npos)
        return ;

    // Loop through the form data, locating boundaries and reading data betweem them
    while (true)
    {
        // find the next boundary
        size_t  next = r.get_body().find(_boundary, pos + _boundary.length());
        if (next == std::string::npos)
            break;
        
        // Read the data between the boundaries
        FormData formData = read_form_data(next);
        if (formData.isValid())
            _form_data.push_back(formData); // Add the form data to the list
    }
}

// Helper function to locate the next boundary in the form data
size_t Response::find_boundary(size_t pos)
{
    return (r.get_body().find(_boundary, pos));
}

// Helper function to read the form data between two boundaries
Response::FormData Response::read_form_data(size_t pos)
{
    FormData form_data;
    std::string line;
    std::string data;

    // Find the starting position of the form data
    size_t start = r.get_body().find(_boundary, pos);
    if (start == std::string::npos)
        return (form_data); // Boundary not found
    
    // Skip the boundary line
    start += _boundary.length() + 2;

    // Read the form data until the next boundary
    size_t end = r.get_body().find(_boundary, start);
    if (end == std::string::npos)
        return (form_data); // Boundary not found
    
    // Extract the form data section
    std::string section = r.get_body().substr(start, end - start);
    
    // Process the section line by line
    std::stringstream ss(section);
    while (std::getline(ss, line))
    {
        if (line.empty())
            continue;
        if (line.find("Content-Disposition: form-data;") != std::string::npos)
        {
            // Extracts the form name and filename from the line
            size_t pos = line.find("name=\"") + sizeof("name=\"") - 1; // similar to line.find() + 6
            size_t end = line.find("\"", pos);
            if (end != std::string::npos)
                formData.form_name = line.substr("pos, end - pos");

            pos = line.find("filename=\"") + sizeof("filename=\"") - 1;
            end = line.find("\"", pos);
            if (end != std::string::npos)
                formData.file_name = line.substr(pos, end - pos);
            
        }
        else if (line.find("Content-Type: ") != std::string::npos)
        {
            // Extracts the content type from the line
            size_t pos = line.find("Content-Type: ") + sizeof("Content-Type: ") - 1;
            formData.content_type = line.substr(pos);
        }
        else
        {
            // It's the form data value
            FormData.data += line + "\n";
        }
    }
    
    return (FormData);
}

/*
POST /example HTTP/1.1
Host: example.com
Content-Type: multipart/form-data; boundary=--------------------------1234567890

----------------------------1234567890
Content-Disposition: form-data; name="username"

johndoe
----------------------------1234567890
Content-Disposition: form-data; name="profile_pic"; filename="profile.jpg"
Content-Type: image/jpeg

_root path + path_found + upload_store
----------------------------1234567890
Content-Disposition: form-data; name="notes"; filename="notes.txt"
Content-Type: text/plain

(Here goes the text content of the notes file)
----------------------------1234567890--
*/

int Respond::get_upload_store()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server[i]._location.size(); j++)
        {
            if (server[i]._location[j].location_name == _path_found)
            {
                if (server[i]._location[j].upload_store.empty())
                    return (0);
                else
                {
                    _upload_store = server[i]._location[j].upload_store;
                    return (1);
                }
            }
        }
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
            // set_response_body("File deleted successfully");
            set_header("Content-Type", content_type);
            set_header("Content-Length", std::to_string(_response_body.length()));
            set_header("Connection", "keep-alive");
        }
        else
        {
            _status_code = "500";
            _status_message = get_response_status(_status_code);
            // set_response_body("Error deleting file");
            set_header("Content-Type", content_type);
            set_header("Content-Length", std::to_string(_response_body.length()));
            set_header("Connection", "keep-alive");
        }
    }
    else
    {
        _status_code = "404";
        _status_message = "Not Found";
        // set_response_body("File not found");
        set_header("Content-Type", content_type);
        set_header("Content-Length", std::to_string(_response_body.length()));
        set_header("Connection", "keep-alive");
    }
}