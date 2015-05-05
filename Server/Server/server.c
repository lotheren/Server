#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "socket_name.h"

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;

    // Remove any old socket and create an unnamed socket for the server.
    unlink(SOCKET_NAME);
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    // Name the socket.
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SOCKET_NAME);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    // Create a connection queue and wait for clients.
    listen(server_sockfd, 5);
    for (;;) {
        printf("Server waiting.\n");

        //  Accept a socket connection.
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, 
                              (struct sockaddr *) &client_address, 
                              &client_len);

        // Read a character from the socket.
        char ch;
        read(client_sockfd, &ch, 1);
        printf("Read character '%c'\n", ch);
        
        // Increment the character and write it back to the socket.
        ch++;
        printf("Writing character '%c'\n", ch);
        write(client_sockfd, &ch, 1);
        
        // Close the socket.
        close(client_sockfd);
    }
}
