#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

typedef struct _Tunnel {
    int size;
    int direction; // - or +
    int pos;
} Tunnel;

typedef struct _Board {
    int width;
    int height;
    int ** blocks;
    int numTunnels;
    Tunnel * tunnels;
    int score;
} Board;

typedef struct _Player {
    int x;
    int y;
    char* avatar;
} Player;

void printScreen(Board *, Player *);
void clearScreen();
bool getInput(Player *);
void getSize(Board *);
void destroyBoard(Board *);
void moveWalls(Board *);
bool checkCollision(Board *, Player *);
void makeLine(Board *);
void changeTunnels(Board *);
void updateTunnelPos(Board *);
