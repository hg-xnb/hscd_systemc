#ifndef __FA_4BIT_H__
#define __FA_4BIT_H__

#include <systemc.h>

SC_MODULE(FA_4BIT) {
    // Ports
    sc_in<bool> EN;            // Enable signal (replaces CLK)
    sc_in<bool> RST;           // Reset input (active high)
    sc_in<sc_uint<4>> A;       // 4-bit data input A
    sc_in<sc_uint<4>> B;       // 4-bit data input B
    sc_in<bool> CARRY_IN;      // Carry input
    sc_out<sc_uint<4>> OUT;    // 4-bit result output
    sc_out<bool> CARRY_OUT;    // Carry output

    SC_CTOR(FA_4BIT) {
        SC_METHOD(process);
        sensitive << EN << RST << A << B << CARRY_IN;
    }

    void process() {
            sc_uint<4> a = A.read();
            sc_uint<4> b = B.read();
            bool carry_in = CARRY_IN.read();

            sc_uint<5> result = a + b + (carry_in ? 1 : 0);
            OUT.write(result.range(3, 0));
            CARRY_OUT.write(result[4]);
    }
};

#endif
