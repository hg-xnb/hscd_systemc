#include <systemc.h>

#ifndef SC_TRACE
#define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif
#ifndef rev
#define rev(i, n) for (int i = n - 1; i >= 0; i--)
#endif
#ifndef var_print /// variadic print
#define var_print
template <class T>
void print(T what) { cout << what; }
template <class T, class... Args>
void print(T what, Args... args)
{
	cout << what;
	print(args...);
}
#endif
#ifndef _mono_pulse
#define _mono_pulse
void mono_pulse(sc_signal<bool> &pulse)
{
	pulse.write(0);
	sc_start(5, SC_NS);
	pulse.write(1);
	sc_start(5, SC_NS);
	pulse.write(0);
};
#endif

#ifndef sc_sim_set
#define sc_sim_set(SIGNAL, VAL) SIGNAL.write(VAL)
#endif

#include "gcd.h"

int sc_main(int argc, char *argv[])
{
	sc_signal<bool> clk("clk");
	sc_signal<bool> en("en");
	sc_signal<sc_uint<WIDTH>> x("x");
	sc_signal<sc_uint<WIDTH>> y("y");
	sc_signal<sc_uint<WIDTH>> out("out");
	sc_signal<bool> done("done");


	GCD gcd("gcd");
	gcd.CLK(clk);
	gcd.EN(en);
	gcd.X(x);
	gcd.Y(y);
	gcd.OUT(out);
	gcd.DONE(done);

	sc_trace_file *trace_file = sc_create_vcd_trace_file("wavetrace_output");
	trace_file->set_time_unit(1, SC_NS);

	sc_set_trace(clk);
	sc_set_trace(en);
	sc_set_trace(x);
	sc_set_trace(y);
	sc_set_trace(out);
	sc_set_trace(done);

	cout << "Start GCD simulation...\n";

	// Reset initial states
	print("\nInitialization...\n");
	sc_sim_set(en, 0);
	sc_sim_set(clk, 0);
	sc_sim_set(x, 0);
	sc_sim_set(y, 0);
	sc_start(10, SC_NS);

	// Apply input
	sc_sim_set(x, 3);
	sc_sim_set(y, 2);
	print("$ x = ", x.read(), ", ");
	print("y = ", y.read(), "\n");
	mono_pulse(clk);
	mono_pulse(clk);
	mono_pulse(clk);
	mono_pulse(clk);
	mono_pulse(clk);
	// Enable the module
	sc_sim_set(en, 1);
	sc_start(10, SC_NS);

	// Wait for DONE signal
	print("\nStart sim...\n");

	sc_sim_set(x, 12);
	sc_sim_set(y, 4);
	print("$ x = ", x.read(), ", ");
	print("y = ", y.read(), "\n");
	while (done.read() == 0)
	{
		mono_pulse(clk);
		print("$ out = ", out.read(), "\n");
		mono_pulse(clk);
	}
	mono_pulse(clk);
	print("GCD(12,4) = ", out.read(), "\n");

	sc_sim_set(x, 9);
	sc_sim_set(y, 3);
	print("$ x = ", x.read(), ", ");
	print("y = ", y.read(), "\n");
	while (done.read() == 0)
	{
		mono_pulse(clk);
		print("$ out = ", out.read(), "\n");
		mono_pulse(clk);
	}
	mono_pulse(clk);
	print("GCD(9, 3) = ", out.read(), "\n");

	sc_sim_set(x, 3);
	sc_sim_set(y, 13);
	print("$ x = ", x.read(), ", ");
	print("y = ", y.read(), "\n");
	while (done.read() == 0)
	{
		mono_pulse(clk);
		print("$ out = ", out.read(), "\n");
		mono_pulse(clk);
	}
	mono_pulse(clk);
	print("GCD(3, 13) = ", out.read(), "\n");

	sc_sim_set(x, 7);
	sc_sim_set(y, 0);
	print("$ x = ", x.read(), ", ");
	print("y = ", y.read(), "\n");
	while (done.read() == 0)
	{
		mono_pulse(clk);
		print("$ out = ", out.read(), "\n");
		mono_pulse(clk);
	}
	mono_pulse(clk);
	print("GCD(7, 0) = ", out.read(), "\n");

	sc_sim_set(x, 5);
	sc_sim_set(y, 5);
	print("$ x = ", x.read(), ", ");
	print("y = ", y.read(), "\n");
	while (done.read() == 0)
	{
		mono_pulse(clk);
		print("$ out = ", out.read(), "\n");
		mono_pulse(clk);
	}
	mono_pulse(clk);
	print("GCD(5, 5) = ", out.read(), "\n");

	cout << "\nSimulation complete!\n";

	sc_close_vcd_trace_file(trace_file);
	return 0;
}
