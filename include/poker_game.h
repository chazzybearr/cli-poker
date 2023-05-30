//
// Created by leonc on 5/27/2023.
//

#ifndef CLI_POKER_POKER_GAME_H
#define CLI_POKER_POKER_GAME_H

#include "../include/poker.h"

/**
* Accepts a connection on the listen_soc
* Returns the newly connected client socket
*/
int accept_client(int listen_soc);

/**
 * Gets the fd that is returned from select
 */
int fetch_fd(fd_set *client_fds, Table *table);

/**
 * Returns the index of the character after "\r\n" in the buf
 * Returns -1 if it does not exist
 */
int find_network_newline(const char *buf, int n);

/**
 * Reads a line from the client socket and returns the line
 * Close_sig set to 1 if the client closed their connection
 *
 * Precondition: client_socket must not be blocked
 */
char *read_command(int client_fd, int *close_sig);

char *get_file(char *filename);

#endif //CLI_POKER_POKER_GAME_H
