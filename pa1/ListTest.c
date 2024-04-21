// Cyrus Amalan, camalan, pa1

#include "List.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
  // Test newList() and freeList()
  List myList = newList();
  printf("List created with length %d\n", length(myList)); //print 0

  // Test append()
  append(myList, 8);
  append(myList, 10);
  append(myList, 9);
  printf("After appending 3 elements, List length is %d\n",
         length(myList)); //print 3
   assert(length(myList) == 3);

  // Test front() and back()
  printf("Front element is %d\n", front(myList)); //print 8
  printf("Back element is %d\n", back(myList));   //print 9
  assert(front(myList) == 8);
  assert(back(myList) == 9);

  // Test prepend()
  prepend(myList, 2);
  printf("prepending one element, Front element: %d\n",
         front(myList)); //print 2
   assert(front(myList) == 2);

  // Test moveFront(), moveBack(), movePrev(), moveNext()
  moveFront(myList);
  printf("Cursor is at front, element is %d\n", get(myList)); //print 2
  moveNext(myList);
  printf("cursor moved next, element is %d\n", get(myList)); //print 8
  moveBack(myList);
  printf("Cursor at back, element is %d\n", get(myList)); //print 9
  movePrev(myList);
  printf("Moved cursor previous, element is %d\n",
         get(myList)); //print 10
   assert(get(myList) == 10);

  // Test insertBefore() and insertAfter()
  insertBefore(myList, 4);
  insertAfter(myList, 12);
  movePrev(myList);                                     // Move to element 4
  printf("Element before cursor is %d\n", get(myList)); // Should print 4
  assert(get(myList) == 4);
  moveNext(myList);
  moveNext(myList);                                    // Move to element 12
  printf("Element after cursor is %d\n", get(myList)); // Should print 12
  assert(get(myList) == 12);

  // Test deleteFront(), deleteBack() and delete()
  deleteFront(myList);
  printf("After deleting front, Front element is %d\n",
         front(myList)); // Should print 8
   assert(front(myList) == 8);
  deleteBack(myList);
  printf("After deleting back, Back element is %d\n",
         back(myList)); // Should print 10
   assert(back(myList) == 10);
  delete (myList);      // Deletes element 12, as the cursor is on it.
  moveBack(myList);
  printf("After deleting cursor element, Back element is %d\n",
         back(myList)); // Should print 10
   assert(back(myList) == 10);

  // Test printlist() and clear()
  printf("Current List: ");
  printList(stdout, myList); // Prints the current list elements
  clear(myList);
  printf("List cleared, Length is %d\n", length(myList)); // Should print 0
  assert(length(myList) == 0);

  // Test copyList()
  append(myList, 100);
  append(myList, 200);
  List copy_list = copyList(myList);
  printf("Copy of List created. Length is %d\n",
         length(copy_list)); // Should print 2
   assert(length(copy_list) == 2);
  printf("Front element of copied list is %d\n",
         front(copy_list)); // Should print 100
   assert(front(copy_list) == 100);

  // Clean up
  freeList(&myList);
  freeList(&copy_list);

  return 0;
}