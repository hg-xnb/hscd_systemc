#ifndef __AND_4BIT_H__
#define __AND_4BIT_H__

#include <systemc.h>

SC_MODULE(AND_4BIT) {
    // Ports
    sc_in<bool> EN;              // Enable signal (replaces CLK)
    sc_in<bool> RST;             // Reset input
    sc_in<sc_uint<4>> A;         // 4-bit input A
    sc_in<sc_uint<4>> B;         // 4-bit input B
    sc_out<sc_uint<4>> OUT;      // 4-bit output (A AND B)

    // Constructor
    SC_CTOR(AND_4BIT) {
        SC_METHOD(process);
        sensitive << EN << RST << A << B;
        dont_initialize(); // Prevent auto-execution at time 0
    }

    // Process method
    void process() {
        if (RST.read()) {
            OUT.write(0);
        } else if (EN.read()) {
            OUT.write(A.read() & B.read());
        }
        // Else: retain previous output
    }
};

#endif
