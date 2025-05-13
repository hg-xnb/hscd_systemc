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
	sc_signal<bool> _X_LT_Y("_X_LT_Y"), _X_GT_Y("_X_GT_Y"),
		_X_EQ_Y("_X_EQ_Y"),
		_REG_X_SEL("_REG_X_SEL"), _REG_X_CLK("_REG_X_CLK"),
		_REG_X_LOAD("_REG_X_LOAD"), _REG_X_RESET("_REG_X_RESET"),
		_REG_Y_SEL("_REG_Y_SEL"), _REG_Y_CLK("_REG_Y_CLK"),
		_REG_Y_LOAD("_REG_Y_LOAD"), _REG_Y_RESET("_REG_Y_RESET"),
		_REG_O_SEL("_REG_O_SEL"), _REG_O_CLK("_REG_O_CLK"),
		_REG_O_LOAD("_REG_O_LOAD"), _REG_O_RESET("_REG_O_RESET"),
		_X_SELF_SUBTRACTION("_X_SELF_SUBTRACTION"), _Y_SELF_SUBTRACTION("_Y_SELF_SUBTRACTION");
	sc_signal<sc_uint<WIDTH>> _internalX("_internalX"),
		_internalY("_internalY");

	GCD gcd("gcd");
	gcd.CLK(clk);
	gcd.EN(en);
	gcd.X(x);
	gcd.Y(y);
	gcd.OUT(out);
	gcd.DONE(done);
	/// for debug
	gcd._X_LT_Y(_X_LT_Y);
	gcd._X_GT_Y(_X_GT_Y);
	gcd._X_EQ_Y(_X_EQ_Y);
	gcd._REG_X_SEL(_REG_X_SEL);
	gcd._REG_X_CLK(_REG_X_CLK);
	gcd._REG_X_LOAD(_REG_X_LOAD);
	gcd._REG_X_RESET(_REG_X_RESET);
	gcd._REG_Y_SEL(_REG_Y_SEL);
	gcd._REG_Y_CLK(_REG_Y_CLK);
	gcd._REG_Y_LOAD(_REG_Y_LOAD);
	gcd._REG_Y_RESET(_REG_Y_RESET);
	gcd._REG_O_SEL(_REG_O_SEL);
	gcd._REG_O_CLK(_REG_O_CLK);
	gcd._REG_O_LOAD(_REG_O_LOAD);
	gcd._REG_O_RESET(_REG_O_RESET);
	gcd._X_SELF_SUBTRACTION(_X_SELF_SUBTRACTION);
	gcd._Y_SELF_SUBTRACTION(_Y_SELF_SUBTRACTION);
	gcd._internalX(_internalX);
	gcd._internalY(_internalY);

	sc_trace_file *trace_file = sc_create_vcd_trace_file("wavetrace_output");
	trace_file->set_time_unit(1, SC_NS);

	sc_set_trace(clk);
	sc_set_trace(en);
	sc_set_trace(x);
	sc_set_trace(y);
	sc_set_trace(out);
	sc_set_trace(done);

	sc_set_trace(_X_LT_Y);
	sc_set_trace(_X_GT_Y);
	sc_set_trace(_X_EQ_Y);
	sc_set_trace(_REG_X_SEL);
	sc_set_trace(_REG_X_CLK);
	sc_set_trace(_REG_X_LOAD);
	sc_set_trace(_REG_X_RESET);
	sc_set_trace(_REG_Y_SEL);
	sc_set_trace(_REG_Y_CLK);
	sc_set_trace(_REG_Y_LOAD);
	sc_set_trace(_REG_Y_RESET);
	sc_set_trace(_REG_O_SEL);
	sc_set_trace(_REG_O_CLK);
	sc_set_trace(_REG_O_LOAD);
	sc_set_trace(_REG_O_RESET);
	sc_set_trace(_X_SELF_SUBTRACTION);
	sc_set_trace(_Y_SELF_SUBTRACTION);
	sc_set_trace(_internalX);
	sc_set_trace(_internalY);

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
