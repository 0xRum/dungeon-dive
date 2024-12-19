#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "roomManip.h"
#include "stringManip.h"
#include "monsterManip.h"

ROOM *createDungeon(ROOM *rooms, int roomCount, int dungeonSize)
{
    if (dungeonSize <= 0 || roomCount <= 0)
        return NULL;

    srand(time(0));

    // make a grid of rooms and allocate memory for it
    ROOM **grid = malloc(dungeonSize * dungeonSize * sizeof(ROOM *));
    // handle if failed
    if (!grid)
    {
        printf("Failed to allocate memory for grid.\n");
        return NULL;
    }
    // list of possible loot items
    const char *lootItems[] = {"Gold", "Potion", "Sword", "Shield", "Gems", "Steel Armor", "Silver", "Ancient Map", "Torch", "Dragon Scale", "Lockpick Set", "Cursed Ring"};
    // calc how many there are
    int lootCount = sizeof(lootItems) / sizeof(lootItems[0]);
    // printf("lootCount = %d\n", lootCount);

    // create rooms and populate grid
    // iterates over each row of the dungeon grid
    // then iterates over each column of the current row
    for (int i = 0; i < dungeonSize; i++)
    {
        for (int j = 0; j < dungeonSize; j++)
        {
            // generates random number within the range of available rooms
            int randomRoomIndex = rand() % roomCount;
            // creates a new ROOM structure based on a randomly selected room and returns a pointer to it
            grid[i * dungeonSize + j] = roomCreate(&rooms[randomRoomIndex]);

            // check if room creation fails
            // for loop goes through all previously created rooms and frees memory
            if (!grid[i * dungeonSize + j])
            {
                printf("Failed to create room.\n");
                for (int k = 0; k < i * dungeonSize + j; k++)
                {
                    free(grid[k]);
                }
                return NULL;
            }
            grid[i * dungeonSize + j]->visited = 0;

            // randomly generates if room should have loot
            if (rand() % 7 == 0)
            { // 1 in 9 percent chance. 14.28%
                int lootIndex = rand() % lootCount;
                strcpy(grid[i * dungeonSize + j]->loot, lootItems[lootIndex]);
                grid[i * dungeonSize + j]->hasLoot = 1;
            }
            else
            {
                grid[i * dungeonSize + j]->hasLoot = 0;
            }
        }
    }

    // time to link the rooms
    // outer loop goes through row
    for (int i = 0; i < dungeonSize; i++)
    {
        for (int j = 0; j < dungeonSize; j++)
        {
            // current equals the room structure from previous for loops
            // 2d grid accessed by i*dungeonSize+j
            ROOM *current = grid[i * dungeonSize + j];

            // links neighbors
            // if room above exists
            if (i > 0)
                current->north = grid[(i - 1) * dungeonSize + j];
            // if room below exists
            if (i < dungeonSize - 1)
                current->south = grid[(i + 1) * dungeonSize + j];
            // if room to the left exists
            if (j > 0)
                current->west = grid[i * dungeonSize + (j - 1)];
            // if room to the right exists
            if (j < dungeonSize - 1)
                current->east = grid[i * dungeonSize + (j + 1)];
        }
    }
    // point to the first room
    ROOM *head = grid[0];
    // free the initial grid
    free(grid);
    // return to start
    return head;
}
void writeUserData(const char *username, char inventory[][50], int inventoryCount)
{
    FILE *fp = fopen("userData.txt", "w");
    if (!fp)
    {
        printf("Failed to open userData.txt\n");
        return;
    }
    fprintf(fp, "User: %s\n", username);
    fprintf(fp, "Inventory:\n");
    for (int i = 0; i < inventoryCount; i++)
    {
        fprintf(fp, "- %s\n", inventory[i]);
    }
    fclose(fp);
}

void printUserData()
{
    FILE *fp = fopen("userData.txt", "r");
    if (!fp)
    {
        printf("Failed to open userData.txt\n");
        return;
    }
    char line[100];
    printf("\n\033[32mGame Summary:\033[0m\n");
    while (fgets(line, sizeof(line), fp))
    {
        printf("%s", line);
    }
    fclose(fp);
}

