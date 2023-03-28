#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 256

using namespace std;

int main(int argc, char *argv[]) {

    int server_socket, client_socket, port_number, client_count = 0;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " port_number" << endl;
        exit(EXIT_FAILURE);
    }

    port_number = atoi(argv[1]);

    // Create a socket for the server
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cerr << "Error: could not create socket" << endl;
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a specific port
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        cerr << "Error: could not bind socket to port " << port_number << endl;
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_socket, MAX_CLIENTS);

    // Handle incoming connections
    while (true) {

        // Accept a new client connection
        client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_length);
        if (client_socket < 0) {
            cerr << "Error: could not accept client connection" << endl;
            continue;
        }

        client_count++;

        // Print information about the new client
        cout << "New client connected from " << inet_ntoa(client_address.sin_addr) << ", port " << ntohs(client_address.sin_port) << endl;

        // Receive data from the client and send a response
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            cerr << "Error: could not receive data from client" << endl;
        }
        else {
            buffer[bytes_received] = '\0';
            cout << "Received data from client " << client_count << ": ";
            cout << endl;
            cout << "------------------------------------" << endl;
            cout <<  buffer << endl;
            cout << "------------------------------------" << endl;
            const char* response = "Message received!";
            int bytes_sent = send(client_socket, response, strlen(response), 0);
            if (bytes_sent < 0) {
                cerr << "Error: could not send response to client" << endl;
            }
        }

        close(client_socket);
    }

    close(server_socket);

    return 0;
}