#ifndef COLOR_H
#define COLOR_H
#include <curses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"

void cyan(User *);
void red(User *);
void green(User *);
void yellow(User *);
void magenta(User *);

void change_color(User *user) {
	clear();
    ITEM **items;			
	MENU *my_menu;
	WINDOW *menu_win;
    int c, i;
    char *choices[] = {"Cyan", "Red", "Green", "Yellow", "Magenta"};
    char choices_numbers[5][2];
    strcpy(choices_numbers[0], " ");
    strcpy(choices_numbers[1], " ");
    strcpy(choices_numbers[2], " ");
    strcpy(choices_numbers[3], " ");
    strcpy(choices_numbers[4], " ");
    if (user -> color == 2) {
        strcpy(choices_numbers[0], "*");
    }
    else if (user -> color == 1) {
        strcpy(choices_numbers[1], "*");
    }
    else if (user -> color == 3) {
        strcpy(choices_numbers[2], "*");
    }
    else if (user -> color == 4) {
        strcpy(choices_numbers[3], "*");
    }
    else if (user -> color == 9) {
        strcpy(choices_numbers[4], "*");
    }
    void (*func[5]) (User *) = {cyan, red, green, yellow, magenta};
    items = (ITEM **)calloc(6, sizeof(ITEM *));
    for(i = 0; i < 5; ++i) {       
        items[i] = new_item(choices_numbers[i], choices[i]);
        set_item_userptr(items[i], func[i]);
	}
	items[5] = (ITEM *)NULL;
	my_menu = new_menu((ITEM **)items);
	menu_win = newwin(5, 40, 32, 100);
	keypad(menu_win, TRUE);
	set_menu_win(my_menu, menu_win);
	set_menu_sub(my_menu, derwin(menu_win, 5, 40, 0, 0));
	mvprintw(LINES - 3, 0, "Press <ENTER> to change the color");
	mvprintw(LINES - 2, 0, "Up and Down arrow keys to navigate (F1 to Log Out)");
	attron(COLOR_PAIR(2) | A_BLINK);
	mvprintw(29, 104, "COLORS");
	attroff(COLOR_PAIR(2) | A_BLINK);
	refresh();
	post_menu(my_menu);
	wrefresh(menu_win);

	while((c = wgetch(menu_win)) != KEY_F(1)) {       
        switch(c) {	
            case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case 10:
			{
                ITEM *cur_item;
				void (*p)(User *);
				cur_item = current_item(my_menu);
				p = (void (*) (User *)) item_userptr(cur_item);
				pos_menu_cursor(my_menu);
				unpost_menu(my_menu);
				for(i = 0; i < 5; ++i)
					free_item(items[i]);
				free_menu(my_menu);
				wrefresh(menu_win);
				(*p)(user);
				return;
			}
		}
	}	
	unpost_menu(my_menu);
	for(i = 0; i < 5; ++i)
		free_item(items[i]);
	free_menu(my_menu);
}

void cyan(User *user) {
    user -> color = 2;
}
void red(User *user) {
    user -> color = 1;
}
void green(User *user) {
    user -> color = 3;
}
void yellow(User *user) {
    user -> color = 4;
}
void magenta(User *user) {
    user -> color = 9;
}

#endif