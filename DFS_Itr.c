#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_SIZE 100 // User defined stack

// Colouring
#define WHITE 0
#define GREY 1
#define BLACK 2

// Adjacency List
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

// Stack
int stack[MAX_SIZE];
int top = -1;

// Stack operations

int isFull()
{
    if (top == MAX_SIZE - 1)
        return 1;
    else
        return 0;
}
int isEmpty()
{
    if (top == -1)
        return 1;
    else
        return 0;
}

// Push
void push(int data)
{
    if (isFull())
    {
        printf("Stack overflow\n");
    }
    else
    {
        top++;
        stack[top] = data;
    }
}
// Pop
int pop()
{
    if (isEmpty())
    {
        printf("Stack underflow\n");
        return INT_MIN;
    }
    else
    {
        int popped = stack[top];
        top--;
        return popped;
    }
}

// Peek
void peek()
{
    if (isEmpty())
        printf("The stack is Empty\n");
    else
        printf("The top element is %d\n", stack[top]);
}
// Current Size
int stackSize()
{
    int size = 0;
    for (int i = 0; i <= top; i++)
    {
        size++;
    }
    return size;
}
// Print
void printStack()
{
    printf("The stack is :");
    for (int i = 0; i <= top; i++)
    {
        printf(" %d", stack[i]);
    }
    printf("\n");
}

// Creation of Adjacency List for Graph

node *createNode(int v)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}
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

// DFS Algorithm

void DFS(graph *G)
{
    int *colour = (int *)malloc(G->numV * sizeof(int)); // to track the discovery
    int *pre = (int *)malloc(G->numV * sizeof(int));    // predecessor for each node ( predecessor in order of vertex )
    int *start = (int *)malloc(G->numV * sizeof(int));  // discovery time of node
    int *finish = (int *)malloc(G->numV * sizeof(int)); // time when BLACK
    int *tree = (int *)malloc(G->numV * sizeof(int));   // for printing the predecessor of each node ( predecessor in order of discovery )
    int *seq = (int *)malloc(G->numV * sizeof(int));    // for printing the sequence of discovery

    int idx = 0, time = 0;

    for (int i = 0; i < G->numV; i++)
    {
        colour[i] = WHITE;
        pre[i] = -1;
        tree[i] = -1;
        seq[i] = -1;
    }

    // Iterative
    for (int i = 0; i < G->numV; i++)
    {
        if (colour[i] == WHITE) // for any disconnected component -> this forms the forest of trees
        {
            push(i); // pushing first root of a tree in the forest
            colour[i] = GREY;
            start[i] = ++time;

            seq[idx] = i;
            tree[idx] = pre[i];
            idx++;

            while (!isEmpty()) // runs till all the pushed vertices are popped
            {
                int u = stack[top];         // taking the last pushed vertex
                node *temp = G->adjList[u]; // accessing adjacent vertex

                int whiteNode = 0; // to check for whether any node in WHITE in the depth

                while (temp)
                {
                    int v = temp->vertex;
                    if (colour[v] == WHITE) // check if the adjacent vertex is WHITE
                    {
                        pre[v] = u;
                        push(v); // pushing the adjacent WHITE vertex into the stack -> this is the top element to be accessed in the next iteration
                        colour[v] = GREY;
                        start[v] = ++time;

                        seq[idx] = v;
                        tree[idx] = pre[v];
                        idx++;

                        whiteNode = 1; // Update to track whether any vertex in the list corresponding to the current vertex is WHITE -> this helps in deciding whether the current vertex would remain GREY or become BLACK in the current iteration
                        break;         // allows to continue in the depth
                    }
                    temp = temp->next; // if adjacent vertex in list is not WHITE we keep on traversing the list
                }
                if (!whiteNode) // If no more depth search is possible through the vertex
                {
                    pop(); // Current node popped out of stack as no other vertex can be traversed through it
                    colour[u] = BLACK;
                    finish[u] = ++time;
                }
            }
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

    // Count starts
    clock_t startTime = clock();

    DFS(G);

    // Count ends
    clock_t endTime = clock();

    double reqTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC; // Calculation for running time of the algorithm

    printf("\nDFS Algorithm ( Iterative ) takes %f seconds to execute\n\n", reqTime);

    return 0;
}

/*

1. The Outer Loop runs for V times total, however it continues only when a vertex is WHITE (unvisited). This ensures every vertex is the root of a search at most once.
2. The Middle Loop is controlled by the stack. Every vertex is pushed at max once and popped at max once. Hence, the loop executes 2V times in total, which can be said to O(V) runs.
3. The Inner Loop iterates over the adjacency list nodes of the current vertex u. With every temp = temp->next, a specific edge is traversed in the list. Since the graph has E edges, the adjacency lists has 2E ( each edge is examined at most twice, once from each direction). Hence, in total it accounts to O(E).

However, although there are 3 nested loops in this iterative version of DFS, the overall time complexity of the algorithm is O(V+E) -> this is because if one loops runs for more number of times, the others run for lesser, and in overall all of them together account to V+E times.
This is how 'amortized analysis' is performed. Instead of looking at the worst-case cost of a single operation, it looks at the total cost averaged over a sequence of operations.

*/