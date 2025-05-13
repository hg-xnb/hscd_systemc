#ifndef __SUB4B_H__
#define __SUB4B_H__

#include <systemc.h>
#define WIDTH 4 

SC_MODULE(SUB4B){
    sc_in<bool> CLK;
    sc_in<sc_uint<WIDTH>> X, Y;
    sc_out<sc_uint<WIDTH>> OUT;

    sc_uint<WIDTH> _reg;

    void process() {
        while (true) {
            auto x = X.read();
            auto y = Y.read();
            _reg = (x > y) ? (x - y) : (y - x);
            OUT.write(_reg);
            wait();
        }
    }

    SC_CTOR(SUB4B) {
        SC_CTHREAD(process, CLK.pos());
    }
};


#endif