#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "roomManip.h"
#include "stringManip.h"
#include "monsterManip.h"

ROOM *createDungeon(ROOM *rooms, int roomCount, int dungeonSize) {
    if (dungeonSize <= 0 || roomCount <= 0)
    return NULL;

    srand(time(0));

    //make a grid of rooms and allocate memory for it
    ROOM **grid = malloc(dungeonSize * dungeonSize * sizeof(ROOM *));
    //handle if failed
    if (!grid) {
        printf("Failed to allocate memory for grid.\n");
        return NULL;
    }
    // list of possible loot items
    const char *lootItems[] = {"Gold", "Potion", "Sword", "Shield", "Gems", "Steel Armor", "Silver", "Ancient Map", "Torch", "Dragon Scale", "Lockpick Set", "Cursed Ring"};
    //calc how many there are
    int lootCount = sizeof(lootItems) / sizeof(lootItems[0]);
    // printf("lootCount = %d\n", lootCount);

    //create rooms and populate grid
    //iterates over each row of the dungeon grid
    //then iterates over each column of the current row
    for (int i = 0; i < dungeonSize; i++) {
        for (int j = 0; j < dungeonSize; j++) {
            //generates random number within the range of available rooms 
            int randomRoomIndex = rand() % roomCount;
            //creates a new ROOM structure based on a randomly selected room and returns a pointer to it 
            grid[i * dungeonSize + j] = roomCreate(&rooms[randomRoomIndex]); 
            
            //check if room creation fails
            //for loop goes through all previously created rooms and frees memory
            if (!grid[i * dungeonSize + j]) {
                printf("Failed to create room.\n");
                for (int k = 0; k < i * dungeonSize + j; k++){
                    free(grid[k]);
                }    
                return NULL;
            }
            grid[i * dungeonSize + j]->visited = 0;

            //randomly generates if room should have loot
            if (rand() % 9 == 0) { // 1 in 9 percent chance
                int lootIndex = rand() % lootCount;
                strcpy(grid[i * dungeonSize + j]->loot, lootItems[lootIndex]);
                grid[i * dungeonSize + j]->hasLoot = 1;
            } else {
                grid[i * dungeonSize + j]->hasLoot = 0;
            }
        }
    }

    //time to link the rooms
    //outer loop goes through row
    for (int i = 0; i < dungeonSize; i++) {
        for (int j = 0; j < dungeonSize; j++) {
            //current equals the room structure from previous for loops
            //2d grid accessed by i*dungeonSize+j
            ROOM *current = grid[i * dungeonSize + j];

            //links neighbors
            //if room above exists
            if(i > 0) current->north = grid[(i - 1) * dungeonSize + j];
            //if room below exists
            if(i < dungeonSize - 1) current->south = grid[(i + 1) * dungeonSize + j];
            //if room to the left exists
            if(j > 0) current->west = grid[i * dungeonSize + (j - 1)];
            //if room to the right exists
            if(j < dungeonSize - 1) current->east = grid[i * dungeonSize + (j + 1)];
        }
    }
    //point to the first room
    ROOM *head = grid[0];
    //free the initial grid
    free(grid);
    //return to start
    return head;
}

void printDungeon(ROOM *dungeon, int dungeonSize, ROOM *currentRoom) {
    if (!dungeon) return;

    ROOM *currentRow = dungeon;
    for (int i = 0; i < dungeonSize; i++) {
        ROOM *currentCol = currentRow;
        for (int j = 0; j < dungeonSize; j++) {
            if (currentCol == currentRoom) {
                printf("[*%s*]", currentCol->code);
                currentCol->visited = 1;
            } else if (currentCol->visited) {
                printf("[*XX*]");
            } else{
                printf("[    ]");
            }

            // ////////////////////////////////////////////////////
            // if (currentCol->hasLoot) {
            //     printf("[%s]", currentCol->loot);
            // }
            // ///////////////////////////////////////////////////
            currentCol = currentCol->east;
        }
        printf("\n");
        currentRow = currentRow->south;
    }
}

//go over dungeon and free memory row by row and column by column to ensure there are no memory leaks
void deleteDungeon(ROOM *dungeon) {
    if(!dungeon) return;
    ROOM *currentRow = dungeon;
    while(currentRow){
        ROOM *currentCol = currentRow;
        ROOM *nextRow = currentRow->south;
        while(currentCol){
            ROOM *nextCol = currentCol->east;
            free(currentCol);
            currentCol = nextCol;
        }
        currentRow = nextRow;
    }
}

