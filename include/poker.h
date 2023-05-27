//
// Created by leonc on 5/27/2023.
//

#ifndef CLI_POKER_POKER_H
#define CLI_POKER_POKER_H

#define MIN_USERNAME 4      // 3 character excluding \n
#define MAX_USERNAME 13     // 12 characters excluding \n

#define MAX_PLAYERS 5

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
    Rank rank;
} Player;

typedef struct table {
    struct player players[5];
    struct card comm_cards[5];
} Table;

#endif //CLI_POKER_POKER_H
