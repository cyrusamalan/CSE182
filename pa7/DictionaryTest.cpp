//Cyrus Amalan, camalan, pa7

#include "Dictionary.h"
#include <iostream>
#include <cassert>

int main() {
    // Create a Dictionary
    //added value pairs
    Dictionary dict;
    dict.setValue("black", 10);
    dict.setValue("green", 20);
    dict.setValue("blue", 30);
    dict.setValue("yellow", 40);
    assert(dict.size() == 4);

    // Test size
    std::cout << "Size after additions: " << dict.size() << std::endl;
    
    assert(dict.contains("green"));
    assert(dict.getValue("green") == 20);
    // Test to_string()
    std::cout << "Dictionary contents:\n" << dict.to_string() << std::endl;

    // Test getValue and contains
    if (dict.contains("green")) {
        std::cout << "Value for 'green': " << dict.getValue("green") << std::endl;
    }

    // Test remove
    dict.remove("green");
    std::cout << "Size after removal: " << dict.size() << std::endl;
    assert(!dict.contains("green"));
    assert(dict.size() == 3);
  
    // Test iterator functions
    dict.begin();
    while (dict.hasCurrent()) {
        std::cout << dict.currentKey() << " : " << dict.currentVal() << std::endl;
        dict.next();
    }

    // Test clear
    dict.clear();
    std::cout << "Size after clear: " << dict.size() << std::endl;
    assert(dict.size() == 0);

    // Test copy constructor and equals
    dict.setValue("black", 10);
    Dictionary dictCopy = dict;
    std::cout << "Copy equals original: " << (dict.equals(dictCopy) ? "Yes" : "No") << std::endl;
    assert(dict.equals(dictCopy));

    // Test pre_string()
    dict.setValue("green", 20);
    dict.setValue("blue", 30);
    std::cout << "Pre-order string representation:\n" << dict.pre_string() << std::endl;

    // operator tests
    Dictionary anotherDict;
    anotherDict.setValue("orange", 50);
    std::cout << "Another dictionary:\n" << anotherDict;
    bool isEqual = dict == anotherDict;
    std::cout << "Original equals another dictionary: " << (isEqual ? "Yes" : "No") << std::endl;
    assert(!isEqual);
    std::cout << "All tests passed." << std::endl;
 
    return 0;
}
