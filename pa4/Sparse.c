//Cyrus Amalan, camalan, pa4

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"
#include "Matrix.h"
int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "error: need 3 arguements\n");
    exit(EXIT_FAILURE);
  }
  FILE* input;
  FILE* output;
  // read file
  input = fopen(argv[1], "r");
  
  if (!input) {
    fprintf(stderr, "cannot open input file\n");
    exit(EXIT_FAILURE);
  }

  output = fopen(argv[2], "w");
  if (!output) {
    fprintf(stderr, "no output file to open\n");
    exit(EXIT_FAILURE);
  }
   int x, y, z;
    fscanf(input, "%d %d %d", &x, &y, &z);
    Matrix A = newMatrix(x);
    Matrix B = newMatrix(x);

    for (int index = 0; index < y; index++) {
        int rows, column;
        double data;
        fscanf(input, "%d %d %lf", &rows, &column, &data);
        changeEntry(A, rows, column, data);
    }

    for (int index = 0; index < z; index++) {
        int rows, column;
        double data;
        fscanf(input, "%d %d %lf", &rows, &column, &data);
        changeEntry(B, rows, column, data);
    }

    fprintf(output, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(output, A);

    fprintf(output, "\nB has %d non-zero entries:\n", NNZ(B));
    printMatrix(output, B);
    fprintf(output, "\n");

    //(1.5)*A
    fprintf(output, "(1.5)*A =\n");
    Matrix C = scalarMult(1.5, A);
    printMatrix(output, C);
    fprintf(output, "\n");
    

    // A+B
    fprintf(output, "A+B =\n");
    Matrix D = sum(A, B);
    printMatrix(output, D);
    fprintf(output, "\n");
    

    // A+A 
    fprintf(output, "A+A =\n");
    Matrix E = sum(A, A);
    printMatrix(output, E);
    fprintf(output, "\n");
    

    // B-A
    fprintf(output, "B-A =\n");
    Matrix F = diff(B, A);
    printMatrix(output, F);
    fprintf(output, "\n");
    

    // A-A
    fprintf(output, "A-A =\n");
    Matrix G = diff(A, A);
    printMatrix(output, G);
    fprintf(output, "\n");
    

    // Transpose(A)
    fprintf(output, "Transpose(A) =\n");
    Matrix H = transpose(A);
    printMatrix(output, H);
    fprintf(output, "\n");
    

    // A*B
    fprintf(output, "A*B =\n");
    Matrix I = product(A, B);
    printMatrix(output, I);
    fprintf(output, "\n");
   

    // B*B
    fprintf(output, "B*B =\n");
    Matrix J = product(B, B);
    printMatrix(output, J);

    fprintf(output, "\n");
    
    freeMatrix(&J);
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    
    fclose(input);
    fclose(output);
}