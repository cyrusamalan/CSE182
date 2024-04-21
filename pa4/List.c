// Cyrus Amalan, camalan, pa4

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

// Creates new type Node, which is a pointer to struct NodeObj
typedef struct NodeObj *Node;

// Represents a node iu a doubly linked list
typedef struct NodeObj {
  void *data;
  struct NodeObj *prev;
  struct NodeObj *next;
} NodeObj;

// Represents the entire doubly linked list containing five members
typedef struct ListObj {
  Node front;
  Node back;
  Node cursor;
  int index;
  int length;
} ListObj;

// Returns reference to new Node object. Initializes next and data fields.
Node newNode(void *data) {
  Node new_node = malloc(sizeof(NodeObj));
  new_node->data = data;
  new_node->next = NULL;
  new_node->prev = NULL;
  return new_node;
}

// Frees heap memory by setting *pl to NULL
void freeNode(Node *pN) {
  if (*pN != NULL && pN != NULL) {
    free(*pN);
    *pN = NULL;
  }
}

// Creates and returns a new empty List.
List newList(void) {
  List L;
  L = malloc(sizeof(ListObj));
  L->back = NULL;
  L->cursor = NULL;
  L->front = NULL;
  L->length = 0;
  L->index = -1; // points to no valid element in list
  return L;
}

void freeList(List *pL) {
  if (pL != NULL && *pL != NULL) {
    while (length(*pL) != 0) {
      deleteFront(*pL);
    }
    free(*pL);
    *pL = NULL;
  }
}

int length(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: calling list contain NULL\n");
    exit(EXIT_FAILURE);
  }
  return L->length;
}

int index(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: calling index of NULL list\n");
    exit(EXIT_FAILURE);
  }
  return L->index;
}

void *front(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: calling front of NULL list\n");
    exit(EXIT_FAILURE);
  }
  if (L->length == 0) {
    fprintf(stderr, "error: Pre: length()>0\n");
    exit(EXIT_FAILURE);
  }
  return L->front->data;
}

void *back(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: calling back of NULL list\n");
    exit(EXIT_FAILURE);
  }
  if (L->length == 0) {
    fprintf(stderr, "error: Pre: length()>0\n");
    exit(EXIT_FAILURE);
  }
  return L->back->data;
}

void *get(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: calling cursor of NULL list\n");
    exit(EXIT_FAILURE);
  }
  if (L->length == 0) {
    fprintf(stderr, "error: Pre: length()>0\n");
    exit(EXIT_FAILURE);
  }
  if (L->index < 0) {
    fprintf(stderr, "error: Pre: index()>=0");
    exit(EXIT_FAILURE);
  }
  return L->cursor->data;
}
void clear(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: list is NULL\n");
    exit(EXIT_FAILURE);
  }
  Node current = L->front;
  while (current != NULL) {
    Node toDelete = current;
    current = current->next;
    freeNode(&toDelete);
  }

  // reset
  L->front = NULL;
  L->back = NULL;
  L->cursor = NULL;
  L->length = 0;
  L->index = -1;
}

void set(List L, void *x) {
  if (L == NULL) {
    fprintf(stderr, "error: calling set with NULL list\n");
    exit(EXIT_FAILURE);
  }
  if (L->length == 0) {
    fprintf(stderr, "error:  Pre: length()>0\n");
    exit(EXIT_FAILURE);
  }
  if (L->index < 0) {
    fprintf(stderr, "error: index()>=0\n");
    exit(EXIT_FAILURE);
  }
  L->cursor->data = x;
}

void moveFront(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: List is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (L->length > 0) {
    L->cursor = L->front;
    L->index = 0;
  }
}

void moveBack(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: List is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (L->length > 0) {
    L->cursor = L->back;
    L->index = L->length - 1;
  }
}

