#include <systemc.h>
#include "updown_counter.h"

#ifndef SC_TRACE
	#define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif
#ifndef rep
	#define rep(i, n) for(int i = 0; i < n; i++)
#endif
#ifndef rev
	#define rev(i, n) for(int i = n-1; i >= 0; i--)
#endif
#ifndef var_print /// variadic print
	#define var_print
	template<class T> void print(T what){cout << what;}
	template<class T, class... Args> void print(T what, Args... args){
		cout << what;
		print(args...);
	}
#endif

int sc_main(int argc, char* argv[]) {
	sc_signal<bool> clk("clk"), pe("pe"), ud("ud"), bd("bd");
	sc_signal<sc_uint<BIT_WIDTH>> p("p"), q("q");

	UPDOWN_COUNTER updown_counter0("updown_counter0");
	updown_counter0.CLK(clk);
	updown_counter0.PE(pe);
	updown_counter0.UD(ud);
	updown_counter0.BD(bd);
	updown_counter0.P(p);
	updown_counter0.Q(q);

    sc_trace_file* trace_file = 
		sc_create_vcd_trace_file("updown_counter_waveform");
	trace_file->set_time_unit(100, SC_PS);
	sc_set_trace(clk);
	sc_set_trace(pe);
	sc_set_trace(ud);
	sc_set_trace(bd);
	sc_set_trace(p);
	sc_set_trace(q);

	auto mono_pulse = [=](sc_signal<bool> &pulse){
		pulse.write(0);
		sc_start(5, SC_NS);
		pulse.write(1);
		sc_start(5, SC_NS);
		pulse.write(0);
	};
	print("---- Start sim ----\n");
	
	print("p = ", 0b1001, "\n");
	p.write(0b1001);
	mono_pulse(clk);

	print("pe = ", 0, "\n");
	pe.write(0); 
	sc_start(5, SC_NS);
	pe.write(1);

	print("bd = ", 1, " ud = ", 1, '\n');
	bd.write(1);
	ud.write(1);
	rep(i, 18) {
		mono_pulse(clk);
		print("\t", "Q = ", q.read(), '\n');
	}

	print("bd = ", 0, " ud = ", 1, '\n');
	bd.write(0);
	ud.write(1);
	rep(i, 18) {
		mono_pulse(clk);
		print("\t", "Q = ", q.read(), '\n');
	}

	print("bd = ", 1, " ud = ", 0, '\n');
	bd.write(1);
	ud.write(0);
	rep(i, 18) {
		mono_pulse(clk);
		print("\t", "Q = ", q.read(), '\n');
	}

	print("bd = ", 0, " ud = ", 0, '\n');
	bd.write(0);
	ud.write(0);
	rep(i, 18) {
		mono_pulse(clk);
		print("\t", "Q = ", q.read(), '\n');
	}

	print("---- Finished ----\n");
    sc_close_vcd_trace_file(trace_file);
	return 0;
}