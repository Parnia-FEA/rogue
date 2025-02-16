#include <curses.h>
#include <form.h>
#include <menu.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "welcome.h"

#define MAX_SIZE 1000
#define MAX_SIZE2 50
#define LENGTH 214
#define WIDTH 66
#define COLOR_GOLD 10
#define COLOR_SILVER 11
#define COLOR_BRONZE 12


int main() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
	noecho();
    keypad(stdscr, TRUE);
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_GREEN, COLOR_GREEN);
    init_pair(8, COLOR_RED, COLOR_RED);
    init_pair(9, COLOR_MAGENTA, COLOR_BLACK);
    init_color(COLOR_GOLD, 255, 215, 0);
    init_color(COLOR_SILVER, 192, 192, 192);
    init_color(COLOR_BRONZE, 205, 127, 50);
    init_pair(10, COLOR_BLACK, COLOR_GOLD);
    init_pair(11, COLOR_BLACK, COLOR_SILVER);
    init_pair(12, COLOR_BLACK, COLOR_BRONZE);
    
    welcome_menu();
    endwin();
    return 0;
}