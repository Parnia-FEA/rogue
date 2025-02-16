#ifndef CREATE_NEW_GAME_H
#define CREATE_NEW_GAME_H
#include <curses.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "user.h"
#include "food_menu.h"
#include "enchant_menu.h"
#include "room.h"
#include "create_map.h"
#include "weapon_menu.h"
#define START 6

#define MAX_SIZE 1000

void game_func(User *);
int move_directly(User *, int, int, int);
int move_indirectly(User *, int, int, int);
void print_screen(User *, int, char, int);
void *hunger_rate(void *);
void *health_rate(void *);
void *add_health(void *);
int enemy_damage(User *);
void change_info(User *, int);
void copy_info();
void user_short_damage(User *, int, int *, int *, int *, int *, int *, int , int);
void user_short_damage2(User *, int, int *, int *, int *, int *, int *, int , int);
void user_long_damage(User *, int, int, int *, int *, int *, int *, int *,int , int );

void create_new_game_func (User *user) {
    (user -> games)++;
    create_map(user);
    user -> visible_mode = 0;
    user -> food = 0;
    user -> power = 1;
    user -> health = 50 + (3 - user -> difficulty) * 25;
    user -> hunger = 14 + (3 - user -> difficulty) * 3;
    user -> gold = 0;
    user -> score = 0;
    (user -> enchant_menu)[0] = 0; //Health
    (user -> enchant_menu)[1] = 0; //Speed
    (user -> enchant_menu)[2] = 0; //Damage
    (user -> weapon_menu)[0] = 1; //Hammer
    (user -> weapon_menu)[1] = 0; //Dagger
    (user -> weapon_menu)[2] = 0; //Wand
    (user -> weapon_menu)[3] = 0; //Normal Arrow
    (user -> weapon_menu)[4] = 0; //Hocho
    (user -> current_weapon) = 0;
    game_func(user);
}

void continue_game_func (User *user) {
    if (user -> resume == 0) {
        attron(COLOR_PAIR(1));
        mvprintw(29, 98, "No Incomplete Game. Start A New Game");
        attroff(COLOR_PAIR(1));
        refresh();
        sleep(2);
        return;
    }
    game_func(user);
}

