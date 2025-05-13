#ifndef __OR_4BIT_H__
#define __OR_4BIT_H__

#include <systemc.h>

SC_MODULE(OR_4BIT) {
    sc_in<bool> RST;              // Reset input
    sc_in<bool> EN;               // Enable input (replaces CLK)
    sc_in<sc_uint<4>> A;          // Input A
    sc_in<sc_uint<4>> B;          // Input B
    sc_out<sc_uint<4>> OUT;       // Output (A | B)

    SC_CTOR(OR_4BIT) {
        SC_METHOD(process);
        sensitive << RST << EN << A << B;
        // EN is used to trigger logic evaluation
    }

    void process() {
        if (RST.read()) {
            OUT.write(0); // Reset output
        } else if (EN.read()) {
            OUT.write(A.read() | B.read()); // Bitwise OR
        }
        // Otherwise, OUT holds its previous value
    }
};

#endif
