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

ROOM *createDungeon(ROOM *rooms, int roomCount, int dungeonSize) {
    if (dungeonSize <= 0 || roomCount <= 0)
    return NULL;

    srand(time(0));

    ROOM *head = NULL; //ptr to start of room
    ROOM *current = NULL; //ptr to current room

    for(int i = 0; i < dungeonSize; i++) {
        //select random room from room array
        int randomRoomIndex = rand() % roomCount;
        ROOM *newRoom = roomCreate(&rooms[randomRoomIndex]);//creates new room copy

        //error handling
        if (!newRoom) {
            printf("Failed to create room.\n");
            return NULL;
        }

        if (head == NULL){
            head = newRoom; //sets first newroom as head
        } else {
            current->east = newRoom;
            newRoom->west = current;
        }
        current = newRoom; //move to newroom in the dungeon
    }
    return head; //return to start
}

void printDungeon(ROOM *dungeon) {
    ROOM *current = dungeon;
    while (current != NULL) {
        printf("Code: %s, Name: %s\n", current->code, current->name);
        current = current->east; //move to next room, east
    }
}

void deleteDungeon(ROOM *dungeon) {
    ROOM *current = dungeon;
    while (current != NULL) {
        ROOM *next = current->east; //store the next room
        free(current); //free current
        current = next; //move to the next room
    }
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

    //convert input to integer and create dungeon
    int dungeonSize = atoi(input);
    ROOM *dungeon = createDungeon(rooms,roomCount, dungeonSize);
    //handle errors
    if (!dungeon) {
        printf("Failed to create dungeon.\n");
        free(rooms);
        return 1;
    }
    //print dungeon
    printDungeon(dungeon);
    //delete dungeon
    deleteDungeon(dungeon);
    //free rooms
    free(rooms);
    return 0;
}
