// Cyrus Amalan, camalan, pa4

#include "Matrix.h"
#include "List.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  // Create matrices with different sizes
  Matrix A = newMatrix(3);
  Matrix B = newMatrix(4);

  // Change entries in matrix A
  changeEntry(A, 1, 1, 1.0); // 1.0
  changeEntry(A, 2, 2, 2.0); // 2.0
  changeEntry(A, 3, 3, 3.0); // 3.0

  // Change entries in matrix B
  changeEntry(B, 1, 4, 4.0); // 4.0
  changeEntry(B, 2, 3, 3.0); // 3.0
  changeEntry(B, 3, 2, 2.0); // 2.0
  changeEntry(B, 4, 1, 1.0); // 1.0

  // Test NNZ
  assert(NNZ(A) == 3);
  assert(NNZ(B) == 4);

  // Test size
  assert(size(A) == 3);
  assert(size(B) == 4);

  // Test scalar multiplication
  Matrix C = scalarMult(2.0, A);
  assert(NNZ(C) == 3); // Test scalar multiplication

  // Test transpose
  Matrix F = transpose(A);
  assert(NNZ(F) == 3);

  // Test copy
  Matrix H = copy(A);
  assert(equals(A, H)); // H should be a copy of A

  // Free matrix
  freeMatrix(&A);
  freeMatrix(&B);
  freeMatrix(&C);
  freeMatrix(&F);
  freeMatrix(&H);

  printf("Tests succsessfully passed.\n");
  return 0;
}
