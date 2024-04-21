#include "List.h"
#include <stdio.h>

#ifndef GRAPH_H
#define GRAPH_H

#define INF -1
#define NIL 0

typedef struct GraphObj *Graph;

/*** Constructors-Destructors ***/
Graph newGraph(
    int n); // returns a new graph pointing to a newly created GraphObj

void freeGraph(Graph *pG); // frees all heap memory that is associated with
                           // Graph *pG, then sets handle to NULL

/*** Access functions ***/
int getOrder(Graph G); // return the corresponding field values

int getSize(Graph G); // return the corresponding field values

int getSource(Graph G); // return the corresponding field values or NIL if BFS()
                        // has not yet been called

int getParent(Graph G, int u); // return the parent of vertex u in the BFS tree
                               // or NIL if BFS() is not called yet

int getDist(Graph G,
            int u); // returns the distance from the most recent BFS source to
                    // vertex u, or INF if BFS() has not yet been called

void getPath(List L, Graph G,
             int u); // appends to the List L the vertices of a shortest path in
                     // G from source to u, or appends to L the value NIL if no
                     // such path exists.

/*** Manipulation procedures ***/
void makeNull(Graph G); // deletes all edges of G, restoring it to its original
                        // (no edge) state.

void addEdge(
    Graph G, int u,
    int v); // inserts a new edge joining u to v, i.e. u is added to the
            // adjacency List of v, and v to the adjacency List of u.

void addArc(Graph G, int u,
            int v); // inserts a new directed edge from u to v, i.e. v is added
                    // to the adjacency List of u

void BFS(Graph G, int s); // runs the BFS algorithm on the Graph G with source s

/*** Other operations ***/
void printGraph(FILE *out, Graph G); // prints the adjacency list representation
                                     // of G to the file pointed to by out

#endif