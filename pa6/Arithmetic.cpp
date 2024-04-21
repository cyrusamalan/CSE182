//Cyrus Amalan, camalan, pa6

#include <iostream>
#include <fstream>
#include <string>
#include "BigInteger.h" 

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

BigInteger pow(const BigInteger& base, int exponent) {
    BigInteger power("1");
    for (int i = 0; i < exponent; ++i) {
        power = power * base;
    }
    return power;
}


//write to file
void Calculations(ifstream& input, ofstream& output) {//converts to big integer objects
    string sA, sB;
    input >> sA >> sB;

    BigInteger A(sA), B(sB);
    BigInteger two("2"), three("3"), nine("9"), sixteen("16");

    output << A << "\n\n"
        << B << "\n\n"
        << A + B << "\n\n"
        << A - B << "\n\n"
        << A - A << "\n\n"
        << (three * A) - (two * B) << "\n\n"
        << A * B << "\n\n"
        << A * A << "\n\n"
        << B * B << "\n\n"
        << (nine * pow(A, 4)) + (sixteen * pow(B, 5)) << "\n\n";
}

int main(int argc, char* argv[]) {
    // Arguement check
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " Needs to have 3 arguements" << endl;
        return EXIT_FAILURE;
    }

    auto infile = openInFile(argv[1]);
    auto outfile = openOutFile(argv[2]);
    
    Calculations(infile, outfile);

    return 0;
}
