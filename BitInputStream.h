#include <iostream>

/**
    Yash Nevatia & Josh Hunter
    BitInputStream
    takes input as bits
*/

class BitInputStream {

    private:

    char buf; // one byte buffer of bits

    int nbits; // how many bits have been read from buf
    std::istream & in;

    public:

    // the input stream to use
    /** Initialize a BitInputStream that will use
    * the given istream for input.
    */

    BitInputStream(std::istream & is) : in(is) {
        buf = 0; // clear buffer
        nbits = 8; // initialize bit index
    }

    /** Fill the buffer from the input */
    void fill() {
        buf = in.get();
        nbits = 0;
    }


    // reads a bit
    int readBit();


    // checks if the buffer is full
    int isBuffFull();

};
