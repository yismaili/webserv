#include <iostream>
#include <string>

void send_token(std::string output) {
    std::string token;
    std::size_t pos = output.find("Set-Cookie:");
    if (pos != std::string::npos) {
        pos += 12;  // length of "Set-Cookie: "
        std::size_t end = output.find(";", pos);
        if (end != std::string::npos) {
            token = output.substr(pos, end - pos);
        } else {
            token = output.substr(pos);
        }
    }
    std::cout << "Content-Type: text/html\r\n";
    if (!token.empty()) {
        std::cout << "Set-Cookie: token=" << token << "\r\n";
    }
    std::cout << "\r\n";
    std::cout << "Response body goes here...\n";
}

#include <string>

void extract_path(std::string location_name) {
    // Find the position of the path name after the "location" keyword
    size_t pos = std::string("location").length();
	for (int i = pos; i < location_name.size(); i++)
	{
		if (!isspace(location_name[i]))
			break;
		pos++;			
	}
	int start = pos;
	int i;
	for (i = pos; i < location_name.size(); i++)
	{
		if (isspace(location_name[i]) || location_name[i] == '{')
			break;
	}
	int end = i;
	//std::cout << location_name[end] << " " << end <<  "\n";
	std::string path;
	std::cout << location_name << "\n";
	std::cout << start << " " << end << "\n";
	path = location_name.substr(start, end );
	std::cout << path << "\n";
    // // Extract the substring from the path name up to the closing curly brace character
    // std::string path = location_name.substr(pos);
    // size_t end_pos = path.find('}');
    // if (end_pos != std::string::npos) {
    //     path = path.substr(0, end_pos);
    // }
    
    // // Remove leading and trailing spaces from the path name
    // size_t start_pos = path.find_first_not_of(" \t");
    // if (start_pos != std::string::npos) {
    //     path = path.substr(start_pos);
    // }
    // size_t trailing_space_pos = path.find_last_not_of(" \t");
    // if (trailing_space_pos != std::string::npos) {
    //     path = path.substr(0, trailing_space_pos + 1);
    // }
    
    //return path;
}


int main() {
    std::string location_name = "location /cgi_bin  . {";
	 extract_path(location_name); // Returns "/cgi_bin"
	//std::cout << path <<std::endl;
    return 0;
}

/*void    response::parseCgiOutput(std::string output){
	std::istringstream  tmp(output);
	std::string         line;
	std::string 		key;

	while (std::getline(tmp, line)){
		line += "\n";

		size_t pos = line.find(":");
		if (pos != std::string::npos){
			key = line.substr(0, pos);

			if (key == "Set-Cookie")
				this->_cookies.push_back(line.substr(pos + 1, line.find("\r\n") - pos - 1));
			else
				this->setHeader(key, line.substr(pos + 1, line.find("\r\n") - pos - 1));
		}
		else if (line.find("\r\n") != std::string::npos){
			break;
		}
	}
	getline(tmp, this->_body, '\0');
	if (!this->_body.empty())
		this->setStatus(200);
	else
		this->setStatus(502);
}*/