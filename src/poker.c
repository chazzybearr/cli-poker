//
// Created by chazzybearr on 5/27/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../include/poker.h"

/**
 * Determines if the waitlist is full
 *
 * Return:
 *  - 0 if waitlist is not full
 *  - 1 if waitlist is full
 */
 int is_full(Waiting *waitlist) {
    for (int i = 0; i < MAX_WAITLIST; i++) {
        if (waitlist == NULL) {
            return 0;
        }
        waitlist = waitlist->next;
    }
    return 1;
 }

/**
* Determines if the client is in the waiting list
*
* Return:
*  - 0 if client is not in the waitlist
*  - 1 if client is in the waitlist
*/
int is_waiting(int client_fd, Waiting *waitlist) {
    for (int i = 0; i < MAX_WAITLIST; i++) {
        if (waitlist->client_fd == client_fd) {
            return 1;
        }
        waitlist = waitlist->next;
    }
    return 0;
}

/**
* Determines if the player is at this table
*
* Return:
*  - 0 if player is not at this table
*  - 1 if player is at this table
*/
 int is_playing(int client_fd, Table *table) {
     for (int i = 0; i < MAX_PLAYERS; i++) {
        if (table->players[i].client_fd == client_fd) {
            return 1;
        }
    }
    return 0;
 }

/**
* Adds a client to the end of the registering queue
*
* Preconditions: The waitlist queue must not be full
*/
void add_registering(int client_fd, Waiting **waitlist) {

    Waiting *new_client = malloc(sizeof(Waiting));
    new_client->client_fd = client_fd;
    new_client->next = NULL;
    memset(new_client->username, '\0', MAX_USERNAME);
    new_client->seat = -1;

    if (*waitlist == NULL) {
        *waitlist = new_client;
        return;
    }

    Waiting *curr = *waitlist;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_client;
}

/**
* Adds the chosen username to the client fd
*
* Preconditions: The client fd must be in the waitlist
*/
void register_username(char *username, int client_fd, Waiting **waitlist) {

    Waiting *curr = *waitlist;
    while (curr != NULL) {
        if (curr->client_fd == client_fd) {
            strncpy(curr->username, username, MAX_USERNAME);
            return;
        }
    }
}