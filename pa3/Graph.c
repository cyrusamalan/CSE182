// Cyrus Amalan, camalan, pa3

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "List.h"

#define WHITE NIL
#define GRAY 1
#define BLACK 2

typedef struct GraphObj {
  List *neighbors;
  int *color;
  int *parent;
  int *discover;
  int *finish;
  int order;
  int size;
} GraphObj;

void explore(Graph G, int vertex, int *currentTime, List stack);

/*** Constructors-Destructors ***/

Graph newGraph(int n) {
  Graph newGraph = malloc(sizeof(struct GraphObj));

  newGraph->neighbors = malloc((n + 1) * sizeof(List));
  newGraph->color = malloc((n + 1) * sizeof(int));
  newGraph->parent = malloc((n + 1) * sizeof(int));
  newGraph->discover = malloc((n + 1) * sizeof(int));
  newGraph->finish = malloc((n + 1) * sizeof(int));
  newGraph->order = n;
  newGraph->size = 0;

  for (int vertex = 1; vertex <= n; vertex++) {
    newGraph->neighbors[vertex] = newList();
    newGraph->color[vertex] = WHITE;
    newGraph->parent[vertex] = NIL;
    newGraph->discover[vertex] = UNDEF;
    newGraph->finish[vertex] = UNDEF;
  }

  return newGraph;
}

void freeGraph(Graph *pG) {
  if (pG != NULL && *pG != NULL) {

    for (int i = 1; i <= (*pG)->order; i++) {
      freeList(&((*pG)->neighbors[i]));
    }

    free((*pG)->neighbors);
    free((*pG)->color);
    free((*pG)->parent);
    free((*pG)->discover);
    free((*pG)->finish);
    free(*pG);
    *pG = NULL;
  }
}

/*** Access functions ***/

// Returns the order of the graph (number of vertices)
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

/* Pre: 1<=u<=n=getOrder(G) */
int getDiscover(Graph G, int u) {
  if (u < 1 || u > getOrder(G)) {
    printf("errr: invalid vertex index\n");
    exit(EXIT_FAILURE);
  }
  return G->discover[u];
}

/* Pre: 1<=u<=n=getOrder(G) */
int getFinish(Graph G, int u) {
  if (u < 1 || u > getOrder(G)) {
    fprintf(stderr, "error: vertex %d is out of range\n", u);
    exit(EXIT_FAILURE);
  }
  return G->finish[u];
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

    if (getParent(G, u) == NIL) {
      append(L, NIL);
      return;
    }

    getPath(L, G, getParent(G, u));
    append(L, u);
  }
}

// Manipulation procedures
void addEdge(Graph G, int u, int v) {
  addArc(G, u, v);
  addArc(G, v, u);
  G->size--;
}

void addArc(Graph G, int u, int v) {
  if (v > getOrder(G) || u > getOrder(G) || v < 1 || u < 1) {
    printf("addArc(): invalid vertex index\n");
    exit(1);
  }

  List targetList = G->neighbors[u];
  if (length(targetList) == 0) {
    append(targetList, v);
  } else {
    for (moveFront(targetList); index(targetList) >= 0; moveNext(targetList)) {
      if (v < get(targetList)) {
        insertBefore(targetList, v);
        break;
      } else if (v == get(targetList)) {
        // Arc already exists, exit the function
        return;
      }
    }
    if (index(targetList) < 0) {
      append(targetList, v);
    }
  }

  G->size++;
}

/* Pre: length(S)==getOrder(G) */
/* DFS function using Visit */
void DFS(Graph G, List S) {

  for (int i = 1; i <= getOrder(G); i++) {
    G->color[i] = WHITE;
    G->parent[i] = NIL;
    G->discover[i] = UNDEF;
    G->finish[i] = UNDEF;
  }
  // Start the time counter
  int currentTime = 0;

  List tempList = copyList(S);
  clear(S);
  // Process each vertex in the temporary list
  for (moveFront(tempList); index(tempList) != -1; moveNext(tempList)) {
    int currentVertex = get(tempList);
    if (G->color[currentVertex] == WHITE) {
      // Recursive helper function to explore vertex
      explore(G, currentVertex, &currentTime, S);
    }
  }

  freeList(&tempList);
}

void explore(Graph G, int vertex, int *currentTime, List stack) {

  G->discover[vertex] = ++(*currentTime);
  G->color[vertex] = GRAY;

  List adjacency = G->neighbors[vertex];
  for (moveFront(adjacency); index(adjacency) != -1; moveNext(adjacency)) {
    int adjacentVertex = get(adjacency);
    if (G->color[adjacentVertex] == WHITE) {
      G->parent[adjacentVertex] = vertex;
      explore(G, adjacentVertex, currentTime, stack);
    }
  }

  G->color[vertex] = BLACK;
  G->finish[vertex] = ++(*currentTime);

  prepend(stack, vertex);
}

// Other Functions
Graph transpose(Graph G) {
  int numVertices = getOrder(G);
  Graph transposedGraph = newGraph(numVertices);

  for (int vertex = 1; vertex <= numVertices; vertex++) {
    List adjacencyList = G->neighbors[vertex];
    for (moveFront(adjacencyList); index(adjacencyList) >= 0;
         moveNext(adjacencyList)) {
      int connectedVertex = get(adjacencyList);
      addArc(transposedGraph, connectedVertex,
             vertex); // Invert the direction of the edge
    }
  }

  return transposedGraph;
}

Graph copyGraph(Graph G) {
  int vertexCount = getOrder(G);
  Graph GraphCopy = newGraph(vertexCount);

  for (int vertex = 1; vertex <= vertexCount; vertex++) {
    List neighbors = G->neighbors[vertex];
    for (moveFront(neighbors); index(neighbors) >= 0; moveNext(neighbors)) {
      int connectedVertex = get(neighbors);
      addArc(GraphCopy, vertex,
             connectedVertex); // Copy the edge to the new graph
    }
  }

  return GraphCopy;
}

void printGraph(FILE *out, Graph G) {
  if (!G || !out) { // Simplified condition
    fprintf(stderr, "printGraph Error\n");
    exit(EXIT_FAILURE);
  }

  for (int v = 1; v <= getOrder(G); v++) {
    fprintf(out, "%d: ", v);
    List adjacentVertices = G->neighbors[v];

    if (length(adjacentVertices) == 0) {
      fprintf(out, "\n");
      continue; // Skip to the next vertex
    }

    for (moveFront(adjacentVertices); index(adjacentVertices) >= 0;
         moveNext(adjacentVertices)) {
      int adjacentVertex =
          get(adjacentVertices); // More descriptive variable name
      fprintf(out, "%d", adjacentVertex);

      // Print a space after the number if it's not the last in the list
      if (index(adjacentVertices) < length(adjacentVertices) - 1) {
        fprintf(out, " ");
      }
    }
    fprintf(out, "\n");
  }
}
