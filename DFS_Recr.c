#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Colouring
#define WHITE 0
#define GREY 1
#define BLACK 2

typedef struct node
{
    int vertex;
    struct node *next;
} node;

typedef struct graph
{
    int numV;
    struct node **adjList;
} graph;

// Each node of graph
node *createNode(int v)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Graph and Adjacency List
graph *createGraph(int vertices)
{
    graph *G = (graph *)malloc(sizeof(graph));
    G->numV = vertices;
    G->adjList = (node **)malloc(vertices * sizeof(node *));
    for (int i = 0; i < vertices; i++)
    {
        G->adjList[i] = NULL;
    }
    return G;
}

// Undirected Graph
void addEdge(graph *G, int src, int dest)
{
    node *newNodeD = createNode(dest);
    newNodeD->next = G->adjList[src];
    G->adjList[src] = newNodeD;

    node *newNodeS = createNode(src);
    newNodeS->next = G->adjList[dest];
    G->adjList[dest] = newNodeS;
}

// Printing Adjacency List
void printGraph(graph *G)
{
    for (int v = 0; v < G->numV; v++)
    {
        node *temp = G->adjList[v];
        printf("Adjacency List of vertex %d ", v);
        while (temp)
        {
            printf("-> %d ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

// DFS - Recursive Algorithm

void DFS_Visit(graph *G, int u, int *tree, int *seq, int *idx, int *time, int *colour, int *pre, int *start, int *finish)
{
    colour[u] = GREY;
    start[u] = ++(*time);

    seq[*idx] = u;
    tree[*idx] = pre[u];
    (*idx)++;

    node *temp = G->adjList[u];
    while (temp)
    {
        int v = temp->vertex;
        if (colour[v] == WHITE)
        {
            pre[v] = u;
            DFS_Visit(G, v, tree, seq, idx, time, colour, pre, start, finish); // Recursion
        }
        temp = temp->next;
    }

    colour[u] = BLACK;
    finish[u] = ++(*time);
}

void DFS(graph *G)
{
    int *colour = (int *)malloc(G->numV * sizeof(int));
    int *pre = (int *)malloc(G->numV * sizeof(int));
    int *start = (int *)malloc(G->numV * sizeof(int));
    int *finish = (int *)malloc(G->numV * sizeof(int));
    int *tree = (int *)malloc(G->numV * sizeof(int));
    int *seq = (int *)malloc(G->numV * sizeof(int));

    int idx = 0, time = 0;

    for (int i = 0; i < G->numV; i++)
    {
        colour[i] = WHITE;
        pre[i] = -1;
    }

    for (int i = 0; i < G->numV; i++)
    {
        if (colour[i] == WHITE)
        {
            DFS_Visit(G, i, tree, seq, &idx, &time, colour, pre, start, finish);
        };
    }

    printf("\n");

    // Node Sequence
    printf("DFS vertex sequence :\n");
    for (int i = 0; i < G->numV; i++)
    {
        printf("%d  ", seq[i]);
    }
    printf("\n");
    printf("\n");

    // DFS Tree
    printf("DFS predecessor sequence :\n");
    for (int i = 0; i < G->numV; i++)
    {
        printf("%d  ", tree[i]);
    }
    printf("\n");
    printf("\n");

    // Start and End Time
    for (int i = 0; i < G->numV; i++)
    {
        printf("Vertex %d : start = %d , finish = %d, predecessor = %d\n", i, start[i], finish[i], pre[i]);
    }

    // free
    free(colour);
    free(pre);
    free(start);
    free(finish);
    free(seq);
    free(tree);
}

/*
         2 --- 4        6
        /  \  /        / \
  0    1    \/        /   \
        \   /\       /     \
          3 -- 5    7 ----- 8
*/

int main()
{
    graph *G = createGraph(9);

    addEdge(G, 1, 2);
    addEdge(G, 1, 3);
    addEdge(G, 2, 4);
    addEdge(G, 2, 5);
    addEdge(G, 3, 5);
    addEdge(G, 3, 4);
    addEdge(G, 6, 7);
    addEdge(G, 6, 8);
    addEdge(G, 7, 8);

    printGraph(G);

    clock_t startTime = clock();

    DFS(G);

    clock_t endTime = clock();

    double reqTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    printf("\nAlgorithm executed in %f seconds.\n", reqTime);

    return 0;
}