// Cyrus Amalan, camalan, pa4
#include <stdio.h>
#include <stdlib.h>

#include "List.h"
#include "Matrix.h"
#include <limits.h>

typedef struct EntryObj *Entry;

// private entry
typedef struct EntryObj {
  double value;
  int column;
} EntryObj;

typedef struct MatrixObj {
  int dimensions;
  int nonZeroElementsCount;
  List *rowEntries;
} MatrixObj;

Entry new_entry(int column, double value) {
  Entry e = malloc(sizeof(EntryObj));
  e->column = column;
  e->value = value;
  return e;
}

void free_entry(Entry *Pe) {
  if (*Pe != NULL && Pe != NULL) {
    free(*Pe);
    *Pe = NULL;
  }
}

Matrix newMatrix(int n) {
  int i;
  Matrix M = malloc(sizeof(MatrixObj));
  M->dimensions = n;
  M->nonZeroElementsCount = 0;
  M->rowEntries = calloc(n + 1, sizeof(List));
  for (i = 1; i <= n; i++) {
    M->rowEntries[i] = newList();
  }
  return M;
}

void freeMatrix(Matrix *pM) {
  int i;
  if (*pM != NULL && pM != NULL) {
    for (i = 1; i <= (*pM)->dimensions; i++) {
      List row = (*pM)->rowEntries[i];
      moveFront(row);
      while (index(row) >= 0) {
        Entry e = (Entry)get(row);
        free_entry(&e);
        moveNext(row);
      }
      freeList(&((*pM)->rowEntries[i]));
    }
    free((*pM)->rowEntries);
    free(*pM);
    *pM = NULL;
  }
}

int size(Matrix M) { return M->dimensions; }

int NNZ(Matrix M) { return M->nonZeroElementsCount; }

int equals(Matrix A, Matrix B) {
  if (A == NULL || B == NULL) {
    fprintf(stderr, "error: calling NULL matrices\n");
    exit(EXIT_FAILURE);
  }
  if (A->dimensions != B->dimensions ||
      A->nonZeroElementsCount != B->nonZeroElementsCount) {
    return 0;
  }
  for (int i = 1; i <= size(A); i++) {
    // Directly compare row lengths as a quick check for inequality
    if (length(A->rowEntries[i]) != length(B->rowEntries[i]))
      return 0;

    // Initialize iterators 
    moveFront(A->rowEntries[i]);
    moveFront(B->rowEntries[i]);

    // Iterate through the entries 
    while (
        index(A->rowEntries[i]) >=
        0) { // Assuming index() returns -1 when iterator is beyond list bounds
      Entry entryA = (Entry)get(A->rowEntries[i]);
      Entry entryB = (Entry)get(B->rowEntries[i]);

      // Check both column index and value 
      if (entryA->column != entryB->column || entryA->value != entryB->value)
        return 0;

      // Move iterators to the next element
      moveNext(A->rowEntries[i]);
      moveNext(B->rowEntries[i]);
    }
  }
  return 1;
}

void makeZero(Matrix M) {
  // Iterate each row in the matrix
  for (int rowIndex = 1; rowIndex <= size(M); rowIndex++) {
    // Clear the contents of each row list
    clear(M->rowEntries[rowIndex]);
  }
  // Reset the count
  M->nonZeroElementsCount = 0;
}

void changeEntry(Matrix M, int i, int j, double x) {
  if (!M) {
    fprintf(stderr, "Error: NULL Matrix\n");
    exit(EXIT_FAILURE);
  }
  if (i < 1 || i > M->dimensions || j < 1 || j > M->dimensions) {
    fprintf(stderr, "Error: out of bounds.\n");
    exit(EXIT_FAILURE);
  }

  List row = M->rowEntries[i];
  for (moveFront(row); index(row) != -1; moveNext(row)) {
    Entry entry = (Entry)get(row);
    if (entry->column == j) {
      if (x == 0.0) {
        delete (row); // removes the current node
        M->nonZeroElementsCount--;
        return;
      } else {
        entry->value = x;
        return;
      }
    }
    if (entry->column > j) {
      if (x != 0.0) {
        insertBefore(row, new_entry(j, x));
        M->nonZeroElementsCount++;
        return;
      } else {
        // If x is 0, no action is needed
        return;
      }
    }
  }

  // append the new entry
  if (x != 0.0) {
    append(row, new_entry(j, x));
    M->nonZeroElementsCount++;
  }
}

