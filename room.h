#ifndef ROOM_H
#define ROOM_H

#define MAX_SIZE 1000

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int x_coor;
    int y_coor;
    int theme;
} Door;

typedef struct {
    int ulx;
    int uly;
    int height;
    int width;
    int floor;
    int room_number;
    int num_of_doors;
    int theme;
    Door *doors[1000];
    int num_of_connected;
    Door *connected[MAX_SIZE];
} Room;

typedef struct {
    int x;
    int y;
} Corridor;

#endif