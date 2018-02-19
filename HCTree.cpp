#include <stdio.h>
#include <string.h>
#include <iostream>     // std::cout
#include <sstream>
#include <algorithm>    // std::sort
#include <vector>  
#include <set>
#include "HCTree.h"


/**
	Yash Nevatia & Josh Hunter
	HCTree
	the functions to build, encode and decode a huffman coding tree
*/

void HCTree::build(const std::vector<int>& freqs) {

	std::priority_queue<HCNode*, std::vector<HCNode*>, std::greater<HCNode*> > queue;

	for(int i = 0; i < FORESTSIZE; i++){
		leaves[i] = new HCNode(freqs[i], (char) (i + 1), 0, 0, 0);
		if((leaves[i])->count != 0) // adds leaves to queue
			queue.push((leaves[i]));
	}
	
	while(queue.size() != 1){ // runs through queue

		// found and removed two least nodes
		HCNode* T1 = queue.top();
		queue.pop();
		HCNode* T2 = queue.top();
		queue.pop();

		HCNode * newroot = new HCNode(T1->count + T2->count, 0, T2, T1); // create parent

		// add as parents
		T1->p = newroot;
		T2->p = newroot;
		
		// add parent
		queue.push(newroot);
    }

    root = queue.top();
    queue.pop();
}

string HCTree::makeHeader(const std::vector<int>& freqs, int numchars) {

	std::set<std::pair<int, int>> freqAscii;

	for(int i = 0; i < FORESTSIZE; i++) // notes and sorts ascii pairs
		freqAscii.insert(make_pair(freqs[i], i + 1));

	string bitstr = headerHelp(root, "") + "?";

	set<pair<int,int>>::reverse_iterator itr;

	// loops through freqAscii for HIGH to LOW, adding to bitstr
	for(itr = freqAscii.rbegin(); itr != freqAscii.rend(); itr++){
		if((*itr).first <= 0) break;
		bitstr += (to_string((*itr).second) + " ");
	}

	// also sends number of chars in original file
	bitstr += ("? " + to_string(numchars));

	return bitstr;
}

string HCTree::headerHelp(HCNode* &curr, string bitstr) {
	
	if(curr->c0){ // left child
		
		if((curr->c0)->symbol != 0) // leaf
			bitstr += "1";
		else {
			bitstr += "0"; // parent
			bitstr = headerHelp(curr->c0, bitstr);	
		}

	} if(curr->c1){ // right child

		if((curr->c1)->symbol != 0) // leaf
			bitstr += "1";
		else {
			bitstr += "0"; // parent
			bitstr = headerHelp(curr->c1, bitstr);
		}
	}

	return bitstr;
}

pair<queue<int>, int> HCTree::getAsciis(string header) {

	std::queue<int> asciis; // makes a queue of ascii vals

    string asciiString = header.substr(header.find("?") + 1);
    string chars = asciiString.substr(asciiString.find("?") + 1);

    int numChars = stoi(chars); // saves the number of chars in original

    std::istringstream iss(asciiString);
    string asciiValue;

    while (std::getline(iss, asciiValue, ' ')) {
        if(asciiValue == "?") break; // adds all asciis to queue
        asciis.push(stoi(asciiValue));
    }

    return make_pair(asciis, numChars);
}

int HCTree::buildFromString(string header) {

	pair<queue<int>, int> p = getAsciis(header);

    char * bit = &(header[0]);

    root = new HCNode(0,0,0,0,0);

    buildHelper(root, bit, p.first);

    return p.second;
}


void HCTree::buildHelper(HCNode * curr, char* &bit, std::queue<int> &asciis){

    if(*bit == '?')
        return;

    int symbol;

    // siblings are handled with a bit each,
    // left and right: xx, start with left:

    // left child
	if(*bit - '0') { // case of "1x"
        symbol = asciis.front();
        asciis.pop(); // this is a left leaf
        curr->c0 = new HCNode(0, symbol, 0, 0, curr);
    } else { // case for "0x"
        curr->c0 = new HCNode(0,0,0,0, curr);
        bit++; // this is a left parent
        buildHelper(curr->c0, bit, asciis);
    }

    ++bit;

    // right child
	if(*bit - '0'){ // case of "x1"
        symbol = asciis.front();
        asciis.pop(); // this is a right leaf
        curr->c1 = new HCNode(0, symbol, 0, 0, curr);
    } else { // case for "x0"
        curr->c1 = new HCNode(0,0,0,0, curr);
        bit++; // this is a right parent
        buildHelper(curr->c1, bit, asciis);
    }
}

void HCTree::encode(byte symbol, ofstream& out) const {
	string bitstr = "";
	HCNode * current = leaves[(int) symbol - 1];
	
	while(current != root){ // travels from leaf to root
		HCNode * parent = current->p;

		// notes the bit representation as it goes
		if(parent->c0 == current)
			bitstr.append("0");
		else
			bitstr.append("1");

		current = parent;
	}

	// flips the string for proper encoding
	reverse(bitstr.begin(), bitstr.end());

	out << bitstr;
}

void HCTree::encode(byte symbol, BitOutputStream& out) const {
	string bitstr = "";
	HCNode* current = leaves[(int) symbol - 1];
	
	while(current != root){ // travels from leaf to root
		HCNode* parent = current->p;

		// notes the bit representation as it goes
		if(parent->c0 == current)
			bitstr.append("0");
		else
			bitstr.append("1");

		current = parent;
	}

	// flips the string for proper encoding
	reverse(bitstr.begin(), bitstr.end());

	for(int i = 0; i < bitstr.length(); i++) // writes strings as bits
		bitstr[i] == '0' ? out.writeBit(0x0) : out.writeBit(0x1);
}

int HCTree::decode(ifstream& in) const {

	int bit;
	char c;
	HCNode * current = root;

	// travels from root to leaf by reading encoding
	while(current->symbol == 0){
		in.get(c);
		bit = c - '0';
		
		if(bit) current = current->c1;
		else current = current->c0;
	}

	// returns appropriate symbol
	return (int) current->symbol;
}

int HCTree::decode(BitInputStream& in) const {

	HCNode * current = root;

	// travels from root to leaf by reading encoding
	while(current->symbol == 0){
		if(in.readBit())
			current = current->c1;
		else
			current = current->c0;
	}
	
	// returns appropriate symbol
	return (int) current->symbol;
}

HCTree::~HCTree() {
	deleteAll(root);
    for(HCNode* leaf : leaves)
        delete(leaf);
}

void HCTree::deleteAll(HCNode* n){
	if (n->c0)
        deleteAll(n->c0);
    if (n->c1)
        deleteAll(n->c1);
    delete(n);
}