Matrix copy(Matrix A) {
  // Create a new matrix with the size as A
  Matrix B = newMatrix(A->dimensions);

  // Iterate through each row of A
  for (int rowIndex = 1; rowIndex <= A->dimensions; rowIndex++) {
    // Access the list for the current row
    List currentRow = A->rowEntries[rowIndex];

    // iterate over the entries of the row
    for (moveFront(currentRow); index(currentRow) >= 0; moveNext(currentRow)) {
      Entry currentEntry = (Entry)get(currentRow);
      // add each entry from A to the corresponding place 
      changeEntry(B, rowIndex, currentEntry->column, currentEntry->value);
    }
  }

  return B;
}

Matrix transpose(Matrix A) {
  // Initialize a new matrix B 
  Matrix B = newMatrix(A->dimensions);

  // Loop over each row of matrix A
  for (int rowIndex = 1; rowIndex <= A->dimensions; rowIndex++) {
    // Retrieve the list A
    List currentRow = A->rowEntries[rowIndex];

    // Iterate through each entry in the current row
    for (moveFront(currentRow); index(currentRow) != -1; moveNext(currentRow)) {
      Entry currentEntry = (Entry)get(currentRow);
      changeEntry(B, currentEntry->column, rowIndex, currentEntry->value);
    }
  }

  return B;
}

Matrix scalarMult(double x, Matrix A) {
  // Initialize a new matrix B
  Matrix B = newMatrix(A->dimensions);

  // Loop through each row of A
  for (int i = 1; i <= A->dimensions; i++) {
    List currentRow = A->rowEntries[i];

    // Iterate over the entries
    for (moveFront(currentRow); index(currentRow) != -1; moveNext(currentRow)) {
      Entry entry = (Entry)get(currentRow);
      double newValue = x * entry->value; // Perform the scalar multiplication

      // If not zero, update the entry in B
      if (newValue != 0.0) {
        changeEntry(B, i, entry->column, newValue);
      }
    }
  }

  return B;
}

Matrix sum(Matrix A, Matrix B) {
  if (!A || !B) {
    fprintf(stderr, "Error: Cannot perform sum on NULL matrices.\n");
    exit(EXIT_FAILURE);
  }
  if (size(A) != size(B)) {
    fprintf(stderr, "Error: Matrices must be of the same size to sum.\n");
    exit(EXIT_FAILURE);
  }
  if (A == B) {
    return scalarMult(2, A); // Optimized case for adding a matrix to itself
  }

  Matrix C = newMatrix(size(A));
  for (int i = 1; i <= size(A); i++) {
    List rowA = A->rowEntries[i];
    List rowB = B->rowEntries[i];
    moveFront(rowA);
    moveFront(rowB);

    while (index(rowA) >= 0 || index(rowB) >= 0) {
      Entry eA;
      Entry eB;
      int colA;
      int colB;

      // Check if is valid (>= 0)
      if (index(rowA) >= 0) {
        // If valid, get the Entry current index
        eA = (Entry)get(rowA);
      } else {
        // If not valid, set eA to NULL
        eA = NULL;
      }

      // Check if  rowB is valid (>= 0)
      if (index(rowB) >= 0) {
        // If valid, get the Entry at rowB's current index
        eB = (Entry)get(rowB);
      } else {
        // If not valid, set eB to NULL
        eB = NULL;
      }

      // Check if eA is not NULL
      if (eA) {
        // If eA is not NULL
        colA = eA->column;
      } else {
        // If eA is NULL, set colA to INT_MAX
        colA = INT_MAX;
      }

      // Check if eB is not NULL
      if (eB) {
        // If eB is not NULL, get the column number from eB
        colB = eB->column;
      } else {
        // If eB is NULL, set colB to INT_MAX
        colB = INT_MAX;
      }

      if (colA == colB) {
        double sumVal = eA->value + eB->value;
        if (sumVal != 0.0) {
          append(C->rowEntries[i], new_entry(colA, sumVal));
        }
        moveNext(rowA);
        moveNext(rowB);
      } else if (colA < colB) {
        append(C->rowEntries[i], new_entry(colA, eA->value));
        moveNext(rowA);
      } else { // colB < colA || colB is INT_MAX
        append(C->rowEntries[i], new_entry(colB, eB->value));
        moveNext(rowB);
      }
    }
  }
  // After merging, compute NNZ for C
  C->nonZeroElementsCount = 0;
  for (int i = 1; i <= size(C); i++) {
    moveFront(C->rowEntries[i]);
    while (index(C->rowEntries[i]) >= 0) {
      C->nonZeroElementsCount++;
      moveNext(C->rowEntries[i]);
    }
  }
  return C;
}

