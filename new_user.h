#ifndef NEW_USER_H
#define NEW_USER_H
#include <curses.h>
#include <form.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_SIZE2 50

void create_account_menu(int, int, int, int);
void add_char(char *, int);
int check_username(char *);
int check_length_password(char *);
int check_password(char *);
int check_email(char *);

void create_account_menu(int is_username, int length_password, int is_password, int is_email) {
    clear();
    FIELD *field[4];
    FORM  *new_account;
    field[0] = new_field(1, MAX_SIZE2-1, 32, 106, 0, 0);
	field[1] = new_field(1, MAX_SIZE2-1, 33, 106, 0, 0);
    field[2] = new_field(1, MAX_SIZE2-1, 34, 111, 0, 0);
	field[3] = NULL; 
	field_opts_off(field[0], O_AUTOSKIP);  	
	field_opts_off(field[1], O_AUTOSKIP);
	field_opts_off(field[2], O_AUTOSKIP);
    new_account = new_form(field);
	post_form(new_account);
	refresh();
    attron(COLOR_PAIR(1));
    int line_number = 25;
    if (is_username) {
        mvprintw(line_number, 91,"The entered username does exist.");
        line_number++;
    }
    if (length_password) {
        mvprintw(line_number, 88,"The password lenght must be at least 7.");
        line_number++;
    }
    if (is_password) {
        mvprintw(line_number, 61,"The password must contain at least one digit, one small character and one capital character.");
        line_number++;
    }
    if (is_email) {
        mvprintw(line_number, 87,"The email must have the format xxx@y.zzz");
    }
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(3));
    mvprintw(31, 101, "Create Account");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(2));
    mvprintw(32, 95, "Username :");
    mvprintw(33, 95, "Password :");
    mvprintw(34, 95, "Email Address :");
    attroff(COLOR_PAIR(2));
    mvprintw(LINES - 2, 0, "Press F1 to return to the previous menu.");
    mvprintw(32, 105, " ");
    refresh();
    int ch;
    char entered_username[MAX_SIZE2], entered_password[MAX_SIZE2], entered_email[MAX_SIZE2];
    entered_username[0] = '\0';
    entered_password[0] = '\0';
    entered_email[0] = '\0';
    int flag = 0;
    while((ch = getch()) != '\n' && ch != KEY_F(1)) {	
        switch(ch) {	
            case KEY_DOWN:
				form_driver(new_account, REQ_NEXT_FIELD);
				form_driver(new_account, REQ_END_LINE);
                flag = (flag + 1) % 3;
				break;
			case KEY_UP:
				form_driver(new_account, REQ_PREV_FIELD);
				form_driver(new_account, REQ_END_LINE);
                flag = (flag + 2) % 3;
				break;
			default:	
				form_driver(new_account, ch);
                switch(flag) {
                    case 0:
                        add_char(entered_username, ch);
                        break;
                    case 1:
                        add_char(entered_password, ch);
                        break;
                    case 2:
                        add_char(entered_email, ch);
                        break;
                }
				break;
		}
	}
    unpost_form(new_account);
	free_form(new_account);
	free_field(field[0]);
	free_field(field[1]);
    free_field(field[2]);
    if (ch == KEY_F(1))
        return;
    //if it's ok the returned value will be 0. Else it will be 1.
    is_username = check_username(entered_username);
    length_password = check_length_password(entered_password);
    is_password = check_password(entered_password);
    is_email = check_email(entered_email);
    if (is_username || length_password || is_password || is_email) {
        create_account_menu(is_username, length_password, is_password, is_email);
    }
    else {
        clear();
        attron(COLOR_PAIR(2) | A_BLINK);
        mvprintw(33, 85,"Sign Up was successful. Now you Can Log in to the game.");
        attroff(COLOR_PAIR(2) | A_BLINK);
        refresh();
        FILE *users;
        users = fopen("users.txt", "a");
        fprintf(users, "{\n%s\n%s\n%s\n100\n0\n1\n2\n0\n0\n0\n}\n", entered_username, entered_password, entered_email);
        fclose(users);
        sleep(5);
        return;
    }
}

void add_char(char *array, int ch) {
    char c[2] = {(char) ch, '\0'};
    strcat(array, c);
}

int check_username(char *array) {
    FILE *users;
    users = fopen("users.txt", "r");
    char line[MAX_SIZE2];
    while (fgets(line, MAX_SIZE2, users)) {
        if (strcmp(line, "{\n") == 0) {
            fgets(line, MAX_SIZE2, users);
            line[strlen(line)-1] = '\0';
            if (strcmp(array, line) == 0) {
                fclose(users);
                return 1;
            }
        }
    }
    fclose(users);
    return 0;
}

int check_length_password(char *array) {
    if (strlen(array) >= 7)
        return 0;
    return 1;
}

int check_password(char *array) {
    int flag_digit = 0;
    int flag_small_ch = 0;
    int flag_capital_ch = 0;
    for (int i = 0; i < strlen(array); i++) {
        if (isdigit(array[i])) {
            flag_digit = 1;
        }
        if (islower(array[i])) {
            flag_small_ch = 1;
        }
        if (isupper(array[i])) {
            flag_capital_ch = 1;
        }
    }
    if (flag_digit && flag_small_ch && flag_capital_ch)
        return 0;
    return 1;
}

int check_email(char *array) {
    int cnt = 0;
    int idx = -1;
    for (int i = 0; i < strlen(array); i++) {
        if (array[i] == '@') {
            cnt++;
            idx = i;
        }
    }
    if (cnt == 1) {
        int cnt2 = 0;
        int idx2 = -1;
        for (int i = idx+1; i < strlen(array); i++) {
            if (array[i] == '.') {
                cnt2++;
                idx2 = i;
            }
        }
        if (cnt2 == 1) {
            if (idx2 == idx + 1 || idx == 0 || idx2 == strlen(array) - 1)
                return 1;
            for (int i = 0; i < strlen(array); i++) {
                if (i != idx && i != idx2 && !isalpha(array[i])) {
                    return 1;
                }
            }
            return 0;
        }
    }
    return 1;
}
#endif