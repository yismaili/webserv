/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:16:17 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/11 00:09:54 by yismaili         ###   ########.fr       */
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

//  std::string build_requist(){
//     //Insert html page or ...
//     std::ostringstream requist; //create the output string stream
//     requist <<" POST / HTTP/1.1";
//     requist <<"Transfer-Encoding: Chunked";
//     requist <<"User-Agent: PostmanRuntime/7.25.0";
//     requist <<"Accept: */*";
//     requist <<"Postman-Token: aedda9fd-74ba-49a9-9bf2-b67fdf86c914";
//     requist <<"Host: localhost:8080";
//     requist <<"Accept-Encoding: gzip, deflate, br";
//     requist <<"Connection: keep-alive";
//     requist <<"Content-Type: application/x-www-form-urlencoded";
//     requist <<"Content-Length: 181;";
//     requist << "7\r\n";
//     requist << "Mozilla\r\n";
//     requist << "9\r\n";
//     requist << "Developer\r\n";
//     requist << "7\r\n";
//     requist << "Network\r\n";
//     requist << "0\r\n";
//     requist << "\r\n";
//     std::string str_t(requist.str());
//     return (str_t);
// }

// std::string unchunk(const std::string &data) {
//     std::string res;
//     std::size_t found  = data.find("\r\n\r\n");
//     std::size_t lenOf_chunk = std::stoi(data.substr(data.find("Content-Length: ") + 16, 10).c_str());
//         exit(1);
//    // if (data.size() < lenOf_chunk + found){
//         std::string head = data.substr(0, data.find("\r\n") - 1);
//         std::string chunks = data.substr(data.find("r\n\r\n")+4, data.size() -1);
//         std::string subchunk = chunks.substr(0,10);
//     // std::cout <<"----"<<head<< std::endl;
//         std::string body = "";
//         int chunk_size = strtol(subchunk.c_str(), NULL, 16);
//         std::size_t i = 0;
//         while (chunk_size)
//         {
//            i = chunks.find("\r\n", i) + 2;
//            body += chunks.substr(i, chunk_size);
//            i += chunk_size + 2;
//            subchunk = chunks.substr(i, 10);
//            chunk_size = strtol(subchunk.c_str(), NULL, 16);
//         }
//        res = head + "\r\n\r\n" + body + "\r\n\r\n";
//    // }
//     return(res);
// }


// int main() {
//     std::string response_body = unchunk(build_requist());
//     std::cout << response_body << std::endl;
//     return 0;
// }

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


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <sys/ioctl.h>

// int main() {
//     int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//     if (sock < 0) {
//         perror("socket");
//         return -1;
//     }

//     struct sockaddr_in addr;
//     memset(&addr, 0, sizeof(addr));
//     addr.sin_family = AF_INET;
//     addr.sin_addr.s_addr = htonl(INADDR_ANY);
//     addr.sin_port = htons(1234);

//     if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
//         perror("bind");
//         close(sock);
//         return -1;
//     }

//     int bufsize = 4096;
//     if (ioctl(sock, FIONBIO, &bufsize) < 0) {
//         perror("ioctl");
//         close(sock);
//         return -1;
//     }

//     char buf[bufsize];
//     struct sockaddr_in client_addr;
//     socklen_t addrlen = sizeof(client_addr);
//     int nbytes = recvfrom(sock, buf, sizeof(buf), 0,
//                           (struct sockaddr *)&client_addr, &addrlen);
//     if (nbytes < 0) {
//         perror("recvfrom");
//         close(sock);
//         return -1;
//     }

//     printf("Received %d bytes from %s:%d\n", nbytes,
//            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

//     close(sock);
//     return 0;
// }


// #include <iostream>
// #include <string>

// std::string join_chunked(const std::string& chunked_msg) {
//     std::string result = "";
//     std::size_t pos = 0;

//     while (true) {
//         // Find the next chunk size
//         std::size_t len_pos = chunked_msg.find("\r\n", pos);
//         std::string len_str = chunked_msg.substr(pos, len_pos - pos);
//         int len = strtol(len_str.c_str(), nullptr, 16);


//         // If the length is 0, we're done
//         if (len == 0) {
//             break;
//         }

//         // Append the chunk data to the result
//         result += chunked_msg.substr(len_pos + 2, len);
//         pos = len_pos + 2 + len + 2;
//     }

//     return result;
// }

// int main() {
//     // Example usage
//     std::string chunked_msg = "POST / HTTP/1.0\r\nContent-Type: application/x-www-form-urlencoded\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nfoo=\r\n3\r\nbar\r\n0\r\n\r\n";
//     std::string	head =  chunked_msg.substr(0,  chunked_msg.find("\r\n") - 1);
//     std::string joined_msg = join_chunked( chunked_msg);
//     //std::cout << "Chunked message: " << chunked_msg << std::endl;
//     std::cout << "Joined message: " << head << std::endl;
//     return 0;
// }

#include <iostream>
#include <string>

std::string join_chunked(const std::string& chunked_msg) {
    std::string result = "";
    std::size_t pos = 0;

    // Find the end of the headers
    std::size_t header_end = chunked_msg.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        // No headers found, return an empty string
        return "";
    }

    // Append the headers to the result
    result += chunked_msg.substr(0, header_end);

    // Find the start of the first chunk
    pos = header_end + 4;

    while (true) {
        // Find the next chunk size
        std::size_t len_pos = chunked_msg.find("\r\n", pos);
        std::string len_str = chunked_msg.substr(pos, len_pos - pos);
        int len = std::stoi(len_str, nullptr, 16);

        // If the length is 0, we're done
        if (len == 0) {
            break;
        }

        // Append the chunk data to the result
        result += chunked_msg.substr(len_pos + 2, len);
        pos = len_pos + 2 + len + 2;
    }

    return result;
}

int main() {
    // Example usage
    std::string chunked_msg = "POST / HTTP/1.0\r\nContent-Type: application/x-www-form-urlencoded\r\nTransfer-Encoding: chunked\r\n\r\n4\r\n\nfoo=\r\n3\r\nbar\r\n0\r\n\r\n";
    std::string joined_msg = join_chunked(chunked_msg);
   // std::cout << "Chunked message: " << chunked_msg << std::endl;
    std::cout << "Joined message: " << joined_msg << std::endl;
    return 0;
}
