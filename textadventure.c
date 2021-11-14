#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Node
{
    int id;
    int connections[4]; //list of id's

    char name[20];
    char description[150];
};

void InitializeNode(struct Node *room, int id, char name[], char desc[])
{
    room->id = id;
    for (int i = 0; i < 4; i++)
        room->connections[i] = -1;

    strcpy(room->name, name);
    strcpy(room->description, desc);
}

void DisplayNode(struct Node room, struct Node tree[50])
{
    printf("==================================================\n"); 
    printf("Name: %s\n\n", room.name);
    printf("Description: %s\n\n", room.description);
    printf("==================================================\n"); 

    for (int i = 0; i < 4; i++)
        if (room.connections[i] != -1) printf ("[%i] - %s\t", room.connections[i], tree[room.connections[i]].name);

    printf("\n\n");
}

int ValidPath(struct Node room, int moveId)
{
    for (int i = 0; i < 4; i++)
        if (room.connections[i] == moveId) return 1;

    return 0;
}

float randomRange(float low, float high)
{
    return ((float)rand() / RAND_MAX) * (high - low) + low;  
}

void RecursiveTreeGeneration(struct Node startnode, int depth)
{
    //To be made :)
}

int main()
{
    srand(0);

    struct Node tree[50];
    struct Node* currentNode = &tree[0];

    //declare rooms manually for testing
    InitializeNode(&tree[0], 0, "Castle", "brrr spooky castle");
    tree[0].connections[0] = 1;
    tree[0].connections[1] = 2;

    InitializeNode(&tree[1], 1, "Graveyard", "kinda spooky but also hot");
    tree[1].connections[0] = 0;

    InitializeNode(&tree[2], 2, "Fields", "large, flat, alberta");
    tree[2].connections[0] = 0;
    tree[2].connections[1] = 3;

    InitializeNode(&tree[3], 3, "Raspberry Patch", "dark and gloomy, no raspberries!");
    tree[3].connections[0] = 2;

    int gameEnd = 0;
    while(!gameEnd)
    {
        DisplayNode(*currentNode, tree);
        printf("Which path would you like to take?: ");
        int chosenPath = -1;

        while ((scanf("%i", &chosenPath) != 1) || !ValidPath(*currentNode, chosenPath))
            printf("Invalid selection. Which path would you like to take?: ");

        currentNode = &tree[chosenPath];
    }
}