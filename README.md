# Walled In
Retro command line game of not touching the walls. Great for wasting time at work! Appearance is fully customizable through command line arguments, see below.

![Show-off video](https://github.com/harrinp/WalledIn/blob/master/Examples/walledinvid.gif "Show off Video")

## Install
```
    git clone https://github.com/harrinp/WalledIn.git
    cd WalledIn
    make install
```
Default install location is usr/local/bin

## Directions

Launch by typing `walledin`

Control with WASD

You get more points the further up you are on screen

Try not to hit the walls

## Details

Fully implemented in c

Colors differ depending on your terminal and preferences

### Command Line arguments
```
 -h            --> help
 -cw arg1 arg2 --> Set color of walls (Foreground) (Background)
 -cp arg1 arg2 --> Set color of player (Foreground) (Background)
                -> Available color arguments: RED, BLACK, WHITE, GREEN, MAGENTA, CYAN, BLUE, NOCOLOR
 -ncp          --> Shortcut to not color player
 -ncw          --> Shortcut to not color wall
 --nocolor     --> Don't color anything (terminal defaults used)
 -nc           --> Shortcut to not color anything

 -p arg1       --> Change the character that the player is displayed as. Limit 1 character
 -w arg1       --> Change the character that the inside of the walls are displayed as. Default is . Limit 1 character
 -t arg1       --> Change the number of tunnels that will exist at any time. Limit 100

 --nocollision --> Turn off collision mechanics, game runs until you quit
 -ncol         --> Shortcut to no collision
```

### Example Configurations

`Args : walledin`
![Picture 1](https://github.com/harrinp/WalledIn/blob/master/Examples/Empty_Walls.png "PIC1")

`Args : walledin -cpn -cw WHITE PINK -w + -p I -t 3`
![Picture 2](https://github.com/harrinp/WalledIn/blob/master/Examples/PinkWithI.png "PIC2")

For extra challenge you can not color the background and use no character in the walls.
`Args : walledin -cp BLUE NOCOLOR -cw CYAN NOCOLOR`
![Picture 3](https://github.com/harrinp/WalledIn/blob/master/Examples/DefaultBackBlue.png "PIC3")
