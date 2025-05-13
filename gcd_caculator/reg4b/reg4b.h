#ifndef __REG4B_H__
#define __REG4B_H__

#include <systemc.h>

#define WIDTH 4

SC_MODULE(REG4B){
    sc_in<sc_uint<WIDTH>> IN;
    sc_in<bool> CLK;
    sc_in<bool> RESET;
    sc_in<bool> LOAD;
    sc_out<sc_uint<WIDTH>> OUT;

    sc_uint<WIDTH> _reg;


    void main_thread(){
        while(true){
            if(RESET.read()){
                OUT.write(0);
            } else 
                if(LOAD.read()){
                    _reg = IN.read();
                }
            OUT.write(_reg);
            wait();
        }
    }

    SC_CTOR(REG4B):
        IN("IN"),
        CLK("CLK"),
        RESET("RESET"),
        LOAD("LOAD"),
        OUT("OUT")
    {
        SC_CTHREAD(main_thread, CLK.pos());
    }

};

#endif