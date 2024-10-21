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
}

ROOM *readroomFile(const char *filename, int *roomCount){
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;
}