#ifndef _LIST_H_INCLUDE_
#define _LIST_H_INCLUDE_
#include <stdbool.h>
#include <stdio.h>

#include "List.h"
typedef struct ListObj *List;

List newList(void);
void freeList(List *pL);

// Access functions -----------------------------------------------------------
int length(List L);
int index(List L);
void *front(List L);
void *back(List L);
void *get(List L);

// Manipulation procedures ----------------------------------------------------
void clear(List L);
void set(List L, void *x);
// Pre: length()>0, index()>=0
void moveFront(List L);
// otherwise does nothing.
void moveBack(List L);
// otherwise does nothing.
void movePrev(List L);

void moveNext(List L);
void prepend(List L, void *x);
void append(List L, void *x);
void insertBefore(List L, void *x);
void insertAfter(List L, void *x);
void deleteFront(List L);
void deleteBack(List L);
void delete (List L);

// Other operations -----------------------------------------------------------
void printList(FILE *out, List L);

#endif