#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Node
{
    int id;
    int connections[4]; //list of id's
    int connectionsindex;

    char name[20];
    char description[150];
};

struct Tree
{
    struct Node nodelist[150];
    int index;

    int depth;
};

void InitializeNode(struct Node *room, int id, char name[], char desc[])
{
    room->id = id;
    for (int i = 0; i < 4; i++)
        room->connections[i] = -1;

    room->connectionsindex = 0;

    strcpy(room->name, name);
    strcpy(room->description, desc);
}

void InitalizeTree(struct Tree *mytree, int depth)
{
    mytree->index = 0;
    mytree->depth = depth;
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

void RecursiveTreeGeneration(struct Node *parentNode, struct Tree *myTree, int depth)
{
    if (depth == 0) return;
    //choose a random number of branches between 2 and 3
    int numberOfBranches = 2;
    if (randomRange(0, 1) >= 0.5) numberOfBranches = 3;

    for (int i = 0; i < numberOfBranches; i++)
    {
        //create a new node
        char name[20];
        char description[150];

        char roomLibrary[6][2][150] = 
        {{"Asylum", "dark, empty, sad"},
        {"Mansion", "very modern"},
        {"Bakery", "bread is dead stinky"},
        {"High School", "kids used to be here ew"},
        {"Greg's Basement", "I'd rather not say..."},
        {"Theatre", "large echo echo echo echo"}};
        int librarySize = 6;

        int randomLibraryIndex = (int)randomRange(0, librarySize - 0.00001);

        InitializeNode(&myTree->nodelist[myTree->index], myTree->index, roomLibrary[randomLibraryIndex][0], roomLibrary[randomLibraryIndex][1]);

        //give it connections to parent and vice versa
        parentNode->connections[parentNode->connectionsindex] = myTree->index;
        parentNode->connectionsindex += 1;
        myTree->nodelist[myTree->index].connections[0] = parentNode->id;
        myTree->nodelist[myTree->index].connectionsindex += 1;
        int temp = myTree->nodelist[myTree->index].connectionsindex;

        //increment index
        myTree->index++;

        //call function on children (50% chance?)
        if (randomRange(0, 1) >= 0.5)
        {
            RecursiveTreeGeneration(&myTree->nodelist[myTree->index-1], myTree, depth - 1);
        }
    }
}

int main()
{
    srand(time(NULL)); //time(NULL)

    struct Tree myTree;
    InitalizeTree(&myTree, 10);

    struct Node* currentNode = &myTree.nodelist[0];

    //declare rooms manually for testing
    InitializeNode(&myTree.nodelist[myTree.index], myTree.index, "Castle", "brrr spooky castle");
    myTree.index++;

    RecursiveTreeGeneration(currentNode, &myTree, 10);

    /*
    InitializeNode(&tree[1], 1, "Graveyard", "kinda spooky!");
    tree[1].connections[0] = 0;

    InitializeNode(&tree[2], 2, "Fields", "large, flat, alberta");
    tree[2].connections[0] = 0;
    tree[2].connections[1] = 3;

    InitializeNode(&tree[3], 3, "Raspberry Patch", "dark and gloomy, no raspberries!");
    tree[3].connections[0] = 2;
    */

    int gameEnd = 0;
    while(!gameEnd)
    {
        DisplayNode(*currentNode, myTree.nodelist);
        printf("Which path would you like to take?: ");
        int chosenPath = -1;

        while ((scanf("%i", &chosenPath) != 1) || !ValidPath(*currentNode, chosenPath))
            printf("Invalid selection. Which path would you like to take?: ");

        currentNode = &myTree.nodelist[chosenPath];
    }
}