#ifndef XOR_4BIT_H
#define XOR_4BIT_H

#include <systemc.h>

SC_MODULE(XOR_4BIT) {
    // Ports
    sc_in<bool> RST;          // Reset input
    sc_in<bool> EN;           // Enable input (replaces CLK)
    sc_in<sc_uint<4>> A;      // 4-bit data input A
    sc_in<sc_uint<4>> B;      // 4-bit data input B
    sc_out<sc_uint<4>> OUT;   // 4-bit data output (A XOR B)

    // Constructor
    SC_CTOR(XOR_4BIT) {
        SC_METHOD(process);
        sensitive << EN << RST << A << B;
        // No need for async_reset_signal_is with SC_METHOD and no CLK
    }

    // Process method
    void process() {
        if (RST.read()) {
            OUT.write(0); // Reset output to 0
        } else if (EN.read()) {
            OUT.write(A.read() ^ B.read()); // Bitwise XOR when enabled
        }
        // If EN is low and RST is low, OUT remains unchanged
    }
};

#endif
