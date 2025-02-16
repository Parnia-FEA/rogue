#ifndef SETTING_H
#define SETTING_H
#include <curses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "difficulty.h"
#include "color.h"
#include "music.h"

void show_settings(User *user) {
	clear();
    ITEM **items;			
	MENU *my_menu;
	WINDOW *menu_win;
    int c, i;
    char *choices[] = {"Difficulty", "Color", "Music"};
    char *choices_numbers[] = {" ", " ", " "};
    void (*func[3]) (User *) = {change_difficulty, change_color, change_music};
    items = (ITEM **)calloc(4, sizeof(ITEM *));
    for(i = 0; i < 3; ++i) {       
        items[i] = new_item(choices_numbers[i], choices[i]);
        set_item_userptr(items[i], func[i]);
	}
	items[3] = (ITEM *)NULL;
	my_menu = new_menu((ITEM **)items);
	menu_win = newwin(3, 40, 32, 100);
	keypad(menu_win, TRUE);
	set_menu_win(my_menu, menu_win);
	set_menu_sub(my_menu, derwin(menu_win, 3, 40, 0, 0));
	mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
	mvprintw(LINES - 2, 0, "Up and Down arrow keys to navigate (F1 to Log Out)");
	attron(COLOR_PAIR(2) | A_BLINK);
	mvprintw(29, 104, "SETTINGS!");
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
				for(i = 0; i < 3; ++i)
					free_item(items[i]);
				free_menu(my_menu);
				wrefresh(menu_win);
				(*p)(user);
                show_settings(user);
                return;
			}
		}
	}	
	unpost_menu(my_menu);
	for(i = 0; i < 3; ++i)
		free_item(items[i]);
	free_menu(my_menu);
}

#endif