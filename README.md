# dungeon-dive

## Overview:

This is a simple dungeon crawler game made with C programming language where the player can navigate through a randomly generated dungeon after entering their username and dungeon size.

## Features:

-They can collect loot and encounter monsters.

-After entering a dungeon size, it constructs a 2d grid of rooms and assigns random loot to the rooms with a 14.28% chance of having loot per room. The rooms are also linked directionally; North, South, East, and West.

-The `main` game loop initializes the game by reading room and monster data from text files.

-Then the player can move through the dungeon by using:`n`,`e`,`s`,`w`.

-The file is written to '`userData.txt`' when the user enters "`q`" or "`quit`" the `game summary` displays.

-This all provides a basic level of interaction for the user.

**_Something not explicitly taught_**:
Text colors. Adding colors to ingame text to break apart the walls of white text. Shows game mechanics that are going on.

## Run Game:

To run the game:

`make` to first compile the game.

Then run:
`./main dungeonRoomList_large.txt monsters.txt`

The game will then start to run, enter your name and the size of the dungeon you want to play. (between 5 & 15 is the min and max size of the game).

To clean up the files after use `make clean`

Happy Gaming!
