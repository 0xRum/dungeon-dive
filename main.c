//Where the program begins. takes roomList as a command line argument. holds the following functions
//createDungeon: takes the pointer to the room array and picks random room in dungeon (create copy of the room then add to the linked list)Make dungeon a 2d linked list that is bi-directional
//printDungeon:takes pointer to start of dungeon, loops through the linked list, prints off room code name in small format
//deleteDungeon:takes pointer to start of dungeon and frees memory allocated for entire dungeon
//main: creates room array from the file, asks user for size of dungeon to be created, creates dungeon that size, prints dungeon, deletes dungeon.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "roomManip.h"
#include "stringManip.h"

//TODO:createDungeon: make pointers to the start room and pointers to the current room
//add randomizier
//create new copy of room
//printDungeon function: should print current room for west to east
//deleteDungeon function: should delete all rooms and free memory
// 
ROOM *createDungeon(ROOM *rooms, int roomCount, int dungeonSize) {
    if (dungeonSize <= 0 || roomCount <= 0)
    return NULL;

    srand(time(0));

    ROOM *head = NULL; //ptr to start of room
    ROOM *current = NULL; //ptr to current room
}

void printDungeon(ROOM *dungeon) {
    ROOM *current = dungeon;
}

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
    char input[10];
    if(fgets(input, sizeof(input), stdin) == NULL) {
        printf("Not today pal.\n");
        free(rooms);
        return 1;
    };

    // clean up dungeon(delete)
    int dungeonSize = atoi(input);
    ROOM *dungeon = createDungeon(rooms,roomCount, dungeonSize);
    //handle errors
    //free up memory
    //print dungeon
    printDungeon(dungeon);
    //delete dungeon
    //free rooms
    free(rooms);
    return 0;
}
