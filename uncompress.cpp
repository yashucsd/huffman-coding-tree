#include <iostream>
#include <string>
#include "HCTree.h"

#define NUMARGS 3
#define FILEINDEX 2

/**
    Yash Nevatia & Josh Hunter
    uncompress
    takes in a input file and an output file
    decodes the contents of the input file with Huffman Coding
    prints decoded file to the output file

*/

using namespace std;

int main(int argc, char** argv){

    if(argc != NUMARGS) { //Wrong number of args.
        cout << "Wrong number of arguments." << endl;
        return -1;
    }

    ifstream ifile(argv[1]);

    if(!ifile.good()){ // checks if file exists
        cout << "Input file not found." << endl;
        return -1;
    }

    BitInputStream infile(ifile);

    ofstream outfile(argv[FILEINDEX]); // outFile is the output file from above.

    HCTree* tree = new HCTree();
    string header = "";
    char c;

    while(ifile.get(c)) {
        if(c == '\n') break;
        header += c;
    }

    // get header
    int numchars = tree->buildFromString(header);
    int numprinted = 0;

    // prints all the characters
    while(ifile.peek() != EOF){
        outfile << (char) tree->decode(infile);
        numprinted++;
    }

    // checks for any stragglers in the buffer
    while(numprinted < numchars){
        outfile << (char) tree->decode(infile);
        numprinted++;
    }

    outfile << endl;

    ifile.close();
    outfile.close();
    delete(tree);
    return 0;
}
