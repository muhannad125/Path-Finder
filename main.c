#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FILE_NAME "sample.txt"
#define STRING_SIZE 60
#define ALLOC_SIZE 50

typedef struct Flights {
    char src_name;
    char dest_name;
    int hours;
    int minutes;
    int total_time;
    int price;
} Flights;

typedef struct Mat {
    int hours;
    int minutes;
    int total_time;
    int price;
} Mat;

typedef struct Graph {
    int n; /* number of vertices */
    Mat **adjMat; /* heads in the graph */
} Graph;

typedef struct Path {
    int array[50];
    int transit;
    int hours;
    int minutes;
    int total_time;
    int price;
    size_t size;
    size_t arraySize;
} Path;

typedef struct VisitedStack {
    int top;
    unsigned capacity;
    int* array;
} VisitedStack;



void addCity(char ***cities, char *city, size_t *size, size_t *alocSize);
int alreadyAdded(char ** cities, char * city, size_t size);
size_t getCityNo(char **cities, char *city, size_t citiesSize);
int isVisited(int cityNo, VisitedStack *visited);
void dfs(char *dest, Graph *flights, size_t citiesSize, VisitedStack *visited, Path *path, char **cities, int *counter);
void addToPaths(Graph* flights, Path *path, VisitedStack* visited,int counter);
int isFull(VisitedStack* stack);
//Stack Functions

VisitedStack* createStack(unsigned capacity);
int isEmpty(VisitedStack* stack);
void push(VisitedStack* stack, int item, char **cities);
int pop(VisitedStack* stack);
int peek(VisitedStack* stack);

int main()
{
    Graph *Gr;
    VisitedStack *visited = createStack(100);
    Path *path;
    char source[30], destination[30], **cities;
    int hours, minutes, price;
    size_t i,j,k, citiesSize;


    Gr = (Graph*)malloc(sizeof(Graph));
    cities = malloc(ALLOC_SIZE * sizeof(char*));
    for ( i = 0; i < ALLOC_SIZE; i++)
        {
            cities[i] = malloc(STRING_SIZE*sizeof(char));
        }
    
    Gr->adjMat = (Mat**)malloc(ALLOC_SIZE * sizeof(Mat*));
    for ( i = 0; i < ALLOC_SIZE; i++)
        {
            Gr->adjMat[i] = (Mat*)calloc(ALLOC_SIZE, sizeof(Mat));
        }
    
    path = (Path*)malloc(ALLOC_SIZE * sizeof(Path));
    

    FILE *fileptr;
    fileptr = fopen(FILE_NAME, "r");
    if (!fileptr) {
        printf("Couldn't read %s", FILE_NAME);
        return 0;
    }

    size_t srcNo, destNo, tmp;
    j = 1, i = 0, k = 0;
    Gr->n = 0;
    while (fscanf(fileptr, " %s %s %d %d %d", source, destination, &hours, &minutes, &price) == 5) {        
        if (!alreadyAdded(cities, source, i))
        {
            strcpy(cities[i], source);
            i++;
        }
        if (!alreadyAdded(cities, destination, i))
        {
            strcpy(cities[i], destination);
            i++;
        } 
        
        citiesSize = i;
        Gr->n = i;
        srcNo = getCityNo(cities, source, citiesSize);
        destNo = getCityNo(cities, destination, citiesSize);
        Gr->adjMat[srcNo][destNo].hours = hours;
        Gr->adjMat[srcNo][destNo].minutes = minutes;
        Gr->adjMat[srcNo][destNo].price = price;
        Gr->adjMat[srcNo][destNo].total_time = hours*60 + minutes;
        Gr->adjMat[destNo][srcNo].hours = hours;
        Gr->adjMat[destNo][srcNo].minutes = minutes;
        Gr->adjMat[destNo][srcNo].price = price;
        Gr->adjMat[destNo][srcNo].total_time = hours*60 + minutes;

    }
    fclose(fileptr);

    printf("\n");
    printf("Available Cities\n");
    for (i = 0; i < citiesSize; i++)
    {
        printf("%zu: %s\n", i, cities[i]);
    }

    printf("\nPrice Adjacency Matrix\n");
    printf("\t");
    for ( i = 0; i < citiesSize; i++)
    {
        printf("%zu\t", i);
    }
    printf("\n");
    for (i = 0; i < Gr->n; i++)
    {
        printf("%zu\t", i);
        for (j = 0; j < Gr->n; j++)
        {
            printf("%d\t", Gr->adjMat[i][j].price);
        }
        printf("\n");
    }

    printf("\nTime Adjacency Matrix(In minutes)\n");
    printf("\t");
    for ( i = 0; i < citiesSize; i++)
    {
        printf("%zu\t", i);
    }
    printf("\n");
    for (i = 0; i < Gr->n; i++)
    {
        printf("%zu\t", i);
        for (j = 0; j < Gr->n; j++)
        {
            printf("%d\t", Gr->adjMat[i][j].total_time);
        }
        printf("\n");
    }
    
    int counter = 0;
    char *src , *dest;
    dest = (char*)malloc(STRING_SIZE*sizeof(char));
    src = (char*)malloc(STRING_SIZE*sizeof(char));
    printf("\n");
    printf("Enter Source: ");
    scanf(" %s", src);
    printf("Enter Destination: ");
    scanf(" %s", dest);

    srcNo = getCityNo(cities, src, citiesSize);
    path->arraySize = 0;
    push(visited, srcNo, cities);
    dfs(dest, Gr, citiesSize, visited, path, cities, &counter);

    printf("\n");
    for ( i = 0; i < path[i].size+1; i++)
    {
        for ( j = 0; j < path[i].arraySize; j++)
        {
            printf("%s ", cities[path[i].array[j]]);
        }
        printf("\n");
        printf("price: %d\n", path[i].price);
        printf("hours: %d\n", path[i].hours);
        printf("minutes: %d\n", path[i].minutes);
        path[i].transit = path[i].arraySize - 1;
        printf("transit: %d\n\n", path[i].transit);

    }
    

    for ( i = 0; i < ALLOC_SIZE ; i++)
    {
        free(cities[i]);
    }
    free(cities);

    for ( i = 0; i < ALLOC_SIZE ; i++)
    {
        free(Gr->adjMat[i]);
    }
    free(Gr->adjMat);
    
    free(visited->array);
    free(visited);
    free(path);

    return 0;
}

