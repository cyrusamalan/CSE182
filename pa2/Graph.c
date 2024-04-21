// Cyrus Amalan, camalan, pa2

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "List.h"

#define WHITE NIL // Not visited
#define GRAY 1    // Discovered but not fully explored
#define BLACK 2   // Fully explored

typedef struct GraphObj {
  List *neighbors; // containing the neighobrs of a vertex
  int *color;      // array to store the color
  int *distance;   // array to store the distance from the source vertex
  int *parent;     // array to store parent of eac vertex
  int order;       // number of vertices in each graph
  int size;        // number of edges in graph
  int source;      // label of the vertex(recently used as source for BFS)
} GraphObj;

Graph newGraph(int n) {
  Graph g = malloc(sizeof(
      GraphObj)); // allocates memory and assigns its address to the pointer G
  // Array Initialization(allocation)
  g->color = calloc(n + 1, sizeof(int));
  g->parent = calloc(n + 1, sizeof(int));
  g->distance = calloc(n + 1, sizeof(int));
  g->neighbors = calloc(n + 1, sizeof(List));
  g->order = n;
  g->size = 0;
  g->source = NIL;

  for (int i = 0; i <= n; i++) {
    g->neighbors[i] = newList();
    g->distance[i] = INF; // set to negative integer(-1)
    g->parent[i] = NIL;   // set to postive integer(0)
  }
  return g;
}

void freeGraph(Graph *pG) {
  if (*pG != NULL && pG != NULL) {
    for (int vertex = 0; vertex < (*pG)->order; vertex++) {
      freeList(&(*pG)->neighbors[vertex]);
    }
    free((*pG)->color);
    free((*pG)->neighbors);
    free((*pG)->distance);
    free((*pG)->parent);
    free(*pG);
    *pG = NULL;
  }
}

int getOrder(Graph G) {
  if (!G) {
    fprintf(stderr, "error: Graph is NULL\n");
    exit(EXIT_FAILURE);
  }
  return G->order; // return order of graph
}

int getSize(Graph G) {
  if (!G) {
    fprintf(stderr, "error: Graph is NULL\n");
    exit(EXIT_FAILURE);
  }
  return G->size; // return size of graph
}

int getSource(Graph G) {
  if (!G) {
    fprintf(stderr, "error: Graph is NULL\n");
    exit(EXIT_FAILURE);
  }
  return G->source; // return the source vertex used in the last call of BFS()
}

