#include "Run.h"
#include "Config.h"

#define NUM_TUNNELS_DIVISOR 15


void makeTunnels(Board *b) {
    if (b->numTunnels == 0){
        b->tunnels    = malloc(sizeof(*b->tunnels) * b->width / NUM_TUNNELS_DIVISOR);
        b->numTunnels = b->width / NUM_TUNNELS_DIVISOR;
    }
    else {
        b->tunnels    = malloc(sizeof(*b->tunnels) * b->numTunnels);
    }
    for (int i = 0; i < b->numTunnels; i++) {
        b->tunnels[i].size      = (rand() % 5) + 3;
        b->tunnels[i].pos       = (rand() % (b->width - 20)) + 10;
        b->tunnels[i].direction = (rand() % 4) - 2;
    }
}

void getSize(Board *b) {
    struct winsize size;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    /* size.ws_row is the number of rows, size.ws_col is the number of columns. */
    b->width  = size.ws_col;
    b->height = size.ws_row;
    b->blocks = malloc(sizeof(*(b->blocks)) * b->height);
    for (int i = 0; i < b->height; i++) {
        b->blocks[i] = malloc(sizeof(**(b->blocks)) * b->width);
    }
    int counter = 0;
    for (int i = 0; i < b->height; i++) {
        b->blocks[i][0] = 1;
        b->blocks[i][b->width-1] = 1;
    }
    for (int i = b->height - b->height / 2; i >= 0; i--) {

        for (int j = 0; j < b->width; j++) {
            if (j < counter || j > b->width - counter) {
                b->blocks[i][j] = 1;
            }
        }
        counter++;
    }
    b->score = 0;
    b->wallColor = true;
    b->playerColor = true;
    b->wallChar = WALLCHAR;
    b->numTunnels = 0;
}

char* _getCharForBlock(Board *b, int line, int pos){
    if (line + 1 > b->height - 1 || pos - 1 <= 0 || pos + 1 >= b->width - 1){
        return b->wallChar;
    }
    if (b->blocks[line][pos + 1] == 0 && b->blocks[line][pos - 1] == 0 ){
        return "V";
    }
    if (b->blocks[line + 1][pos] == 0){
        if (b->blocks[line + 1][pos - 1] == 1 && b->blocks[line][pos + 1] == 0){
            /*
                X0
               10
            */
            return "/";
        }
        else if (b->blocks[line + 1][pos + 1] == 1 && b->blocks[line][pos - 1] == 0){
            /*
               0X
                01
            */
            return "\\";
        }
        else if (b->blocks[line + 1][pos - 1] == 0 && b->blocks[line][pos + 1] == 0){
            /*
                X0
              100
            */
            return "/";
        }
        else if (b->blocks[line + 1][pos + 1] == 0 && b->blocks[line][pos - 1] == 0){
            /*
               0X
                001
            */
            return "\\";
        }
    }
    if (b->blocks[line + 1][pos] == 1){
        if (b->blocks[line + 1][pos - 1] == 1 && b->blocks[line][pos - 1] == 0){
            /*
              0X
              11
            */
            return "/";
        }
        else if (b->blocks[line + 1][pos + 1] == 1 && b->blocks[line][pos + 1] == 0){
            /*
                X0
                11
            */
            return "\\";
        }

    }
    if (b->blocks[line][pos + 1] == 0 || b->blocks[line][pos - 1] == 0 ){
        return "|";
    }
    if (b->blocks[line+1][pos] == 0){
        return "_";
    }
    else {
        return b->wallChar;
    }
}

