//Cyrus Amalan, camalan, pa4
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "List.h"

int main() {
    // Create a new list
    List myList = newList();
    assert(length(myList) == 0); // Check if new list is empty

    // Append elements to the list
    int data1 = 1, data2 = 2, data3 = 3;
    append(myList, &data1);
    append(myList, &data2);
    append(myList, &data3);
    assert(length(myList) == 3); // Verify the list has 3 elements

    // Test front, back, and get functions
    assert(*(int*)front(myList) == 1);
    assert(*(int*)back(myList) == 3);

    //test get function
    moveFront(myList);
    assert(*(int*)get(myList) == 1);
    moveNext(myList);
    assert(*(int*)get(myList) == 2);

    // Test insertBefore and insertAfter functions
    int data4 = 4;
    insertBefore(myList, &data4);
    movePrev(myList); // Move cursor to the new element
    assert(*(int*)get(myList) == 4);

    int data5 = 5;
    insertAfter(myList, &data5);
    moveNext(myList); // Move element inserted after
    assert(*(int*)get(myList) == 5);

    // Test deleteFront, deleteBack, and delete functions
    deleteFront(myList);
    assert(length(myList) == 4); // decrease by 1
    deleteBack(myList);
    assert(length(myList) == 3); // decrease by 1
    delete(myList); // Delete cursor element
    assert(length(myList) == 2); // decrease by 1

    // Test clear function
    freeList(&myList);
    assert(length(myList) == 0); // empty list

    printf("All tests passed successfully.\n");
    return 0;
}
