//
// Created by chazzybearr on 5/27/2023.
//
#include <stdio.h>
#include <stddef.h>
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
