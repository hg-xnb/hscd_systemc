#include <systemc.h>
#include "mux_4_1.h"


#ifndef SC_TRACE
	#define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif

int sc_main(int argc, char* argv[]) {
	sc_vector<sc_signal<bool>> 	in("IN", 4);
	sc_signal<sc_uint<2>>	sel("SEL");
	sc_signal<bool>			en("EN");
	sc_signal<bool>			out("OUT");

	MUX_4_1 mux41("mux41");
	mux41.EN(en);
	mux41.SEL(sel);
	mux41.IN(in);
	mux41.OUT(out);

	sc_trace_file* trace_file = 
	sc_create_vcd_trace_file("mux_4_1_waveform");
	trace_file->set_time_unit(1, SC_NS);

	sc_set_trace(in[3]);
	sc_set_trace(in[2]);
	sc_set_trace(in[1]);
	sc_set_trace(in[0]);
	sc_set_trace(sel);
	sc_set_trace(en);
	sc_set_trace(out);

	auto mono_pulse = [=](sc_signal<bool> &pulse){
		pulse.write(0);
		sc_start(5, SC_NS);
		pulse.write(1);
		sc_start(5, SC_NS);
		pulse.write(0);
	};

	auto make_in_cycle = [=] (
		sc_vector<sc_signal<bool>> &in, 
		sc_signal<bool> &out
	){
		for(uint8_t n = 0; n < 16; ++n){
			in[0].write(bool(n & 0x1));	in[1].write(bool(n & 0x2)); 
			in[2].write(bool(n & 0x4)); in[3].write(bool(n & 0x8));

			cout << ">> in[4:0] = " 
				<< bool(n & 0x8) 
				<< bool(n & 0x4) 
				<< bool(n & 0x2) 
				<< bool(n & 0x1) << '\t';
			cout << ">> out = " << out << '\n';
			sc_start(5, SC_NS);
		}
		cout << '\n';
	};

	cout << "Start sim...\n";

	en.write(0); cout << "en = 0" << '\n';
		sel.write(0); cout << "sel = 0\n";
		make_in_cycle(in, out); 
		sel.write(1); cout << "sel = 1\n";
		make_in_cycle(in, out); 
		sel.write(2); cout << "sel = 2\n";
		make_in_cycle(in, out); 
		sel.write(3); cout << "sel = 3\n";
		make_in_cycle(in, out); 

	en.write(1); cout << "en = 1" << '\n';
		sel.write(0); cout << "sel = 0\n";
		make_in_cycle(in, out); 
		sel.write(1); cout << "sel = 1\n";
		make_in_cycle(in, out); 
		sel.write(2); cout << "sel = 2\n";
		make_in_cycle(in, out); 
		sel.write(3); cout << "sel = 3\n";
		make_in_cycle(in, out); 

    cout << "\nFinished!\n";
    sc_close_vcd_trace_file(trace_file);
	return 0;
}
