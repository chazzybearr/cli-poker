//
// Created by leonc on 5/27/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../include/poker_game.h"
#include "../include/poker.h"

/**
* Accepts a connection on the listen_soc
* Returns the newly connected client socket
*/
int accept_client(int listen_soc) {
    // Accept one client
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    unsigned int client_len = sizeof(struct sockaddr_in);

    int client_socket = accept(listen_soc, (struct sockaddr *) &client_addr, &client_len);
    if (client_socket < 0) {
        perror("accept");
        exit(1);
    }
    return client_socket;
}