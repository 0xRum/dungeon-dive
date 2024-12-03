//header file for roomManip

#ifndef ROOMMANIP_H
#define ROOMMANIP_H

typedef struct ROOM ROOM;

struct ROOM{
    char code[10];
    char name[256];
    char description[1024];
    char riddle[256];
    int visited;
    ROOM *north, *east, *south, *west;
};

ROOM *readRoomFile(const char *filename, int *roomCount);
ROOM *roomCreate(ROOM *room);
#endif
