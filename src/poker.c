//
// Created by chazzybearr on 5/27/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include "../include/poker.h"
#include "../include/poker_game.h"

void add_simulated_players(Table *table) {

    strcpy(table->players[0].username, "leon");
    table->players[0].client_fd = 3;
    table->players[0].folded = 0;
    table->players[0].bet_amount = 100;
    table->players[0].money = 1000;

    strcpy(table->players[1].username, "heather");
    table->players[1].client_fd = 4;
    table->players[1].folded = 1;
    table->players[1].bet_amount = 200;
    table->players[1].money = 289;

    strcpy(table->players[2].username, "lucas");
    table->players[2].client_fd = 5;
    table->players[2].folded = 0;
    table->players[2].bet_amount = 149;
    table->players[2].money = 345;

    strcpy(table->players[3].username, "jacky");
    table->players[3].client_fd = 6;
    table->players[3].folded = 0;
    table->players[3].bet_amount = 655;
    table->players[3].money = 3457;
}

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
* Determines if the client has a username
*
* Return:
*  - 0 if client is not registered
*  - 1 if client is registered
*/
int is_registered(int client_fd, Waiting *waitlist) {
    for (int i = 0; i < MAX_WAITLIST; i++) {
        if (waitlist->client_fd == client_fd) {
            if (strcmp(waitlist->username, "") == 0) {
                return 0;
            }
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
        curr = curr->next;
    }
}

/**
* Check if there are enough people at this table for a game
*
* Return:
*  0 if there are not enough people
*  1 if there are enough people
*/
int enough_players(Table *table) {
    int players = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (strcmp(table->players->username, "") != 0) {
            players++;
        }
    }

    Waiting *curr = table->waiting;
    while (curr != NULL) {
        if (strcmp(curr->username, "") != 0) {
            players++;
        }
        curr = curr->next;
    }
    if (players > 1) {
        return 1;
    }
    return 0;
}

/**
* Removes a client from the waiting list, fills the username and client_fd fields
*
* Return:
*  0 if there are no more clients in the waitlist
*  1 if there are clients in the waitlist
*/
int pop_waiting(Waiting **waitlist, char **username, int *client_fd) {

    if (*waitlist == NULL) {
        return 0;
    }

    *username = (*waitlist)->username;
    *client_fd = (*waitlist)->client_fd;


    *waitlist = (*waitlist)->next;


    return 1;
}


/**
 * Add the players in the waiting list to the table and updates the player count
 */
 void add_players(Table *table) {
     for (int i = 0; i < MAX_PLAYERS; i++) {
         if (strcmp(table->players[i].username, "") == 0) {
            char *username;
            int client_fd;
            if (pop_waiting(&table->waiting, &username, &(client_fd)) == 0) {
                break;
            }

            strcpy(table->players[i].username, username);
            table->players[i].client_fd = client_fd;
            (table->playing)++;
         }
     }
 }

/**
* Sends a message to all the sockets at this table
*/
void send_all(char *message, Table *table) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (table->players[i].client_fd != 0) {
            if (write(table->players[i].client_fd, message, strlen(message)) == -1) {
                perror("write");
                exit(1);
            }
        }
    }
}

/**
* Starts a table at this game, and sends the initial board, hand, and messages to the players
*/
void start_game(Table *table) {
    fprintf(stderr, "game started\n");
    send_all("game started\r\n", table);
    char *blank_table = get_file(TABLE_TXT);
    send_all(blank_table, table);


}

/**
* Lists the players in the game, delimited with '\n'
*/
char *list_players(Table *table) {
    int size = 1;                                               // Null terminator
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (strcmp(table->players[i].username, "") != 0) {
            size += strlen(table->players[i].username) + 2;      // Accounts for \r\n
        }
    }

    char *player_list = malloc(sizeof(char) * size);
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (strcmp(table->players[i].username, "") != 0) {
            strcat(player_list, table->players[i].username);
            strcat(player_list, "\r\n");
        }
    }
    strcat(player_list, "\0");
    return player_list;
}

char *draw_name(char *name, int i, char *table) {

    int length = strlen(name);
    switch (i) {
        case 0:
            memmove(table + 75, name, length);
            break;
        case 1:
            memmove(table + 97, name, length);
            break;
        case 2:
            memmove(table + 1096, name, length);
            break;
        case 3:
            memmove(table + 1117, name, length);
            break;
    }
}

char *draw_money(char *money, int i, char *table) {

    int length = strlen(money);
    switch (i) {
        case 0:
            memmove(table + 143, money, length);
            break;
        case 1:
            memmove(table + 165, money, length);
            break;
        case 2:
            memmove(table + 1164, money, length);
            break;
        case 3:
            memmove(table + 1185, money, length);
            break;
    }
}

char *draw_state(Table *table) {
    char *blank_table = get_file(TABLE_TXT);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player player = table->players[i];
        char *name = player.username;
        int money = player.money;
        int bet_amount = player.bet_amount;
        int folded = player.folded;

        draw_name(name, i, blank_table);

        char s_money[MAX_MESSAGE];
        sprintf(s_money, "$%d", money);
        draw_money(s_money, i, blank_table);
    }

    return blank_table;

}