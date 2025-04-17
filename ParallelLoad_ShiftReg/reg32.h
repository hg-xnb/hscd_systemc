#ifndef __REG32_H__
#define __REG32_H__

#include <bits/stdc++.h>
using namespace std;

#include <systemc.h>

#ifndef rep
	#define rep(i, n) for(int i = 0; i < n; i++)
#endif
#ifndef rev
	#define rev(i, n) for(int i = n-1; i >= 0; i--)
#endif

#define BIT_WIDTH   8
#define ROWS        4
#define MODE_BIT    2

enum enum_modes {
    SHIFT_RIGHT = 0b00,
    SHIFT_LEFT,
    LOAD_INPUT,     
    HOLD_VALUE
};

SC_MODULE(REG32){
    sc_in<bool> clk;
    sc_in<sc_uint<MODE_BIT>> SH;
    sc_in<sc_uint<BIT_WIDTH>> input;
    sc_out<sc_uint<BIT_WIDTH>> output;
    
    sc_uint<BIT_WIDTH> data[ROWS];
    sc_uint<3> clock_count;

    void pop_last_row(){
        for(uint8_t i = ROWS-1; i > 0; i--){
            data[i] = data[i-1];
        }
    }

    void main_thread(){
        auto &first_row = data[0];
        auto &last_row = data[ROWS-1];
        while(true){
            wait();
            if(clk.read() == 1){
                switch (SH.read()){
                    case HOLD_VALUE:
                        continue;
                    case SHIFT_RIGHT:
                        last_row >>= 1;
                        break;
                    case SHIFT_LEFT:
                        last_row <<= 1;
                        break;
                    case LOAD_INPUT:
                        pop_last_row();
                        first_row = input.read();
                        break;
                }
                output.write(last_row);
            }
        }
    }
    
    SC_CTOR(REG32):
    clk("clk"), SH("SH"), 
    input("input"), output("output")
    {
        SC_THREAD(main_thread);
        sensitive << clk.pos();
    }
};


#endif