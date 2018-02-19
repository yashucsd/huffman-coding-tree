#include <iostream>
#include "BitOutputStream.h"

using namespace std;

/**
    Yash Nevatia & Josh Hunter
    BitOutputStream
    sends out bits to an outstream
*/

void BitOutputStream::writeBit(int i) {
    if(nbits == 8) flush();
    
    // write least signficant i bit to buffer;
    // buf = (buf << 1) | (i & 1);
    buf = buf & ~(1 << nbits);
    buf = buf | ((i & 1) << nbits);

    nbits++;
}