Matrix diff(Matrix A, Matrix B) {
  if (A->dimensions != B->dimensions) {
    fprintf(stderr, "Error: matrices not the same size.\n");
    exit(EXIT_FAILURE);
  }
  return sum(A, scalarMult(-1.0, B));
}

double vectorDot(List P, List Q) {
  double d_Product = 0.0; 
  moveFront(P);
  moveFront(Q);

  while (index(P) >= 0 && index(Q) >= 0) {
    Entry entryP = (Entry)get(P); // Current entry in P
    Entry entryQ = (Entry)get(Q); // Current entry in Q

    // Compare column indices
    if (entryP->column == entryQ->column) {
      d_Product += entryP->value * entryQ->value;
      moveNext(P); // movenext in P
      moveNext(Q); // movenext in Q
    } else if (entryP->column < entryQ->column) {
      moveNext(P); //advance in P if its column index is less
    } else {
      moveNext(Q);
    }
  }

  return d_Product; // Return the dot product
}

Matrix product(Matrix A, Matrix B) {
  if (!A || !B) {
    fprintf(stderr, "Error: NULL matrices.\n");
    exit(EXIT_FAILURE);
  }
  if (A->dimensions != B->dimensions) {
    fprintf(stderr,
            "Error: need matrices of equal dimensions.\n");
    exit(EXIT_FAILURE);
  }

  Matrix result = newMatrix(A->dimensions); // Create result matrix
  if (A->nonZeroElementsCount == 0 || B->nonZeroElementsCount == 0)
    return result; // Return an empty matrix if A or B is empty

  Matrix transposedB = transpose(B); // Transpose B

  for (int i = 1; i <= A->dimensions; i++) {
    if (length(A->rowEntries[i]) == 0)
      continue; // Skip empty rows in A

    for (int j = 1; j <= transposedB->dimensions; j++) {
      if (length(transposedB->rowEntries[j]) == 0)
        continue; // Skip empty rows in transposedB

      double dotProduct =
          vectorDot(A->rowEntries[i], transposedB->rowEntries[j]);
      if (dotProduct != 0.0) {
        changeEntry(result, i, j,
                    dotProduct); // Use changeEntry to calculate NNZ count
      }
    }
  }

  freeMatrix(&transposedB); // Free
  return result;
}

void printMatrix(FILE *out, Matrix M) {
  if (!out || !M) {
    fprintf(stderr, "Error: Attempted to print from/to a NULL reference.\n");
    exit(EXIT_FAILURE);
  }

  for (int row = 1; row <= size(M); row++) {
    List currentRow = M->rowEntries[row];
    if (length(currentRow) == 0)
      continue; // Skip empty rows

    fprintf(out, "%d: ", row);
    moveFront(currentRow); // Position at the start of the row

    while (index(currentRow) != -1) {
      Entry entry = (Entry)get(currentRow);
      fprintf(out, "(%d, %.1f) ", entry->column, entry->value);
      moveNext(currentRow); // Move to the next entry
    }

    fprintf(out, "\n"); // New line
  }
}
