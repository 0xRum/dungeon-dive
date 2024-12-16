#ifndef MONSTERMANIP_H
#define MONSTERMANIP_H

typedef struct MONSTER MONSTER;

struct MONSTER {
    char name[2048];
};


char *str_troll(char *str);
MONSTER *monsterRoomFile(const char *fileName, int *monsterCount);

#endif