void game_func (User *user) {
    curs_set(0);
    int end = 0;
    user -> end = 0;
    user -> rate = 1;
    user -> speed = 1;
    pthread_t thread_hunger;
    pthread_t thread_health;
    pthread_t thread_add_health;
    //pthread_t thread_enemy_damage;
    pthread_create(&thread_hunger, NULL, hunger_rate, (void *)user);
    pthread_create(&thread_health, NULL, health_rate, (void *)user);
    pthread_create(&thread_add_health, NULL, add_health, (void *)user);
    //pthread_create(&thread_enemy_damage, NULL, enemy_damage, (void *)(user));
    char gamer = toupper((user -> username)[0]);
    int c;
    clear();
    for (int i = 0; i < 60; i++) {
        for (int j = 0; j < 200; j++) {
            if ((user -> visible)[user -> current_floor][i][j] || user -> visible_mode) {
                if ((user -> map_screen)[user -> current_floor][i][j] > 0) {
                    attron(COLOR_PAIR(((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme));
                    if (((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme == 5) {
                        attron(A_BLINK);
                    }
                }
                if ((user -> map_screen_char)[user -> current_floor][i][j] == '-') {
                    mvaddch(START + i, START + j, '_');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == '!') {
                    mvaddch(START + i, START + j, '|');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 't') {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && (user -> current_y != i || user -> current_x != j)) {
                    mvprintw(START + i, START + j, "%s", "\U0001F354");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'b' && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'b' && (user -> current_y != i || user -> current_x != j)) {
                    mvprintw(START + i, START + j, "%s", "\U0001F354");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'j' && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'j' && (user -> current_y != i || user -> current_x != j)) {
                    mvprintw(START + i, START + j, "%s", "\U0001F363");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'k' && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'k' && (user -> current_y != i || user -> current_x != j)) {
                    mvprintw(START + i, START + j, "%s", "\U0001F36D");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'x') {
                    mvprintw(START + i, START + j, "%s", "\U0001F921");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'w') {
                    mvprintw(START + i, START + j, "%s", "\U0001F608");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'q') {
                    mvprintw(START + i, START + j, "%s", "\U00002603");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'z') {
                    attron(COLOR_PAIR(3));
                    mvprintw(START + i, START + j, "%s", "\U000020B0");
                    attroff(COLOR_PAIR(3));
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'u') {
                    mvprintw(START + i, START + j, "%s", "\U00002620");
                }
                else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && (user -> current_y != i || user -> current_x != j)) {
                    if ((user -> map_screen_char)[user -> current_floor][i][j] == '0') {
                        mvprintw(START + i, START + j, "%s", "\U0001F528");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '1') {
                        mvprintw(START + i, START + j, "%s", "\U0001F5E1");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '2') {
                        attron(COLOR_PAIR(4));
                        mvprintw(START + i, START + j, "%s", "\U0001327B");
                        attroff(COLOR_PAIR(4));
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '3') {
                        mvprintw(START + i, START + j, "%s", "\U0001F3F9");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '4') {
                        mvprintw(START + i, START + j, "%s", "\U0001F52A");
                    }
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == '$') {
                    attron(COLOR_PAIR(4));
                    mvprintw(START + i, START + j, "%s", "\U00002735");
                    attroff(COLOR_PAIR(4));
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'G') {
                    attron(COLOR_PAIR(2));
                    mvprintw(START + i, START + j, "%s", "\U00002742");
                    attron(COLOR_PAIR(2));
                }
                else if (j == 0 || ((user -> map_screen_char)[user -> current_floor][i][j - 1] != 'B' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'b' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'j' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'k' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'x' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'w' && isdigit((user -> map_screen_char)[user -> current_floor][i][j - 1]) == 0)){
                    mvaddch(START + i, START + j, (user -> map_screen_char)[user -> current_floor][i][j]);
                }
                if ((user -> map_screen)[user -> current_floor][i][j] > 0) {
                    attroff(COLOR_PAIR(((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme));
                    if (((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme == 5) {
                        attroff(A_BLINK);
                    }
                }
            }
        }
    }
    attron(COLOR_PAIR(user -> color) | A_BLINK);
    mvaddch(user -> current_y + START, user -> current_x + START, gamer);
    attroff(COLOR_PAIR(user -> color) | A_BLINK);
    mvprintw(2, 0, "health");
    for (int i = 0; i < (user -> health); i++) {
        if (user -> health < 20) {
            attron(COLOR_PAIR(8));
        }
        else {
            attron(COLOR_PAIR(7));
        }
        mvprintw(2, i + 7, " ");
        if (user -> health < 20) {
            attroff(COLOR_PAIR(8));
        }
        else {
            attroff(COLOR_PAIR(7));
        }
    }
    attron(COLOR_PAIR(4));
    mvprintw(3, 0, "gold %d", user -> gold);
    mvprintw(3, 50, "score %d", user -> score);
    attroff(COLOR_PAIR(4));
    refresh();
    while (user -> health > 0 && (c = getch()) != KEY_F(1)) {
        int flag = 0;
        int flag_stair = 0;
        int flag_g = 0;
        int to_enemies[MAX_SIZE2];
        int health[MAX_SIZE2];
        int to = 0;
        int flag_c = 0;
        int to_damage[MAX_SIZE2];
        int explode[MAX_SIZE2];
        //instruction
        if (c == 'w') {
            user -> current_weapon = -1;
            mvprintw(0, 0, "Now, You Have No Weapons In Hand!");
            refresh();
            c = getch();
        }
        if (c == 's') {
            if ((user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x] > 0) {
                if (user -> current_y - 1 >= 0 && ((user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] = '?';
                }
                if (user -> current_y - 1 >= 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] = '^';
                }

                if (user -> current_y + 1 < 60 && ((user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] = '?';
                }
                if (user -> current_y + 1 < 60 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] = '^';
                }

                if (user -> current_x - 1 >= 0 && ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '?';
                }
                if (user -> current_x - 1 >= 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '^';
                }

                if (user -> current_x + 1 < 200 && ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] = '?';
                }
                if (user -> current_x + 1 < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] = '^';
                }

                if (user -> current_y - 1 >= 0 && user -> current_x - 1 >= 0 && ((user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] = '?';
                }
                if (user -> current_y - 1 >= 0 && user -> current_x - 1 >= 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] = '^';
                }

                if (user -> current_y - 1 >= 0 && user -> current_x + 1 < 200 && ((user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] = '?';
                }
                if (user -> current_y - 1 >= 0 && user -> current_x + 1 < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] = '^';
                }

                if (user -> current_y + 1 < 60 && user -> current_x - 1 >= 0 && ((user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] = '?';
                }
                if (user -> current_y + 1 < 60 && user -> current_x - 1 >= 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] = '^';
                }

                if (user -> current_y + 1 < 60 && user -> current_x + 1 < 200 && ((user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == '!')) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] = '?';
                }
                if (user -> current_y + 1 < 60 && user -> current_x + 1 < 200 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == 't') {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] = '^';
                }
            }
            c = getch();
        }
        if (c == 'g') {
            flag_g = 1;
            c = getch();
        }
        if (c == 'm') {
            user -> visible_mode = 1 - (user -> visible_mode);
            flag = 1;
        }
        else if (c == 'e') {
            food_menu_func(user);
            flag = 1;
        }
        else if (c == 'r') {
            enchant_menu_func(user);
            flag = 1;
        }
        else if (c == 'i') {
            weapon_menu_func(user);
            flag = 1;
        }
        else if (c == ' ') {
            if (user -> current_weapon == -1 || (user -> weapon_menu)[user -> current_weapon] == 0) {
                flag_c = 1;
                attron(COLOR_PAIR(1));
                mvprintw(4, 0, "You Have No Weapons In Hand!");
                attron(COLOR_PAIR(1));
            }
            else {
                if (user -> current_weapon == 0 || user -> current_weapon == 4) {
                    int power = (user -> power) * 5;
                    if (user -> current_weapon == 4) {
                        power *= 2;
                    }
                    //short range hammer
                    user_short_damage(user, power, to_enemies, explode, to_damage, health, &to, 0, -1);
                    user_short_damage(user, power, to_enemies, explode, to_damage, health, &to, 0, 1);
                    user_short_damage(user, power, to_enemies, explode, to_damage, health, &to, 1, -1);
                    user_short_damage(user, power, to_enemies, explode, to_damage, health, &to, 1, 0);
                    user_short_damage(user, power, to_enemies, explode, to_damage, health, &to, 1, 1);
                    user_short_damage(user, power, to_enemies, explode, to_damage, health, &to, -1, -1);
                    user_short_damage(user, power, to_enemies, explode, to_damage, health, &to, -1, 0);
                    user_short_damage(user, power, to_enemies, explode, to_damage, health, &to, -1, 1);
                    user_short_damage2(user, power, to_enemies, explode, to_damage, health, &to, -2, -1);
                    user_short_damage2(user, power, to_enemies, explode, to_damage, health, &to, -2, 0);
                    user_short_damage2(user, power, to_enemies, explode, to_damage, health, &to, -2, 1);
                }
                else {
                    //long range
                    c = getch();
                    int distance = 5;
                    int power;
                    if (user -> current_weapon == 1) {
                        power = 12;
                    }
                    else if (user -> current_weapon == 2) {
                        distance = 10;
                        power = 15;
                    }
                    else if (user -> current_weapon == 3) {
                        power = 5;
                    }
                    power *= (user -> power);
                    if (c == KEY_UP) {
                        user_long_damage(user, distance, power, to_enemies, explode, to_damage, health, &to, 0, -1);
                    }
                    else if (c == KEY_LEFT) {
                        user_long_damage(user, distance, power, to_enemies, explode, to_damage, health, &to, -1, 0);
                    }
                    else if (c == KEY_RIGHT) {
                        user_long_damage(user, distance, power, to_enemies, explode, to_damage, health, &to, 1, 0);
                    }
                    else if (c == KEY_DOWN) {
                        user_long_damage(user, distance, power, to_enemies, explode, to_damage, health, &to, 0, 1);
                    }
                    c = ' ';
                    flag = 1;
                }

            }
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '>' && c == '>') {
            (user -> current_floor)++;
            user -> current_x = (user -> in_staircase)[user -> current_floor] -> x;
            user -> current_y = (user -> in_staircase)[user -> current_floor] -> y;
            flag = 1;
            flag_stair = 1;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '<' && c == '<') {
            (user -> current_floor)--;
            user -> current_x = (user -> out_staircase)[user -> current_floor] -> x;
            user -> current_y = (user -> out_staircase)[user -> current_floor] -> y;
            flag = 1;
            flag_stair = 1;
        }
        else if (c == 'f') {
            c = getch();
            if (c == 'j') {
                //up
                while(move_directly(user, 0, -1, 1)) {
                    print_screen(user, flag_stair, gamer, 1);
                }
                flag = 1;
            }
            else if (c == 'k') {
                //down
                while(move_directly(user, 0, 1, 1)) {
                    print_screen(user, flag_stair, gamer, 1);
                }
                flag = 1;
            }
            else if (c == 'h') {
                //left
                while(move_directly(user, -1, 0, 1)) {
                    print_screen(user, flag_stair, gamer, 1);
                }
                flag = 1;
            }
            else if (c == 'l') {
                //right
                while(move_directly(user, 1, 0, 1)) {
                    print_screen(user, flag_stair, gamer, 1);
                }
                flag = 1;
            }
            else if (c == 'y') {
                //up left
                while(move_indirectly(user, -1, -1, 1)) {
                    print_screen(user, flag_stair, gamer, 1);
                }
                flag = 1;
            }
            else if (c == 'u') {
                //up right
                while(move_indirectly(user, 1, -1, 1)) {
                    print_screen(user, flag_stair, gamer, 1);
                }
                flag = 1;
            }
            else if (c == 'b') {
                //down left
                while(move_indirectly(user, -1, 1, 1)) {
                    print_screen(user, flag_stair, gamer, 1);
                }
                flag = 1;
            }
            else if (c == 'n') {
                //down right
                while(move_indirectly(user, 1, 1, 1)) {
                    print_screen(user, flag_stair, gamer, 1);
                }
                flag = 1;
            }
        }
        else if (c == 'j') {
            //up
            for (int i = 0; i < user -> speed; i++) {
                if (move_directly(user, 0, -1, 0)) {
                    flag = 1;
                }
            }
        }
        else if (c == 'k') {
            //down
            for (int i = 0; i < user -> speed; i++) {
                if (move_directly(user, 0, 1, 0)) {
                    flag = 1;
                }
            }
        }
        else if (c == 'h') {
            //left
            for (int i = 0; i < user -> speed; i++) {
                if (move_directly(user, -1, 0, 0)) {
                    flag = 1;
                }
            }
        }
        else if (c == 'l') {
            //right
            for (int i = 0; i < user -> speed; i++) {
                if (move_directly(user, 1, 0, 0)) {
                    flag = 1;
                }
            }
        }
        else if (c == 'y') {
            //up left
            for (int i = 0; i < user -> speed; i++) {
                if (move_indirectly(user, -1, -1, 0)) {
                    flag = 1;
                }
            }
        }
        else if (c == 'u') {
            //up right
            for (int i = 0; i < user -> speed; i++) {
                if (move_indirectly(user, 1, -1, 0)) {
                    flag = 1;
                }
            }
        }
        else if (c == 'b') {
            //down left
            for (int i = 0; i < user -> speed; i++) {
                if (move_indirectly(user, -1, 1, 0)) {
                    flag = 1;
                }
            }
        }
        else if (c == 'n') {
            //down right
            for (int i = 0; i < user -> speed; i++) {
                if (move_indirectly(user, 1, 1, 0)) {
                    flag = 1;
                }
            }
        }
        //mvprintw(0, 0, "%d", (user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x]);
        if ((user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x] > 0) {
            Room *current_room = (user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x] - 1];
            //mvprintw(0, 10, "%d %d %d %d", current_room -> uly, current_room -> ulx, current_room -> height, current_room -> width);
            for (int i = current_room -> uly; i < current_room -> uly + current_room -> height; i++) {
                for (int j = current_room -> ulx; j < current_room -> ulx + current_room -> width; j++) {
                    (user -> visible)[user -> current_floor][i][j] = 1;
                }
            }
        }
        (user -> visible)[user -> current_floor][user -> current_y][user -> current_x] = 1;
        if (flag) {
            clear();
            if (flag_stair) {
                mvprintw(0, 0, "New Floor!");
                refresh();
            }
            for (int i = 0; i < 60; i++) {
                for (int j = 0; j < 200; j++) {
                    int flag1 = 0;
                    if ((user -> map_screen)[user -> current_floor][i][j] < 0) {
                        if (user -> current_y == i) {
                            if (j > user -> current_x && j <= user -> current_x + 5) {
                                int flag2 = 1;
                                for (int k = user -> current_x + 1; k < j; k++) {
                                    if ((user -> map_screen)[user -> current_floor][i][k] > -1) {
                                        flag2 = 0;
                                    }
                                }
                                if (flag2) {
                                    flag1 = 1;
                                }
                            }
                            if (j < user -> current_x && j >= user -> current_x - 5) {
                                int flag2 = 1;
                                for (int k = user -> current_x - 1; k > j; k--) {
                                    if ((user -> map_screen)[user -> current_floor][i][k] > -1) {
                                        flag2 = 0;
                                    }
                                }
                                if (flag2) {
                                    flag1 = 1;
                                }
                            }
                        }
                        if (user -> current_x == j) {
                            if (i > user -> current_y && i <= user -> current_y + 5) {
                                int flag2 = 1;
                                for (int k = user -> current_y + 1; k < i; k++) {
                                    if ((user -> map_screen)[user -> current_floor][k][j] > -1) {
                                        flag2 = 0;
                                    }
                                }
                                if (flag2) {
                                    flag1 = 1;
                                }
                            }
                            if (i < user -> current_y && i >= user -> current_y - 5) {
                                int flag2 = 1;
                                for (int k = user -> current_y - 1; k > i; k--) {
                                    if ((user -> map_screen)[user -> current_floor][k][j] > -1) {
                                        flag2 = 0;
                                    }
                                }
                                if (flag2) {
                                    flag1 = 1;
                                }
                            }
                        }
                    }
                    if ((user -> visible)[user -> current_floor][i][j]) {
                        flag1 = 1;
                    }
                    if (flag1 || user -> visible_mode) {
                        if ((user -> map_screen)[user -> current_floor][i][j] > 0) {
                            attron(COLOR_PAIR(((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme));
                            if (((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme == 5) {
                                attron(A_BLINK);
                            }
                        }
                        if ((user -> map_screen_char)[user -> current_floor][i][j] == '-') {
                            mvaddch(START + i, START + j, '_');
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == '!') {
                            mvaddch(START + i, START + j, '|');
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 't') {
                            mvaddch(START + i, START + j, '.');
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && user -> current_y == i && user -> current_x == j + 1) {
                            mvaddch(START + i, START + j, '.');
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && (user -> current_y != i || user -> current_x != j)) {
                            mvprintw(START + i, START + j, "%s", "\U0001F354");
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'b' && user -> current_y == i && user -> current_x == j + 1) {
                            mvaddch(START + i, START + j, '.');
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'b' && (user -> current_y != i || user -> current_x != j)) {
                            mvprintw(START + i, START + j, "%s", "\U0001F354");
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'j' && user -> current_y == i && user -> current_x == j + 1) {
                            mvaddch(START + i, START + j, '.');
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'j' && (user -> current_y != i || user -> current_x != j)) {
                            mvprintw(START + i, START + j, "%s", "\U0001F363");
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'k' && user -> current_y == i && user -> current_x == j + 1) {
                            mvaddch(START + i, START + j, '.');
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'k' && (user -> current_y != i || user -> current_x != j)) {
                            mvprintw(START + i, START + j, "%s", "\U0001F36D");
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'x') {
                            mvprintw(START + i, START + j, "%s", "\U0001F921");
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'w') {
                            mvprintw(START + i, START + j, "%s", "\U0001F608");
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'q') {
                            mvprintw(START + i, START + j, "%s", "\U00002603");
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'z') {
                            attron(COLOR_PAIR(3));
                            mvprintw(START + i, START + j, "%s", "\U000020B0");
                            attroff(COLOR_PAIR(3));
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'u') {
                            mvprintw(START + i, START + j, "%s", "\U00002620");
                        }
                        else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && user -> current_y == i && user -> current_x == j + 1) {
                            mvaddch(START + i, START + j, '.');
                        }
                        else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && (user -> current_y != i || user -> current_x != j)) {
                            if ((user -> map_screen_char)[user -> current_floor][i][j] == '0') {
                                mvprintw(START + i, START + j, "%s", "\U0001F528");
                            }
                            else if ((user -> map_screen_char)[user -> current_floor][i][j] == '1') {
                                mvprintw(START + i, START + j, "%s", "\U0001F5E1");
                            }
                            else if ((user -> map_screen_char)[user -> current_floor][i][j] == '2') {
                                attron(COLOR_PAIR(4));
                                mvprintw(START + i, START + j, "%s", "\U0001327B");
                                attroff(COLOR_PAIR(4));
                            }
                            else if ((user -> map_screen_char)[user -> current_floor][i][j] == '3') {
                                mvprintw(START + i, START + j, "%s", "\U0001F3F9");
                            }
                            else if ((user -> map_screen_char)[user -> current_floor][i][j] == '4') {
                                mvprintw(START + i, START + j, "%s", "\U0001F52A");
                            }
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == '$') {
                            attron(COLOR_PAIR(4));
                            mvprintw(START + i, START + j, "%s", "\U00002735");
                            attroff(COLOR_PAIR(4));
                        }
                        else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'G') {
                            attron(COLOR_PAIR(2));
                            mvprintw(START + i, START + j, "%s", "\U00002742");
                            attron(COLOR_PAIR(2));
                        }
                        else if (j == 0 || ((user -> map_screen_char)[user -> current_floor][i][j - 1] != 'B' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'b' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'j' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'k' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'x' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'w' && isdigit((user -> map_screen_char)[user -> current_floor][i][j - 1]) == 0)){
                            mvaddch(START + i, START + j, (user -> map_screen_char)[user -> current_floor][i][j]);
                        }

                        if ((user -> map_screen)[user -> current_floor][i][j] > 0) {
                            attroff(COLOR_PAIR(((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme));
                            if (((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme == 5) {
                                attroff(A_BLINK);
                            }
                        }
                    }
                }
            }
            attron(COLOR_PAIR(user -> color) | A_BLINK);
            mvaddch(user -> current_y + START, user -> current_x + START, gamer);
            attroff(COLOR_PAIR(user -> color) | A_BLINK);
            mvprintw(2, 0, "health");
            for (int i = 0; i < (user -> health); i++) {
                if (user -> health < 20) {
                    attron(COLOR_PAIR(8));
                }
                else {
                    attron(COLOR_PAIR(7));
                }
                mvprintw(2, i + 7, " ");
                if (user -> health < 20) {
                    attroff(COLOR_PAIR(8));
                }
                else {
                    attroff(COLOR_PAIR(7));
                }
            }
            attron(COLOR_PAIR(4));
            mvprintw(3, 0, "gold %d", user -> gold);
            mvprintw(3, 50, "score %d", user -> score);
            attroff(COLOR_PAIR(4));
            refresh();
        }
        if (to > 0) {
            print_screen(user, 0, gamer, 1);
            mvprintw(4, 0, "");
            attron(COLOR_PAIR(2));
            for (int i = 0; i < to; i++) {
                char enemy_name[MAX_SIZE2];
                if (to_enemies[i] == 0) {
                    strcpy(enemy_name, "Deamon");
                }
                else if (to_enemies[i] == 1) {
                    strcpy(enemy_name, "Monster");
                }
                else if (to_enemies[i] == 2) {
                    strcpy(enemy_name, "Giant");
                }
                else if (to_enemies[i] == 3) {
                    strcpy(enemy_name, "Snake");
                }
                else if (to_enemies[i] == 4) {
                    strcpy(enemy_name, "Undeed");
                }

                printw("%d damages to %s", to_damage[i], enemy_name);
                if (explode[i] == 1) {
                    attron(A_BLINK);
                    printw(" ENEMY EXPLODED   ");
                    attroff(A_BLINK);
                }
                else {
                    printw(" CURRENT HEALTH : %d   ", health[i]);
                }
            }
            attroff(COLOR_PAIR(2));
            refresh();
        }
        else if (c == ' ' && flag_c == 0) {
            mvprintw(4, 0, "No Enemies were damaged!");
            refresh();
        }
        if (user -> current_x == user -> end_x && user -> current_y == user -> end_y) {
            end = 1;
            user -> end = 1;
            pthread_join(thread_hunger, NULL);
            pthread_join(thread_health, NULL);
            pthread_join(thread_add_health, NULL);
            //pthread_join(thread_enemy_damage, NULL);
            sleep(1);
            clear();
            refresh();
            (user -> complete_games)++;
            (user -> total_gold) += (user -> gold);
            (user -> total_gold) += ((user -> difficulty) * 50);
            (user -> resume) = 0;
            (user -> total_score) += (user -> gold);
            (user -> total_score) += (user -> score);
            attron(COLOR_PAIR(2) | A_BLINK);
            mvprintw(29, 100, "YOU HAVE WON THE GAME!");
            attroff(COLOR_PAIR(2) | A_BLINK);
            attron(COLOR_PAIR(4));
            mvprintw(30, 103, "golds        %d", user -> gold);
            mvprintw(31, 103, "total golds  %d", user -> total_gold);
            mvprintw(33, 103, "score        %d", user -> score);
            mvprintw(34, 103, "total score        %d", user -> total_score);
            //prints
            attroff(COLOR_PAIR(4));
            refresh();
            sleep(5);
            //changes
            if (user -> is_guest == 0) {
                change_info(user, 0);
                copy_info();
            }
            return;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '>') {
            mvprintw(0, 0, "If you want to use staircase to go to the next floor, press '>'!");
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '<') {
            mvprintw(0, 0, "If you want to use staircase to go to the previous floor, press '<'!");
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '-' || (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '!') {
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '?';
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 't') {
            (user -> health)--;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '^';
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '^') {
            (user -> health)--;
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'B') {
            if (user -> food < 5) {
                (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
                attron(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + START, gamer);
                attroff(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + 1 + START, '.');
                (user -> food_menu)[user -> food] = 0;
                (user -> food)++;
                refresh();
            }
        }
        else if (flag_g == 0 && user -> current_x > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == 'B') {
            if (user -> food < 5) {
                (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '.';
                attron(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + START, gamer);
                attroff(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x - 1 + START, '.');
                (user -> food_menu)[user -> food] = 0;
                (user -> food)++;
                refresh();
            }
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'b') {
            if (user -> food < 5) {
                (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
                attron(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + START, gamer);
                attroff(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + 1 + START, '.');
                (user -> food_menu)[user -> food] = 3;
                (user -> food)++;
                refresh();
            }
        }
        else if (flag_g == 0 && user -> current_x > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == 'b') {
            if (user -> food < 5) {
                (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '.';
                attron(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + START, gamer);
                attroff(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x - 1 + START, '.');
                (user -> food_menu)[user -> food] = 3;
                (user -> food)++;
                refresh();
            }
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'j') {
            if (user -> food < 5) {
                (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
                attron(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + START, gamer);
                attroff(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + 1 + START, '.');
                (user -> food_menu)[user -> food] = 1;
                (user -> food)++;
                refresh();
            }
        }
        else if (flag_g == 0 && user -> current_x > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == 'j') {
            if (user -> food < 5) {
                (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '.';
                attron(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + START, gamer);
                attroff(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x - 1 + START, '.');
                (user -> food_menu)[user -> food] = 1;
                (user -> food)++;
                refresh();
            }
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'k') {
            if (user -> food < 5) {
                (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
                attron(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + START, gamer);
                attroff(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + 1 + START, '.');
                (user -> food_menu)[user -> food] = 2;
                (user -> food)++;
                refresh();
            }
        }
        else if (flag_g == 0 && user -> current_x > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == 'k') {
            if (user -> food < 5) {
                (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '.';
                attron(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x + START, gamer);
                attroff(COLOR_PAIR(user -> color) | A_BLINK);
                mvaddch(user -> current_y + START, user -> current_x - 1 + START, '.');
                (user -> food_menu)[user -> food] = 2;
                (user -> food)++;
                refresh();
            }
        }
        else if (flag_g == 0 && isdigit((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x])) {
            attron(COLOR_PAIR(user -> color) | A_BLINK);
            mvaddch(user -> current_y + START, user -> current_x + START, gamer);
            attroff(COLOR_PAIR(user -> color) | A_BLINK);
            mvprintw(0, 0, "WEAPON COLLECTED!");
            mvaddch(user -> current_y + START, user -> current_x + 1 + START, '.');
            int weapon_theme = (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] - '0';
            if ((user -> weapon_type)[user -> current_floor][user -> current_y][user -> current_x] == 1) {
                (user -> weapon_menu)[weapon_theme]++;
            }
            else if (weapon_theme == 1) {
                (user -> weapon_menu)[weapon_theme] += 10;
            }
            else if (weapon_theme == 2) {
                (user -> weapon_menu)[weapon_theme] += 8;
            }
            else if (weapon_theme == 3) {
                (user -> weapon_menu)[weapon_theme] += 20;
            }
            else if (weapon_theme == 4) {
                if ((user -> weapon_menu)[weapon_theme] == 0) {
                    (user -> weapon_menu)[weapon_theme] = 1;
                }
            }
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            //mvprintw(0, 0, "%c", (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x]);
            refresh();
        }
        else if (flag_g == 0 && user -> current_x > 0 && isdigit((user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1])) {
            attron(COLOR_PAIR(user -> color) | A_BLINK);
            mvaddch(user -> current_y + START, user -> current_x + START, gamer);
            attroff(COLOR_PAIR(user -> color) | A_BLINK);
            mvprintw(0, 0, "WEAPON COLLECTED!");
            mvaddch(user -> current_y + START, user -> current_x - 1 + START, '.');
            int weapon_theme = (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] - '0';
            if ((user -> weapon_type)[user -> current_floor][user -> current_y][user -> current_x - 1] == 1) {
                (user -> weapon_menu)[weapon_theme] ++;
            }
            if (weapon_theme == 1) {
                (user -> weapon_menu)[weapon_theme] += 10;
            }
            else if (weapon_theme == 2) {
                (user -> weapon_menu)[weapon_theme] += 8;
            }
            else if (weapon_theme == 3) {
                (user -> weapon_menu)[weapon_theme] += 20;
            }
            else if (weapon_theme == 4) {
                if ((user -> weapon_menu)[weapon_theme] == 0) {
                    (user -> weapon_menu)[weapon_theme] = 1;
                }
            }
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] = '.';
            //mvprintw(0, 0, "%c", (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1]);
            refresh();
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == '$') {
            (user -> gold) += 5;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            mvprintw(0, 0, "5 Golds Collected");
            refresh();
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'G') {
            (user -> gold) += 20;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            mvprintw(0, 0, "20 Golds Collected");
            refresh();
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'H') {
            (user -> enchant_menu)[0]++;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            attron(COLOR_PAIR(3));
            mvprintw(0, 0, "HEALTH ENCHANT COLLECTED!");
            attroff(COLOR_PAIR(3));
            refresh();
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'S') {
            (user -> enchant_menu)[1]++;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            attron(COLOR_PAIR(3));
            mvprintw(0, 0, "SPEED ENCHANT COLLECTED!");
            attroff(COLOR_PAIR(3));
            refresh();
        }
        else if (flag_g == 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] == 'D') {
            (user -> enchant_menu)[2]++;
            (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x] = '.';
            attron(COLOR_PAIR(3));
            mvprintw(0, 0, "HEALTH ENCHANT COLLECTED!");
            attroff(COLOR_PAIR(3));
            refresh();
        }
        int damage2 = enemy_damage(user);
        if (damage2 > 0) {
            print_screen(user, 0, gamer, 1);
            attron(COLOR_PAIR(1));
            mvprintw(0, 0, "%d damage from enemy!", damage2);
            attroff(COLOR_PAIR(1));
            refresh();
            if (to > 0) {
                mvprintw(4, 0, "");
                attron(COLOR_PAIR(2));
                for (int i = 0; i < to; i++) {
                    char enemy_name[MAX_SIZE2];
                    if (to_enemies[i] == 0) {
                        strcpy(enemy_name, "Deamon");
                    }
                    else if (to_enemies[i] == 1) {
                        strcpy(enemy_name, "Monster");
                    }
                    else if (to_enemies[i] == 2) {
                        strcpy(enemy_name, "Giant");
                    }
                    else if (to_enemies[i] == 3) {
                        strcpy(enemy_name, "Snake");
                    }
                    else if (to_enemies[i] == 4) {
                        strcpy(enemy_name, "Undeed");
                    }

                    printw("%d damages to %s", to_damage[i], enemy_name);
                    if (explode[i] == 1) {
                        attron(A_BLINK);
                        printw(" ENEMY EXPLODED   ");
                        attroff(A_BLINK);
                    }
                    else {
                        printw(" CURRENT HEALTH : %d   ", health[i]);
                    }
                }
                attroff(COLOR_PAIR(2));
                refresh();
            }
            else if (c == ' ') {
                mvprintw(4, 0, "No Enemies were damaged!");
                refresh();
            }
        }
        for (int i = 0; i < (user -> rooms_num)[user -> current_floor]; i++) {
            if ((user -> map_rooms)[user -> current_floor][i] -> theme == 6) {
                int food = rand() % 100;
                if (food == 0) {
                    int food_x = ((user -> map_rooms)[user -> current_floor][i] -> ulx) + 1 + rand() % ((user -> map_rooms)[user -> current_floor][i] -> width - 2);
                    int food_y = ((user -> map_rooms)[user -> current_floor][i] -> uly) + 1 + rand() % ((user -> map_rooms)[user -> current_floor][i] -> height - 2);
                    int steps = 100;
                    while (steps > 0 && ((user -> map_screen_char)[user -> current_floor][food_y][food_x] != '.' || (user -> map_screen_char)[user -> current_floor][food_y][food_x+1] != '.' || (user -> map_screen_char)[user -> current_floor][food_y][food_x-1] != '.')) {
                        food_x = ((user -> map_rooms)[user -> current_floor][i] -> ulx) + 1 + rand() % ((user -> map_rooms)[user -> current_floor][i] -> width - 2);
                        food_y = ((user -> map_rooms)[user -> current_floor][i] -> uly) + 1 + rand() % ((user -> map_rooms)[user -> current_floor][i] -> height - 2);
                        steps --;
                    }
                    if (steps > 0) {
                        int food_theme = rand() % 21;
                        if (food_theme >= 0 && food_theme < 10) {
                            (user -> map_screen_char)[user -> current_floor][food_y][food_x] = 'B';
                        }
                        else if (food_theme >= 10 && food_theme < 15) {
                            (user -> map_screen_char)[user -> current_floor][food_y][food_x] = 'b';
                        }
                        else if (food_theme >= 15 && food_theme < 18) {
                            (user -> map_screen_char)[user -> current_floor][food_y][food_x] = 'j';
                        }
                        else {
                            (user -> map_screen_char)[user -> current_floor][food_y][food_x] = 'k';
                        }
                    }
                }
            }
        }
    
    }
    clear();
    mvprintw(29, 104, "LOADING...");
    refresh();
    end = 1;
    user -> end = 1;
    pthread_join(thread_hunger, NULL);
    pthread_join(thread_health, NULL);
    pthread_join(thread_add_health, NULL);
    //pthread_join(thread_enemy_damage, NULL);
    clear();
    if (user -> health <= 0) {
        (user -> total_gold) += (user -> gold);
        (user -> resume) = 0;
        (user -> total_score) += (user -> score);
        attron(COLOR_PAIR(1) | A_BLINK);
        mvprintw(29, 104, "YOU LOST");
        attroff(COLOR_PAIR(1) | A_BLINK);
        attron(COLOR_PAIR(4));
        mvprintw(30, 98, "golds        %d", user -> gold);
        mvprintw(31, 98, "total golds  %d", user -> total_gold);
        mvprintw(33, 98, "score        %d", user -> score);
        mvprintw(34, 98, "total score        %d", user -> total_score);
        //prints
        attroff(COLOR_PAIR(4));
        refresh();
        sleep(3);
    }
    else {
        if (user -> is_guest == 0) {
            mvprintw(29, 91, "Do you want to save the game?(y/n)");
            c = getch();
            if (c == 'y') {
                user -> resume = 1;
            }
            else {
                user -> resume = 0;
            }
            change_info(user, 0);
            copy_info();
        }
    }
    refresh();
    return;
    
}

int move_directly(User *user, int x, int y, int type) {
    if (type == 0 && 
        user -> current_y + y >= 0 && user -> current_y + y < 60 && 
        user -> current_x + x >= 0 && user -> current_x + x < 200 && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != ' ' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '|' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '_' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'O' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'x' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'w' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'q' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'z' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'u' &&
        (user -> current_x + x == 0 || (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x - 1] != 'x') &&
        (user -> current_x + x == 0 || (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x - 1] != 'w')) {
        (user -> current_y) += y;
        (user -> current_x) += x;
        return 1;
    }
    if (type == 1 && 
        user -> current_y + y >= 0 && user -> current_y + y < 60 && 
        user -> current_x + x >= 0 && user -> current_x + x < 200 &&
        (user -> current_x + x == 0 || (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x - 1] != 'x') &&
        (user -> current_x + x == 0 || (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x - 1] != 'w') &&
        ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == '.' || (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == '#')) {
        (user -> current_y) += y;
        (user -> current_x) += x;
        return 1;
    }
    return 0;
}

int move_indirectly(User *user, int x, int y, int type) {
    if (type == 0 && 
        user -> current_y + y >= 0 && user -> current_y + y < 60 && 
        user -> current_x + x >= 0 && user -> current_x + x < 200 && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != ' ' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '|' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '_' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'O' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '#' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '-' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != '!' &&
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'x' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'w' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'q' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'z' && 
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] != 'u' &&
        (user -> current_x + x == 0 || (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x - 1] != 'x') &&
        (user -> current_x + x == 0 || (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x - 1] != 'w')) {
        (user -> current_y) += y;
        (user -> current_x) += x;
        return 1;
    }
    if (type == 1 && 
        user -> current_y + y >= 0 && user -> current_y + y < 60 && 
        user -> current_x + x >= 0 && user -> current_x + x < 200 && 
        (user -> current_x + x == 0 || (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x - 1] != 'x') &&
        (user -> current_x + x == 0 || (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x - 1] != 'w') &&
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == '.') {
        (user -> current_y) += y;
        (user -> current_x) += x;
        return 1;
    }
    return 0;
}

void print_screen(User *user, int flag_stair, char gamer, int sp) {
    if ((user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x] > 0) {
        Room *current_room = (user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][user -> current_y][user -> current_x] - 1];
        for (int i = current_room -> uly; i < current_room -> uly + current_room -> height; i++) {
            for (int j = current_room -> ulx; j < current_room -> ulx + current_room -> width; j++) {
                (user -> visible)[user -> current_floor][i][j] = 1;
            }
        }
    }
    (user -> visible)[user -> current_floor][user -> current_y][user -> current_x] = 1;
    clear();
    if (flag_stair) {
        mvprintw(0, 0, "New Floor!");
        refresh();
    }
    for (int i = 0; i < 60; i++) {
        for (int j = 0; j < 200; j++) {
            int flag1 = 0;
            if ((user -> map_screen)[user -> current_floor][i][j] < 0) {
                if (user -> current_y == i) {
                    if (j > user -> current_x && j <= user -> current_x + 5) {
                        int flag2 = 1;
                        for (int k = user -> current_x + 1; k < j; k++) {
                            if ((user -> map_screen)[user -> current_floor][i][k] > -1) {
                                flag2 = 0;
                            }
                        }
                        if (flag2) {
                            flag1 = 1;
                        }
                    }
                    if (j < user -> current_x && j >= user -> current_x - 5) {
                        int flag2 = 1;
                        for (int k = user -> current_x - 1; k > j; k--) {
                            if ((user -> map_screen)[user -> current_floor][i][k] > -1) {
                                flag2 = 0;
                            }
                        }
                        if (flag2) {
                            flag1 = 1;
                        }
                    }
                }
                if (user -> current_x == j) {
                    if (i > user -> current_y && i <= user -> current_y + 5) {
                        int flag2 = 1;
                        for (int k = user -> current_y + 1; k < i; k++) {
                            if ((user -> map_screen)[user -> current_floor][k][j] > -1) {
                                flag2 = 0;
                            }
                        }
                        if (flag2) {
                            flag1 = 1;
                        }
                    }
                    if (i < user -> current_y && i >= user -> current_y - 5) {
                        int flag2 = 1;
                        for (int k = user -> current_y - 1; k > i; k--) {
                            if ((user -> map_screen)[user -> current_floor][k][j] > -1) {
                                flag2 = 0;
                            }
                        }
                        if (flag2) {
                            flag1 = 1;
                        }
                    }
                }
            }
            if ((user -> visible)[user -> current_floor][i][j]) {
                flag1 = 1;
            }
            if (flag1 || user -> visible_mode) {
                if ((user -> map_screen)[user -> current_floor][i][j] > 0) {
                    attron(COLOR_PAIR(((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme));
                    if (((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme == 5) {
                        attron(A_BLINK);
                    }
                }
                if ((user -> map_screen_char)[user -> current_floor][i][j] == '-') {
                    mvaddch(START + i, START + j, '_');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == '!') {
                    mvaddch(START + i, START + j, '|');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 't') {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'B' && (user -> current_y != i || user -> current_x != j)) {
                    mvprintw(START + i, START + j, "%s", "\U0001F354");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'b' && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'b' && (user -> current_y != i || user -> current_x != j)) {
                    mvprintw(START + i, START + j, "%s", "\U0001F354");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'j' && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'j' && (user -> current_y != i || user -> current_x != j)) {
                    mvprintw(START + i, START + j, "%s", "\U0001F363");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'k' && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'k' && (user -> current_y != i || user -> current_x != j)) {
                    mvprintw(START + i, START + j, "%s", "\U0001F36D");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'x') {
                    mvprintw(START + i, START + j, "%s", "\U0001F921");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'w') {
                    mvprintw(START + i, START + j, "%s", "\U0001F608");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'q') {
                    mvprintw(START + i, START + j, "%s", "\U00002603");
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'z') {
                    attron(COLOR_PAIR(3));
                    mvprintw(START + i, START + j, "%s", "\U000020B0");
                    attroff(COLOR_PAIR(3));
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'u') {
                    mvprintw(START + i, START + j, "%s", "\U00002620");
                }
                else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && user -> current_y == i && user -> current_x == j + 1) {
                    mvaddch(START + i, START + j, '.');
                }
                else if (isdigit((user -> map_screen_char)[user -> current_floor][i][j]) && (user -> current_y != i || user -> current_x != j)) {
                    if ((user -> map_screen_char)[user -> current_floor][i][j] == '0') {
                        mvprintw(START + i, START + j, "%s", "\U0001F528");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '1') {
                        mvprintw(START + i, START + j, "%s", "\U0001F5E1");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '2') {
                        attron(COLOR_PAIR(4));
                        mvprintw(START + i, START + j, "%s", "\U0001327B");
                        attroff(COLOR_PAIR(4));
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '3') {
                        mvprintw(START + i, START + j, "%s", "\U0001F3F9");
                    }
                    else if ((user -> map_screen_char)[user -> current_floor][i][j] == '4') {
                        mvprintw(START + i, START + j, "%s", "\U0001F52A");
                    }
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == '$') {
                    attron(COLOR_PAIR(4));
                    mvprintw(START + i, START + j, "%s", "\U00002735");
                    attroff(COLOR_PAIR(4));
                }
                else if ((user -> map_screen_char)[user -> current_floor][i][j] == 'G') {
                    attron(COLOR_PAIR(2));
                    mvprintw(START + i, START + j, "%s", "\U00002742");
                    attron(COLOR_PAIR(2));
                }
                else if (j == 0 || ((user -> map_screen_char)[user -> current_floor][i][j - 1] != 'B' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'b' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'j' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'k' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'x' && (user -> map_screen_char)[user -> current_floor][i][j - 1] != 'w' && isdigit((user -> map_screen_char)[user -> current_floor][i][j - 1]) == 0)){
                    mvaddch(START + i, START + j, (user -> map_screen_char)[user -> current_floor][i][j]);
                }
                if ((user -> map_screen)[user -> current_floor][i][j] > 0) {
                    attroff(COLOR_PAIR(((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme));
                    if (((user -> map_rooms)[user -> current_floor][(user -> map_screen)[user -> current_floor][i][j] - 1]) -> theme == 5) {
                        attroff(A_BLINK);
                    }
                }
            }
        }
    }
    attron(COLOR_PAIR(user -> color) | A_BLINK);
    mvaddch(user -> current_y + START, user -> current_x + START, gamer);
    attroff(COLOR_PAIR(user -> color) | A_BLINK);
    mvprintw(2, 0, "health");
    for (int i = 0; i < (user -> health); i++) {
        if (user -> health < 20) {
            attron(COLOR_PAIR(8));
        }
        else {
            attron(COLOR_PAIR(7));
        }
        mvprintw(2, i + 7, " ");
        if (user -> health < 20) {
            attroff(COLOR_PAIR(8));
        }
        else {
            attroff(COLOR_PAIR(7));
        }
    }
    attron(COLOR_PAIR(4));
    mvprintw(3, 0, "gold %d", user -> gold);
    mvprintw(3, 50, "score %d", user -> score);
    attroff(COLOR_PAIR(4));
    refresh();
    usleep(20000 * sp);
}

void *hunger_rate(void *arguments) {
    User *args = (User *) arguments;
    while (!(args -> end)) {
        for (int i = 0; i < 8 +(3 - args -> difficulty) * 5; i++) {
            if (args -> end) {
                return NULL;
            }
            sleep(1);
        }
        (args -> hunger)--;
        if ((args -> hunger) < 0) {
            (args -> hunger) = 0;
        }
    }
    return NULL;
}

int enemy_damage(User *user) {
    int damage = 0;
    if (user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == 'x') {
        (user -> health) -= 2;
        damage += 2;
    }
    if (user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == 'x') {
        (user -> health) -= 2;
        damage += 2;
    }
    if (user -> current_x < 199 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == 'x') {
        (user -> health) -= 2;
        damage += 2;
    }
    if (user -> current_x > 1 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 2] == 'x') {
        (user -> health) -= 2;
        damage += 2;
    }
    if (user -> current_x < 199 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == 'x') {
        (user -> health) -= 2;
        damage += 2;
    }
    if (user -> current_x < 199 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == 'x') {
        (user -> health) -= 2;
        damage += 2;
    }
    if (user -> current_x > 0 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == 'x') {
        (user -> health) -= 2;
        damage += 2;
    }
    if (user -> current_x > 0 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == 'x') {
        (user -> health) -= 2;
        damage += 2;
    }
    if (user -> current_x > 1 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 2] == 'x') {
        (user -> health) -= 2;
        damage += 2;
    }
    if (user -> current_x > 1 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 2] == 'x') {
        (user -> health) -= 2;
        damage += 2;
    }


    if (user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == 'w') {
        (user -> health) -= 4;
        damage += 4;
    }
    if (user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == 'w') {
        (user -> health) -= 4;
        damage += 4;
    }
    if (user -> current_x < 199 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == 'w') {
        (user -> health) -= 4;
        damage += 4;
    }
    if (user -> current_x > 1 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 2] == 'w') {
        (user -> health) -= 4;
        damage += 4;
    }
    if (user -> current_x < 199 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == 'w') {
        (user -> health) -= 4;
        damage += 4;
    }
    if (user -> current_x < 199 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == 'w') {
        (user -> health) -= 4;
        damage += 4;
    }
    if (user -> current_x > 0 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == 'w') {
        (user -> health) -= 4;
        damage += 4;
    }
    if (user -> current_x > 0 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == 'w') {
        (user -> health) -= 4;
        damage += 4;
    }
    if (user -> current_x > 1 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 2] == 'w') {
        (user -> health) -= 4;
        damage += 4;
    }
    if (user -> current_x > 1 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 2] == 'w') {
        (user -> health) -= 4;
        damage += 4;
    }

    if (user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == 'q') {
        (user -> health) -= 6;
        damage += 6;
    }
    if (user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == 'q') {
        (user -> health) -= 6;
        damage += 6;
    }
    if (user -> current_x < 199 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == 'q') {
        (user -> health) -= 6;
        damage += 6;
    }
    if (user -> current_x > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == 'q') {
        (user -> health) -= 6;
        damage += 6;
    }
    if (user -> current_x > 0 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == 'q') {
        (user -> health) -= 6;
        damage += 6;
    }
    if (user -> current_x > 0 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == 'q') {
        (user -> health) -= 6;
        damage += 6;
    }
    if (user -> current_x < 199 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == 'q') {
        (user -> health) -= 6;
        damage += 6;
    }
    if (user -> current_x > 199 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == 'q') {
        (user -> health) -= 6;
        damage += 6;
    }


    if (user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == 'z') {
        (user -> health) -= 8;
        damage += 8;
    }
    if (user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == 'z') {
        (user -> health) -= 8;
        damage += 8;
    }
    if (user -> current_x < 199 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == 'z') {
        (user -> health) -= 8;
        damage += 8;
    }
    if (user -> current_x > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == 'z') {
        (user -> health) -= 8;
        damage += 8;
    }
    if (user -> current_x > 0 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == 'z') {
        (user -> health) -= 8;
        damage += 8;
    }
    if (user -> current_x > 0 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == 'z') {
        (user -> health) -= 8;
        damage += 8;
    }
    if (user -> current_x < 199 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == 'z') {
        (user -> health) -= 8;
        damage += 8;
    }
    if (user -> current_x > 199 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == 'z') {
        (user -> health) -= 8;
        damage += 8;
    }


    if (user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x] == 'u') {
        (user -> health) -= 12;
        damage += 12;
    }
    if (user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x] == 'u') {
        (user -> health) -= 12;
        damage += 12;
    }
    if (user -> current_x < 199 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x + 1] == 'u') {
        (user -> health) -= 12;
        damage += 12;
    }
    if (user -> current_x > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y][user -> current_x - 1] == 'u') {
        (user -> health) -= 12;
        damage += 12;
    }
    if (user -> current_x > 0 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x - 1] == 'u') {
        (user -> health) -= 12;
        damage += 12;
    }
    if (user -> current_x > 0 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x - 1] == 'u') {
        (user -> health) -= 12;
        damage += 12;
    }
    if (user -> current_x < 199 && user -> current_y > 0 && (user -> map_screen_char)[user -> current_floor][user -> current_y - 1][user -> current_x + 1] == 'u') {
        (user -> health) -= 12;
        damage += 12;
    }
    if (user -> current_x > 199 && user -> current_y < 59 && (user -> map_screen_char)[user -> current_floor][user -> current_y + 1][user -> current_x + 1] == 'u') {
        (user -> health) -= 12;
        damage += 12;
    }
    return damage;
}

void *add_health(void *arguments) {
    User *args = (User *) arguments;
    while (!(args -> end)) {
        for (int i = 0; i < (5 + (args -> difficulty) * 2)/(args -> rate); i++) {
            if (args -> end) {
                return NULL;
            }
            sleep(1);
        }
        if ((args -> hunger) == 14 + (3 - (args -> difficulty)) * 3) {
            (args -> health)++;
            if ((args -> health) > 50 + (3 - (args -> difficulty)) * 25) {
                (args -> health) = 50 + (3 - (args -> difficulty)) * 25;
            }
        }
    }
    return NULL;
}

void *health_rate(void *arguments) {
    User *user = (User *) arguments;
    while (!(user -> end)) {
        for (int i = 0; i <  8 + (3 - (user -> difficulty)); i++) {
            if (user -> end) {
                return NULL;
            }
            sleep(1);
        }
        if ((user -> hunger) <= 5) {
            (user -> health)--;
        }
    }
    return NULL;
}

void change_info(User *user, int type) {
    FILE *users;
    users = fopen("users.txt", "r");
    FILE *users_copy;
    users_copy = fopen("users_copy.txt", "w");
    fclose(users_copy);
    users_copy = fopen("users_copy.txt", "a");
    char line[MAX_SIZE];
    while (fgets(line, MAX_SIZE, users)) {
        if (strcmp(line, "{\n") == 0) {
            fgets(line, MAX_SIZE, users);
            line[strlen(line)-1] = '\0';
            fprintf(users_copy, "{\n%s\n", line);
            if (strcmp(user -> username, line) == 0) {
                fgets(line, MAX_SIZE, users); //password
                fprintf(users_copy, "%s", line);
                fgets(line, MAX_SIZE, users); // email
                fprintf(users_copy, "%s", line);
                fgets(line, MAX_SIZE, users); //total gold
                fprintf(users_copy, "%d\n", user -> total_gold);
                fgets(line, MAX_SIZE, users); //total score
                fprintf(users_copy, "%d\n", user -> total_score);
                fgets(line, MAX_SIZE, users); //difficulty
                fprintf(users_copy, "%d\n", user -> difficulty);
                fgets(line, MAX_SIZE, users); //color
                fprintf(users_copy, "%d\n", user -> color);
                fgets(line, MAX_SIZE, users); //games
                fprintf(users_copy, "%d\n", user -> games);
                fgets(line, MAX_SIZE, users); //complete games
                fprintf(users_copy, "%d\n", user -> complete_games);
                fgets(line, MAX_SIZE, users); //resume game
                fprintf(users_copy, "%d\n", user -> resume);
                if (user -> resume) {
                    int i = 0;
                    for (i = 0; i < 4; i++) {
                        fprintf(users_copy, "%d ", (user -> rooms_num)[i]);
                    }
                    fprintf(users_copy, "\n");
                    //map_rooms
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < (user -> rooms_num)[f]; i++) {
                            //ulx
                            fprintf(users_copy, "%d\n", (user -> map_rooms)[f][i] -> ulx);
                            //uly
                            fprintf(users_copy, "%d\n", (user -> map_rooms)[f][i] -> uly);
                            //height
                            fprintf(users_copy, "%d\n", (user -> map_rooms)[f][i] -> height);
                            //width
                            fprintf(users_copy, "%d\n", (user -> map_rooms)[f][i] -> width);
                            //theme
                            fprintf(users_copy, "%d\n", (user -> map_rooms)[f][i] -> theme);
                            if (type) {
                                free((user -> map_rooms)[f][i]);
                            }
                        }
                    }

                    //map_screen
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            for (int j = 0; j < 200; j++) {
                                fprintf(users_copy, "%d ", (user -> map_screen)[f][i][j]);
                            }
                            fprintf(users_copy, "\n");
                        }
                    }

                    //map_screen_char
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            for (int j = 0; j < 200; j++) {
                                fprintf(users_copy, "%c", (user -> map_screen_char)[f][i][j]);
                            }
                            fprintf(users_copy, "\n");
                        }
                    }

                    //weapon type
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            for (int j = 0; j < 200; j++) {
                                fprintf(users_copy, "%d ", (user -> weapon_type)[f][i][j]);
                            }
                            fprintf(users_copy, "\n");
                        }
                    }

                    //in_staircase 1 2 3
                    for (i = 1; i < 4; i++) {
                        fprintf(users_copy, "%d %d ", (user -> in_staircase)[i] -> x, (user -> in_staircase)[i] -> y);
                        if (type)
                            free((user -> in_staircase)[i]);
                    }
                    fprintf(users_copy, "\n");
                    //out_staircases 0 1 2
                    for (i = 0; i < 3; i++) {
                        fprintf(users_copy, "%d %d ", (user -> out_staircase)[i] -> x, (user -> out_staircase)[i] -> y);
                        if (type)
                            free((user -> out_staircase)[i]);
                    }
                    fprintf(users_copy, "\n");

                    //visible
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            for (int j = 0; j < 200; j++) {
                                fprintf(users_copy, "%d ", (user -> visible)[f][i][j]);
                            }
                            fprintf(users_copy, "\n");
                        }
                    }

                    //end_x
                    fprintf(users_copy, "%d\n", user -> end_x);

                    //end_y
                    fprintf(users_copy, "%d\n", user -> end_y);

                    //current_floor
                    fprintf(users_copy, "%d\n", user -> current_floor);

                    //current_x
                    fprintf(users_copy, "%d\n", user -> current_x);

                    //current_y
                    fprintf(users_copy, "%d\n", user -> current_y);

                    //health
                    fprintf(users_copy, "%d\n", user -> health);

                    //gold
                    fprintf(users_copy, "%d\n", user -> gold);

                    //score
                    fprintf(users_copy, "%d\n", user -> score);

                    //power
                    fprintf(users_copy, "%d\n", user -> power);

                    //current weapon
                    fprintf(users_copy, "%d\n", user -> current_weapon);

                    //enemy health
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            for (int j = 0; j < 200; j++) {
                                fprintf(users_copy, "%d ", (user -> enemy_health)[f][i][j]);
                            }
                            fprintf(users_copy, "\n");
                        }
                    }

                    //enemy move
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            for (int j = 0; j < 200; j++) {
                                fprintf(users_copy, "%d ", (user -> enemy_move)[f][i][j]);
                            }
                            fprintf(users_copy, "\n");
                        }
                    }

                    //food
                    fprintf(users_copy, "%d\n", user -> food);

                    //food_menu
                    for (i = 0; i < (user -> food); i++) {
                        fprintf(users_copy, "%d ", (user -> food_menu)[i]);
                    }
                    fprintf(users_copy, "\n");

                    //enchant menu
                    for (i = 0; i < 3; i++) {
                        fprintf(users_copy, "%d ", (user -> enchant_menu)[i]);
                    }
                    fprintf(users_copy, "\n");
                    //weapon menu
                    for (i = 0; i < 5; i++) {
                        fprintf(users_copy, "%d ", (user -> weapon_menu)[i]);
                    }
                    fprintf(users_copy, "\n");

                    //visible mode
                    fprintf(users_copy, "%d\n", user -> visible_mode);

                    //hunger
                    fprintf(users_copy, "%d\n", user -> hunger);


                }
                fprintf(users_copy, "}\n");
            }
            else {
                while (fgets(line, MAX_SIZE, users)) {
                    fprintf(users_copy, "%s", line);
                    if (strcmp(line, "}\n") == 0) {
                        break;
                    }
                }
            }
        }
    }
    fclose(users);
    fclose(users_copy);
}

