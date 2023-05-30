//
// Created by leonc on 5/27/2023.
//

#ifndef CLI_POKER_POKER_H
#define CLI_POKER_POKER_H

#define MIN_USERNAME 4      // 3 character excluding \n
#define MAX_USERNAME 13     // 12 characters excluding \n

#define MAX_PLAYERS 5

#define MAX_WAITLIST 5

#define MAX_MESSAGE 100

#define MAX_READ 100

#define TABLE_TXT "table.txt"

#define MALLOC(ptr, size) \
    do { \
           (ptr) = malloc((size)); \
           if ((ptr) == NULL) { \
               perror("malloc"); \
               exit(1); \
           } \
    } while (0)

enum Rank {
    HIGH_CARD,
    PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};

typedef struct card {
    char suit;              // 's', 'h', 'd', 'c'
    char value;             // accommodating face cards
} Card;

typedef struct hand {
    struct card cards[2];
} Hand;


typedef struct player {
    char username[MAX_USERNAME];
    int money;
    struct hand *hand;
    enum Rank rank;
    int client_fd;
    int folded;
    int bet_amount;
} Player;

typedef struct waiting {
    char username[MAX_USERNAME];
    int client_fd;
    int seat;
    struct waiting *next;
} Waiting;

typedef struct table {
    struct player players[MAX_PLAYERS];
    struct card comm_cards[5];
    struct player dealer;
    struct player current_turn;
    int num_comm_cards;
    struct waiting *waiting;
    int playing;                    // 0 indicates no game is being played at this table
} Table;

/**
 * Determines if the waitlist is full
 *
 * Return:
 *  - 0 if waitlist is not full
 *  - 1 if waitlist is full
 */
int is_full(Waiting *waiting);

/**
* Determines if the client is in the waiting list
*
* Return:
*  - 0 if client is not in the waitlist
*  - 1 if client is in the waitlist
*/
int is_waiting(int client_fd, Waiting *waitlist);

/**
* Determines if the client has a username
*
* Return:
*  - 0 if client is not registered
*  - 1 if client is registered
*/
int is_registered(int client_fd, Waiting *waitlist);

/**
* Determines if the player is at this table
*
* Return:
*  - 0 if player is not at this table
*  - 1 if player is at this table
*/
int is_playing(int client_fd, Table *table);

/**
* Adds a client to the end of the registering queue
*
* Preconditions: The waitlist queue must not be full
*/
void add_registering(int client_fd, Waiting **waitlist);

/**
* Adds the chosen username to the client fd
*
* Preconditions: The client fd must be in the waitlist
*/
void register_username(char *username, int client_fd, Waiting **waitlist);

/**
* Check if there are enough people at this table for a game
*
* Return:
 *  0 if there are not enough people
 *  1 if there are enough people
*/
int enough_players(Table *table);

/**
 * Add the players in the waiting list to the table and updates the player count
 */
void add_players(Table *table);

/**
* Starts a table at this game, and sends the initial board, hand, and messages to the players
*/
void start_game(Table *table);

/**
* Lists the players in the game, delimited with '\n'
*/
char *list_players(Table *table);

#endif //CLI_POKER_POKER_H
