#ifndef GUEST_H
#define GUEST_H
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "game_menu.h"


#define MAX_SIZE 1000


void guest_user() {
    clear();
    User user;
    strcpy(user.username, "G");
    user.is_guest = 1;
    user.resume = 0;
    user.total_gold = 100;
    user.difficulty = 1;
    user.total_score = 0;
    user.color = 2;
    user.games = 0;
    user.complete_games = 0;
    game_menu_func(&user);
}

#endif