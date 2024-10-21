//Where the program begins. takes roomList as a command line argument. holds the following functions
//createDungeon: takes the pointer to the room array and picks random room in dungeon (create copy of the room then add to the linked list)Make dungeon a 2d linked list that is bi-directional
//printDungeon:takes pointer to start of dungeon, loops through the linked list, prints off room code name in small format
//deleteDungeon:takes pointer to start of dungeon and frees memory allocated for entire dungeon
//main: creates room array from the file, asks user for size of dungeon to be created, creates dungeon that size, prints dungeon, deletes dungeon.

#include <stdio.h>
#include <stdlib.h>
#include "roomManip.h"
#include "stringManip.h"


int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Please use with: %s <nameofroomlist.txt>\n", argv[0]);
        return 1;
    }
    int roomCount;
    ROOM *rooms = readRoomFile(argv[1], &roomCount);
    if (!rooms) {
        printf("Failed to load rooms.\n");
        return 1;
    }
    
    printf("Enter dungeon size: ");
    int dungeonSize;
    char input[10];
    fgets(input, sizeof(input), stdin);

    //TODO: add dungeoncreate and print.
    //free rooms
    free(rooms);
    return 0;
}
