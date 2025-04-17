#include <systemc.h>
#include "dec_2_1.h"

#ifndef SC_TRACE
	#define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif

int sc_main(int argc, char* argv[]) {
	sc_signal<bool>			en("EN");
	sc_signal<sc_uint<2>>	in("IN");
	sc_signal<sc_uint<4>>	out("OUT");

	DEC_2_1 dec21("dec21");
	dec21.EN(en);
	dec21.IN(in);
	dec21.OUT(out);

	sc_trace_file* trace_file = 
	sc_create_vcd_trace_file("dec_2_4_waveform");
	trace_file->set_time_unit(1, SC_PS);

	sc_set_trace(en);
	sc_set_trace(in);
	sc_set_trace(out);

	cout << "---- Start sim... ----\n";

	en.write(0); cout << "en = 0" << '\n';
		for( uint32_t n = 0; n < 4; n++){
			in.write(n);
			cout << ">> in = " << n << '\t';
			sc_start(5, SC_NS);
			cout << "out = " << out.read() << '\n';
		}
		cout << '\n';
	en.write(1); cout << "en = 1" << '\n';
		for( uint32_t n = 0; n < 4; n++){
			in.write(n);
			cout << ">> in = " << n << '\t';
			sc_start(5, SC_NS);
			cout << "out = " << out.read() << '\n';
		}
		cout << '\n';
	
    cout << "\n---- Finished! ----\n";
    sc_close_vcd_trace_file(trace_file);
	return 0;
}
