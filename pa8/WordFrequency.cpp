//Cyrus Amalan, camalan, pa8

#include <fstream>
#include <cctype> 
#include "Dictionary.h"

using namespace std;

//  check if character is delimiter
bool isDelimiter(char ch, const string& delimiters) {
    return delimiters.find(ch) != string::npos;
}

// Convert string to lowercase
string toLowerCase(const string& s) {
    string result;
    for (char ch : s) {
        result += tolower(static_cast<unsigned char>(ch));
    }
    return result;
}

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

int main(int argc, char* argv[]) {

    string line, word;
    string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";
    Dictionary D;

    if (argc != 3) {
        cerr << "Needs to habe 3 arguements" << endl;
        return EXIT_FAILURE;
    }

    auto input_file = openInFile(argv[1]);
    auto output_file = openOutFile(argv[2]);
   

    while (getline(input_file, line)) {
        size_t i = 0, start = 0, len = line.length();
        while (i <= len) {
            if (i == len || isDelimiter(line[i], delim)) {
                if (start < i) {
                    word = toLowerCase(line.substr(start, i - start));
                    if (D.contains(word)) {
                        D.getValue(word)++;
                    } else {
                        D.setValue(word, 1);
                    }
                }
                start = i + 1;
            }
            i++;
        }
    }

    output_file << D << endl;

    input_file.close();
    output_file.close();

    return EXIT_SUCCESS;
}
