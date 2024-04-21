//Cyrus Amalan, camalan, pa7

#include <iostream>
#include <fstream>
#include "Dictionary.h"

using namespace std;

//function for reading
ifstream openInFile(const char* filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Can't open file" << filename << " to reading" << endl;
        exit(EXIT_FAILURE);
    }
    return inFile;
}

// Function to open a file 
ofstream openOutFile(const char* filename) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Can't to open file " << filename << " to write in" << endl;
        exit(EXIT_FAILURE);
    }
    return outfile;
}


int main(int argc, char* argv[]){
    //argument check
    if(argc != 3){
        std::cout << "Needs to have 3 arguements" << endl;
        exit(EXIT_FAILURE);
    }


    auto input_file = openInFile(argv[1]);
    auto output_file = openOutFile(argv[2]);

    Dictionary dictionary;
    std::string key;
    int value;
    value = 1;
    //iterate through input file
    while(std::getline(input_file, key)){
        dictionary.setValue(key, value);
        value += 1;
    }

    output_file << dictionary.to_string() << endl;
    output_file << dictionary.pre_string() << endl;

    //clear dictionary
    dictionary.clear();

    //close files
    input_file.close();
    output_file.close();

    return 0;

    
}