int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Please use with: %s <nameofroomlist.txt> <monsters.txt>\n", argv[0]);
        return 1;
    }
    int roomCount;
    //array to store items
    char inventory[10][50]; //max of 10 items
    //initialize count to zero
    int inventoryCount = 0;
    ROOM *rooms = readRoomFile(argv[1], &roomCount);
    if (!rooms) {
        printf("Failed to load rooms.\n");
        return 1;
    }


    int monsterCount;
    MONSTER *monsters  = monsterRoomFile(argv[2], &monsterCount);
    if (!monsters) {
        printf("Failed to load monsters.\n");
        free(rooms);
        free(monsters);
        return 1;
    }

    // get the users name 
    printf("\033[5;36mEnter your name: \033[0;0m");
    char username[50];
    if(fgets(username, sizeof(username), stdin) == NULL) {
        printf("Not today pal.\n");
        free(rooms);
        free(monsters);
        return 1;
    };


    //get dungeon size from user
    printf("\033[5;36mEnter dungeon size: \033[0;0m");
    // printf("\u2592\n");
    char input[10];
    if(fgets(input, sizeof(input), stdin) == NULL) {
        printf("Not today pal.\n");
        free(rooms);
        free(monsters);
        return 1;
    };

    //convert input to integer and create dungeon
    int dungeonSize = atoi(input);
    ROOM *dungeon = createDungeon(rooms,roomCount, dungeonSize);
    // Ensure dungeonSize is within limits
    if (dungeonSize < 1 || dungeonSize > 10) {
        printf("Dungeon size must be between 1 and 10.\n");
        free(rooms);
        free(monsters);
        return 1;
    }
    //handle errors
    if (!dungeon) {
        printf("Failed to create dungeon.\n");
        free(rooms);
        free(monsters);
        return 1;
    }

    ROOM *currentRoom = dungeon;
    //loop to play game
    while(1){
        printf("\n");

        printDungeon(dungeon, dungeonSize, currentRoom);
        if (currentRoom->hasLoot) {
            printf("\033[33m\nCongratulations, you found some treasure! %s!\n", currentRoom->loot);

            currentRoom->hasLoot = 0;
            if (inventoryCount < 10){
                strcpy(inventory[inventoryCount++], currentRoom->loot);
                printf("%s has been added to the inventory.\n\033[0m", currentRoom->loot);
            } else {
                printf("Inventory is full. You can't pick up another item %s.\n", currentRoom->loot);
            }
            strcpy(currentRoom->loot, "");
        }
        printf("\033[32m\nCurrent Room Code:\033[0m %s, \033[32mRoom Name:\033[0m %s\n", currentRoom->code, currentRoom->name);

        printf("\033[32mDescription:\033[0m %s\n", currentRoom->description);
        printf("\n");


        printf("\033[32mAvailable rooms to exit:\033[0m ");
        if (currentRoom->north) printf("North ");
        if (currentRoom->east) printf("East ");
        if (currentRoom->south) printf("South ");
        if (currentRoom->west) printf("West ");
        printf("\n");

        printf("Make a move <n/e/s/w> or quit: ");
        if (fgets(input, sizeof(input), stdin) == NULL){
            printf("Not today pal.\n");
            continue;
        }
        str_trim(input);

        //add case statements for user input direction
        switch(input[0]){
            case 'N':
            case 'n':
                if (currentRoom->north) {
                    currentRoom->north->visited = 1;
                    currentRoom = currentRoom->north;
                    printf("\n\n\n");
                    printf("Moving north my leige...\n");
                } else {
                    printf("\n\n\n");
                    printf("You walk into a wall and break your nose.\n");
                }
                break;
            
            case 'E':
            case 'e':
                if (currentRoom->east) {
                    currentRoom->east->visited = 1;
                    currentRoom = currentRoom->east;
                    printf("\n\n\n");
                    printf("Moving east my leige...\n");
                } else {
                    printf("\n\n\n");
                    printf("East? I thought you said weest. Theres a wall there.\n");
                }
                break;

            case 'S':
            case 's':
                if (currentRoom->south) {
                    currentRoom->south->visited = 1;
                    currentRoom = currentRoom->south;
                    printf("\n\n\n");
                    printf("Moving south my leige...\n");
                } else {
                    printf("\n\n\n");
                    printf("If only you had a saw to get through this wall. Try again.\n");
                }
                break;
            
            case 'W':
            case 'w':
                if (currentRoom->west) {
                    currentRoom->west->visited = 1;
                    currentRoom = currentRoom->west;
                    printf("\n\n\n");
                    printf("Moving west my leige...\n");
                } else {
                    printf("\n\n\n");
                    printf("It's dark. You walk into a wall and break your toe.\n");
                }
                break;

            case 'Q':
            case 'q':
                if(strcmp(input, "quit") == 0 ||
                strcmp(input, "Q") == 0 ||
                strcmp(input, "q") == 0) {
                    printf("Goodbye!\n");
                    printf("\n");
                    deleteDungeon(dungeon);
                    free(monsters);
                    free(rooms);
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

    //clean up and exit
    //delete dungeon
    deleteDungeon(dungeon);
    //free rooms
    free(rooms);
    free(monsters);
    return 0;
}
