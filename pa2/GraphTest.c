// Cyrus Amalan, camalan, pa2

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Graph.h"
#include "List.h"

int main() {
    // Create a Graph
    printf("Creating a Graph G with 5 vertices.\n");
    int num = 10;
    Graph G = newGraph(num);
    assert(G != NULL);
    assert(getOrder(G) == num);


    // Add Edges
    printf("Adding edges to Graph G.\n");
    addEdge(G, 1, 3);
    addEdge(G, 2, 4);
    addEdge(G, 3, 5);
    addEdge(G, 4, 8);
    addEdge(G, 5, 9);
    addEdge(G, 6, 8);
    addEdge(G, 7, 9);
    assert(getSize(G) == 7);

    // Print Graph
    printf("Printing Graph G:\n");
    printGraph(stdout, G);

    // Perform BFS
    printf("Performing BFS on Graph G from vertex 1.\n");
    BFS(G, 1);
    assert(getSource(G) == 1); // Assert BFS source is 1

    // Test Graph properties
    printf("Order of G: %d\n", getOrder(G));
    assert(getOrder(G) == 10); // Assert the order is still 5
    printf("Size of G: %d\n", getSize(G));
    assert(getSize(G) == 7); // Assert the size is still 4
    printf("Source used last in BFS: %d\n", getSource(G));

    // Check distance, parent, and path from source to each vertex
    for (int i = 1; i <= getOrder(G); i++) {
        printf("\nFor vertex %d:\n", i);
        printf("Parent: %d\n", getParent(G, i));
        printf("Distance from source: %d\n", getDist(G, i));
        List path = newList();
        getPath(path, G, i);
        printf("Path from source: ");
        printList(stdout, path);
        printf("\n");
        freeList(&path);
    }

    // Free graph memory
    freeGraph(&G);
    assert(!G);
    printf("\n\nALL TESTS PASSED\n");

    return 0;
}
