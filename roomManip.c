// contains any function related to "Rooms"
// such as
// room structure definition w/ following properties
// Room name|Room code|Room Description|4 pointers to other rooms. held in .h file
// holds roomCreate
// holds readRoomFile

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "roomManip.h"
#include "stringManip.h"

// roomCreate initializes all pointers
ROOM *roomCreate(ROOM *room)
{
    ROOM *copy = malloc(sizeof(ROOM)); // allocate memory for new room

    if (copy)
    {
        strcpy(copy->code, room->code);
        strcpy(copy->name, room->name);
        strcpy(copy->description, room->description);
        copy->north = NULL;
        copy->east = NULL;
        copy->south = NULL;
        copy->west = NULL;
    }
    return copy; // return new room copy
}

// to read the room file from dungeonroomlist and take information from it to populate rooms structure
ROOM *readRoomFile(const char *filename, int *roomCount)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Error opening file %s\n", filename);
        return NULL;
    }

    int roomCapacity = 2;
    int roomIndex = 0;
    ROOM *rooms = malloc(sizeof(ROOM) * roomCapacity);

    char line[2048];
    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "Room Name:", 10) == 0)
        {
            if (roomIndex >= roomCapacity)
            {
                roomCapacity *= 2; // double the room
                ROOM *temp = realloc(rooms, sizeof(ROOM) * roomCapacity);
                if (!temp)
                {
                    free(rooms); // free original memory
                    fclose(fp);
                    return NULL;
                }
                rooms = temp;
            }
            strcpy(rooms[roomIndex].name, str_trim(line + 10));
        }
        else if (strncmp(line, "Room Code:", 10) == 0)
        {
            strcpy(rooms[roomIndex].code, str_trim(line + 10));
        }
        else if (strncmp(line, "Room Description:", 17) == 0)
        {
            strcpy(rooms[roomIndex].description, str_trim(line + 17));
            while (fgets(line, sizeof(line), fp) && strcmp(str_trim(line), "") != 0)
            {
                strcat(rooms[roomIndex].description, "\n");
                strcat(rooms[roomIndex].description, line);
            }
            roomIndex++; // move to the next room after completing one entry
        }
    }
    fclose(fp);
    *roomCount = roomIndex;
    return rooms;
}