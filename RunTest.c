#include "Run.h"

int main(int argc, char *argv[]) {
    Board b;
    getSize(&b);
    Player p = {b.width/2,b.height - 3,'A'};
    int timeCount = 0;
    while (1) {
        if(getInput(&p)){
            break;
        }
        if (timeCount % 2 == 0){
            updateTunnelPos(&b);
            moveWalls(&b);
            makeLine(&b);
        }
        if (timeCount % 5 == 0){
            changeTunnels(&b);
        }

        // printScreen(&b, &p);
        // if (checkCollision(&b, &p)){
        //     break;
        // }
        // refresh();
        usleep(200000);
        timeCount++;
    }

    // endwin();
    destroyBoard(&b);
    printf("THANKS FOR PLAYING\n");
    return 0;
}
