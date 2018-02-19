#include <iostream>
#include "BitInputStream.h"
#define BYTESIZE 8

using namespace std;

/**
    Yash Nevatia & Josh Hunter
    BitInputStream
    takes input as bits
*/

int BitInputStream::readBit() {
    int r;

    // If all bits in the buffer are read, fill the buffer first
    if(isBuffFull()) fill();

    // Get the bit at the appriopriate location in the bit
    // buffer, and return the appropriate int
    r = (buf >> nbits) & 1;

    // Increment the index
    nbits++;

    return r;
}

int BitInputStream::isBuffFull() {
    return nbits == 8;
}
