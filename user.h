#ifndef USER_H
#define USER_H

#include "room.h"

#define MAX_SIZE 1000


typedef struct {
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char email[MAX_SIZE];
    int rooms_num[4];
    Room *map_rooms[4][10];
    int map_screen[4][60][200];
    char map_screen_char[4][60][200];
    Point *in_staircase[4];
    Point *out_staircase[4];
    int visible[4][60][200];
    int enemy_health[4][60][200];
    int enemy_move[4][60][200];
    int weapon_type[4][60][200];
    int end_x;
    int end_y;
    int end;
    int rate;
    int speed;
    int current_floor;
    int current_x;
    int current_y;
    int health;
    int total_gold;
    int gold;
    int food;
    int food_menu[5];
    int enchant_menu[3];
    int weapon_menu[5];
    int visible_mode;
    int hunger;
    int difficulty;
    int color;
    int score;
    int total_score;
    int games;
    int complete_games;
    int resume;
    int is_guest;
    int current_weapon;
    int power;
} User;

#endif