//Cyrus Amalan, camalan, pa5

#include <stdexcept>
#include <iomanip>
#include <string>
#include "List.h"

using namespace std;

void shuffle(List &D){
    List List1;
    List List2;
    int half;
    int a, b;
    int len = D.length();
    //create a half
    half = len / 2;
    D.moveFront();
    do{
        a = D.moveNext();
        List1.insertBefore(a);
    }while(D.position() < half);

    while(D.position() < len){
        b = D.moveNext();
        List2.insertBefore(b);
    }

    D.clear();
    List1.moveFront();
    List2.moveFront();
    //shuffle the elements

    while(List1.length() > List1.position() && List2.length() > List2.position()){
        a = List2.moveNext();
        D.insertBefore(a);
        b = List1.moveNext();
        D.insertBefore(b);
    }

    //If there are anymore elements from A && B, add them
    while(List1.length() > List1.position()){
        b = List1.moveNext();
        D.insertBefore(b);
    }

    while(List2.length() > List2.position()){
        a = List2.moveNext();
        D.insertBefore(a);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " must have 2 arguements" << endl;
        return EXIT_FAILURE;
    }

    int num = stoi(argv[1]);
    if (num <= 0) {
        cerr << "Error: decksize must be greater than 1" << endl;
        return EXIT_FAILURE;
    }

    cout << "deck size\tshuffle count" << endl;
    cout << "------------------------------" << endl;

    for (int n = 1; n <= num; ++n) {
        List Deck, copy;
        // Initialize the Deck
        for (int i = 0; i < n; ++i) {
            Deck.insertBefore(i);
        }

        int count = 0;
        copy = Deck; // Copy original Deck to copy variable
        do {
            shuffle(Deck);
            ++count;
        } while (!(Deck == copy)); // Continue shuffling until Deck is back to original

        cout << setw(16) << left << n << count << endl;
    }

    return EXIT_SUCCESS;
}







