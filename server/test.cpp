// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   test.cpp                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/03/25 17:16:17 by yismaili          #+#    #+#             */
// /*   Updated: 2023/04/14 15:53:50 by yismaili         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <sys/poll.h>
// // #include <sys/socket.h>
// // #include <netinet/in.h>
// // #include <unistd.h> // for read() and close()
// // #define MAX_CLIENTS 10
// // #define BUFFER_SIZE 1024

// // int main() {
// //     int server_fd, new_socket, i, poll_ret;
// //     struct sockaddr_in address;
// //     int addrlen = sizeof(address);
// //     char buffer[BUFFER_SIZE] = {0};
// //     struct pollfd clients[MAX_CLIENTS];

// //     // Create server socket
// //     server_fd = socket(AF_INET, SOCK_STREAM, 0);

// //     // Bind socket to a port
// //     address.sin_family = AF_INET;
// //     address.sin_addr.s_addr = INADDR_ANY;
// //     address.sin_port = htons(8080);
// //     bind(server_fd, (struct sockaddr *)&address, sizeof(address));

// //     // Listen for incoming connections
// //     listen(server_fd, 3);

// //     // Add server socket to poll list
// //     clients[0].fd = server_fd;
// //     clients[0].events = POLLIN;

// //     // Initialize remaining poll list
// //     for (i = 1; i < MAX_CLIENTS; i++) {
// //         clients[i].fd = -1;
// //     }

// //     while (1) {
// //         // Wait for events on any of the monitored file descriptors
// //         poll_ret = poll(clients, MAX_CLIENTS, -1);
		
// //         // Check for events on server socket
// //         if (clients[0].revents & POLLIN) {
// //             // Accept incoming connection
// //             new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

// //             // Add new socket to poll list
// //             for (i = 1; i < MAX_CLIENTS; i++) {
// //                 if (clients[i].fd == -1) {
// //                     clients[i].fd = new_socket;
// //                     clients[i].events = POLLIN;
// //                     break;
// //                 }
// //             }
// //         }

// //         // Check for events on client sockets
// //         for (i = 1; i < MAX_CLIENTS; i++) {
// //             if (clients[i].fd != -1 && clients[i].revents & POLLIN) {
// //                 // Read data from client
// //                 if (read(clients[i].fd, buffer, BUFFER_SIZE) == 0) {
// //                     // Connection closed by client
// //                     close(clients[i].fd);
// //                     clients[i].fd = -1;
// //                 } else {
// //                     // Process incoming data
// //                     printf("Received data from client %d: %s\n", clients[i].fd, buffer);
// //                     // Send response back to client
// //                     send(clients[i].fd, "Server received your message\n", 30, 0);
// //                 }
// //             }
// //         }
// //     }
// //     return 0;
// // }


// #include <iostream>
// #include <vector>
// #include <poll.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <cstring>

// #define BUFFER_SIZE 1024
// #define MAX_CLIENTS 10

// int main() {
//     int server_fd, new_socket, i, poll_ret;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
//     char buffer[BUFFER_SIZE] = {0};
//     std::vector<pollfd> clients;

//     // Create server socket
//     server_fd = socket(AF_INET, SOCK_STREAM, 0);

//     // Bind socket to a port
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(8080);
//     bind(server_fd, (struct sockaddr *)&address, sizeof(address));

//     // Listen for incoming connections
//     listen(server_fd, 3);

//     // Add server socket to poll list
//     pollfd server_pollfd;
//     server_pollfd.fd = server_fd;
//     server_pollfd.events = POLLIN;
//     clients.push_back(server_pollfd);

//     while (1) {
//         // Wait for events on any of the monitored file descriptors
//         poll_ret = poll(clients.data(), clients.size(), -1);

//         // Check for events on server socket
//         if (clients[0].revents & POLLIN) {
//             // Accept incoming connection
//             new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

//             // Add new socket to poll list
//             pollfd new_client_pollfd;
//             new_client_pollfd.fd = new_socket;
//             new_client_pollfd.events = POLLIN;
//             clients.push_back(new_client_pollfd);
//         }

//         // Check for events on client sockets
//         for (std::vector<pollfd>::iterator it = clients.begin() + 1; it != clients.end(); ++it) {
//             if (it->fd != -1 && it->revents & POLLIN) {
//                 // Read data from client
//                 if (read(it->fd, buffer, BUFFER_SIZE) == 0) {
//                     // Connection closed by client
//                     close(it->fd);
//                     it->fd = -1;
//                 } else {
//                     // Process incoming data
//                     printf("Received data from client %d: %s\n", it->fd, buffer);
//                     // Send response back to client
//                     send(it->fd, "Server received your message\n", 30, 0);
//                 }
//             }
//         }

//         // Remove closed client sockets
//        //clients.erase(std::remove_if(clients.begin(), clients.end(), [](const pollfd& pfd) { return pfd.fd == -1; }), clients.end());
//     }

//     return 0;
// }
// #include <iostream>
// #include <string>

// std::string join_chunked(const std::string& chunked_msg) {
//     std::string result = "";
//     std::size_t pos = 0;

//     // Find the end of the headers
//     std::size_t header_end = chunked_msg.find("\r\n\r\n");
//     if (header_end == std::string::npos) {
//         // No headers found, return an empty string
//         return "";
//     }
//     // Append the headers to the result
//     result += chunked_msg.substr(0, header_end);


//     // Find the start of the first chunk
//     pos = header_end + 4;

//     while (true) {
//         // Find the next chunk size
//         std::size_t len_pos = chunked_msg.find("\r\n", pos);
//         std::string len_str = chunked_msg.substr(pos, len_pos - pos);
//         int len = std::stoi(len_str, nullptr, 16);

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
//     std::string chunked_msg = "POST / HTTP/1.0\r\nContent-Type: application/x-www-form-urlencoded\r\nTransfer-Encoding: chunked\r\n\r\n4\r\n\nfoo=\r\n3\r\nbar\r\n0\r\n\r\n";
//     std::string joined_msg = join_chunked(chunked_msg);
//     std::cout << "Chunked message: " << chunked_msg << std::endl;
//     std::cout << "Joined message: " << joined_msg << std::endl;

//     return 0;
// }

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main() {
    // Simulated chunked response message
    string response = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/plain\r\n"
                      "Transfer-Encoding: chunked\r\n"
                      "\r\n"
                      "7\r\n"
                      "Mozilla\r\n"
                      "11\r\n"
                      "Developer Network\r\n"
                      "0\r\n"
                      "\r\n";

    // Find the start of the message body
    size_t pos = response.find("\r\n\r\n");
    std::string head_str = response.substr(0, pos);
    if (pos == string::npos) {
        cerr << "Error: message body not found\n";
        return 1;
    }
    pos += 4;

    // Parse the chunks and join them together
    stringstream ss(response.substr(pos));
    string chunk, body;
    size_t size;
    while (getline(ss, chunk)) {
        // Extract the chunk size
        size = stoul(chunk, nullptr, 16);
        if (size == 0) {
            // End of message body
            break;
        }

        // Extract the chunk data
        chunk.resize(size);
        ss.read(&chunk[0], size);
        ss.ignore(2); // Ignore the "\r\n" sequence after the chunk

        // Append the chunk to the message body
        body += chunk;
    }

    // Print the complete message body
    cout << head_str <<"\nMessage body: " << body << endl;

    return 0;
}






/*

HTTP/1.1 200 OK\r\n
Content-Type: text/plain\r\n
Transfer-Encoding: chunked\r\n
\r\n
7\r\n
Mozilla\r\n
11\r\n"
Developer Network\r\n
0\r\n
\r\n

*/