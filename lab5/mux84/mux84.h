#ifndef __MUX84_H__
#define __MUX84_H__

#include <systemc.h>

SC_MODULE(MUX84) {
    sc_in<sc_uint<4>> IN0, IN1;    
    sc_in<bool> SEL;
    sc_out<sc_uint<4>> OUT;

    void main_process() {
        if (SEL.read() == 0) {
            OUT.write(IN0.read());
        } else {
            OUT.write(IN1.read());
        }
    }

    SC_CTOR(MUX84):
        IN0("IN0"), IN1("IN1"), SEL("SEL"), OUT("OUT") {
        SC_METHOD(main_process);
        sensitive << IN0 << IN1 << SEL;
        dont_initialize();
    }
};

#endif