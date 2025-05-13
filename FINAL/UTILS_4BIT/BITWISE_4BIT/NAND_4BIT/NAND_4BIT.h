#ifndef __NAND_4BIT_H__
#define __NAND_4BIT_H__

#include <systemc.h>

SC_MODULE(NAND_4BIT) {
    // Ports
    sc_in<bool> EN;              // Enable input (replaces CLK)
    sc_in<bool> RST;             // Reset input
    sc_in<sc_uint<4>> A;         // 4-bit input A
    sc_in<sc_uint<4>> B;         // 4-bit input B
    sc_out<sc_uint<4>> OUT;      // 4-bit output (A NAND B)

    // Constructor
    SC_CTOR(NAND_4BIT) {
        SC_METHOD(process);
        sensitive << RST << EN << A << B;
        dont_initialize();  // optional: don’t run process() at time 0
    }

    // Process method
    void process() {
        if (RST.read()) {
            OUT.write(0);  // Clear output on reset
        } else if (EN.read()) {
            OUT.write(~(A.read() & B.read())); // Bitwise NAND
        }
        // Else: retain previous OUT value
    }
};

#endif
