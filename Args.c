#include "Run.h"

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

bool handleArgs(Board * b, Player * p, int argc, char * argv[], short * cpf, short * cpb, short * cwf, short * cwb, bool * contact){
    bool noPlayer = false;
    bool noWall = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0){
            printf("Help for Walled In:\n");
            printf(" -h            --> help\n");
            printf(" -cw arg1 arg2 --> Set color of walls (Foreground) (Background) \n");
            printf(" -cp arg1 arg2 --> Set color of player (Foreground) (Background) \n");
            printf("                -> Available color arguments: RED, BLACK, WHITE, GREEN, MAGENTA, CYAN, BLUE, NOCOLOR\n" );
            printf(" -ncp          --> Shortcut to not color player \n");
            printf(" -ncw          --> Shortcut to not color wall \n");
            printf(" --nocolor     --> Don't color anything (terminal defaults used)\n");
            printf(" -nc           --> Shortcut to not color anything \n\n");
            printf(" -p arg1       --> Change the character that the player is displayed as. Limit 1 character\n");
            printf(" -w arg1       --> Change the character that the inside of the walls are displayed as. Default is " ". Limit 1 character\n");
            printf(" -t arg1       --> Change the number of tunnels that will exist at any time. Limit 100\n\n");
            printf(" --nocollision --> Turn off collision mechanics, game runs until you quit\n");
            printf(" -ncol         --> Shortcut to no collision\n");

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
        if (strcmp(argv[i], "-nc") == 0 || strcmp(argv[i], "--nocolor") == 0){
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
        if (strcmp(argv[i], "-ncol") == 0 || strcmp(argv[i], "--nocollision") == 0){
            *contact = false;
        }
    }
    return false;
}