void movePrev(List L) {
  if (L == NULL) { // Checks if list is NULL
    fprintf(stderr, "error: List is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (L->cursor == L->front) {
    L->cursor = NULL;
    L->index = -1;
  } else {
    L->index -= 1;
    L->cursor = L->cursor->prev; // if cursor not at front, decrements the index
                                 // and moves the cursor to prev node
  }
}

void moveNext(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: List is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (L->cursor != NULL) {
    if (L->cursor != L->back) {
      L->index += 1;
      L->cursor = L->cursor->next;
    } else {
      L->cursor = NULL;
      L->index = -1;
    }
  }
}

void prepend(List L, void *x) {
  Node N = newNode(x);
  if (L->length == 0) {
    L->front = N;
    L->back = N;
    L->cursor = N;
    L->index = 0;
  } else {
    N->next = L->front;
    L->front->prev = N;
    L->front = N;
    L->index++;
  }
  L->length++;
}

void append(List L, void *x) {
  Node N = newNode(x);
  if (L->length == 0) {
    L->front = L->back = N; // inertion takes place after back element
  } else {
    N->prev = L->back;
    L->back->next = N;
    L->back = N;
  }
  L->length++;
}

// Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, void *x) {
  if (L == NULL) {
    fprintf(stderr, "error: List is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (L->length <= 0 || L->index < 0) {
    fprintf(stderr, "error: Pre: length()>0, index()>=0\n");
    exit(EXIT_FAILURE);
  }
  if (L->cursor != NULL) {
    Node N = newNode(x);
    if (L->cursor == L->front) {
      N->next = L->front;
      L->front->prev = N;
      L->front = N;
    } else {
      N->prev = L->cursor->prev;
      N->next = L->cursor;
      L->cursor->prev->next = N;
      L->cursor->prev = N;
    }
    L->length++;
    L->index++;
  }
}

// Insert new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, void *x) {
  if (L == NULL) {
    fprintf(stderr, "error: List is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (L->length == 0 || L->index < 0) {
    fprintf(stderr, "error: Pre: length()>0, index()>=0\n");
    exit(EXIT_FAILURE);
  }

  if (L->cursor != NULL) {
    Node N = newNode(x);
    if (L->cursor == L->back) {
      N->prev = L->back;
      L->back->next = N;
      L->back = N;
    } else {
      N->next = L->cursor->next;
      N->prev = L->cursor;
      L->cursor->next = N;
    }
    L->length++;
  }
}

void deleteFront(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: List is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (L->length == 0) {
    fprintf(stderr, "error: Pre: length()>0\n");
    exit(EXIT_FAILURE);
  }
  if (L->length > 0) {
    Node nodeToRemove = L->front;
    if (L->cursor == L->front) {
      L->cursor = NULL;
      L->index = -1;
    }
    if (L->length > 1) {
      L->front = L->front->next;
      L->front->prev = NULL;
      if (L->index != -1) {
        L->index--;
      }
    } else {
      L->front = L->back = NULL;
      if (L->index != -1) {
        L->index--;
      }
    }
    L->length--;
    freeNode(&nodeToRemove);
  }
}

void deleteBack(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: List is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (L->length <= 0) {
    fprintf(stderr, "error: Pre: length()>0\n");
    exit(EXIT_FAILURE);
  }
  Node toRemove = L->back;
  if (L->cursor == L->back) {
    L->cursor = NULL;
    L->index = -1;
  }
  if (length(L) == 1) {
    L->front = NULL;
    L->back = NULL;
    L->length = 0;
    freeNode(&toRemove);
  } else {
    L->back = toRemove->prev;
    L->back->next = NULL;
    L->length--;
    freeNode(&toRemove);
  }
}

void delete (List L) {
  if (L == NULL) {
    fprintf(stderr, "error: List is NULL\n");
    exit(EXIT_FAILURE);
  }
  if (L->length == 0 || L->index < 0) {
    fprintf(stderr, "error: Pre: length()>0, index()>=0\n");
    exit(EXIT_FAILURE);
  }
  Node toFree = L->cursor;
  L->cursor = NULL;
  if (toFree == L->back) {
    L->back = toFree->prev;
    if (L->back != NULL) {
      L->back->next = NULL;
    }
  } else if (toFree == L->front) {
    L->front = toFree->next;
    if (L->front != NULL) {
      L->front->prev = NULL;
    }
  } else {
    toFree->next->prev = toFree->prev;
    toFree->prev->next = toFree->next;
  }
  L->length--;
  L->index = -1;
  L->cursor = NULL;
  freeNode(&toFree);
}

void printList(FILE *out, List L) {
  if (out == NULL || L == NULL) {
    fprintf(stderr, "Error: printList() called with NULL parameters\n");
    return;
  }
  for (Node current = L->front; current != NULL; current = current->next) {
    fprintf(out, "%p ", current->data);
  }
}

List copyList(List L) {
  if (L == NULL) {
    fprintf(stderr, "error: List is NULL\n");
    exit(EXIT_FAILURE);
  }
  List new_list = newList();
  for (Node N = L->front; N != NULL; N = N->next) {
    append(new_list, N->data);
  }
  return new_list;
}