int alreadyAdded(char **cities, char *city, size_t size){
    size_t i;
    for (i = 0; i < size; i++)
    {
        if (!strcmp(cities[i], city))
        {   
            return 1;
        }
    }
    return 0;
}

size_t getCityNo(char **cities, char *city, size_t citiesSize){
    size_t i;
    for (i = 0; i < citiesSize; i++)
    {
        if (!strcmp(cities[i], city))
        {   
            return i;
        }
    }
    printf("the city is not found in the available destinations.\n");
    return -1;
}

void dfs(char *dest, Graph *flights, size_t citiesSize, VisitedStack *visited, Path *path, char **cities, int *counter){
    size_t destCity, i;
    destCity = getCityNo(cities, dest, citiesSize);
    size_t currCity = peek(visited);

    printf("\ncurrentCity: %s, destCity: %s\n", cities[currCity], cities[destCity]);
    if (flights->adjMat[currCity][destCity].total_time != 0 )
    {   
        push(visited, destCity, cities);
        addToPaths(flights, path, visited, *counter);
        pop(visited);
        pop(visited);

        *counter = *counter+1;
        return;
    }
    
    for (i = 0; i < citiesSize; i++)
    {   
        if (!isVisited(i, visited) && flights->adjMat[currCity][i].total_time != 0 )
        {
            printf("Path Found Between %s and %s\n", cities[currCity], cities[i]);
            push(visited, i, cities);
            dfs(dest, flights, citiesSize, visited, path, cities, counter);
            
        }
    }
    pop(visited);
}

int isVisited(int cityNo, VisitedStack *visited){
    size_t i;
    for ( i = 0; i < visited->top + 1; i++)
    {
        if (visited->array[i] == cityNo)
        {
            return 1;
        }
    }
    return 0;
}

void addToPaths(Graph* flights, Path* path, VisitedStack* visited,int counter){
    size_t i;
    
    for (i = 0; i < visited->top+1; i++)
    {
        path[counter].array[i] = visited->array[i];
        path[counter].arraySize = path[counter].arraySize + 1;
        if (i < visited->top)
        {
            path[counter].price += flights->adjMat[visited->array[i]][visited->array[i+1]].price;
            path[counter].hours += flights->adjMat[visited->array[i]][visited->array[i+1]].hours;
            path[counter].minutes += flights->adjMat[visited->array[i]][visited->array[i+1]].minutes;
            path[counter].total_time += flights->adjMat[visited->array[i]][visited->array[i+1]].total_time;
            path[counter].size = counter + 1;
        }
        
    }
}
 
// function to create a stack of given capacity. It initializes size of
// stack as 0
VisitedStack* createStack(unsigned capacity)
{
    VisitedStack* stack = (VisitedStack*)malloc(sizeof(VisitedStack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

// Stack is empty when top is equal to -1
int isEmpty(VisitedStack* stack)
{
    return stack->top == -1;
}

int isFull(VisitedStack* stack)
{
    return stack->top == stack->capacity - 1;
}

// Function to add an item to stack.  It increases top by 1
void push(VisitedStack* stack, int item, char **cities)
{   
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
    printf("%s pushed to visited stack\n", cities[item]);
}
 
// Function to remove an item from stack.  It decreases top by 1
int pop(VisitedStack* stack)
{
    if (isEmpty(stack))
        return -1;
    return stack->array[stack->top--];
}
 
// Function to return the top from stack without removing it
int peek(VisitedStack* stack)
{
    if (isEmpty(stack))
        return -1;
    return stack->array[stack->top];
}