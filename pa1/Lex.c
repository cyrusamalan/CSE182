// Cyrus Amalan, camalan, pa1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

int main(int argc, char *argv[]) {
  // Check command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
    exit(1);
  }

  // Open the input file
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "could not open file '%s' to read\n", argv[1]);
    exit(1);
  }

  // Count the number of lines in the input file
  int line_count = 0;
  char line[1024];
  while (fgets(line, 1024, fp) != NULL) {
    line_count++;
  }
  rewind(fp); // Reset file pointer to the beginning for reading lines

  // Allocate memory for the array of lines
  char **lines = malloc(line_count * sizeof(char *));
  if (lines == NULL) {
    fprintf(stderr, "Error: memory allocation failed\n");
    exit(1);
  }

  // Read the lines of the input file
  int i = 0;
  while (fgets(line, 1024, fp) != NULL && i < line_count) {
    lines[i] = malloc((strlen(line) + 1) * sizeof(char));
    strcpy(lines[i], line);
    i++;
  }
  fclose(fp);

  // Create a list of indices into the array, sorted by lexicographic order
  List L = newList();
  append(L, 0);
  for (int i = 1; i < line_count; i++) {
    moveFront(L);
    while (index(L) >= 0 && strcmp(lines[i], lines[get(L)]) > 0) {
      moveNext(L);
    }
    if (index(L) >= 0) {
      insertBefore(L, i);
    } else {
      append(L, i);
    }
  }

  // Write sorted to output file
  FILE *out_fp = fopen(argv[2], "w");
  if (out_fp == NULL) {
    fprintf(stderr, "Error: could not open file '%s' to write in\n", argv[2]);
    exit(1);
  }
  moveFront(L);
  while (index(L) >= 0) {
    fprintf(out_fp, "%s", lines[get(L)]);
    moveNext(L);
  }
  fclose(out_fp);

  // Clean up memory
  freeList(&L);
  for (int i = 0; i < line_count; i++) {
    free(lines[i]);
  }
  free(lines);

  return 0;
}