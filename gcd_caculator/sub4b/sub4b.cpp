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

#include "sub4b.h"

int sc_main(int argc, char *argv[])
{
	sc_signal<bool> clk("clk");
	sc_signal<sc_uint<WIDTH>> x("x");
	sc_signal<sc_uint<WIDTH>> y("y");
	sc_signal<sc_uint<WIDTH>> out("out");

	SUB4B sub4b("sub4b");
	sub4b.CLK(clk);
	sub4b.X(x);
	sub4b.Y(y);
	sub4b.OUT(out);

	sc_trace_file *trace_file =
		sc_create_vcd_trace_file("wavetrace_output");
	trace_file->set_time_unit(1, SC_NS);

	sc_set_trace(clk);
	sc_set_trace(x);
	sc_set_trace(y);
	sc_set_trace(out);

	cout << "Start sim...\n";

	/// Initialization
	print("\nInitialization...\n");
	sc_sim_set(clk, 0);
	sc_sim_set(x, 0x0);
	sc_sim_set(y, 0x0);
	sc_start(1, SC_NS); // allow signals to update
	print("$ clk = ", clk.read(), "\n");
	print("$ x = ", x.read(), "\n");
	print("$ y = ", y.read(), "\n");
	mono_pulse(clk);
	sc_start(1, SC_NS); // allow output to update
	print(" ---> out = ", out.read(), "\n");

	/// Testcase 1: x > y
	print("\nTestcase 1: x > y\n");
	sc_sim_set(x, 0xA); // 10
	sc_sim_set(y, 0x3); // 3
	sc_start(1, SC_NS);
	print("$ x = ", x.read(), "\n");
	print("$ y = ", y.read(), "\n");
	mono_pulse(clk);
	sc_start(1, SC_NS);
	print(" ---> out = ", out.read(), "\n");

	/// Test Case 2: x < y
	print("\nTestcase 2: x < y\n");
	sc_sim_set(x, 0x2); // 2
	sc_sim_set(y, 0x7); // 7
	sc_start(1, SC_NS);
	print("$ x = ", x.read(), "\n");
	print("$ y = ", y.read(), "\n");
	mono_pulse(clk);
	sc_start(1, SC_NS);
	print(" ---> out = ", out.read(), "\n");

	/// Test Case 3: x == y
	print("\nTestcase 3: x == y\n");
	sc_sim_set(x, 0x5); // 5
	sc_sim_set(y, 0x5); // 5
	sc_start(1, SC_NS);
	print("$ x = ", x.read(), "\n");
	print("$ y = ", y.read(), "\n");
	mono_pulse(clk);
	sc_start(1, SC_NS);
	print(" ---> out = ", out.read(), "\n");

	/// Test Case 4: both x and y are zero
	print("\nTestcase 4: x = y = 0\n");
	sc_sim_set(x, 0x0);
	sc_sim_set(y, 0x0);
	sc_start(1, SC_NS);
	print("$ x = ", x.read(), "\n");
	print("$ y = ", y.read(), "\n");
	mono_pulse(clk);
	sc_start(1, SC_NS);
	print(" ---> out = ", out.read(), "\n");

	/// Test Case 5: Maximum values
	print("\nTestcase 5: x = max, y = max\n");
	sc_sim_set(x, (1 << WIDTH) - 1); // Max value for WIDTH bits
	sc_sim_set(y, (1 << WIDTH) - 1);
	sc_start(1, SC_NS);
	print("$ x = ", x.read(), "\n");
	print("$ y = ", y.read(), "\n");
	mono_pulse(clk);
	sc_start(1, SC_NS);
	print(" ---> out = ", out.read(), "\n");

	cout << "\nFinished!\n";
	sc_close_vcd_trace_file(trace_file);
	return 0;
}
