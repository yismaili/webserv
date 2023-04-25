#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
    int server_socket, client_socket, port_number, client_address_len;
    struct sockaddr_in server_address, client_address;
    char buffer[1024];
    char response[1024];
    string file_contents;

    // Read the HTML file into a string
    ifstream file("index.html");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            file_contents += line;
        }
        file.close();
    } else {
        cout << "Error: Could not open file" << endl;
        return 1;
    }

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cout << "Error: Could not create server socket" << endl;
        return 1;
    }
    // reuse port 
    int optval = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    // Bind the server socket to a port
    port_number = 8080;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        cout << "Error: Could not bind server socket" << endl;
        return 1;
    }

    // Listen for incoming connections
    listen(server_socket, 5);
    cout << "Server running on port " << port_number << endl;

    while (true) {
        // Accept a client connection
        client_address_len = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&client_address_len);
        if (client_socket < 0) {
            cout << "Error: Could not accept client connection" << endl;
            continue;
        }

        // Read the client request
        bzero(buffer, 1024);
        read(client_socket, buffer, 1024);
        cout << "Received request:\n";
        cout << "--------------------" << endl;
        cout << buffer << endl;
        cout << "--------------------" << endl;

        // Construct the HTTP response
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n%s", file_contents.length(), file_contents.c_str());

        // Send the response to the client
        write(client_socket, response, strlen(response));
        cout << "Sent response:\n";
        cout << "--------------------" << endl;
        cout << response << endl;
        cout << "--------------------" << endl;

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}