#ifndef LOGIN_H
#define LOGIN_H
#include <curses.h>
#include <form.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "user.h"
#include "game_menu.h"

#define MAX_SIZE2 50
#define MAX_SIZE 1000

void login_menu(int, int);
void add_char1(char *, int);
int has_username(char *, char *, char *);
int check_password_correct(char *, char *);
void build_user(User *);

void login_menu(int is_username, int is_password) {
    curs_set(1);
    clear();
    FIELD *field[3];
    FORM  *login;
    field[0] = new_field(1, MAX_SIZE2-1, 33, 106, 0, 0);
	field[1] = new_field(1, MAX_SIZE2-1, 34, 106, 0, 0);
	field[2] = NULL; 
	field_opts_off(field[0], O_AUTOSKIP);  	
	field_opts_off(field[1], O_AUTOSKIP);
    login = new_form(field);
	post_form(login);
	refresh();
    attron(COLOR_PAIR(1));
    if (is_username) {
        mvprintw(28, 89,"The entered username doesn't exist.");
    }
    if (is_password) {
        mvprintw(28, 90,"The entered password is incorrect.");
    }
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(3));
    mvprintw(31, 104, "LOG IN");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(2));
    mvprintw(33, 95, "Username :");
    mvprintw(34, 95, "Password :");
    attroff(COLOR_PAIR(2));
    mvprintw(LINES - 2, 0, "Press F1 to return to the previous menu.");
    mvprintw(33, 105, " ");
    refresh();
    int ch;
    char entered_username[MAX_SIZE2], entered_password[MAX_SIZE2];
    entered_username[0] = '\0';
    entered_password[0] = '\0';
    int flag = 0;
    while((ch = getch()) != '\n' && ch != KEY_F(1)) {	
        switch(ch) {	
            case KEY_DOWN:
				form_driver(login, REQ_NEXT_FIELD);
				form_driver(login, REQ_END_LINE);
                flag = (flag + 1) % 2;
				break;
			case KEY_UP:
				form_driver(login, REQ_PREV_FIELD);
				form_driver(login, REQ_END_LINE);
                flag = (flag + 1) % 2;
				break;
			default:	
				form_driver(login, ch);
                switch(flag) {
                    case 0:
                        add_char1(entered_username, ch);
                        break;
                    case 1:
                        add_char1(entered_password, ch);
                        break;
                }
				break;
		}
	}
    unpost_form(login);
	free_form(login);
	free_field(field[0]);
	free_field(field[1]);
    if (ch == KEY_F(1))
        return;
    char file_password[MAX_SIZE2], file_email[MAX_SIZE2];
    is_username = has_username(entered_username, file_password, file_email);
    if (is_username) {
        login_menu(1, 0);
    }
    else {
        is_password = check_password_correct(entered_password, file_password);
        if (is_password) {
            login_menu(0, 1);
            return;
        }
        else {
            attron(COLOR_PAIR(2) | A_BLINK);
            mvprintw(33, 107 - ((strlen(entered_username)+8)/2),"Welcome %s", entered_username);
            attroff(COLOR_PAIR(2) | A_BLINK);
            refresh();
            sleep(3);
            clear();
            User *user_ptr = (User *) malloc(sizeof(User));
            strcpy(user_ptr->username, entered_username);
            strcpy(user_ptr->password, entered_password);
            strcpy(user_ptr->email, file_email);
            build_user(user_ptr);
            user_ptr -> is_guest = 0;
            game_menu_func(user_ptr);
            login_menu(0, 0);
        }
    }
}

void add_char1(char *array, int ch) {
    char c[2] = {(char) ch, '\0'};
    strcat(array, c);
}

int has_username(char *entered_username, char *file_password, char *file_email) {
    FILE *users;
    users = fopen("users.txt", "r");
    char line[MAX_SIZE2];
    while (fgets(line, MAX_SIZE2, users)) {
        if (strcmp(line, "{\n") == 0) {
            fgets(line, MAX_SIZE2, users);
            line[strlen(line)-1] = '\0';
            if (strcmp(entered_username, line) == 0) {
                fgets(line, MAX_SIZE2, users);
                line[strlen(line)-1] = '\0';
                strcpy(file_password, line);
                fgets(line, MAX_SIZE2, users);
                line[strlen(line)-1] = '\0';
                strcpy(file_email, line);
                fclose(users);
                return 0;
            }
        }
    }
    fclose(users);
    return 1;
}

