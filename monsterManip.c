#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "monsterManip.h"

char *str_troll(char *str) {
    char *end;

    // Trim leading whitespace
    while (isspace((unsigned char)*str)) str++;

    if (*str == '\0') // All spaces
        return str;

    // Trim trailing whitespace
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = '\0';

    return str;
}

 MONSTER *monsterRoomFile(const char *filename, int *monsterCount){
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }
    
    int monsterCapacity = 2; 
    int monsterIndex = 0; 
    MONSTER *monsters = malloc(sizeof(MONSTER) * monsterCapacity);

    if(!monsters){
        printf("Error allocating memory for monsters\n");
        fclose(fp);
        return NULL;
    }

    char line[50];

    while(fgets(line, sizeof(line), fp)) {
        if ((line[0] == '*')){
            if(monsterIndex >= monsterCapacity) {
                monsterCapacity *= 2;
                MONSTER *temp = realloc(monsters, sizeof(MONSTER) * monsterCapacity);
                if(!temp){
                    printf("Error reallocating memory for monsters\n");
                    free(monsters);
                    fclose(fp);
                    return NULL;
                }
                monsters = temp;
            }

            //trim line for monster
            strcpy(monsters[monsterIndex].name, line);

            while(fgets(line, sizeof(line), fp) && strcmp(str_troll(line), "") !=0){
                strcat(monsters[monsterIndex].name, "\n");
                strcat(monsters[monsterIndex].name, line);
            }

            monsterIndex++;
            
        }

    }
    
    fclose(fp);
    *monsterCount = monsterIndex;
    return monsters;
 }