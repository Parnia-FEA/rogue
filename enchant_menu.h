#ifndef ENCHANT_MENU_H
#define ENCHANT_MENU_H
#include <curses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include "create_new_game.h"
#include "scoreboard.h"
#include "setting.h"
#include "user.h"

#define MAX_SIZE 1000

void health_enchant(User *, int);
void speed_enchant(User *, int);
void damage_enchant(User *, int);
void *thread_damage_func(void *);
void *thread_speed_func(void *);
void *thread_health_enchant_func(void *);

void enchant_menu_func(User *user) {
	clear();
    int enchant = (user -> enchant_menu)[0] +(user -> enchant_menu)[1] + (user -> enchant_menu)[2];
    if (enchant  == 0) {
        attron(COLOR_PAIR(2) | A_BLINK);
        mvprintw(32, 100, "NO ENCHANT!");
        attroff(COLOR_PAIR(2) | A_BLINK);
        sleep(3);
        return;
    }
    ITEM **items;			
	MENU *my_menu;
	WINDOW *menu_win;
    int c;
    char choices[3][100];
    char *choices_numbers[3] = {"Health", "Speed", "Damage"};
    void (*func[3]) (User *, int) = {health_enchant, speed_enchant, damage_enchant};
    sprintf(choices[0], "%d", (user -> enchant_menu)[0]);
    sprintf(choices[1], "%d", (user -> enchant_menu)[1]);
    sprintf(choices[2], "%d", (user -> enchant_menu)[2]);
    items = (ITEM **)calloc(4, sizeof(ITEM *));
    for(int i = 0; i < 3; i++) {       
        items[i] = new_item(choices_numbers[i], choices[i]);
        set_item_userptr(items[i], func[i]);
	}
	items[3] = (ITEM *)NULL;
	my_menu = new_menu((ITEM **)items);
	menu_win = newwin(3, 40, 32, 100);
	keypad(menu_win, TRUE);
	set_menu_win(my_menu, menu_win);
	set_menu_sub(my_menu, derwin(menu_win, 3, 40, 0, 0));
	mvprintw(LINES - 2, 0, "Press <ENTER> to select the enchant");
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
				void (*p)(User *, int);
				cur_item = current_item(my_menu);
				p = (void (*) (User *, int)) item_userptr(cur_item);
				pos_menu_cursor(my_menu);
				unpost_menu(my_menu);
				for(int i = 0; i < 3; ++i)
					free_item(items[i]);
				free_menu(my_menu);
				wrefresh(menu_win);
				(*p)(user, 1);
				return;
			}
		}
	}	
	unpost_menu(my_menu);
	for(int i = 0; i < 3; i++)
		free_item(items[i]);
	free_menu(my_menu);
}

void health_enchant(User * user, int type) {
	if (type) 
		(user -> enchant_menu)[0]--;
	(user -> rate) *= 2;
	pthread_t thread_health_enchant;
	pthread_create(&thread_health_enchant, NULL, thread_health_enchant_func, (void *)(user));
}

void speed_enchant(User *user, int type) {
	if (type)
		(user -> enchant_menu)[1]--;
	(user -> speed) *= 2;
	pthread_t thread_speed;
	pthread_create(&thread_speed, NULL, thread_speed_func, (void *)(user));
}

void damage_enchant(User *user, int type) {
	if (type)
		(user -> enchant_menu)[2]--;
	(user -> power) *= 2;
	pthread_t thread_damage;
	pthread_create(&thread_damage, NULL, thread_damage_func, (void *)(user));
}

void *thread_damage_func(void *arg) {
	User *user = (User *)arg;
	sleep(10);
	(user -> power) /= 2;
	return NULL;
}

void *thread_speed_func(void *arg) {
	User *user = (User *)arg;
	sleep(10);
	(user -> speed) /= 2;
	return NULL;
}

void *thread_health_enchant_func(void *arg) {
	User *user = (User *)arg;
	sleep(10);
	(user -> rate) /= 2;
	return NULL;
}

#endif