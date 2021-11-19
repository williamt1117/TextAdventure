#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define CHILDSPAWNPERCENTAGE 0.60
#define DEPTH 8

struct Node
{
    int id;
    int connections[4]; //list of id's
    int connectionsindex;

    char name[30];
    char description[150];
};

struct Tree
{
    struct Node nodelist[5000];
    int index;
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

void InitalizeTree(struct Tree *mytree)
{
    mytree->index = 0;
}

void DisplayNode(struct Node room, struct Node tree[50])
{
    printf("==================================================\n"); 
    printf("Name: %s\n\n", room.name);
    printf("Description: %s\n\n", room.description);
    printf("==================================================\n"); 

    for (int i = 0; i < 4; i++)
        if (room.connections[i] != -1) printf ("[%i]: %s\t", i+1, tree[room.connections[i]].name);

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

void RecursiveTreeGeneration(struct Node *parentNode, struct Tree *myTree, int depth, int librarySize, char library[librarySize][2][150])
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

        int randomLibraryIndex = (int)randomRange(0, librarySize - 0.00001);

        InitializeNode(&myTree->nodelist[myTree->index], myTree->index, library[randomLibraryIndex][0], library[randomLibraryIndex][1]);

        //give it connections to parent and vice versa
        parentNode->connections[parentNode->connectionsindex] = myTree->index;
        parentNode->connectionsindex += 1;
        myTree->nodelist[myTree->index].connections[0] = parentNode->id;
        myTree->nodelist[myTree->index].connectionsindex += 1;
        int temp = myTree->nodelist[myTree->index].connectionsindex;

        //increment index
        myTree->index++;

        //call function on children
        if (randomRange(0, 1) >= (1 - CHILDSPAWNPERCENTAGE))
        {
            RecursiveTreeGeneration(&myTree->nodelist[myTree->index-1], myTree, depth - 1, librarySize, library);
        }
    }
}

void ReadRoomLibrary(FILE* in, int librarysize, char library[librarysize][2][150])
{
    for (int i = 0; i < librarysize; i++)
    {
        char line[150];
        fgets(line, 150, in);
        int hashIndex = 0;
        for (hashIndex = 0; line[hashIndex] != '#'; hashIndex++)
        {

        }
        for (int j = 0; j < hashIndex; j++)
        {
            library[i][0][j] = line[j];
        }
        library[i][0][hashIndex] = '\0';
        int j;
        for (int j = hashIndex + 1; line[j-1] != '\0'; j++)
        {
            library[i][1][j-(hashIndex+1)] = line[j];
            if (line[j] == '\n') 
                library[i][1][j-(hashIndex+1)] = '\0';
        }
    }
}

int main()
{
    srand(time(NULL));

    //open file and find the amount of lines.
    FILE* inputFile = fopen("roomlibrary.txt", "r");
    if (inputFile == NULL)
    {
        printf("Unable to open input file.\n");
        return 1;
    }

    int numOfLines = 0;
    char line[181]; //name length + seperand + description max length
    while (fgets(line, 100, inputFile) != NULL)
    {
        numOfLines++;
    }
    rewind(inputFile);
    char roomlibrary[numOfLines][2][150];

    ReadRoomLibrary(inputFile, numOfLines, roomlibrary);

    struct Tree myTree;
    InitalizeTree(&myTree);

    struct Node* currentNode = &myTree.nodelist[0];

    //declare rooms manually for testing
    InitializeNode(&myTree.nodelist[myTree.index], myTree.index, "Castle", "brrr spooky castle");
    myTree.index++;

    RecursiveTreeGeneration(currentNode, &myTree, DEPTH, numOfLines, roomlibrary);

    int gameEnd = 0;
    while(!gameEnd)
    {
        DisplayNode(*currentNode, myTree.nodelist);
        printf("Which path would you like to take?: ");
        int chosenPath = -1; //(will be between 1 and 4 when valid)

        while ((scanf("%i", &chosenPath) != 1) || !ValidPath(*currentNode, currentNode->connections[chosenPath - 1])) //VALID PATH NOT WORKING!!!!!!!!!!!!
            printf("Invalid selection. Which path would you like to take?: ");

        currentNode = &myTree.nodelist[currentNode->connections[chosenPath - 1]];
    }
}