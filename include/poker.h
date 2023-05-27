//
// Created by leonc on 5/27/2023.
//

#ifndef CLI_POKER_POKER_H
#define CLI_POKER_POKER_H

#define MIN_USERNAME 4      // 3 character excluding \n
#define MAX_USERNAME 13     // 12 characters excluding \n

#define MAX_PLAYERS 5

#define MAX_WAITLIST 5

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
    struct player players[5];
    struct card comm_cards[5];
    struct player dealer;
    struct player current_turn;
    int num_comm_cards;
    struct waiting *waiting;
} Table;

/**
 * Determines if the waitlist is full
 *
 * Return:
 *  - 0 if waitlist is not full
 *  - 1 if waitlist is full
 */
int is_full(Waiting *waiting);

#endif //CLI_POKER_POKER_H
