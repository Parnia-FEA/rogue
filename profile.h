#ifndef PROFILE_H
#define PROFILE_H
#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "user.h"

#define MAX_SIZE 1000

void show_profile(User *user) {
	clear();
    if (user -> is_guest == 1) {
        attron(COLOR_PAIR(1));
        mvprintw(27, 95, "First, Log In To Your Account");
        attroff(COLOR_PAIR(1));
        refresh();
        sleep(3);
        return;
    }
    attron(COLOR_PAIR(2));
    mvprintw(27, 104, "PROFILE");
    attroff(COLOR_PAIR(2));
    mvprintw(29, 104, "Username : %s", user -> username);
    mvprintw(30, 104, "Password : %s", user -> password);
    mvprintw(31, 104, "Email : %s", user -> email);
    mvprintw(32, 104, "Experience : %d Games Completed", user -> complete_games);
    mvprintw(33, 104, "Score : %d", user -> total_score);
    mvprintw(34, 104, "Gold : %d", user -> total_gold);
    refresh();
    int c;
    while ((c = getch()) != KEY_F(1)) {

    }
}

#endif