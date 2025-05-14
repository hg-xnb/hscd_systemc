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

SC_MODULE(NN_DIGITMULT) {
    sc_in<bool> EN;                         // Enable signal
    sc_in<sc_uint<WIDTH>> A, B;             // 32-bit inputs

    sc_out<bool> DONE;                      // Done signal
    sc_vector<sc_out<sc_uint<WIDTH>>> OUT;  // Two 32-bit outputs

    void compute_comb_logic() {
        if (EN.read()) {
            sc_uint<16> alow = _1ST_16BITS(A.read());
            sc_uint<16> ahigh = _2ND_16BITS(A.read());
            sc_uint<16> blow = _1ST_16BITS(B.read());
            sc_uint<16> bhigh = _2ND_16BITS(B.read());

            sc_uint<32> low = (sc_uint<32>)alow * blow;
            sc_uint<32> mixed = ((sc_uint<32>)alow * bhigh) + ((sc_uint<32>)ahigh * blow);
            sc_uint<32> high = (sc_uint<32>)ahigh * bhigh;

            low += (mixed << 16);
            high += (mixed >> 16);

            OUT[0].write(low);
            OUT[1].write(high);
            DONE.write(1);
        } else {
            OUT[0].write(0);
            OUT[1].write(0);
            DONE.write(0);
        }
    }

    SC_CTOR(NN_DIGITMULT) :
        EN("EN"), A("A"), B("B"), OUT("OUT", 2), DONE("DONE")
    {
        SC_METHOD(compute_comb_logic);
        sensitive << A << B << EN;
        dont_initialize();
    }
};

#endif