void copy_info() {
    FILE *users_copy;
    FILE *users;
    users = fopen("users.txt", "w");
    fclose(users);
    users_copy = fopen("users_copy.txt", "r");
    users = fopen("users.txt", "a");
    char line[MAX_SIZE];
    while (fgets(line, MAX_SIZE, users_copy)) {
        fprintf(users, "%s", line);
    }
    fclose(users_copy);
    fclose(users);
}

void user_short_damage(User *user, int power,int *enemy, int *explode, int *damage, int *health, int *num,int x, int y) {
    if (user -> current_y + y >= 0 && user -> current_y + y < 60 &&
        user -> current_x + x >= 0 && user -> current_x + x < 200) {
            if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'x') {
                (*(enemy + (*num))) = 0;
            }
            else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'w') {
                (*(enemy + (*num))) = 1;
            }
            else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'q') {
                (*(enemy + (*num))) = 2;
            }
            else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'z') {
                (*(enemy + (*num))) = 3;
            }
            else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'u') {
                (*(enemy + (*num))) = 4;
            }
            else {
                return;
            }
    }
    else {
        return;
    }
    (*(explode + (*num))) = 0;
    if ((user -> enemy_health)[user -> current_floor][user -> current_y + y][user -> current_x + x] <= power) {
        (*(damage + (*num))) = (user -> enemy_health)[user -> current_floor][user -> current_y + y][user -> current_x + x];
        (user -> enemy_health)[user -> current_floor][user -> current_y + y][user -> current_x + x] = 0;
        if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'x') {
            (user -> score) += 100;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'w') {
            (user -> score) += 200;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'q') {
            (user -> score) += 300;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'z') {
            (user -> score) += 400;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'u') {
            (user -> score) += 500;
        }
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] = '.';
        (*(explode + (*num))) = 1;
        (*(health + (*num))) = 0;
    }
    else {
        (*(damage + (*num))) = power;
        (user -> enemy_health)[user -> current_floor][user -> current_y + y][user -> current_x + x] -= power;
        (*(health + (*num))) = (user -> enemy_health)[user -> current_floor][user -> current_y + y][user -> current_x + x];
    }
    (*num)++;
}

