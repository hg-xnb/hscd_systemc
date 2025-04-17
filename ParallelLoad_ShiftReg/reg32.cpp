#include <bits/stdc++.h>
using namespace std;

#include <systemc.h>
using namespace sc_core;

#ifndef sc_set_trace
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

#include "reg32.h"

int sc_main(int argc, char* argv[]) {
	sc_signal<bool> clk("clk");
	sc_signal<sc_uint<MODE_BIT>> sh("sh");
	sc_signal<sc_uint<BIT_WIDTH>> input("input");
	sc_signal<sc_uint<BIT_WIDTH>> output("output");

	REG32 reg32("reg32");
	reg32.clk(clk);
	reg32.SH(sh);
	reg32.input(input);
	reg32.output(output);

	sc_trace_file* trace_file = 
		sc_create_vcd_trace_file("reg32_waveform");
	trace_file->set_time_unit(100, SC_PS);
	sc_set_trace(clk);
	sc_set_trace(sh);
	sc_set_trace(input);
	sc_set_trace(output);

	auto mono_pulse = [=](sc_signal<bool> &pulse){
		pulse.write(0);
		sc_start(5, SC_NS);
		pulse.write(1);
		sc_start(5, SC_NS);
		pulse.write(0);
	};

	print("---- Start sim ----\n");
	
	print("Assign all rows of the register...\n");
	rep(i, 10){
		print("$ input = ", 0b10011001*(i+3) %256);
		input.write(0b10011001*(i+3));
		
		print(", sh = ", LOAD_INPUT);
		sh.write(LOAD_INPUT);
		mono_pulse(clk);
		
		print("\t-> output = ", output.read(), '\n');
	}

	print("Hold data...\n");
		print("$ sh = ", HOLD_VALUE);
		sh.write(HOLD_VALUE);
		rep(i, ROWS){
			print("$ input = ", 0b10011001*(i+7) %256);
			input.write(0b10011001*(i+7));
			mono_pulse(clk);
			print("\t-> output = ", output.read(), '\n');
		}
	
	print("Shift left the row index...\n");
	print("$ sh = ", SHIFT_LEFT);
	sh.write(SHIFT_LEFT);
	rep(i, 10){
		print("$ input = ", 0b10011001*(i+7) %256);
		input.write(0b10011001*(i+7));
		mono_pulse(clk);
			print("\t-> output = ", output.read(), '\n');
		}
		
	print("Reload the input for right shifting...\n");
		print("$ input = ", 0b10011001*(7) %256);
		input.write(0b10011001*(7));
		print(", sh = ", LOAD_INPUT);
		sh.write(LOAD_INPUT);
		mono_pulse(clk);
		print("\t-> output = ", output.read(), '\n');
		
	print("Shift right the row index...\n");
		print("$ sh = ", SHIFT_RIGHT);
		sh.write(SHIFT_RIGHT);
		rep(i, 10){
			print("$ input = ", 0b10011001*(i+7) %256);
			input.write(0b10011001*(i+7));
			mono_pulse(clk);
			print("\t-> output = ", output.read(), '\n');
		}

	print("---- Finished ----\n");
    sc_close_vcd_trace_file(trace_file);
	return 0;
}
