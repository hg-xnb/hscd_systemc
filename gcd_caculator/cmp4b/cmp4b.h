#ifndef __CMP4B_H__
#define __CMP4B_H__

#include <systemc.h>

#define WIDTH 4

SC_MODULE(CMP4B){
    sc_in<sc_uint<WIDTH> > X, Y;
    sc_out<bool> X_LT_Y, X_GT_Y, X_EQ_Y;

    void compare() {
        X_LT_Y.write(X.read() < Y.read());
        X_GT_Y.write(X.read() > Y.read());
        X_EQ_Y.write(X.read() == Y.read());
    }
    
    SC_CTOR(CMP4B) {
        SC_METHOD(compare);
        sensitive << X << Y;
    }

};

#endif