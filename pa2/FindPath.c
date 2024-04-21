//Cyrus Amalan, camalan, pa2

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

int main(int argc, char* argv[]){
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE* INFILE = fopen(argv[1], "r");
    FILE* OUTFILE = fopen(argv[2], "w");

    if(INFILE == NULL){
        fprintf(stderr, "error: cannot open input file\n");
        exit(EXIT_FAILURE);
    }
    if(OUTFILE == NULL){
        fprintf(stderr, "error: cannot open output file\n");
        exit(EXIT_FAILURE);
    }

    int x, y, z;
    fscanf(INFILE, "%d", &x);
    Graph g = newGraph(x);
    //read and add the edges to the graph
    while(fscanf(INFILE, "%d %d", &y, &z) == 2){
        if(z == 0 && y == 0){
            break;
        }
        addEdge(g, y, z);
    }
    printGraph(OUTFILE, g);
    while(fscanf(INFILE, "%d %d", &y, &z) == 2){
        if(z == 0 && y == 0){
            break;
        }
        BFS(g, y);
        if(getDist(g, z) == INF){
            fprintf(OUTFILE, "The distance from %d to %d is infinity\n", y, z);
            fprintf(OUTFILE, "No %d-%d path exists\n", y, z);
        }else{
            fprintf(OUTFILE, "\n");
            fprintf(OUTFILE, "The distance from %d to %d is %d\n", y, z, getDist(g, z));
            fprintf(OUTFILE, "A shortest %d-%d path is: ", y, z);
            List l = newList();
            getPath(l, g, z);
            printList(OUTFILE, l);
            freeList(&l);
        }
        fprintf(OUTFILE, "\n");
    }
    freeGraph(&g);
    fclose(INFILE);
    fclose(OUTFILE);
}