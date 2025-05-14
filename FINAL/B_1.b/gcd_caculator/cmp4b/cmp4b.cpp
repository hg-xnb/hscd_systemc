#include <systemc.h>
#include "cmp4b.h" // Include the header for your CMP4B module

#ifndef SC_TRACE
#define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif
#ifndef rev
#define rev(i, n) for (int i = n - 1; i >= 0; i--)
#endif
#ifndef var_print /// variadic print
#define var_print
template <class T>
void print(T what) {
  cout << what;
}
template <class T, class... Args>
void print(T what, Args... args) {
  cout << what;
  print(args...);
}
#endif
#ifndef _mono_pulse
#define _mono_pulse
void mono_pulse(sc_signal<bool> &pulse) {
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

int sc_main(int argc, char *argv[]) {
  // Signals
  sc_signal<sc_uint<4> > X("X");
  sc_signal<sc_uint<4> > Y("Y");
  sc_signal<bool> X_LT_Y_out("X_LT_Y_out");
  sc_signal<bool> X_GT_Y_out("X_GT_Y_out");
  sc_signal<bool> X_EQ_Y_out("X_EQ_Y_out");

  // Instantiate the CMP4B module
  CMP4B cmp4b("cmp4b");
  cmp4b.X(X);
  cmp4b.Y(Y);
  cmp4b.X_LT_Y(X_LT_Y_out);
  cmp4b.X_GT_Y(X_GT_Y_out);
  cmp4b.X_EQ_Y(X_EQ_Y_out);

  // Create a VCD trace file with the name from [1]
  sc_trace_file *trace_file = sc_create_vcd_trace_file("wavetrace_output");
  trace_file->set_time_unit(1, SC_NS);

  // Trace the signals
  sc_set_trace(X);
  sc_set_trace(Y);
  sc_set_trace(X_LT_Y_out);
  sc_set_trace(X_GT_Y_out);
  sc_set_trace(X_EQ_Y_out);

  cout << "Start CMP4B simulation...\n";

  // Initialize signals
  sc_sim_set(X, 0);
  sc_sim_set(Y, 0);
  sc_start(5, SC_NS); // Give the module a little time to stabilize

  // Test all possible combinations of X and Y (exhaustive testing)
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 16; ++j) {
      sc_sim_set(X, i);
      sc_sim_set(Y, j);
      sc_start(5, SC_NS); // Allow time for comparison
      print("$ X = ", i, ", Y = ", j, "\n");
      print("---> X_LT_Y = ", X_LT_Y_out.read(), ", ");
      print("X_GT_Y = ", X_GT_Y_out.read(), ", ");
      print("X_EQ_Y = ", X_EQ_Y_out.read(), "\n");
    }
  }

  cout << "Finished CMP4B simulation!\n";
  sc_close_vcd_trace_file(trace_file);
  return 0;
}
