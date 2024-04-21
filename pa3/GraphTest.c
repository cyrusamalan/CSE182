// Cyrus Amalan, camalan, pa3

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "List.h"

int main() {
  // Create a Graph with 5 vertices
  int numVertices = 5;
  Graph G = newGraph(numVertices);

  // Add edges
  addEdge(G, 1, 2);
  addEdge(G, 2, 3);
  addEdge(G, 3, 4);
  addEdge(G, 5, 1);

  // Print the graph
  printf("Initial Graph:\n");
  printGraph(stdout, G);

  // Perform DFS on the graph
  List S = newList();
  for (int i = 1; i <= numVertices; i++) {
    append(S, i);
  }

  DFS(G, S);

  printf("\nVertices after DFS:\n");

  int expectedVertices[] = {1, 5, 2, 3,
                            4}; // Define the expected order of vertices
  int idx = 0; // Index to track the position in the expectedVertices array

  for (moveFront(S); index(S) >= 0; moveNext(S)) {
    int currentVertex = get(S);
    printf("%d ", currentVertex);  
    assert(currentVertex == expectedVertices[idx]);
    idx++;
  }
  assert(idx == 5);

//expected discovery and finish times for each vertex
int expectedDiscoveryTimes[] = {1, 2, 3, 4, 8 };
int expectedFinishTimes[] = {10, 7, 6, 5, 9};

printf("\nDiscovery and Finish times:\n");
for (int i = 1; i <= numVertices; i++) {
    int discoverTime = getDiscover(G, i);
    int finishTime = getFinish(G, i);
    printf("Vertex %d: Discovered at %d, Finished at %d\n", i, discoverTime, finishTime);

    // Assert that the discovery and finish times match the expected times
    assert(discoverTime == expectedDiscoveryTimes[i - 1]);
    assert(finishTime == expectedFinishTimes[i - 1]);
}


  // Check parent of each vertex
  int expected_parent[] = {-1, 1, 2, 3, 1};
  printf("\nParent of each vertex:\n");
  for (int i = 1; i <= numVertices; i++) {
    int parent = getParent(G, i);
    printf("Vertex %d's parent: %d\n", i, getParent(G, i));
    assert(parent == expected_parent[i - 1]);
  }
  

  // Get path from vertex 1 to others
  int expected_paths[] = {1,2,3,4,5};
  printf("\nPaths from vertex 1 to others:\n");
  for (int i = 1; i <= numVertices; i++) {
    List path = newList();
    printf("Path from 1 to %d: ", i);
    assert(i == expected_paths[i - 1]);
    printList(stdout, path);
    printf("\n");
    freeList(&path);
  }

  // Free graph and list
  freeList(&S);
  freeGraph(&G);

  return 0;
}
