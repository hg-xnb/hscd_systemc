#ifndef __NN_DIGITMULT_H__
#define __NN_DIGITMULT_H__

#include <systemc.h>

#ifndef WIDTH
#define WIDTH 32
#endif

#ifndef _1ST_16BITS
#define _1ST_16BITS(x) ((x) & 0xFFFF)
#endif
#ifndef _2ND_16BITS
#define _2ND_16BITS(x) (((x) >> 16) & 0xFFFF)
#endif

enum fsm_state { WAIT = 0, EXECUTE, OUTPUT, FINISH };

SC_MODULE(NN_DIGITMULT) {
    sc_in<bool> CLK;          // Clock input
    sc_in<bool> RST;        // Reset input
    sc_in<bool> EN;           // Enable signal
    sc_in<sc_uint<WIDTH>> A, B;

    sc_out<bool> DONE;
    sc_vector<sc_out<sc_uint<WIDTH>>> OUT;
    // State as a register
    fsm_state state;
    // Registers for intermediate values
    sc_uint<WIDTH> alow, ahigh, bhigh, blow, mixed_lh;
    sc_uint<WIDTH> res_low;  // Register for low result
    sc_uint<WIDTH> res_high; // Register for high result

    void split_bits() {
        alow = _1ST_16BITS(A.read());
        ahigh = _2ND_16BITS(A.read());
        blow = _1ST_16BITS(B.read());
        bhigh = _2ND_16BITS(B.read());
    }

    void compute_result() {
        // Compute cross terms
        mixed_lh  = (ahigh * blow) + (alow * bhigh);  
        // Lower 32 bits
        res_low = alow * blow + (_1ST_16BITS(mixed_lh)<<16);
        // Upper 32 bits    
        res_high = ahigh * bhigh + _2ND_16BITS(mixed_lh);
    }

    void processing() {
        // Reset behavior
        state = WAIT;
        DONE.write(0);
        res_low = 0;
        res_high = 0;

        wait();  // Wait for first clock edge

        while (true) {
            switch (state) {
                case WAIT:
                    if (EN.read()) {
                        split_bits();
                        compute_result();
                        state = EXECUTE;
                    }
                    DONE.write(0);
                    break;
                case EXECUTE:
                    state = OUTPUT;
                    break;
                case OUTPUT:
                    OUT[0].write(res_low);
                    OUT[1].write(res_high);
                    DONE.write(1);
                    state = FINISH;
                    break;
                case FINISH:
                    if (EN.read()) {
                        DONE.write(1);
                    } else {
                        DONE.write(0);
                        state = WAIT;
                    }
                    break;
            }
            wait();
        }
    }

    SC_CTOR(NN_DIGITMULT) :
        CLK("CLK"), RST("RST"), EN("EN"), A("A"), B("B"),
        OUT("OUT", 2), DONE("DONE")
    {
        SC_CTHREAD(processing, CLK.pos());
        reset_signal_is(RST, true);  // Active-high RST
    }
};

#endif