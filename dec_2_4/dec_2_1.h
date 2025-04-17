#ifndef __DEC_2_1_H__
#define __DEC_2_1_H__

#include <systemc.h>

#ifndef LOW
	#define LOW 	0x0
#endif
#ifndef HIGH
	#define HIGH 	0x1
#endif

SC_MODULE(DEC_2_1){
    sc_in<bool> EN;
    sc_in<sc_uint<2>> IN;
    sc_out<sc_uint<4>> OUT;

    void decoder_action(){
        if(EN.read() == HIGH){
            switch (IN.read()){
                case 0x0: OUT.write(0x1); break;
                case 0x1: OUT.write(0x2); break;
                case 0x2: OUT.write(0x4); break;
                case 0x3: OUT.write(0x8); break;
            }
        }else{
            OUT.write(0x0);
        }
    }

    SC_CTOR(DEC_2_1)
        : EN("EN"), IN("IN"), OUT("OUT")
    {
        SC_METHOD(decoder_action);
        sensitive << EN << IN;
    }

};

#endif