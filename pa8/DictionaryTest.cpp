//Cyrus Amalan, camalan, pa8

#include<iostream>
#include<string>
#include<stdexcept>
#include<cassert>
#include"Dictionary.h"

using namespace std;

int main() {
    string S[] = {
        "regret",
        "navy",
        "fight",
        "sour",
        "objective",
        "pity",
        "village",
        "correspondence",
        "jail",
        "camp",
        "construct",
        "drink",
        "cancer",
        "inspector",
        "prestige",
        "despair",
        "agony",
        "deteriorate",
        "pressure",
        "baseball",
        "shed",
        "rotation",
        "smash",
        "atmosphere",
        "fence",
        "remain",
        "try",
        "predict"
    };

    // Use a subset of S[] 
    string T[] = {
        "regret",
        "navy",
        "fight",
        "sour",
        "objective",
        "pity",
        "village"
    };

    Dictionary A;
    Dictionary B;

    cout << endl;


    // Insert some pairs into A from the updated S[] list
    for(size_t i = 0; i < sizeof(S) / sizeof(S[0]); i++) {
        A.setValue(S[i], i+1);
        assert(A.getValue(S[i]) == static_cast<valType>(i+1));
    }

    assert(A.size() == sizeof(S) / sizeof(S[0]));

    // Call operator=() to create B as a copy of A
    B = A;

    // Perform additional inserts to differentiate B from A
    B.setValue("dynamic", 101);
    B.setValue("framework", 102);
    B.setValue("algorithm", 103);
    B.setValue("data", 104);
    B.setValue("structure", 105);

    // Call copy constructor to create C as a copy of B
    Dictionary C = B;

    cout << "A.size() = " << A.size() << endl << A << endl;
    cout << "B.size() = " << B.size() << endl << B << endl;
    cout << "C.size() = " << C.size() << endl << C << endl;

    // Verify the copy constructor
    cout << "Copy constructor creates identical trees: " << (C.pre_string() == B.pre_string() ? "true" : "false") << endl;
    cout << endl;

    // Check operator
    cout << "A==B is " << (A==B ? "true" : "false") << endl;
    cout << "B==C is " << (B==C ? "true" : "false") << endl;
    cout << "C==A is " << (C==A ? "true" : "false") << endl << endl;

    // Test modification and retrieval
    cout << "Modifying and retrieving values:" << endl;
    A.setValue("predict", 200);
    cout << "A.getValue(\"predict\") after modification = " << A.getValue("predict") << endl << endl;
    assert(A.getValue("predict") == 200);

    // Remove some pairs from A using T[]
    for(size_t i = 0; i < sizeof(T) / sizeof(T[0]); i++) {
        A.remove(T[i]);
    }

    // Verify state of A 
    cout << "A.size() after removals = " << A.size() << endl << A << endl;
    cout << "A pre-order after removals:" << endl << A.pre_string() << endl;

    // Forward iteration on B 
    cout << "Forward iteration on B, modifying values:" << endl;
    for(B.begin(); B.hasCurrent(); B.next()) {
        cout << "(" << B.currentKey() << ", " << B.currentVal() << ") ";
        B.currentVal()++; // Increment value
        cout << "-> (" << B.currentKey() << ", " << B.currentVal() << ")" << endl;
    }
    cout << endl;

    // Reverse iteration on C 
    cout << "Reverse iteration on C, modifying values:" << endl;
    for(C.end(); C.hasCurrent(); C.prev()) {
        cout << "(" << C.currentKey() << ", " << C.currentVal() << ") ";
        C.currentVal()++; // Increment value
        cout << "-> (" << C.currentKey() << ", " << C.currentVal() << ")" << endl;
    }
    cout << endl;

     try {
        A.getValue("nonexistent");
        assert(false); // assertion fails if this is reached
    } catch(const logic_error& e) {
        assert(true); // exception
    }


    cout << "All assert cases passed" << endl;
    string testKeys;
}