int check_password_correct(char *entered_password, char *file_password) {
    if (strcmp(entered_password, file_password) == 0)
        return 0;
    return 1;
}

void build_user(User *user) {
    FILE *users;
    users = fopen("users.txt", "r");
    char line[MAX_SIZE];
    while (fgets(line, MAX_SIZE, users)) {
        if (strcmp(line, "{\n") == 0) {
            fgets(line, MAX_SIZE, users);
            line[strlen(line)-1] = '\0';
            if (strcmp(user -> username, line) == 0) {
                fgets(line, MAX_SIZE, users); //password
                fgets(line, MAX_SIZE, users); // email
                fgets(line, MAX_SIZE, users); //total gold
                line[strlen(line)-1] = '\0';
                sscanf(line, "%d", &(user -> total_gold));
                fgets(line, MAX_SIZE, users); //total_score
                line[strlen(line)-1] = '\0';
                sscanf(line, "%d", &(user -> total_score));
                fgets(line, MAX_SIZE, users); //difficulty
                line[strlen(line)-1] = '\0';
                sscanf(line, "%d", &(user -> difficulty));
                fgets(line, MAX_SIZE, users); //color
                line[strlen(line)-1] = '\0';
                sscanf(line, "%d", &(user -> color));
                fgets(line, MAX_SIZE, users); //games
                line[strlen(line)-1] = '\0';
                sscanf(line, "%d", &(user -> games));
                fgets(line, MAX_SIZE, users); //complete games
                line[strlen(line)-1] = '\0';
                sscanf(line, "%d", &(user -> complete_games));
                fgets(line, MAX_SIZE, users); //resume game
                line[strlen(line)-1] = '\0';
                sscanf(line, "%d", &(user -> resume));
                if (user -> resume) {
                    fgets(line, MAX_SIZE, users); //rooms_num
                    line[strlen(line)-1] = '\0';
                    char *ptr = strtok(line, " ");
                    int i = 0;
                    while(i < 4) {
                        sscanf(ptr, "%d", &((user -> rooms_num)[i]));
                        i++;
                        ptr = strtok(NULL, " ");
                    }
                    //map_rooms
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < (user -> rooms_num)[f]; i++) {
                            (user -> map_rooms)[f][i] = (Room *) malloc(sizeof(Room));
                            fgets(line, MAX_SIZE, users); //ulx
                            line[strlen(line)-1] = '\0';
                            sscanf(line, "%d", &((user -> map_rooms)[f][i] -> ulx));
                            fgets(line, MAX_SIZE, users); //uly
                            line[strlen(line)-1] = '\0';
                            sscanf(line, "%d", &((user -> map_rooms)[f][i] -> uly));
                            fgets(line, MAX_SIZE, users); //height
                            line[strlen(line)-1] = '\0';
                            sscanf(line, "%d", &((user -> map_rooms)[f][i] -> height));
                            fgets(line, MAX_SIZE, users); //width
                            line[strlen(line)-1] = '\0';
                            sscanf(line, "%d", &((user -> map_rooms)[f][i] -> width));
                            (user -> map_rooms)[f][i] -> floor = f + 1;
                            (user -> map_rooms)[f][i] -> room_number = i + 1;
                            fgets(line, MAX_SIZE, users); //theme
                            line[strlen(line)-1] = '\0';
                            sscanf(line, "%d", &((user -> map_rooms)[f][i] -> theme));
                        }
                    }
                    

                    //map_screen
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            fgets(line, MAX_SIZE, users);
                            line[strlen(line)-1] = '\0';
                            ptr = strtok(line, " ");
                            for (int j = 0; j < 200; j++) {
                                sscanf(ptr, "%d", &((user -> map_screen)[f][i][j]));
                                ptr = strtok(NULL, " ");
                            }
                        }
                    }
                    

                    //map_screen_char
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            fgets(line, MAX_SIZE, users);
                            for (int j = 0; j < 200; j++) {
                                (user -> map_screen_char)[f][i][j] = line[j];
                            }
                        }
                    }

                    //weapon type
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            fgets(line, MAX_SIZE, users);
                            line[strlen(line)-1] = '\0';
                            ptr = strtok(line, " ");
                            for (int j = 0; j < 200; j++) {
                                sscanf(ptr, "%d", &((user -> weapon_type)[f][i][j]));
                                ptr = strtok(NULL, " ");
                            }
                        }
                    }
                    

                    //in_staircase 1 2 3
                    fgets(line, MAX_SIZE, users);
                    line[strlen(line)-1] = '\0';
                    ptr = strtok(line, " ");
                    for (i = 1; i < 4; i++) {
                        (user -> in_staircase)[i] = (Point *) malloc(sizeof(Point));
                        sscanf(ptr, "%d", &((user -> in_staircase)[i] -> x));
                        ptr = strtok(NULL, " ");
                        sscanf(ptr, "%d", &((user -> in_staircase)[i] -> y));
                        ptr = strtok(NULL, " ");
                    }
                    
                    //out_staircases 0 1 2
                    fgets(line, MAX_SIZE, users);
                    line[strlen(line)-1] = '\0';
                    ptr = strtok(line, " ");
                    for (i = 0; i < 3; i++) {
                        (user -> out_staircase)[i] = (Point *) malloc(sizeof(Point));
                        sscanf(ptr, "%d", &((user -> out_staircase)[i] -> x));
                        ptr = strtok(NULL, " ");
                        sscanf(ptr, "%d", &((user -> out_staircase)[i] -> y));
                        ptr = strtok(NULL, " ");
                    }
                    

                    //visible
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            fgets(line, MAX_SIZE, users);
                            line[strlen(line)-1] = '\0';
                            ptr = strtok(line, " ");
                            for (int j = 0; j < 200; j++) {
                                sscanf(ptr, "%d", &((user -> visible)[f][i][j]));
                                ptr = strtok(NULL, " ");
                            }
                        }
                    }
            

                    fgets(line, MAX_SIZE, users); //end_x
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> end_x));

                    fgets(line, MAX_SIZE, users); //end_y
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> end_y));

                    fgets(line, MAX_SIZE, users); //current_floor
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> current_floor));

                    fgets(line, MAX_SIZE, users); //current_x
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> current_x));

                    fgets(line, MAX_SIZE, users); //current_y
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> current_y));

                    fgets(line, MAX_SIZE, users); //health
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> health));

                    fgets(line, MAX_SIZE, users); //gold
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> gold));

                    fgets(line, MAX_SIZE, users); //score
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> score));

                    fgets(line, MAX_SIZE, users); //power
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> power));

                    fgets(line, MAX_SIZE, users); //current weapon
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> current_weapon));

                    //enemy health
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            fgets(line, MAX_SIZE, users);
                            line[strlen(line)-1] = '\0';
                            ptr = strtok(line, " ");
                            for (int j = 0; j < 200; j++) {
                                sscanf(ptr, "%d", &((user -> enemy_health)[f][i][j]));
                                ptr = strtok(NULL, " ");
                            }
                        }
                    }

                    //enemy move
                    for (int f = 0; f < 4; f++) {
                        for (i = 0; i < 60; i++) {
                            fgets(line, MAX_SIZE, users);
                            line[strlen(line)-1] = '\0';
                            ptr = strtok(line, " ");
                            for (int j = 0; j < 200; j++) {
                                sscanf(ptr, "%d", &((user -> enemy_move)[f][i][j]));
                                ptr = strtok(NULL, " ");
                            }
                        }
                    }

                    fgets(line, MAX_SIZE, users); //food
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> food));

                    fgets(line, MAX_SIZE, users); //food_menu
                    line[strlen(line)-1] = '\0';
                    ptr = strtok(line, " ");
                    for (i = 0; i < (user -> food); i++) {
                        sscanf(ptr, "%d", &((user -> food_menu)[i]));
                        ptr = strtok(NULL, " ");
                    }

                    //enchant menu
                    fgets(line, MAX_SIZE, users);
                    line[strlen(line)-1] = '\0';
                    ptr = strtok(line, " ");
                    for (i = 0; i < 3; i++) {
                        sscanf(ptr, "%d", &((user -> enchant_menu)[i]));
                        ptr = strtok(NULL, " ");
                    }
                    //weapon menu
                    fgets(line, MAX_SIZE, users);
                    line[strlen(line)-1] = '\0';
                    ptr = strtok(line, " ");
                    for (i = 0; i < 5; i++) {
                        sscanf(ptr, "%d", &((user -> weapon_menu)[i]));
                        ptr = strtok(NULL, " ");
                    }

                    fgets(line, MAX_SIZE, users); //visible mode
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> visible_mode));

                    fgets(line, MAX_SIZE, users); //hunger
                    line[strlen(line)-1] = '\0';
                    sscanf(line, "%d", &(user -> hunger));


                }
                fclose(users);
                return ;
            }
        }
    }
    fclose(users);
}

#endif