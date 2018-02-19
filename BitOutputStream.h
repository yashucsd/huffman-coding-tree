#include <iostream>

/**
    Yash Nevatia & Josh Hunter
    BitOutputStream
    sends out bits to an outstream
*/

class BitOutputStream {

    private:

    char buf; // one byte buffer of bits
    int nbits; // how many bits have been written to buf

    std::ostream & out; // reference to the output stream to use

    public:

    /** Initialize a BitOutputStream that will use
    * the given ostream for output */
    BitOutputStream(std::ostream & os) : out(os), buf(0), nbits(0) {}

     // writes bit to buffer
    void writeBit(int i);

    /** Send the buffer to the output, and clear it */
    void flush() {
        out.put(buf);
        out.flush();
        buf = nbits = 0;
    }
};

