/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:16:17 by yismaili          #+#    #+#             */
/*   Updated: 2023/03/31 22:10:10 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iterator>

// int main() {
//     std::vector<int> sockfd;
//     struct sockaddr_in server1, server2;
//     char buffer[1024];

//     // Create the first socket
//     sockfd.push_back(socket(AF_INET, SOCK_STREAM, 0));
//     memset(&server1, 0, sizeof(server1));
//     server1.sin_family = AF_INET;
//     server1.sin_port = htons(8080);
//     server1.sin_addr.s_addr = inet_addr("127.0.0.1");

//     // Create the second socket
//     sockfd.push_back(socket(AF_INET, SOCK_STREAM, 0));
//     memset(&server2, 0, sizeof(server2));
//     server2.sin_family = AF_INET;
//     server2.sin_port = htons(8081);
//     server2.sin_addr.s_addr = inet_addr("127.0.0.1");

//     // Connect to the servers
//     connect(*sockfd.begin(), (struct sockaddr*)&server1, sizeof(server1));
//     connect(*(sockfd.begin()+1), (struct sockaddr*)&server2, sizeof(server2));

//     // Monitor the sockets for input
//     fd_set readfds;
//     int i = 0;
//     int maxfd;
//     std::vector<int>::iterator it = sockfd.begin();
//      while (i < sockfd.size()) {
//         while (1) {
//                 FD_ZERO(&readfds);
//                 FD_SET(*it, &readfds);
//                 FD_SET(*(it+i), &readfds);
//                 maxfd = (*it > *(it+i)) ? *it : *(it+i);
//                 // Wait for input
//                 select(maxfd + i, &readfds, NULL, NULL, NULL);

//                 // Check which socket has input
//                 if (FD_ISSET(*it, &readfds)) {
//                     // Read from it
//                     read(*it, buffer, sizeof(buffer));
//                     printf("\nReceived from it: %s", buffer);
//                 }
//                 if (FD_ISSET(*(it+i), &readfds)) {
//                     // Read from *(it+i)
//                     read(*(it+i), buffer, sizeof(buffer));
//                     printf("\nReceived from *(it+1): %s", buffer);
//                 }
//             }
//       i++;
//     }

//     return 0;
// }
// /*---------------------------------------*/
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <string.h>
// #include <poll.h>


// int main() {
//     int sockfd1, sockfd2;
//     struct sockaddr_in server1, server2;
//     char buffer[1024];

//     // Create the first socket
//     sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
//     memset(&server1, 0, sizeof(server1));
//     server1.sin_family = AF_INET;
//     server1.sin_port = htons(8080);
//     server1.sin_addr.s_addr = inet_addr("127.0.0.1");

//     // Create the second socket
//     sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
//     memset(&server2, 0, sizeof(server2));
//     server2.sin_family = AF_INET;
//     server2.sin_port = htons(8081);
//     server2.sin_addr.s_addr = inet_addr("127.0.0.1");

//     // Connect to the servers
//     connect(sockfd1, (struct sockaddr*)&server1, sizeof(server1));
//     connect(sockfd2, (struct sockaddr*)&server2, sizeof(server2));

//     // Monitor the sockets for input
//     struct pollfd fds[2];
//     fds[0].fd = sockfd1;
//     fds[0].events = POLLIN;
//     fds[1].fd = sockfd2;
//     fds[1].events = POLLIN;
//     while (1) {
//         // Wait for input
//         poll(fds, 2, -1);

//         // Check which socket has input
//         if (fds[0].revents & POLLIN) {
//             // Read from sockfd1
//             read(sockfd1, buffer, sizeof(buffer));
//             printf("Received from sockfd1: %s", buffer);
//         }

//         if (fds[1].revents & POLLIN) {
//             // Read from sockfd2
//             read(sockfd2, buffer, sizeof(buffer));
//             printf("Received from sockfd2: %s", buffer);
//         }
//     }

//     return 0;
// }

#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>

int main() {
    fd_set readfds, writefds, exceptfds;
    struct timeval timeout;
    int nfds, retval;

    // Initialize file descriptor sets
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    // Add file descriptors to readfds set
    FD_SET(STDIN_FILENO, &readfds);
    FD_SET(STDOUT_FILENO, &readfds);

    // Set timeout to 5 seconds
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    // Get the highest-numbered file descriptor
    nfds = STDOUT_FILENO + 1;

    // Call select() function
    retval = select(nfds, &readfds, &writefds, &exceptfds, &timeout);

    // Check for errors
    if (retval == -1) {
        perror("select()");
        return 1;
    } else if (retval == 0) {
        printf("No data within five seconds.\n");
        return 0;
    }

    // Check which file descriptors are ready for reading
    if (FD_ISSET(STDIN_FILENO, &writefds)) {
        printf("stdin is ready for reading.\n");
    }

    if (FD_ISSET(STDOUT_FILENO, &writefds)) {
        printf("stdout is ready for reading.\n");
    }

    return 0;
}
