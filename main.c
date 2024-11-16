//Where the program begins. takes roomList as a command line argument. holds the following functions
//createDungeon: takes the pointer to the room array and picks random room in dungeon (create copy of the room then add to the linked list)Make dungeon a 2d linked list that is bi-directional
//printDungeon:takes pointer to start of dungeon, loops through the linked list, prints off room code name in small format
//deleteDungeon:takes pointer to start of dungeon and frees memory allocated for entire dungeon
//main: creates room array from the file, asks user for size of dungeon to be created, creates dungeon that size, prints dungeon, deletes dungeon.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "roomManip.h"
#include "stringManip.h"

ROOM *createDungeon(ROOM *rooms, int roomCount, int dungeonSize) {
    if (dungeonSize <= 0 || roomCount <= 0)
    return NULL;

    srand(time(0));

    //make a grid of rooms and allocate memory for it
    ROOM **grid = malloc(dungeonSize * dungeonSize * sizeof(ROOM *));
    if (!grid) {
        printf("Failed to allocate memory for grid.\n");
        return NULL;
    }

    //create rooms and populate grid
    for (int i = 0; i < dungeonSize; i++) {
        for (int j = 0; j < dungeonSize; j++) {
            int randomRoomIndex = rand() % roomCount;
            grid[i * dungeonSize + j] = roomCreate(&rooms[randomRoomIndex]);
            if (!grid[i * dungeonSize + j]) {
                printf("Failed to create room.\n");
                return NULL;
            }
        }
    }

    //time to link the rooms
    for (int i = 0; i < dungeonSize; i++) {
        for (int j = 0; j < dungeonSize; j++) {
            ROOM *current = grid[i * dungeonSize + j];
            if(i > 0) current->north = grid[(i - 1) * dungeonSize + j];
            if(i < dungeonSize - 1) current->south = grid[(i + 1) * dungeonSize + j];
            if(j > 0) current->west = grid[i * dungeonSize + (j - 1)];
            if(j < dungeonSize - 1) current->east = grid[i * dungeonSize + (j + 1)];
        }
    }

    // ROOM *head = NULL; //ptr to start of room
    // ROOM *current = NULL; //ptr to current room

    // for(int i = 0; i < dungeonSize; i++) {
    //     //select random room from room array
    //     int randomRoomIndex = rand() % roomCount;
    //     ROOM *newRoom = roomCreate(&rooms[randomRoomIndex]);//creates new room copy

    //     //error handling
    //     if (!newRoom) {
    //         printf("Failed to create room.\n");
    //         return NULL;
    //     }

    //     if (head == NULL){
    //         head = newRoom; //sets first newroom as head
    //     } else {
    //         current->east = newRoom;
    //         newRoom->west = current;
    //     }
    //     current = newRoom; //move to newroom in the dungeon
    // }
    ROOM *head = grid[0];
    return head; //return to start
}

void printDungeon(ROOM *dungeon) {
    ROOM *current = dungeon;
    while (current != NULL) {
        printf("Room Code: %s, Room Name: %s\n", current->code, current->name);
        current = current->east; //move to next room, east
    }
}

void deleteDungeon(ROOM *dungeon, int dungeonSize) {
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
    
    //get dungeon size from user
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

    ROOM *currentRoom = dungeon;
    while(1){
        printf("\nCurrent Room Code: %s, Room Name: %s\n", currentRoom->code, currentRoom->name);
        printf("Description: %s\n", currentRoom->description);
        printf("Rooms to the: ");

        if (currentRoom->north) printf("North ");
        if (currentRoom->east) printf("East ");
        if (currentRoom->south) printf("South ");
        if (currentRoom->west) printf("West ");
        printf("\n");

        printf("What would you like to do? <n/e/s/w> or quit: ");
        if (fgets(input, sizeof(input), stdin) == NULL){
            printf("Not today pal.\n");
            continue;
        }
        str_trim(input);

        switch(input[0]){
            case 'N':
            case 'n':
            if (currentRoom->north) {
                currentRoom = currentRoom->north;
                printf("\n");
                printf("Moving north my leige...\n");
            } else {
                printf("You can't go that way.\n");
            }
            break;
            
            case 'E':
            case 'e':
            if (currentRoom->east) {
                currentRoom = currentRoom->east;
                printf("\n");
                printf("Moving east my leige...\n");
            } else {
                printf("You can't go that way.\n");
            }
            break;

            case 'S':
            case 's':
            if (currentRoom->south) {
                currentRoom = currentRoom->south;
                printf("\n");
                printf("Moving south my leige...\n");
            } else {
                printf("You can't go that way.\n");
            }
            break;
            
            case 'W':
            case 'w':
            if (currentRoom->west) {
                currentRoom = currentRoom->west;
                printf("\n");
                printf("Moving west my leige...\n");
            } else {
                printf("You can't go that way.\n");
            }
            break;

            case 'Q':
            case 'q':
            if(strcmp(input, "quit") == 0) {
                printf("Goodbye!\n");
                return 0; //exit the loop
            } else {
                printf("Invalid command. Please try again.\n");
            }
            break;

            default:
            printf("Invalid command. Please try again.\n");
            break;
        }
    }

    //print dungeon
    // printDungeon(dungeon);
    //delete dungeon
    deleteDungeon(dungeon, dungeonSize);
    //free rooms
    free(rooms);
    return 0;
}
