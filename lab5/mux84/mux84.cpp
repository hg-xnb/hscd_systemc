#include <systemc.h>

#ifndef SC_TRACE
	#define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif
#ifndef rev
	#define rev(i, n) for(int i = n-1; i >= 0; i--)
#endif
#ifndef var_print
	#define var_print
	template<class T> void print(T what){cout << what;}
	template<class T, class... Args> void print(T what, Args... args){
		cout << what;
		print(args...);
	}
#endif
#ifndef _mono_pulse
	#define _mono_pulse
	void mono_pulse(sc_signal<bool> &pulse){
		pulse.write(0);
		sc_start(5, SC_NS);
		pulse.write(1);
		sc_start(5, SC_NS);
		pulse.write(0);
	};
#endif


#include "mux84.h"

int sc_main(int argc, char* argv[]) {
	sc_signal<sc_uint<4>> in0("in0"), in1("in1");
	sc_signal<bool> sel("sel");
	sc_signal<sc_uint<4>> out("out");
	
	mux84 mux("mux");
	mux.IN0(in0);
	mux.IN1(in1);
	mux.SEL(sel);
	mux.OUT(out);

	sc_trace_file* trace_file = 
	sc_create_vcd_trace_file("wavetrace_output");
	trace_file->set_time_unit(1, SC_NS);

	sc_set_trace(in0);
	sc_set_trace(in1);
	sc_set_trace(sel);
	sc_set_trace(out);

	cout << "Start sim...\n";

	for(uint8_t i = 0; i < 32; ++i){
		in0.write(i);
		in1.write(i*2);
		sel.write(i%4 > 1);
		sc_start(10, SC_NS);
		print(
			" $ IN0: ", in0.read(), 
			"\tIN1: ", in1.read(), 
			"\t\tSEL: ", sel.read(), 
			"\t --> OUT: ", out.read(), '\n');
	}
	
	cout << "\nFinished!\n";
    sc_close_vcd_trace_file(trace_file);
	return 0;
}
