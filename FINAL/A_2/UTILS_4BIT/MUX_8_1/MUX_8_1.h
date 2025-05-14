#ifndef __MUX_8_1__
#define __MUX_8_1__

#include <systemc.h>

#ifndef LOW
    #define LOW     0x0
#endif
#ifndef HIGH
    #define HIGH    0x1
#endif

SC_MODULE(MUX_8_1) {
    sc_in<bool> EN;                    // Enable signal
    sc_in<sc_uint<3>> SEL;             // 3-bit select input
    sc_vector<sc_in<sc_uint<4>>> IN;   // 8 4-bit input vector
    sc_out<sc_uint<4>> OUT;           // 4-bit output

    void mux_action() {
        if (EN.read() == true) {
            switch (SEL.read()) {
                case 0x0: OUT.write(IN[0x0].read()); break;
                case 0x1: OUT.write(IN[0x1].read()); break;
                case 0x2: OUT.write(IN[0x2].read()); break;
                case 0x3: OUT.write(IN[0x3].read()); break;
                case 0x4: OUT.write(IN[0x4].read()); break;
                case 0x5: OUT.write(IN[0x5].read()); break;
                case 0x6: OUT.write(IN[0x6].read()); break;
                case 0x7: OUT.write(IN[0x7].read()); break;
                default:  OUT.write(LOW); break;
            }
        } else {
            OUT.write(LOW); // If not enabled, default output
        }
    }

    SC_CTOR(MUX_8_1)
        : IN("IN", 8)
    {
        SC_METHOD(mux_action);
        sensitive << EN << SEL << IN;
        for (int i = 0; i < 8; ++i)
            sensitive << IN[i];
    }
};

#endif