void user_short_damage2(User *user, int power,int *enemy, int *explode, int *damage, int *health, int *num,int x, int y) {
    if (user -> current_y + y >= 0 && user -> current_y + y < 60 &&
        user -> current_x + x >= 0 && user -> current_x + x < 200) {
            if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'x') {
                (*(enemy + (*num))) = 0;

            }
            else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'w') {
                (*(enemy + (*num))) = 1;
            }
            else {
                return;
            }
    }
    else {
        return;
    }
    (*(explode + (*num))) = 0;
    if ((user -> enemy_health)[user -> current_floor][user -> current_y + y][user -> current_x + x] <= power) {
        (*(damage + (*num))) = (user -> enemy_health)[user -> current_floor][user -> current_y + y][user -> current_x + x];
        (user -> enemy_health)[user -> current_floor][user -> current_y + y][user -> current_x + x] = 0;
        if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'x') {
            (user -> score) += 100;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] == 'w') {
            (user -> score) += 200;
        }
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y][user -> current_x + x] = '.';
        (*(explode + (*num))) = 1;
        (*(health + (*num))) = 0;
    }
    else {
        (*(damage + (*num))) = power;
        (user -> enemy_health)[user -> current_floor][user -> current_y + y][user -> current_x + x] -= power;
        (*(health + (*num))) = (user -> enemy_health)[user -> current_floor][user -> current_y + y][user -> current_x + x];
    }
    (*num)++;
}

