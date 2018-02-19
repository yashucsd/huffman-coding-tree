#include "HCNode.h"


using namespace std;

/**
    Yash Nevatia & Josh Hunter
    A class, instances of which are nodes in an HCTree.
*/

/** Less-than comparison, so HCNodes will work in std::priority_queue
 *  We want small counts to have high priority.
 *  And we want to break ties deterministically.
 */
bool operator<( const HCNode& curr, const HCNode& other){
    if(curr.count != other.count) return curr.count > other.count;

    // counts are equal. use symbol value to break tie.
    // (for this to work, internal HCNodes must have symb set.)
    return curr.symbol < other.symbol;
}

