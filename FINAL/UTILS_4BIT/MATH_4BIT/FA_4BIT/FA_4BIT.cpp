#include <bits/stdc++.h>
using namespace std;

#include <systemc.h>
#include "FA_4BIT.h"

#ifndef SC_TRACE
	#define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif

#ifndef rep
	#define rep(var, start, stop) for(int var = (start); var < (stop); ++var)
#endif
#ifndef rev
	#define rev(var, start, stop) for(int var = (start); var > (stop); --var)
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

int sc_main(int argc, char* argv[]) {
	// Define signals
	sc_signal<bool> en("en");
	sc_signal<bool> rst("rst");
	sc_signal<sc_uint<4>> a("a");
	sc_signal<sc_uint<4>> b("b");
	sc_signal<bool> carry_in("carry_in");
	sc_signal<sc_uint<4>> out("out");
	sc_signal<bool> carry_out("carry_out");

	// Instantiate FA_4BIT
	FA_4BIT fa("fa");
	fa.EN(en);
	fa.RST(rst);
	fa.A(a);
	fa.B(b);
	fa.CARRY_IN(carry_in);
	fa.OUT(out);
	fa.CARRY_OUT(carry_out);

	// Set up VCD trace file
	sc_trace_file* trace_file = sc_create_vcd_trace_file("wavetrace_output");
	trace_file->set_time_unit(1, SC_NS);

	// Trace all signals
	sc_set_trace(en);
	sc_set_trace(rst);
	sc_set_trace(a);
	sc_set_trace(b);
	sc_set_trace(carry_in);
	sc_set_trace(out);
	sc_set_trace(carry_out);

	cout << "Start sim...\n";

	// Initialize signals
	rst.write(false);
	en.write(false);
	a.write(0);
	b.write(0);
	carry_in.write(false);
	sc_start(10, SC_NS);

	// Print table header
	cout << "+----------+-------+-------+----------+-------+\n";
	cout << "| CARRY_IN | A     | B     | CARRY_OUT | OUT  |\n";
	cout << "+----------+-------+-------+----------+-------+\n";

	// Run 2*16*16 test cases
	rep(cin_val, 0, 2){
		rep(i, 0, 16){
			rep(j, 0, 16){
				// Apply test case
				carry_in.write(cin_val);
				a.write(i);
				b.write(j);

				// Random test RST
				if (rand() % 1007 == 0) {
					rst.write(true);
					sc_start(5, SC_NS);
					mono_pulse(en);
					mono_pulse(en);
					mono_pulse(en);
					rst.write(false);
				}

				// Simulate one enable pulse
				mono_pulse(en);

				// Print table row
				cout << "| " << std::left << std::setw(9) << carry_in.read()
				     << " | " << std::setw(5) << a.read()
				     << " | " << std::setw(5) << b.read()
				     << " | " << std::setw(9) << carry_out.read()
				     << " | " << std::setw(5) << out.read()
				     << " |\n";
			}
		}
	}

	// Print table footer
	cout << "+----------+-------+-------+----------+-------+\n";

	cout << "\nFinished!\n";
	sc_close_vcd_trace_file(trace_file);
	return 0;
}
