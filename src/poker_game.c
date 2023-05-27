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

/**
 * Gets the fd that is returned from select
 */
int fetch_fd(fd_set *client_fds, Table *table) {
    // Check if fd is at the table
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (FD_ISSET(table->players[i].client_fd, client_fds)) {
            return table->players[i].client_fd;
        }
    }
    // Check if fd is waiting
    Waiting *curr = table->waiting;
    while (curr != NULL) {
        if (FD_ISSET(curr->client_fd, client_fds)) {
            return curr->client_fd;
        }
        curr = curr->next;
    }
    return -1;
}

/**
 * Returns the index of the character after "\r\n" in the buf
 * Returns -1 if it does not exist
 */
int find_network_newline(const char *buf, int n) {
    // Searching for \r
    for (int i = 0; i < n; i++) {

        if (buf[i] == '\n') {
            return i + 1;
        }
//        if (buf[i] == '\r') {
//            if (i < n-1 && buf[i+1] == '\n') {
//                return i + 2;
//            }
//        }
    }
    return -1;
}

/**
 * Reads a line from the client socket and returns the line
 * Close_sig set to 1 if the client closed their connection
 *
 * Precondition: client_socket must not be blocked
 */
char *read_command(int client_fd, int *close_sig) {

    char *buf = malloc(sizeof(char) * MAX_MESSAGE);
    strncpy(buf, "\0", MAX_MESSAGE);
    // Receive messages
    char *next_command;
    int inbuf = 0;           // How many bytes currently in buffer?
    int room = sizeof(buf);  // How many bytes remaining in buffer?
    char *after = buf;       // Pointer to position after the data in buf
    int nbytes;
    while ((nbytes = read(client_fd, after, room)) > 0) {
        // Update inbuf
        inbuf += nbytes;
        int where;
        // Determine if a full line has been read from the client.
        while ((where = find_network_newline(buf, inbuf)) > 0) {
            // Next command is the full line, not including the "\r\n",
            buf[where - 1] = '\0';
            MALLOC(next_command, sizeof(char) * strlen(buf) + 1);
            strcpy(next_command, buf);
            return next_command;

            // Update inbuf and remove the full line from the buffer
            inbuf = inbuf - where;
            memmove(buf, &(buf[where]), inbuf);
        }
        // Update after and room, in preparation for the next read.
        after = &(buf[inbuf]);
        room = sizeof(buf) - inbuf;
    }

    // Read failed
    if (nbytes == -1) {
        perror("read");
        exit(1);
    }
    // Client closed connection only when nbytes returns 0
    *close_sig = 1;
    return "";
}