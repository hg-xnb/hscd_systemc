/// why SC_THREAD? https://stackoverflow.com/questions/18646936/will-sc-thread-in-systemc-create-a-real-thread

#include "dlock.h"
#include "string"

int sc_main(int argc, char* argv[]) {
	sc_signal<bool> clk, rst, b0, b1;
	sc_signal<bool> ul;
	DLOCK dlock0("dlock0");
	
	dlock0.CLK(clk);
	dlock0.RST(rst);
	dlock0.B0(b0);
	dlock0.B1(b1);
	dlock0.UNLOCK(ul);

    sc_trace_file* trace_file = sc_create_vcd_trace_file("dlock_waveform");
    trace_file->set_time_unit(1, SC_NS);
    sc_trace(trace_file, clk, "CLK");
    sc_trace(trace_file, rst, "RST");
    sc_trace(trace_file, b0, "B0");
    sc_trace(trace_file, b1, "B1");
    sc_trace(trace_file, ul, "UNLOCK");

	auto mono_pulse = [=](sc_signal<bool> &pulse){
		pulse.write(0);
		sc_start(5, SC_NS);
		pulse.write(1);
		sc_start(5, SC_NS);
		pulse.write(0);
	};

	cout << "Start sim...\n\n";

	/// make reset state
	b0.write(0);b1.write(0);
	rst.write(1); 
	mono_pulse(clk);
	rst.write(0);
	sc_start(5, SC_NS);
	/// Enter pin
    cout << "Enter binary PIN (8digits): ";
	std::string PIN; 
	cin >> PIN;

	for(auto const& B:PIN){
		if(B == '0')
			{b0.write(1); b1.write(0);}
		else
			{b0.write(0); b1.write(1);}
		mono_pulse(clk);
		
		/// result
		cout << "processing " << B <<   "\t -> Status: " << ((ul.read() == 1)?"UNLOCK\n":"LOCK\n");
		if(ul.read()==true){
			mono_pulse(clk);
			mono_pulse(clk);
			rst.write(1); 
			mono_pulse(clk);
			rst.write(0);
		}
	}

	mono_pulse(clk);
	mono_pulse(clk);

	cout << "Final status: " << ((ul.read() == 1)?"UNLOCK\n":"LOCK\n");
    cout << "\nFinished!\n";
    sc_close_vcd_trace_file(trace_file);
	return 0;
}




// #include <systemc.h>

// #define MAX_W 8
// #define KEY 0xB3

// SC_MODULE(DLOCK){
// 	/// inputs
// 	sc_in<bool>reset;
// 	sc_in<bool>clk;
// 	sc_in<bool>b0_in;
// 	sc_in<bool>b1_in;
// 	/// output
// 	sc_out<bool> out;
// 	/// local regs
// 	sc_uint<MAX_W> state;
	
// 	void __reset(){
// 		state = 0x0;
// 		out.write(0);
// 	}

// 	SC_CTOR(DLOCK){
// 		/// at reset state
// 		__reset();
// 		/// thread
// 		SC_THREAD(main_thread);
// 		/// add sensitive list
// 		sensitive << clk.pos();
// 	}
	
// 	void main_thread(void){
// 		sc_uint<1> b;
// 		sc_uint<1> expected_b;
// 		while(0x1){
// 			if(clk.read() == true){
// 				if(reset.read() == true) 
// 					__reset();
// 				else{
// 					if(b0_in.read() != b1_in.read()){ /// valid input ?
// 						b = (b0_in.read() == true) ? 0x0 : 0x1; /// enter 0-bit or 1-bit?
// 						expected_b = bool(KEY & 1<<(state)); /// expected bit?
// 						if( b != expected_b ) /// correct ?
// 							__reset(); /// nope --> reset
// 						else{
// 							if(state == 7 )/// correct! 
// 								out.write(1); /// --> Unlock 
// 							else
// 								state++; /// move to next state
// 						}
// 					}else{
// 						__reset();
// 					}
// 				}
// 			}else{
// 				out.write(out.read());
// 			}
// 			wait();
// 		}
// 	}
// };

// int sc_main(int argc, char* argv[]) {
//     sc_signal<bool> B0, B1, CLK, RST, UNLOCK;
//     DLOCK dlock0("dlock0");

// 	dlock0.clk(CLK);
// 	dlock0.reset(RST);
// 	dlock0.b0_in(B0);
// 	dlock0.b1_in(B1);
// 	dlock0.out(UNLOCK);

//     sc_trace_file* trace_file = sc_create_vcd_trace_file("dlock_waveform");
//     trace_file->set_time_unit(1, SC_NS);
//     sc_trace(trace_file, CLK, "CLK");
//     sc_trace(trace_file, RST, "RST");
//     sc_trace(trace_file, B0, "B0");
//     sc_trace(trace_file, B1, "B1");
//     sc_trace(trace_file, UNLOCK, "UNLOCK");

//     cout << "Start sim...\n";
    
//     for(uint16_t n = 0; n < 15; ++n){
//         B0.write(bool(n&0x1)); 
// 		B1.write(bool(n&0x2));
// 		RST.write(bool(n&0x10));
// 		CLK.write(bool(n&0x40));
//         sc_start(5, SC_NS);
//         cout    << "CLK: " << CLK.read() << " "
//                 << "RST: " << RST.read() << " "
//                 << "B0: " << B0.read() << " "
//                 << "B1: " << B1.read() << " "
//                 << "UL: " << UNLOCK.read() << '\n';
//     }

//     cout << "\nFinished!\n";
//     sc_close_vcd_trace_file(trace_file);
// 	return 0;
// }
