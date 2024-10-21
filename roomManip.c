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
    if (room == NULL) return NULL;

    ROOM *copy = malloc(sizeof(ROOM));
    if (!copy) return NULL;
    *copy = *room;
}

ROOM *readroomFile(const char *filename, int *roomCount){
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }
    int roomba = 2;
    int nextRooms = 0;
    ROOM *rooms = malloc(sizeof(ROOM) * roomba);
}