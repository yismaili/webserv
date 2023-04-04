/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:16:17 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/04 20:54:12 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iterator>
#include <string>
#include <iostream>
#include <sstream> 
#include <vector>
#include <curl/curl.h>
#include <cstring>

 std::string build_requist(){
    //Insert html page or ...
    std::ostringstream requist; //create the output string stream
    requist << "HTTP/1.1 200 OK";
    requist << "Content-Type: text/plain";
    requist << "Transfer-Encoding: chunked";
    requist << "7\r\n";
    requist << "Mozilla\r\n";
    requist << "9\r\n";
    requist << "Developer\r\n";
    requist << "7\r\n";
    requist << "Network\r\n";
    requist << "0\r\n";
    requist << "\r\n";
    std::string str_t(requist.str());
    return (str_t);
}

// std::string unchunk(const std::string &chunked) {
//     std::stringstream ss(chunked);
//     std::string line;
//     std::string result;
    
//     while (std::getline(ss, line)){
//          char chunk[1024];           
//          ss.read(chunk, 8);
//          ss.ignore(2); // skip CRLF
//          result += chunk;
//      }   
//     return (result);
// }
#include <string>
#include <sstream>
// template <typename T>
bool fromHex(const std::string& hexValue, int & result)
{
    std::stringstream ss;
    ss << std::hex << hexValue;
    ss >> result;

    return !ss.fail();
}

std::string unchunk(const std::string& chunked) {
  std::stringstream ss(chunked);
  std::string result;
  while (ss.good()) {
    std::string size_str;
    std::getline(ss, size_str, '\r');
    ss.ignore();  // skip '\n'
    if (size_str.empty()) {
      break;
    }
   // exit(1);
    int size ;
    size = 7;
    std::string chunk(size, ' ');
    ss.read(&chunk[0], size);
    result += chunk;
    ss.ignore(2);  // skip '\r\n'
  }
  return result;
}

int main() {
    std::string response_body = unchunk(build_requist());
    std::cout << response_body << std::endl;
    return 0;
}

// HTTP/1.1 200 OK Content-Type: text/plainTransfer-Encoding: chunked7\r\nMozilla\r\n9\r\nDeveloper\r\n7\r\nNetwork\r\n0\r\n\r\n

// using namespace std;

// // Callback function that receives the response body
// size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
//     string *response = (string *) userdata;
//     response->append(ptr, size * nmemb);
//     return size * nmemb;
// }

// int main() {
//         // Split the response into chunks
//         stringstream ss(build_requist());
//         vector<char> chunks;
//         std::string str = ss.str();
//         const char* cstr = str.c_str();
//         //std::cout<<cstr<<std::endl;
//         int i = 0;
//         while(cstr[i]){
//             //chunks.push_back(cstr[i]);
//             // if (cstr[i + 2] == '\r' && cstr[i + 3] == '\n'){
//             //     break;
//             // }
//             if (isnumber(cstr[i]) && cstr[i + 1] == '\r'){
//                 std::cout<<cstr[i]<<std::endl;  
//             }
//             i++;
//         }
//             std::cout<<cstr +(i + 1) <<std::endl;
//     return 0;
// }

