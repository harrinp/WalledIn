#include "Run.h"

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
    b->wallChar = " ";
    b->numTunnels = 0;
}

char* _getCharForBlock(Board *b, int line, int pos){
    //printf("line : %d pos : %d\n", line, pos);
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
        printf("%c\n",c );
        switch (c) {
        case 'w': p->y--;
            break;

        case 's': p->y++;
            break;

        case 'a': p->x--;
            break;

        case 'd': p->x++;
            break;

        case 'q': quit = true;
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

void _printTunnel(Board *b, int tunnelIndex){
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

bool _insertColorFromArg(char * a, short * s){
    if ( strcmp(a, "RED") == 0){
        (*s) = COLOR_RED;
    }
    else if (strcmp(a, "BLACK") == 0){
        (*s) = COLOR_BLACK;
    }
    else if (strcmp(a, "WHITE") == 0){
        (*s) = COLOR_WHITE;
    }
    else if (strcmp(a, "GREEN") == 0){
        (*s) = COLOR_GREEN;
    }
    else if (strcmp(a, "MAGENTA") == 0){
        (*s) = COLOR_MAGENTA;
    }
    else if (strcmp(a, "CYAN") == 0){
        (*s) = COLOR_CYAN;
    }
    else if (strcmp(a, "BLUE") == 0){
        (*s) = COLOR_BLUE;
    }
    else if (strcmp(a, "NOCOLOR") == 0){
        (*s) = -1;
    }
    else {
        return true;
    }
    return false;
}

bool handleArgs(Board * b, Player * p, int argc, char * argv[], short * cpf, short * cpb, short * cwf, short * cwb){
    bool noPlayer = false;
    bool noWall = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0){
            printf("Help for Walled In:\n");
            printf(" -h            --> help\n");
            printf(" -cw arg1 arg2 --> Set color of walls (Foreground) (Background) \n");
            printf(" -cp arg1 arg2 --> Set color of player (Foreground) (Background) \n");
            printf("    Available color arguments: RED, BLACK, WHITE, GREEN, MAGENTA, CYAN, BLUE, NOCOLOR\n" );
            printf(" -ncp          --> Shortcut to not color player \n");
            printf(" -ncw          --> Shortcut to not color wall \n");
            printf(" -nc           --> Shortcut to not color anything \n");
            printf(" -p arg1       --> Change the character that the player is displayed as. Limit 1 character\n");
            printf(" -w arg1       --> Change the character that the inside of the walls are displayed as. Default is " ". Limit 1 character\n");
            printf(" -t arg1       --> Change the number of tunnels that will exist at any time. Limit 100\n");
            printf("\nWalled In by Paul Harrington\n");
            return true;
        }
        if (!noWall && strcmp(argv[i], "-cw") == 0 && argc > i + 2){
            if (_insertColorFromArg(argv[i + 1], cwf) || _insertColorFromArg(argv[i + 2], cwb)){
                printf("Bad color code - default assumed\n");
            }
        }
        if (!noPlayer && strcmp(argv[i], "-cp") == 0 && argc > i + 2){
            if (_insertColorFromArg(argv[i + 1], cpf) || _insertColorFromArg(argv[i + 2], cpb)){
                printf("Bad color code - default assumed\n");
            }
        }
        if (strcmp(argv[i], "-ncp") == 0){
            (*cpf) = 100;
            noPlayer = true;
        }
        if (strcmp(argv[i], "-ncw") == 0){
            (*cwf) = 100;
            noWall = true;
        }
        if (strcmp(argv[i], "-nc") == 0){
            *cwf = 100;
            *cpf = 100;
            noWall = true;
            noPlayer = true;
        }
        if (strcmp(argv[i], "-p") == 0 && argc > i + 1){
            int length = 0;
            while (1){
                if (argv[i + 1][length] == '\0'){
                    break;
                }
                else {
                    length++;
                }
            }
            if (length == 1){
                p->avatar = argv[i + 1];
            }
            else {
                printf("Bad length for player string\n");
            }
        }
        if (strcmp(argv[i], "-w") == 0 && argc > i + 1){
            int length = 0;
            while (1){
                if (argv[i + 1][length] == '\0'){
                    break;
                }
                else {
                    length++;
                }
            }
            if (length == 1){
                b->wallChar = argv[i + 1];
            }
            else {
                printf("Bad length for wall string\n");
            }
        }
        if (strcmp(argv[i], "-t") == 0 && argc > i + 1){
            bool allNum = true;
            int length = 0;
            while (1){
                if (argv[i + 1][length] == '\0'){
                    break;
                }
                else {
                    if (!(argv[i + 1][length] > 47 && argv[i + 1][length] < 58)){
                        allNum = false;
                    }
                }
                length++;
            }
            if (allNum && atoi(argv[i + 1]) < 100 && atoi(argv[i + 1]) > 0){
                b->numTunnels = atoi(argv[i + 1]);
            }
            else {
                printf("Too many tunnels\n");
            }
        }
    }
    return false;
}
