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
        if (FD_ISSET(listen_soc, &ready_fds)) {
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
                // Add to fd list
                FD_SET(client_socket, &client_fds);

                // Add to registering queue
                add_registering(client_socket, &(table->waiting));
                continue;

            // Kick the client
            } else {
                if (write(client_socket, "Sorry, the server is currently full. Try again later.\r\n", sizeof("Sorry, the server is currently full. Try again later.\r\n")) == -1) {
                    perror("write");
                    exit(1);
                }
            }
            continue;
        }

        // The message comes from a client socket





            // If it is the clients turn

            // Check if move is valid

            // Play the move

            // Check for round over


            // Update current player

            // Check for game over

            // Update dealer


            // Reset round

        // Not the client's turn
        int client_fd = fetch_fd(&ready_fds, table);

        // Reading a message
        int close_sig = 0;
        char *command = read_command(client_fd, &close_sig);
        if (close_sig) {
            // Close client
            continue;
        }
        fprintf(stderr, "read %s from %d\n", command, client_fd);

        // Case 1: Registering a username
        if (!is_playing(client_fd, table) && !is_registered(client_fd, table->waiting)) {
            fprintf(stderr, "REGISTERING USERNAME\n");
            register_username(command, client_fd, &(table->waiting));

            // No game being played
            if (table->playing == 0) {
                // Check if a game can be start
                if (enough_players(table)) {
                    add_players(table);
                    start_game(table);
                    fprintf(stderr, "players list: \n%s\n", list_players(table));
                }
            }
            continue;
        }

        // Case 2: Ignore
        fprintf(stderr, "IGNORE MESSAGE\n");

    }


}