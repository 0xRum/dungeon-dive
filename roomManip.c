//contains any function related to "Rooms"
//such as
//room structure definition w/ following properties
//Room name|Room code|Room Description|4 pointers to other rooms.

//holds roomCreate
//holds readRoomFile
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "roomManip.h"
#include "stringManip.h"

ROOM *roomCreate(ROOM *room){
    ROOM *copy = malloc(sizeof(ROOM)); //allocate memory for enw room
    if (!copy) return NULL;
    *copy = *room; //copy room contents
    return copy; //return new room copy
}

ROOM *readRoomFile(const char *filename, int *roomCount){
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }
    
    int roomba = 2; //starts with 2 rooms
    int nextRooms = 0; //next rooms begins at 0
    ROOM *rooms = malloc(sizeof(ROOM) * roomba);
    char line[2048];

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "Room Name:", 10) == 0) {
            if (roomba >= nextRooms) {
                nextRooms *= 2;  // double the room
                ROOM *temp = realloc(rooms, sizeof(ROOM) * nextRooms);
                if (!temp) {
                    free(rooms);  // free original memory
                    return NULL;
                }
                rooms = temp;

            }
            strcpy(rooms[roomba].name, str_trim(line + 10));
        } else if (strncmp(line, "Room Code:", 10) == 0) {
            strcpy(rooms[roomba].code, str_trim(line + 10));
        } else if (strncmp(line, "Room Description:", 17) == 0) {
            strcpy(rooms[roomba].description, str_trim(line + 17));
            roomba++; // move to the next room after completing one entry
        }
    }
    fclose(fp);
    *roomCount = nextRooms;
    return rooms;
}