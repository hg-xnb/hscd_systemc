#ifndef __FS_4BIT_H__
#define __FS_4BIT_H__

#include <systemc.h>

SC_MODULE(FS_4BIT)
{
    // Ports
    sc_in<bool> EN;          // Enable signal (acts like CLK in this design)
    sc_in<bool> RST;         // Reset input
    sc_in<sc_uint<4>> A;     // 4-bit data input A
    sc_in<sc_uint<4>> B;     // 4-bit data input B
    sc_in<bool> BORROW_IN;   // Borrow input for LSB
    sc_out<sc_uint<4>> OUT;  // 4-bit data output (A - B - BORROW_IN)
    sc_out<bool> BORROW_OUT; // Borrow output from MSB

    void subtract_thread()
    {
        if (EN.read())
        {
            sc_uint<4> a = A.read();
            sc_uint<4> b = B.read();
            bool borrow_in = BORROW_IN.read();

            sc_uint<5> result = a - b - (borrow_in ? 1 : 0);
            OUT.write(result.range(3, 0));
            BORROW_OUT.write(a < (b + (borrow_in ? 1 : 0)));
        }
    }

    SC_CTOR(FS_4BIT)
    {
        SC_METHOD(subtract_thread);
        sensitive << EN << RST << A << B << BORROW_IN;
    }
};

#endif