void user_long_damage(User *user, int distance, int power, int *enemy, int *explode, int *damage, int *health, int *num,int x, int y) {
    for (int i = 1; i <= distance; i++) {
        (*(enemy + (*num))) = -1;
        if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] == 'x') {
            (*(enemy + (*num))) = 0;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] == 'w') {
            (*(enemy + (*num))) = 1;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] == 'q') {
            (*(enemy + (*num))) = 2;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] == 'z') {
            (*(enemy + (*num))) = 3;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] == 'u') {
            (*(enemy + (*num))) = 4;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i - 1] == 'x') {
            (*(enemy + (*num))) = 5;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i - 1] == 'w') {
            (*(enemy + (*num))) = 6;
        }
        else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] != '.') {
            if (i > 1) {
                (user -> weapon_menu)[user -> current_weapon]--;
                //putting one arrow
                (user -> map_screen_char)[user -> current_floor][user -> current_y + y * (i-1)][user -> current_x + x * (i-1)] = (user -> current_weapon) + '0';
                (user -> weapon_type)[user -> current_floor][user -> current_y + y * (i-1)][user -> current_x + x * (i-1)] = 1;
            }
            return;
        }
        if ((*(enemy + (*num))) >= 0) {
            (user -> weapon_menu)[user -> current_weapon]--;
            (*(explode + (*num))) = 0;
            if ((user -> enemy_health)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] <= power) {
                (*(damage + (*num))) = (user -> enemy_health)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i];
                (user -> enemy_health)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] = 0;
                if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] == 'x' || (user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i - 1] == 'x') {
                    (user -> score) += 100;
                }
                else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] == 'w' || (user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i - 1] == 'w') {
                    (user -> score) += 200;
                }
                else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] == 'q') {
                    (user -> score) += 300;
                }
                else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] == 'z') {
                    (user -> score) += 400;
                }
                else if ((user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] == 'u') {
                    (user -> score) += 500;
                }
                if ((*(enemy + (*num))) < 5) {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] = '.';
                }
                else {
                    (user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i - 1] = '.';
                }
                (*(explode + (*num))) = 1;
                (*(health + (*num))) = 0;
            }
            else {
                (*(damage + (*num))) = power;
                if ((*(enemy + (*num))) < 5) {
                    (user -> enemy_health)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] -= power;
                    (*(health + (*num))) = (user -> enemy_health)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i];
                }
                else {
                    (user -> enemy_health)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i - 1] -= power;
                    (*(health + (*num))) = (user -> enemy_health)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i - 1];
                }
            }
            (*num)++;
            return;
        }
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] = user -> current_weapon + '0';
        print_screen(user, 0, toupper((user -> username)[0]), 4);
        (user -> map_screen_char)[user -> current_floor][user -> current_y + y * i][user -> current_x + x * i] = '.';
        
    }
    (user -> weapon_menu)[user -> current_weapon]--;
}

#endif