#include "Run.h"

Board           b;
Player          p;
bool            quit  = false;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void _printScore(){
    mvprintw(0, 0, ".                .");
    mvprintw(1, 0, " Score: %.*d  ", 8, b.score);
    mvprintw(2, 0, ".                .");

    mvprintw(0, b.width - 11, ".         .");
    mvprintw(1, b.width - 11, " WALLED IN ");
    mvprintw(2, b.width - 11, ".         .");
}


void *_screenUpdate() {
    int timeCount = 0;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (timeCount % 2 == 0) {
            updateTunnelPos(&b);
            moveWalls(&b);
            makeLine(&b);
        }
        if (timeCount % 5 == 0) {
            changeTunnels(&b);
        }
        if (checkCollision(&b, &p)) {
            quit = true;
            break;
        }
        if (quit) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        clear();
        printScreen(&b, &p);
        use_default_colors();
        _printScore();
        refresh();
        pthread_mutex_unlock(&mutex);
        usleep(200000);
        timeCount++;
        b.score += b.height - p.y;
    }
    return NULL;
}

void *_checkForInput() {
    while (!quit) {
        pthread_mutex_lock(&mutex);
        quit = getInput(&p);
        pthread_mutex_unlock(&mutex);
        usleep(2000);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    getSize(&b);

    p.x      = b.width / 2;
    p.y      = b.height - 3;
    p.avatar = "A";

    short colorPlayerFore, colorPlayerBack, colorWallFore, colorWallBack;
    colorWallFore = COLOR_WHITE;
    colorWallBack = COLOR_BLACK;
    colorPlayerFore = COLOR_RED;
    colorPlayerBack = COLOR_WHITE;
    if (handleArgs(&b, &p, argc, argv, &colorPlayerFore, &colorPlayerBack, &colorWallFore, &colorWallBack)){
        destroyBoard(&b);
        return 0;
    }
    makeTunnels(&b);
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);

    start_color();
    if (colorWallFore == 100 || colorWallBack == 100){
        b.wallColor = false;
    }
    else {
        init_pair(1, colorWallFore, colorWallBack); // Walls
    }

    if (colorPlayerFore == 100 || colorPlayerBack == 100){
        b.playerColor = false;
    }
    else {
        init_pair(2, colorPlayerFore, colorPlayerBack); // Walls
    }

    use_default_colors();

    pthread_mutexattr_t shared;
    pthread_mutexattr_init(&shared);
    pthread_mutexattr_setpshared(&shared, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&mutex, &shared);


    pthread_t pthV; // View thread id
    pthread_t pthI; // Input thread id

    pthread_create(&pthV, NULL, _screenUpdate, NULL);
    pthread_create(&pthI, NULL, _checkForInput, NULL);

    pthread_join(pthV, NULL);
    pthread_join(pthI, NULL);

    mvprintw(p.y + 1, p.x - 10, ",----------------------,");
    mvprintw(p.y + 2, p.x - 10, "|  THANKS FOR PLAYING  |");
    mvprintw(p.y + 3, p.x - 10, "| >Press any key twice |");
    mvprintw(p.y + 4, p.x - 10, "'----------------------'");

    refresh();

    nodelay(stdscr, FALSE);
    while (getch() == ERR){

    }
    // while ('q' != getch()){}
    getch();
    endwin();
    nocbreak();
    destroyBoard(&b);
    return 0;
}
