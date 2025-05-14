#include <bits/stdc++.h>
using namespace std;

#include <systemc.h>
#include "NAND_4BIT.h"

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
		sc_start(5, SC_NS);
	};
#endif

int sc_main(int argc, char* argv[]) {
	// Define signals
	sc_signal<bool> rst("rst");
	sc_signal<bool> en("en"); // Replaces clk
	sc_signal<sc_uint<4>> a("a");
	sc_signal<sc_uint<4>> b("b");
	sc_signal<sc_uint<4>> out("out");

	// Instantiate NAND_4BIT
	NAND_4BIT nand_mod("nand_mod");
	nand_mod.EN(en);
	nand_mod.RST(rst);
	nand_mod.A(a);
	nand_mod.B(b);
	nand_mod.OUT(out);

	// Set up VCD trace file
	sc_trace_file* trace_file = sc_create_vcd_trace_file("wavetrace_output");
	trace_file->set_time_unit(1, SC_NS);

	// Trace all signals
	sc_set_trace(en);
	sc_set_trace(rst);
	sc_set_trace(a);
	sc_set_trace(b);
	sc_set_trace(out);

	cout << "Start sim...\n";

	// Initialize signals
	rst.write(false);
	en.write(false);
	a.write(0);
	b.write(0);
	sc_start(10, SC_NS);

	// Print table header
	cout << "+-------+-------+-------+\n";
	cout << "| A     | B     | OUT   |\n";
	cout << "+-------+-------+-------+\n";

	// Run 16×16 test cases
	rep(i, 0, 16){
		rep(j, 0, 16){
			a.write(i);
			b.write(j);

			// Random test reset
			if (rand() % 1007 == 0) {
				rst.write(true);
				sc_start(5, SC_NS);
				rst.write(false);
				sc_start(5, SC_NS);
			}

			// Trigger evaluation via EN pulse
			mono_pulse(en);

			// Print output row
			cout << "| " << std::left << std::setw(5) << a.read()
			     << " | " << std::setw(5) << b.read()
			     << " | " << std::setw(5) << out.read()
			     << " |\n";
		}
	}

	// Print table footer
	cout << "+-------+-------+-------+\n";

	cout << "\nFinished!\n";
	sc_close_vcd_trace_file(trace_file);
	return 0;
}