void printMonsters(MONSTER *monsters, int monsterCount)
{
    printf("\nMonsters in the dungeon:\n");
    for (int i = 0; i < monsterCount; i++)
    {
        printf("%s", monsters[i].name);
    }
}

void placeMonstersInDungeon(ROOM *dungeon, int dungeonSize, MONSTER *monsters, int monsterCount)
{
    srand(time(0));
    for (int i = 0; i < monsterCount; i++)
    {
        int randomRow = rand() % dungeonSize;
        int randomCol = rand() % dungeonSize;
        ROOM *room = dungeon;
        for (int j = 0; j < randomRow; j++)
        {
            if (room->south)
                room = room->south;
        }
        for (int k = 0; k < randomCol; k++)
        {
            if (room->east)
                room = room->east;
        }

        /////////////////////////////////////////////////
        strcpy(monsters[i].roomCode, room->code);
        // printf("Monster %s placed in room %s (%s)\n", monsters[i].name, room->code, room->name);
        /////////////////////////////////////////////////
    }
}
void printMonstersInRoom(MONSTER *monsters, int monsterCount, const char *roomCode)
{
    // printf("\e[1;31mMonsters in this room:\033[0m\n");s

    int found = 0;
    for (int i = 0; i < monsterCount; i++)
    {
        if (strcmp(monsters[i].roomCode, roomCode) == 0)
        {
            printf("\e[1;31mPrepare to get eaten!!!!!\033[0m\n");
            sleep(1.75);
            printf("%s\n", monsters[i].name);
            found = 1;
        }
    }
    if (!found)
    {
        printf("You look around, there are no monsters in this room.\n\n");
    }
}

