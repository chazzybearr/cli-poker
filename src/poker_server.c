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
#include "../include/poker.h"
#include "../include/poker_game.h"

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
    // Set up an set of fds
    fd_set client_fds;
    FD_ZERO(&client_fds);
    FD_SET(listen_soc, &client_fds);
    int maxfd = listen_soc + 1;

    // Setting up a table
    Table *table = calloc(1, sizeof(Table));


    // Main loop
    while (1) {
        // Setting all fds
        fd_set ready_fds = client_fds;

        // Listening for a message
        if (select(maxfd, &ready_fds, NULL, NULL, NULL) != 1) {
            perror("select");
            exit(1);
        }

        // If the messages comes from the listen socket
        if (FD_ISSET(listen_soc, &client_fds)) {
            int client_socket = accept_client(listen_soc);
            if (maxfd - 1 < client_socket) {
                maxfd = client_socket + 1;
            }

            // If the waitlist is not full
            if (!is_full(table->waiting)) {

                // If just connecting, prompt for username
                if (write(client_socket, "What is your username?\r\n", sizeof("What is your username?\r\n")) == -1) {
                    perror("write");
                    exit(1);
                }
                continue;

            // Kick the client
            } else {

            }


        }

        // Check for space availability



        // If the message comes from a client socket

        // If it is not the client's turn, ignore

        // If it is the clients turn

        // Check if move is valid

        // Play the move

        // Check for round over


        // Update current player

        // Check for game over

        // Update dealer


        // Reset round
    }


}