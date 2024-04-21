//Cyrus Amalan, camalan, pa6

#include <iostream>
#include <cassert>
#include "List.h"

int main() {
    // Test default
    List myList;
    std::cout << "Length: " << myList.length() << std::endl;

    // Test insertion functions
    myList.insertBefore(1);
    myList.insertAfter(2);
    std::cout << "Length: " << myList.length() << std::endl;
    assert(myList.length() == 2);

    // Test the move functions
    myList.moveFront();
    std::cout << "PeekNext: " << myList.peekNext() << std::endl;
    assert(myList.front() == 1);
    myList.moveNext();
    std::cout << "PeekNext: " << myList.peekNext() << std::endl;
    assert(myList.moveNext() == 2);
    myList.moveBack();
    std::cout << "PeekPrev: " << myList.peekPrev() << std::endl;

    // Test copy constructor
    List myListCopy(myList);
    std::cout << "Copy length: " << myListCopy.length() << std::endl;

    // Test assignment operator
    List myListAssign;
    myListAssign = myList;
    std::cout << "Assign length: " << myListAssign.length() << std::endl;

    // Test for equality
    std::cout << "Lists are equal: " << (myList == myListCopy ? "True" : "False") << std::endl;

    // Test clear function
    myList.clear();
    std::cout << "Length: " << myList.length() << std::endl;
    assert(myList.length() == 0);

    // Test destructor 
    List* dynamic_list = new List();
    dynamic_list->insertAfter(3);
    delete dynamic_list;

    // If your program reached here without crashing, the basic functionality is likely correct
    std::cout << "Test successful" << std::endl;

    return 0;
}