int getParent(Graph G, int u) {
  if (!G) {
    fprintf(stderr, "error: Graph is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (1 > u || u > getOrder(G)) {
    fprintf(stderr, "error: called with vertex out of range\n");
    exit(EXIT_FAILURE);
  }
  return G->parent[u]; // returns the parent of vertex u in the BFS tree
}

int getDist(Graph G, int u) {
  if (!G) {
    fprintf(stderr, "error: Graph is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (1 > u || u > getOrder(G)) {
    fprintf(stderr, "error: called with vertex out of range\n");
    exit(EXIT_FAILURE);
  }
  return G->distance[u]; // return the distance from the vertex
}

void getPath(List L, Graph G, int u) {
  if (!G) {
    fprintf(stderr, "error: Graph is NULL\n"); // NULL graph check
    exit(EXIT_FAILURE);
  }
  // Check for vertex out of range
  int totalVertices = getOrder(G);
  if (1 > u || u > totalVertices) {
    fprintf(stderr,
            "error: called with vertex out of range\n"); // vertex range check
    exit(EXIT_FAILURE);
  }
  if (getSource(G) == NIL) {
    fprintf(stderr, "error: function called before BFS()\n"); // BFS check
    exit(EXIT_FAILURE);
  }
   int source = getSource(G);
    if (u == source) {
        append(L, u);
        return;
    } else if (getParent(G, u) == NIL) {
        append(L, NIL);
        return;
    }

    // Recursive case: construct the path for the parent of u first
    getPath(L, G, getParent(G, u));
    
    // Append u to the list after constructing the path for its parent
    append(L, u);
}


void makeNull(Graph G) {
  if (!G) {
    fprintf(stderr, "Error: graph is already NULL\n");
    exit(EXIT_FAILURE);
  }
  for (int vertex = 2; vertex <= getOrder(G); vertex++) {
    clear(G->neighbors[vertex]);
    G->color[vertex] = NIL;
    G->parent[vertex] = NIL;
    G->distance[vertex] = INF;
  }
  int reset = getOrder(G);
  G->order = reset;
  G->size = 0; 
  G->source = NIL;
}

void addEdge(Graph G, int u, int v) {
  addArc(G, u, v);
  addArc(G, v, u);
  G->size--;
}

void addArc(Graph G, int u, int v) {
  List targetList = G->neighbors[u];
  moveFront(targetList); // Position at the start of the list
  while (index(targetList) != -1 &&
         v > get(targetList)) { // Renamed for clarity
    moveNext(targetList);       // Move to the next element
  }
  if (index(targetList) == -1) { // Check if at the end of the list
    append(targetList, v);       // Append if no larger element is found
  } else {
    insertBefore(targetList, v); // Otherwise, insert before the larger element
  }
  G->size++; // Increment the number of edges
}
//helper function
//For each vertex in the graph, the function sets the color to WHITE.
void initializeGraphAttributes(Graph G, int s) {
    for (int i = 1; i <= getOrder(G); i++) {
        G->color[i] = WHITE;
        G->distance[i] = INF;//distance to INF, represents a large number for unreachable or unvisited
        G->parent[i] = NIL;
    }
    G->source = s;//sets the BFS search to the starting vertex `s`
}
//helper function
void exploreNeighbors(Graph G, List queue, int u) {
    List neighbors = G->neighbors[u];//iterates through the neighbors of the vertex `u`
    moveFront(neighbors);
    while (index(neighbors) != -1) {
        int v = get(neighbors);
        if (G->color[v] == WHITE) {
            G->color[v] = GRAY;
            G->distance[v] = G->distance[u] + 1;
            G->parent[v] = u;
            append(queue, v);
        }
        moveNext(neighbors);
    }
}
//Performs Breadth-First Search (BFS) on a graph 
void BFS(Graph G, int s) {
    initializeGraphAttributes(G, s);
    
    List queue = newList();
    append(queue, s);//append queue to keep track of the next vertices
    G->color[s] = GRAY;
    G->distance[s] = 0;
    G->parent[s] = NIL;

    while (length(queue) != 0) {//while loop to dequeue each vertex
        int u = front(queue);
        deleteFront(queue);// will keep going until the queue is empty
        
        exploreNeighbors(G, queue, u);

        G->color[u] = BLACK; // Mark as fully explored
    }

    freeList(&queue);//free the queue from memory
}


void printGraph(FILE *out, Graph G) {
    if (!G || !out) { // Simplified condition
        fprintf(stderr, "printGraph Error\n");
        exit(EXIT_FAILURE);
    }

    for (int v = 1; v <= getOrder(G); v++) { // Changed variable name to 'v'
        fprintf(out, "%d: ", v);
        List adjacentVertices = G->neighbors[v]; // More descriptive variable name

        // Check if the adjacency list is empty
        if (length(adjacentVertices) == 0) {
            fprintf(out, "\n");
            continue; // Skip to the next vertex
        }

        // Iterate through the adjacency list and print each vertex
        for (moveFront(adjacentVertices); index(adjacentVertices) >= 0; moveNext(adjacentVertices)) {
            int adjacentVertex = get(adjacentVertices); // More descriptive variable name
            fprintf(out, "%d", adjacentVertex);
            
            // Print a space after the number if it's not the last in the list
            if (index(adjacentVertices) < length(adjacentVertices) - 1) {
                fprintf(out, " ");
            }
        }
        fprintf(out, "\n");
    }
}