void printScreen(Board *b, Player *p) {
    if (b->playerColor){
        attron(COLOR_PAIR(2));
        mvprintw(p->y, p->x, p->avatar);
        attroff(COLOR_PAIR(2));
    }
    else {
        mvprintw(p->y, p->x, p->avatar);
    }
    if (b->wallColor){
        attron(COLOR_PAIR(1));
    }
    for (int i = 0; i < b->height; i++) {
        for (int j = 0; j < b->width; j++) {
            if (p->x != j || p->y != i) {
                switch (b->blocks[i][j]) {
                case 0:
                    break;

                case 1: mvprintw(i, j, _getCharForBlock(b, i, j));
                    break;

                default: break;
                }
            }
        }
    }
    if (b->wallColor){
        attroff(COLOR_PAIR(1));
    }
}

void moveWalls(Board *b) {
    for (int i = b->height - 1; i > 0; i--) {
        for (int j = 0; j < b->width; j++) {
            b->blocks[i][j] = b->blocks[i - 1][j];
        }
    }
}

bool getInput(Player *p) {
    bool quit = false;

    while (1) {
        int c = getch();
        if (c == EOF || c == ERR) {
            break;
        }
        switch (c) {
        case UP: p->y--;
            break;

        case DOWN: p->y++;
            break;

        case LEFT: p->x--;
            break;

        case RIGHT: p->x++;
            break;

        case QUIT: quit = true;
            break;
        }
    }
    return quit;
}

void destroyBoard(Board *b) {
    for (int i = 0; i < b->height; i++) {
        free(b->blocks[i]);
    }
    free(b->blocks);
    free(b->tunnels);
}

bool checkCollision(Board *b, Player *p) {
    if (p->y >= b->height - 5){
        p->y = b->height - 5;
    }
    if (p->y <= 0){
        p->y = 0 + 1;
    }
    if (p->x >= b->width){
        p->y -= 1;
    }
    if (b->blocks[p->y][p->x] == 1) {
        return true;
    }
    return false;
}

bool _isThin(Board *b, int i, bool inverse) {
    int countLeft  = 0;
    int countRight = 0;

    while (b->blocks[1][i - countLeft] == 1 * !inverse) {
        countLeft++;
    }
    while (b->blocks[1][i - countRight] == 1 * !inverse) {
        countRight++;
    }
    if (countLeft + countRight < b->width / 10) {
        return true;
    }
    return false;
}

void makeLine(Board *b) {
    for (int i = 1; i < b->width - 1; i++) {
        b->blocks[0][i] = 1;
    }
    for (int i = 0; i < b->numTunnels; i++) {
        int start = b->tunnels[i].pos - b->tunnels[i].size / 2;
        for (int j = start; j < start + b->tunnels[i].size; j++) {
            if (!(j < 0 || j >= b->width)){
                b->blocks[0][j] = 0;
            }
        }
    }
    b->blocks[0][0]            = 1;
    b->blocks[0][b->width - 1] = 1;
}

void _printTunnel(Board *b, int tunnelIndex){   //For testing
    printf("Pos: %d Size: %d\n", b->tunnels[tunnelIndex].pos, b->tunnels[tunnelIndex].size);
}

void _updateTunnel(Board *b, int tunnelIndex) {
    b->tunnels[tunnelIndex].size     += (rand() % 3) - 1;
    b->tunnels[tunnelIndex].direction = (rand() % 5) - 2;
    if (b->tunnels[tunnelIndex].size >= 10){
        b->tunnels[tunnelIndex].size -= (rand() % 2) + 1;
    }
    if (b->tunnels[tunnelIndex].size <= 3){
        b->tunnels[tunnelIndex].size += (rand() % 2);
    }
}

void updateTunnelPos(Board *b) {

    for (int i = 0; i < b->numTunnels; i++) {
        b->tunnels[i].pos += b->tunnels[i].direction;
        if (b->tunnels[i].pos + b->tunnels[i].size / 2 > b->width || b->tunnels[i].pos + b->tunnels[i].size / 2 < 0) {
            b->tunnels[i].direction *= -1;
            b->tunnels[i].pos += b->tunnels[i].direction;
        }
    }
}

void changeTunnels(Board *b) {
    for (int i = 0; i < b->numTunnels; i++) {
        _updateTunnel(b, i);
    }
}