void printDungeon(ROOM *dungeon, int dungeonSize, ROOM *currentRoom)
{
    if (!dungeon)
        return;

    ROOM *currentRow = dungeon;
    for (int i = 0; i < dungeonSize; i++)
    {
        ROOM *currentCol = currentRow;
        for (int j = 0; j < dungeonSize; j++)
        {
            if (currentCol == currentRoom)
            {
                printf("[*%s*]", currentCol->code);
                currentCol->visited = 1;
            }
            else if (currentCol->visited)
            {
                printf("[*XX*]");
            }
            else
            {
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

// go over dungeon and free memory row by row and column by column to ensure there are no memory leaks
void deleteDungeon(ROOM *dungeon)
{
    if (!dungeon)
        return;
    ROOM *currentRow = dungeon;
    while (currentRow)
    {
        ROOM *currentCol = currentRow;
        ROOM *nextRow = currentRow->south;
        while (currentCol)
        {
            ROOM *nextCol = currentCol->east;
            free(currentCol);
            currentCol = nextCol;
        }
        currentRow = nextRow;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Please use with: %s <nameofroomlist.txt> <monsters.txt>\n", argv[0]);
        return 1;
    }
    int roomCount;
    // array to store items
    char inventory[10][50]; // max of 10 items
    // initialize count to zero
    int inventoryCount = 0;
    ROOM *rooms = readRoomFile(argv[1], &roomCount);
    if (!rooms)
    {
        printf("Failed to load rooms.\n");
        return 1;
    }

    int monsterCount;
    MONSTER *monsters = monsterRoomFile(argv[2], &monsterCount);
    if (!monsters)
    {
        printf("Failed to load monsters.\n");
        free(rooms);
        free(monsters);
        return 1;
    }

    //////////////////////////////////////////
    // printMonsters(monsters, monsterCount);
    //////////////////////////////////////////

    // get the users name
    printf("\033[5;36mEnter your name: \033[0;0m");
    char username[50];
    if (fgets(username, sizeof(username), stdin) == NULL)
    {
        printf("Not today pal.\n");
        free(rooms);
        free(monsters);
        return 1;
    };
    username[strcspn(username, "\n")] = 0;

    // get dungeon size from user
    printf("\033[5;36mEnter dungeon size: \033[0;0m");
    // printf("\u2592\n");
    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        printf("Not today pal.\n");
        free(rooms);
        free(monsters);
        return 1;
    };

    // convert input to integer and create dungeon
    int dungeonSize = atoi(input);
    ROOM *dungeon = createDungeon(rooms, roomCount, dungeonSize);
    // Ensure dungeonSize is within limits
    if (dungeonSize < 5 || dungeonSize > 15)
    {
        printf("Dungeon size must be between 5 and 15.\n");
        free(rooms);
        free(monsters);
        return 1;
    }
    // handle errors
    if (!dungeon)
    {
        printf("Failed to create dungeon.\n");
        free(rooms);
        free(monsters);
        return 1;
    }

    placeMonstersInDungeon(dungeon, dungeonSize, monsters, monsterCount);

    ROOM *currentRoom = dungeon;
    // loop to play game
    while (1)
    {
        printf("\n");

        printDungeon(dungeon, dungeonSize, currentRoom);
        if (currentRoom->hasLoot)
        {
            printf("\033[33m\nCongratulations, you found some treasure! %s!\n", currentRoom->loot);

            currentRoom->hasLoot = 0;
            if (inventoryCount < 10)
            {
                strcpy(inventory[inventoryCount++], currentRoom->loot);
                printf("%s has been added to the inventory.\n\033[0m", currentRoom->loot);
            }
            else
            {
                printf("Inventory is full. You can't pick up another item %s.\n", currentRoom->loot);
            }
            strcpy(currentRoom->loot, "");
        }
        printf("\033[32m\nCurrent Room Code:\033[0m %s, \033[32mRoom Name:\033[0m %s\n", currentRoom->code, currentRoom->name);

        printf("\033[32mDescription:\033[0m %s\n", currentRoom->description);
        printf("\n");
        printMonstersInRoom(monsters, monsterCount, currentRoom->code);

        printf("\033[32mAvailable rooms to exit:\033[0m ");
        if (currentRoom->north)
            printf("North ");
        if (currentRoom->east)
            printf("East ");
        if (currentRoom->south)
            printf("South ");
        if (currentRoom->west)
            printf("West ");
        printf("\n");

        printf("Make a move <n/e/s/w> or quit: ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Not today pal.\n");
            continue;
        }
        str_trim(input);

        // add case statements for user input direction
        switch (input[0])
        {
        case 'N':
        case 'n':
            if (currentRoom->north)
            {
                currentRoom->north->visited = 1;
                currentRoom = currentRoom->north;
                printf("\n\n\n");
                printf("Moving north my leige...\n");
            }
            else
            {
                printf("\n\n\n");
                printf("You walk into a wall and break your nose.\n");
            }
            break;

        case 'E':
        case 'e':
            if (currentRoom->east)
            {
                currentRoom->east->visited = 1;
                currentRoom = currentRoom->east;
                printf("\n\n\n");
                printf("Moving east my leige...\n");
            }
            else
            {
                printf("\n\n\n");
                printf("East? I thought you said weest. Theres a wall there.\n");
            }
            break;

        case 'S':
        case 's':
            if (currentRoom->south)
            {
                currentRoom->south->visited = 1;
                currentRoom = currentRoom->south;
                printf("\n\n\n");
                printf("Moving south my leige...\n");
            }
            else
            {
                printf("\n\n\n");
                printf("If only you had a saw to get through this wall. Try again.\n");
            }
            break;

        case 'W':
        case 'w':
            if (currentRoom->west)
            {
                currentRoom->west->visited = 1;
                currentRoom = currentRoom->west;
                printf("\n\n\n");
                printf("Moving west my leige...\n");
            }
            else
            {
                printf("\n\n\n");
                printf("It's dark. You walk into a wall and break your toe.\n");
            }
            break;

        case 'Q':
        case 'q':
            if (strcmp(input, "quit") == 0 ||
                strcmp(input, "Q") == 0 ||
                strcmp(input, "q") == 0)
            {
                writeUserData(username, inventory, inventoryCount);
                // printf("\nUser data has been written to file.\n");
                printUserData();
                printf("\n\033[31mGoodbye!\033[0m\n");
                printf("\n");
                deleteDungeon(dungeon);
                free(monsters);
                free(rooms);
                return 0; // exit the loop
            }
            else
            {
                printf("Invalid command. Please try again.\n");
            }
            break;

        default:
            printf("Invalid command. Please try again.\n");
            break;
        }
    }

    // clean up and exit
    // delete dungeon
    deleteDungeon(dungeon);
    // free rooms
    free(rooms);
    free(monsters);
    return 0;
}
