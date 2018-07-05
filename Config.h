#ifndef CONFIG_H
#define CONFIG_H

/*
    Welcome to the WalledIn config file! Change this before you make so that you can change the defaults. Command line arguments will still work, this just changes default behavior.
*/

#define NOCOLOR -1  // Don't change this

// Change controls
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
// Key to quit the game
#define QUIT 'q'

// Change the interior wall character or player avatar character
#define WALLCHAR " "
#define PLAYERCHAR "A"

//OPTIONS: COLOR_RED, COLOR_BLACK, COLOR_WHITE, COLOR_GREEN, COLOR_MAGENTA, COLOR_CYAN, COLOR_LUE, NOCOLOR
#define WALLCOLOR COLOR_WHITE
#define WALLBACKGROUND COLOR_BLACK
#define PLAYERCOLOR COLOR_RED
#define PLAYERBACKGROUND COLOR_WHITE

#endif
