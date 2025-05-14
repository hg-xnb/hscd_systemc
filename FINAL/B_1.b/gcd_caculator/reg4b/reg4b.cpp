#include <systemc.h>

#ifndef SC_TRACE
	#define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
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
#ifndef _mono_pulse
	#define _mono_pulse
	void mono_pulse(sc_signal<bool> &pulse){
		pulse.write(1);
		sc_start(5, SC_NS);
		pulse.write(0);
		sc_start(5, SC_NS);
	};
#endif

#ifndef sc_sim_set
	#define sc_sim_set(SIGNAL, VAL) SIGNAL.write(VAL)
#endif

#include "reg4b.h"

int sc_main(int argc, char* argv[]) {
	sc_signal<bool> clk("clk");
	sc_signal<bool> reset("reset");
	sc_signal<bool> load("load");
	sc_signal<sc_uint<WIDTH>> data_in("data_in");
	sc_signal<sc_uint<WIDTH>> data_out("data_out");

	REG4B reg4b("reg4b");
	reg4b.CLK(clk);
	reg4b.RESET(reset);
	reg4b.LOAD(load);
	reg4b.IN(data_in);
	reg4b.OUT(data_out);

	sc_trace_file* trace_file = 
	sc_create_vcd_trace_file("wavetrace_output");
	trace_file->set_time_unit(1, SC_NS);

	sc_set_trace(clk);
	sc_set_trace(reset);
	sc_set_trace(load);
	sc_set_trace(data_in);
	sc_set_trace(data_out);

	cout << "Start sim...\n";
	/// initialization & reset
	print("\nInitialization... & reset...\n");
	sc_sim_set(reset, 1);		
	print("$ reset = ",  reset.read(), "\n");
	sc_sim_set(clk, 0);			
	print("$ clk = ",  clk.read(), "\n");
	sc_sim_set(load, 0);		
	print("$ load = ",  load.read(), "\n");
	sc_sim_set(data_in, 0xF);	
	print("$ data_in = ",  data_in.read(), "\n");
	mono_pulse(clk);
	sc_start(5, SC_NS);
	print(" ---> data_out = ", data_out.read(), "\n");

	/// load
	print("\nLoad...\n");
	mono_pulse(clk);
	sc_sim_set(reset, 0);		
	print("$ reset = ",  reset.read(), "\n");
	sc_sim_set(load, 1);		
	print("$ load = ",  load.read(), "\n");
	mono_pulse(clk);
	print(" ---> data_out = ", data_out.read(), "\n");
	sc_sim_set(data_in, 0x3);	
	print("$ data_in = ",  data_in.read(), "\n");
	mono_pulse(clk);
	print(" ---> data_out = ", data_out.read(), "\n");
	sc_sim_set(data_in, 0xD);	
	print("$ data_in = ",  data_in.read(), "\n");
	mono_pulse(clk);
	print(" ---> data_out = ", data_out.read(), "\n");

	/// reset
	print("\nReset...\n");
	sc_sim_set(load, 0);		
	print("$ load = ",  load.read(), "\n");
	sc_sim_set(load, 1);		
	print("$ load = ",  load.read(), "\n");
	mono_pulse(clk);
	print(" ---> data_out = ", data_out.read(), "\n");
	mono_pulse(clk);
	print(" ---> data_out = ", data_out.read(), "\n");
	sc_sim_set(data_in, 0x5);	
	print("$ data_in = ",  data_in.read(), "\n");
	mono_pulse(clk);
	print(" ---> data_out = ", data_out.read(), "\n");
	sc_sim_set(data_in, 0xB);	
	print("$ data_in = ",  data_in.read(), "\n");
	mono_pulse(clk);
	print(" ---> data_out = ", data_out.read(), "\n");
	
	cout << "\nFinished!\n";
    sc_close_vcd_trace_file(trace_file);
	return 0;
}

