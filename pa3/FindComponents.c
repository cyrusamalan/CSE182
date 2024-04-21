// Cyrus Amalan, camalan, pa3

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "List.h"

int main(int argc, char *argv[]) {
  FILE *input, *output;
  // declare variables
  int num, b, c, d = 0;
  int count = 0;

  Graph g, new_g;
  List L = newList();
  // check to see if it is giving the correct amount of arguements
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  // Null input file check
  if ((input = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "error: cannot open input file\n");
    exit(EXIT_FAILURE);
  }
  // NULL ouput file check
  if ((output = fopen(argv[2], "w")) == NULL) {
    fprintf(stderr, "error: cannot write into file\n");
    exit(EXIT_FAILURE);
  }
  // get the number of vertices
  fscanf(input, " %d", &num);

  // Create a new graph with "num" vertices
  g = newGraph(num);

  // add the edges found to the graph
  while (1) {
    fscanf(input, " %d %d", &b, &c);
    if (b == 0 || c == 0)
      break;
    if (feof(input))
      break;
    addArc(g, b, c);
  }
  // print the adj list of G
  fprintf(output, "Adjacency list representation of G:\n");
  printGraph(output, g);
  fprintf(output, "\n");

  // Get transpose of G
  new_g = transpose(g);

  // Add to the stack
  for (int i = 1; i <= num; i++) {
    append(L, i);
  }

  // call DFS() function
  DFS(g, L);
  DFS(new_g, L);

  // count the strongly connected components
  int i = 1;
  while (i <= num) {
    if (getParent(new_g, i) == NIL) {
      d++;
    }
    i++;
  }

  // create an array
  List *strong_comp;
  strong_comp = calloc(d, sizeof(List));

  for (int i = 0; i < d; i++) {
    strong_comp[i] = newList();
  }

  // move vertice back
  moveBack(L);

  for (moveBack(L); index(L) >= 0; movePrev(L)) {
    int y = get(L);
    prepend(strong_comp[count], y);
    if (getParent(new_g, y) == NIL) {
      count++;
    }
  }

  // print out the strongly connected components
  fprintf(output, "G contains %d strongly connected components:\n", d);

  for (int i = 0; i < d; i++) {
    fprintf(output, "Component %d: ", i + 1);
    printList(output, strong_comp[i]);
    fprintf(output, "\n");
    freeList(&strong_comp[i]);
  }

  // free memory
  free(strong_comp);
  freeGraph(&g);
  freeGraph(&new_g);
  freeList(&L);
  fclose(input);
  fclose(output);
}