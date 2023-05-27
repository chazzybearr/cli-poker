//
// Created by leonc on 5/27/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#ifndef PORT
    #define PORT 56523
#endif

int main () {

    // Setting up the socket
    int listen_soc = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_soc == -1) {
        perror("socket");
        exit(1);
    }

    // Configure socket to wait for connections
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(addr.sin_zero), 0, 8);

    if (bind(listen_soc, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1) {
        perror("bind");
        close(listen_soc);
        exit(1);
    }

    // Listening for connections
    if (listen(listen_soc, 5) < 0) {
        perror("listen");
        exit(1);
    }



}