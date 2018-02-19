#ifndef HCTREE_H
#define HCTREE_H

#include <queue>
#include <vector>
#include <fstream>
#include "HCNode.h"
#include "BitInputStream.h"
#include "BitOutputStream.h"

#define FORESTSIZE 256

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */

class HCNodePtrComp {
public:
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        return *lhs < *rhs;
    }
};

/**
    Yash Nevatia & Josh Hunter
    HCTree
    the functions to build, encode and decode a huffman coding tree
    A Huffman Code Tree class.
    Not very generic:  Use only if alphabet consists
    of unsigned chars.
 */

class HCTree {
private:

    HCNode* root;
    vector<HCNode*> leaves;
public:
    
    // explicit keyword is used to avoid accidental implicit conversions
    explicit HCTree() : root(0) {
        leaves = vector<HCNode*>(256, (HCNode*) 0);
    }

    ~HCTree();

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void build(const vector<int>& freqs);

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /** Write to the given ofstream
     *  the sequence of bits (as ASCII) coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
     *  BE USED IN THE FINAL SUBMISSION.
     */
    void encode(byte symbol, ofstream& out) const;


    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int decode(BitInputStream& in) const;

    /** Return the symbol coded in the next sequence of bits (represented as 
     *  ASCII text) from the ifstream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
     *  IN THE FINAL SUBMISSION.
     */
    int decode(ifstream& in) const;

    /**
    *   recursively delete node and and all of its children
    *   called by destructor
    */
    void deleteAll(HCNode* n);

    /**
        takes a the bitstring of the improved header and makes a queue
        of the ascii values it saves; in addition, it returns the number
        of character in the original file
    */
    pair<queue<int>, int> getAsciis(string bitstr);

    /**
        builds a tree from the improved header string
    */
    int buildFromString(string bitstr);

    /**
        recurisviely applies the efficient header algo to recreate the tree
    */
    string headerHelp(HCNode* &curr, string bitstr);

    /**
        produces the efficient header with use of frequencies
    */
    string makeHeader(const std::vector<int>& freqs, int numchars);

    /**
        helps build tree from bits and ascii queue
    */
    void buildHelper(HCNode * curr, char* &bit, std::queue<int> &asciis);
};

#endif // HCTREE_H
