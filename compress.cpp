#include <iostream>
#include "HCTree.h"

using namespace std;

/**
	Yash Nevatia & Josh Hunter
	compress
	takes in a input file and an output file
	encodes the contents of the input file with Huffman Coding
	prints encoded file and a heading to the output file
*/

#define NUMARGS 3
#define FILEINDEX 2

int main(int argc, char** argv) {

	if(argc != NUMARGS) { // wrong number of arguments
        cout << "Wrong number of arguments." << endl; 
        return -1;
    }

	int numchars = 0;
	ifstream infile;
    infile.open(argv[1]);

    if(!infile.good()) { // checks if file exists
         cout << "File not found." << endl;
         return -1;
    }

	vector<int> freqs = vector<int>(256);
	HCTree* tree = new HCTree(); // creates new tree and frequency vector

	char c;
	while(infile.get(c)){ // fills frequency vector for each byte
		freqs[(int) c - 1]++;
		numchars++;
	}
    
	infile.close();

	tree->build(freqs); // builds the tree with the frequencies

	ofstream outfile;
	outfile.open(argv[FILEINDEX]);
	BitOutputStream bos(outfile);

	// adds efficient header to outfile
	outfile << tree->makeHeader(freqs, numchars) << endl;

	infile.open(argv[1]);

	while(infile.get(c)) // encodes each byte and adds it to out
        tree->encode(c, bos);

   	bos.flush(); // in case there are remaining bits in buffer

	infile.close();
	outfile.close();
    delete tree;
    return 0;
}
