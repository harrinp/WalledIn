#include "Run.h"

Board  b;
Player p;
bool   quit = false;

int main(int argc, char *argv[]) {
    srand(time(NULL));
    getSize(&b);

    p.x      = b.width / 2;
    p.y      = b.height - 3;
    p.avatar = 'A';

    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    int timeCount = 0;

    while (1) {
        if (timeCount % 2 == 0) {
            updateTunnelPos(&b);
            moveWalls(&b);
            makeLine(&b);
        }
        quit = getInput(&p);
        if (timeCount % 5 == 0) {
            changeTunnels(&b);
        }
        if (checkCollision(&b, &p)) {
            break;
        }
        clear();
        printScreen(&b, &p);
        if (quit) {
            break;
        }
        refresh();
        usleep(200000);
        timeCount++;
    }
    mvprintw(p.y + 1, p.x - 10, "|------------------|");
    mvprintw(p.y + 3, p.x - 10, "|THANKS FOR PLAYING|");
    mvprintw(p.y + 3, p.x - 10, "|------------------|");
    endwin();
    destroyBoard(&b);
    nocbreak();
    return 0;
}
