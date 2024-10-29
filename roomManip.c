//contains any function related to "Rooms"
//such as
//room structure definition w/ following properties
//Room name|Room code|Room Description|4 pointers to other rooms. held in .h file
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
    
    int roomCapacity = 2; 
    int roomIndex = 0; 
    ROOM *rooms = malloc(sizeof(ROOM) * roomCapacity);

    char line[2048];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "Room Name:", 10) == 0) {
            if (roomIndex >= roomCapacity) {
                roomCapacity *= 2;  // double the room
                ROOM *temp = realloc(rooms, sizeof(ROOM) * roomCapacity);
                if (!temp) {
                    free(rooms);  // free original memory
                    fclose(fp);
                    return NULL;
                }
                rooms = temp;

            }
            strcpy(rooms[roomIndex].name, str_trim(line + 10));
        } else if (strncmp(line, "Room Code:", 10) == 0) {
            strcpy(rooms[roomIndex].code, str_trim(line + 10));
        } else if (strncmp(line, "Room Description:", 17) == 0) {
            strcpy(rooms[roomIndex].description, str_trim(line + 17));
            roomIndex++; // move to the next room after completing one entry
        }
    }
    fclose(fp);
    *roomCount = roomIndex;
    return rooms;
}