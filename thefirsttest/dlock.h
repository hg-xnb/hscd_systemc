#include <systemc.h>

#define MAX_W 8

SC_MODULE (DLOCK){
	sc_in<bool> CLK, RST, B0, B1; 
	sc_out<bool> UNLOCK;

	sc_uint<1> b, expected_b;
    sc_uint<MAX_W> state;
	static const sc_uint<MAX_W> KEY;

	SC_CTOR(DLOCK):
	CLK("CLK"), RST("RST"), 
	B0("B0"), B1("B1"), UNLOCK("UNLOCK")
	{
		SC_THREAD(main_thread);
		sensitive << CLK.pos();
	}

	void main_thread(void){
		UNLOCK.write(false); 
		state = 0x0;
		while(true){
			if(CLK.read() == true){
				if(RST.read() == true){
					/// reset the lock
					UNLOCK.write(false);
					state = 0x0;
				}else{
                    /// check state
                    if(state > 0x7) {
                        /// reset if press more :))
                        state = 0x0;
                        UNLOCK.write(false);
                    }
                    /// check input?
                    if( B0.read() != B1.read() ){
                        b = (B0.read() == true) ? 0x0 : 0x1;
                        expected_b = bool(KEY & (1<<state));
                        if( b != expected_b ){
                            /// reset
                            UNLOCK.write(false);
                            state = 0x0;
                        }else{
                            if(state == 7){
                                /// unlock :v
                                UNLOCK.write(true);
                                state = 0x8;
                            }else{
                                /// move to next state
                                state = (state < 0x7)?(state+0x1):0x0;
                            }
                        }
                    }else{
                        /// do nothin'
                    }
				}
			}else{
				/// do nothin'
			}
            wait_for_next_event:
			wait();
		}
	}

};

const sc_uint<MAX_W> DLOCK::KEY = 0xAB; /// 10101011 -> 11010101