/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_handling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:52:50 by aoumad            #+#    #+#             */
/*   Updated: 2023/05/19 12:36:33 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "respond.hpp"

void    Respond::handle_get_response(std::vector<server> servers)
{
    // step 2: check if it's a CGI or not (like if `index` of the configuration file has .py or .php...etc)
    if (_is_cgi == true)
    {
        run_cgi(r, *this);
        return ;
    }
    // std::cout << "___---222222__________@@@@@-______-" << std::endl;
    // step 3: check if it's a file or not
    if (ft_check_file() == true)
    {
        ft_handle_file();
        return ;
    }
    // else
    // {
    //     std::cout << "___--_------__------_-_-_--_-__-_-_-_-HEREEEEE_--_-_-_--_-_-_-_-" << std::endl;
    //     handle_error_response(404);
    //     return ;
    // }
    // step 4 : check the index in the configuration file and render it
    int rtn_index = ft_handle_index(servers);
    if (rtn_index == 0)
        return ;
    else if (rtn_index == 1)
    {
        if (ft_handle_autoindex(servers))
        {
            handle_error_response(403);
            return ;
        }
    }
    else if (rtn_index == 2)
    {
        handle_error_response(404);
        return ;
    }
    // step 5: check if the autoindex if on or off

}

void    Respond::handle_post_response(std::vector<server> server)
{
    // step 1: check if the request body is empty or not
    if (r.get_body().empty())
    {
        set_response_body("Body request is missing");
        return ;
    }
    if (_is_cgi == false && (server[_server_index]._location[_location_index].get_upload_store().empty() && server[_server_index]._location[_location_index].get_upload() == false))
        return ;
    _upload_store_path = _rooted_path;
    _upload_store.append(_upload_store);
    struct stat st;
    if (stat(_upload_store_path.c_str(), &st) != 0)
    {
        mkdir(_upload_store_path.c_str(), 0777);
        // or return error
    }
    if (check_post_type() == "x-www-form-urlencoded")
    {
        if (_is_cgi == true)
        {
            // khasni n7t query homa request body
            run_cgi(r, *this);
        }
        else
        {
            // need to create a file that has `Key` as it's name and the content of it as `value`
            handle_urlencoded();
            create_decode_files();
        }
    }
    if (check_post_type() == "form-data")
    {
        handle_form_data();
    }

}

void    Respond::handle_urlencoded()
{
    std::string line = r.get_body();
    Url_encoded encoded_form;
    std::string::size_type pos = 0;
    std::string::size_type end_pos = 0;
    size_t index = 0;
    while ((index = line.find("%20", index)) != std::string::npos)
    {
        std::cout << index << std::endl;
        line = line.substr(0, index) + " " + line.substr(index + 3);
        index += 1; // Move past the inserted space
    }
    while (pos != std::string::npos)
    {
        end_pos = line.find('&', pos);
        std::string pair = line.substr(pos, end_pos - pos);
        std::string::size_type pivot = pair.find("=");
        if (pivot != std::string::npos)
        {
            encoded_form.key = pair.substr(0, pivot);
            encoded_form.value = pair.substr(pivot + 1);
            _url_decode.push_back(encoded_form);
        }

        if (end_pos == std::string::npos)
            break;
        pos = end_pos + 1;
    }
}

void    Respond::create_decode_files()
{
    std::string path = _upload_store_path;
    std::string file_name;
    std::string file_content;
    std::ofstream file;
    std::vector<Url_encoded>::iterator it = _url_decode.begin();
    
    while (it != _url_decode.end())
    {
        file_name = path;
        file_name.append(it->key);
        file.open(file_name.c_str());
        file << it->value;
        file.close();
        it++;
    }
}

void    Respond::handle_form_data()
{
    // std::cout << r.get_body() << std::endl;
    // Find the first boundary
    size_t  pos = r.get_body().find(_boundary);
    if (!pos)
        return ;
    // Loop through the form data, locating boundaries and reading data betweem them
    while (true)
    {
        // find the next boundary
        pos = r.get_body().find(_boundary, pos);
        if (pos == std::string::npos)
            break;

        // Read the data between the boundaries
        FormData formData = read_form_data(pos); // escape /r/n
        if (formData.isValid())
            _form_data.push_back(formData); // Add the form data to the list
        // std::cout << "pos before: " << pos << std::endl;
        if (_last_boundary == true)
            break;
        pos += _boundary.length() + 2;
    }
    // iterat over formData class and print it's attributes
    // std::vector<FormData>::iterator it = _form_data.begin();
    // while (it != _form_data.end())
    // {
    //     std::cout << "name: " << it->get_name() << std::endl;
    //     std::cout << "filename: " << it->get_file_name() << std::endl;
    //     std::cout << "content-type: " << it->get_content_type() << std::endl;
    //     std::cout << "data: " << it->get_data() << " END"<< std::endl;
    //     it++;
    //}
    // }
    
    create_form_data();
}

void    Respond::create_form_data()
{
    std::string path = _upload_store_path;
    std::string file_name;
    std::string file_content;
    std::ofstream file;
    std::vector<FormData>::iterator it = _form_data.begin();

    while (it != _form_data.end())
    {
        if (it->get_content_type().empty())
        {
            it++;
            continue;
        }
        file_name = path;
        file_name.append(it->get_file_name());
        file.open(file_name.c_str());
        file << it->get_data();
        file.close();
        it++;
    }
}
// Helper function to locate the next boundary in the form data
size_t Respond::find_boundary(size_t pos)
{
    return (r.get_body().find(_boundary, pos));
}

FormData Respond::read_form_data(size_t pos)
{
    FormData form_data;
    std::string line;
    std::string data;

    size_t start = r.get_body().find(_boundary, pos);
    if (start == std::string::npos)
        return form_data; // boundary not found

    // skip the boundary line
    start += _boundary.length() + 2;
    std::string last_boundary = _boundary + "--";
    size_t end = r.get_body().find(_boundary, start);
    if (end == std::string::npos)
        return form_data; // Boundary not found

    size_t end_last = r.get_body().find(last_boundary, start);
    if (end_last != std::string::npos && end_last == end)
    {
        _last_boundary = true;
    }

    // Extracts the form data section
    std::string section = r.get_body().substr(start, end - start);
    // Process the headers
    size_t header_end = section.find("\r\n\r\n");
    if (header_end != std::string::npos)
    {
        std::string header_section = section.substr(0, header_end);
        // Find the form name in the headers
        size_t name_start = header_section.find("name=\"") + sizeof("name=\"") - 1;
        size_t name_end = header_section.find("\"", name_start);
        if (name_end != std::string::npos)
            form_data.name = header_section.substr(name_start, name_end - name_start);
        
        // Find the filename in the headers
        size_t filename_start = header_section.find("filename=\"") + sizeof("filename=\"") - 1;
        size_t filename_end = header_section.find("\"", filename_start);
        if (filename_end != std::string::npos)
            form_data.file_name = header_section.substr(filename_start, filename_end - filename_start);
        
        size_t content_type_start = header_section.find("Content-Type: ") + sizeof("Content-Type: ") - 1;
        form_data.content_type = header_section.substr(content_type_start);
    }

    // Process the data content
    std::string data_content = section.substr(header_end + 4); // Skip the CRLF delimiter
    form_data.data = data_content;

    return (form_data);
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

std::string Respond::check_post_type()
{
    if(r.get_header("Content-Type").find("multipart/form-data") != std::string::npos)
        return ("form-data");
    else if(r.get_header("Content-Type").find("application/x-www-form-urlencoded") != std::string::npos)
        return ("x-www-form-urlencoded");
    else
        return ("regular");
}

void    Respond::handle_delete_response()
{
        // std::cout << "DKHLAAAAAAAAAAT" << std::endl;
        // std::cout << "rooted path:" << _rooted_path << std::endl;
    if (std::remove(_rooted_path.c_str()) == 0)
    {
        _status_code = 200;
        _status_message = get_response_status(_status_code);
        set_header("Content-Type", r.get_header("Content-Type"));
        set_header("Content-Length", std::to_string(_response_body.length()));
        set_header("Connection", "keep-alive");
    }
    else
    {
        _status_code = 500;
        _status_message = get_response_status(_status_code);
        // set_response_body("Error deleting file");
        set_header("Content-Type", r.get_header("Content-Type"));
        set_header("Content-Length", std::to_string(_response_body.length()));
        set_header("Connection", "keep-alive");
    }
}