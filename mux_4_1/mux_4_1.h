#ifndef __MUX_4_1__
#define __MUX_4_1__

#include <systemc.h>


#ifndef LOW
	#define LOW 	0x0
#endif
#ifndef HIGH
	#define HIGH 	0x1
#endif

SC_MODULE(MUX_4_1){
    sc_in<bool> EN; /// active-high
    sc_in<sc_uint<2>> SEL;
    sc_vector<sc_in<bool>> IN; 
    sc_out<bool> OUT;

	void mux_action(){
		if(EN.read() == HIGH){
			switch (SEL.read()){
				case 0x0:
					OUT.write(IN[0x0].read()); break;
				case 0x1:
					OUT.write(IN[0x1].read()); break;
				case 0x2:
					OUT.write(IN[0x2].read()); break;
				case 0x3:
					OUT.write(IN[0x3].read()); break;
			}
		}else{
			OUT.write(LOW);
		}
	}
    SC_CTOR(MUX_4_1)
        : IN("IN", 4), EN("EN"), SEL("SEL"), OUT("OUT")
    {
        SC_METHOD(mux_action);
        sensitive << EN << SEL;
        for (int i = 0; i < 4; ++i) sensitive << IN[i];
    }

};

#endif