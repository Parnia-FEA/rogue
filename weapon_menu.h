#ifndef WEAPON_MENU_H
#define WEAPON_MENU_H
#include <curses.h>
#include "user.h"

#define MAX_SIZE 1000

void weapon_menu_func(User *user) {
	clear();
	attron(COLOR_PAIR(2));
	mvprintw(25, 104, "WEAPONS");
	attroff(COLOR_PAIR(2));

	attron(COLOR_PAIR(3));
	mvprintw(27, 102, "Short Range");
	mvprintw(29, 83, "weapon      character      number          power");
	attroff(COLOR_PAIR(3));
	mvprintw(31, 83, "Hammer          h           1                %d", 5 * (user -> power));
	mvprintw(32, 83, "Sword           s           %d               %d", (user -> weapon_menu)[4], 10 * (user -> power));

	attron(COLOR_PAIR(3));
	mvprintw(34, 103, "Long Range");
	mvprintw(36, 73, "weapon      character      number          distance          power");
	attroff(COLOR_PAIR(3));
	mvprintw(38, 73, "Dagger          d           %d                5                %d", (user -> weapon_menu)[1], 12 * (user -> power));
	mvprintw(39, 73, "Wand            m           %d                10               %d", (user -> weapon_menu)[2], 15 * (user -> power));
	mvprintw(40, 73, "Arrow           a           %d                5                %d", (user -> weapon_menu)[3], 5 * (user -> power));
	refresh();
	int c;
	while ((c = getch()) != KEY_F(1)) {
		if (c == (int)'h') {
			if (user -> current_weapon != -1) {
				clear();
				attron(COLOR_PAIR(1));
				mvprintw(31, 77, "First, You Have To Put Your Current Weapon In The BackPack.");
				attroff(COLOR_PAIR(1));
				user -> current_weapon = 0;
				refresh();
				sleep(2);
				return;
			}
			if ((user -> weapon_menu)[0] > 0) {
				clear();
				attron(COLOR_PAIR(2));
				mvprintw(31, 88, "Now, Your Current Weapon Is A Hammer!");
				attroff(COLOR_PAIR(2));
				user -> current_weapon = 0;
				refresh();
				sleep(2);
				return;
			}
		}
		else if (c == (int)'d') {
			if (user -> current_weapon != -1) {
				clear();
				attron(COLOR_PAIR(1));
				mvprintw(31, 77, "First, You Have To Put Your Current Weapon In The BackPack.");
				attroff(COLOR_PAIR(1));
				user -> current_weapon = 1;
				refresh();
				sleep(2);
				return;
			}
			if ((user -> weapon_menu)[1] > 0) {
				clear();
				attron(COLOR_PAIR(2));
				mvprintw(31, 88, "Now, Your Current Weapon Is A Dagger!");
				attroff(COLOR_PAIR(2));
				user -> current_weapon = 1;
				refresh();
				sleep(2);
				return;
			}
			clear();
			attron(COLOR_PAIR(1));
			mvprintw(31, 89, "There Are No Daggers In Backpack!");
			attroff(COLOR_PAIR(1));
			refresh();
			sleep(2);
			weapon_menu_func(user);
			return;
		}
		else if (c == (int)'m') {
			if (user -> current_weapon != -1) {
				clear();
				attron(COLOR_PAIR(1));
				mvprintw(31, 77, "First, You Have To Put Your Current Weapon In The BackPack.");
				attroff(COLOR_PAIR(1));
				refresh();
				user -> current_weapon = 2;
				sleep(2);
				return;
			}
			if ((user -> weapon_menu)[2] > 0) {
				clear();
				attron(COLOR_PAIR(2));
				mvprintw(31, 88, "Now, Your Current Weapon Is A Wand!");
				attroff(COLOR_PAIR(2));
				refresh();
				user -> current_weapon = 2;
				sleep(2);
				return;
			}
			clear();
			attron(COLOR_PAIR(1));
			mvprintw(31, 89, "There Are No Wands In Backpack!");
			attroff(COLOR_PAIR(1));
			refresh();
			sleep(2);
			weapon_menu_func(user);
		}
		else if (c == (int)'a') {
			if (user -> current_weapon != -1) {
				clear();
				attron(COLOR_PAIR(1));
				mvprintw(31, 77, "First, You Have To Put Your Current Weapon In The BackPack.");
				attroff(COLOR_PAIR(1));
				user -> current_weapon = 3;
				refresh();
				sleep(2);
				return;
			}
			if ((user -> weapon_menu)[3] > 0) {
				clear();
				attron(COLOR_PAIR(2));
				mvprintw(31, 88, "Now, Your Current Weapon Is An Arrow!");
				attroff(COLOR_PAIR(2));
				user -> current_weapon = 3;
				refresh();
				sleep(2);
				return;
			}
			clear();
			attron(COLOR_PAIR(1));
			mvprintw(31, 89, "There Are No Arrows In Backpack!");
			attroff(COLOR_PAIR(1));
			refresh();
			sleep(2);
			weapon_menu_func(user);
		}
		else if (c == (int)'s') {
			if (user -> current_weapon != -1) {
				clear();
				attron(COLOR_PAIR(1));
				mvprintw(31, 77, "First, You Have To Put Your Current Weapon In The BackPack.");
				attroff(COLOR_PAIR(1));
				user -> current_weapon = 4;
				refresh();
				sleep(2);
				return;
			}
			if ((user -> weapon_menu)[4] > 0) {
				clear();
				attron(COLOR_PAIR(2));
				mvprintw(31, 88, "Now, Your Current Weapon Is A Sword!");
				attroff(COLOR_PAIR(2));
				user -> current_weapon = 4;
				refresh();
				sleep(2);
				return;
			}
			clear();
			attron(COLOR_PAIR(1));
			mvprintw(31, 89, "There Are No Swords In Backpack!");
			attroff(COLOR_PAIR(1));
			refresh();
			sleep(2);
			weapon_menu_func(user);
		}
	}
}

#endif