#include <systemc.h>
#include "gcd_ctl.h"

#ifndef SC_TRACE
#define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif
#ifndef rev
#define rev(i, n) for (int i = n - 1; i >= 0; i--)
#endif
#ifndef var_print
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

// Function to print all outputs
void print_outputs(
    sc_signal<bool> &REG_X_SEL, sc_signal<bool> &REG_X_CLK, sc_signal<bool> &REG_X_LOAD, sc_signal<bool> &REG_X_RESET,
    sc_signal<bool> &REG_Y_SEL, sc_signal<bool> &REG_Y_CLK, sc_signal<bool> &REG_Y_LOAD, sc_signal<bool> &REG_Y_RESET,
    sc_signal<bool> &REG_O_SEL, sc_signal<bool> &REG_O_CLK, sc_signal<bool> &REG_O_LOAD, sc_signal<bool> &REG_O_RESET,
    sc_signal<bool> &X_SELF_SUBTRACTION, sc_signal<bool> &Y_SELF_SUBTRACTION) 
{
    print("Outputs:\n");
    print("  REG_X_SEL = ", REG_X_SEL.read(), ", REG_X_CLK = ", REG_X_CLK.read(), 
          ", REG_X_LOAD = ", REG_X_LOAD.read(), ", REG_X_RESET = ", REG_X_RESET.read(), "\n");
    print("  REG_Y_SEL = ", REG_Y_SEL.read(), ", REG_Y_CLK = ", REG_Y_CLK.read(), 
          ", REG_Y_LOAD = ", REG_Y_LOAD.read(), ", REG_Y_RESET = ", REG_Y_RESET.read(), "\n");
    print("  REG_O_SEL = ", REG_O_SEL.read(), ", REG_O_CLK = ", REG_O_CLK.read(), 
          ", REG_O_LOAD = ", REG_O_LOAD.read(), ", REG_O_RESET = ", REG_O_RESET.read(), "\n");
    print("  X_SELF_SUBTRACTION = ", X_SELF_SUBTRACTION.read(), 
          ", Y_SELF_SUBTRACTION = ", Y_SELF_SUBTRACTION.read(), "\n\n");
}

int sc_main(int argc, char *argv[]) {
    sc_clock CLK("CLK", 10, SC_NS, 0.5, 0, SC_NS, true); // Tạo clock tự động

    sc_signal<bool> EN("EN");
    sc_signal<bool> X_LT_Y("X_LT_Y"), X_GT_Y("X_GT_Y"), X_EQ_Y("X_EQ_Y");
    sc_signal<bool> REG_X_SEL("REG_X_SEL"), REG_X_CLK("REG_X_CLK"), REG_X_LOAD("REG_X_LOAD"), REG_X_RESET("REG_X_RESET");
    sc_signal<bool> REG_Y_SEL("REG_Y_SEL"), REG_Y_CLK("REG_Y_CLK"), REG_Y_LOAD("REG_Y_LOAD"), REG_Y_RESET("REG_Y_RESET");
    sc_signal<bool> REG_O_SEL("REG_O_SEL"), REG_O_CLK("REG_O_CLK"), REG_O_LOAD("REG_O_LOAD"), REG_O_RESET("REG_O_RESET");
    sc_signal<bool> X_SELF_SUBTRACTION("X_SELF_SUBTRACTION"), Y_SELF_SUBTRACTION("Y_SELF_SUBTRACTION");

    GCD_CTL gcd_ctl("gcd_ctl");
    gcd_ctl.CLK(CLK);
    gcd_ctl.EN(EN);
    gcd_ctl.X_LT_Y(X_LT_Y);
    gcd_ctl.X_GT_Y(X_GT_Y);
    gcd_ctl.X_EQ_Y(X_EQ_Y);
    gcd_ctl.REG_X_SEL(REG_X_SEL);
    gcd_ctl.REG_X_CLK(REG_X_CLK);
    gcd_ctl.REG_X_LOAD(REG_X_LOAD);
    gcd_ctl.REG_X_RESET(REG_X_RESET);
    gcd_ctl.REG_Y_SEL(REG_Y_SEL);
    gcd_ctl.REG_Y_CLK(REG_Y_CLK);
    gcd_ctl.REG_Y_LOAD(REG_Y_LOAD);
    gcd_ctl.REG_Y_RESET(REG_Y_RESET);
    gcd_ctl.REG_O_SEL(REG_O_SEL);
    gcd_ctl.REG_O_CLK(REG_O_CLK);
    gcd_ctl.REG_O_LOAD(REG_O_LOAD);
    gcd_ctl.REG_O_RESET(REG_O_RESET);
    gcd_ctl.X_SELF_SUBTRACTION(X_SELF_SUBTRACTION);
    gcd_ctl.Y_SELF_SUBTRACTION(Y_SELF_SUBTRACTION);

    // Tạo file VCD để trace
    sc_trace_file *trace_file = sc_create_vcd_trace_file("wavetrace_output");
    trace_file->set_time_unit(1, SC_NS);

    sc_set_trace(CLK);
    sc_set_trace(EN);
    sc_set_trace(X_LT_Y);
    sc_set_trace(X_GT_Y);
    sc_set_trace(X_EQ_Y);
    sc_set_trace(REG_X_SEL);
    sc_set_trace(REG_X_CLK);
    sc_set_trace(REG_X_LOAD);
    sc_set_trace(REG_X_RESET);
    sc_set_trace(REG_Y_SEL);
    sc_set_trace(REG_Y_CLK);
    sc_set_trace(REG_Y_LOAD);
    sc_set_trace(REG_Y_RESET);
    sc_set_trace(REG_O_SEL);
    sc_set_trace(REG_O_CLK);
    sc_set_trace(REG_O_LOAD);
    sc_set_trace(REG_O_RESET);
    sc_set_trace(X_SELF_SUBTRACTION);
    sc_set_trace(Y_SELF_SUBTRACTION);

    cout << "Start GCD_CTL simulation...\n";

    EN.write(1);

    // ========= TEST CASE 1: X < Y =========
    cout << "\n==== TEST CASE 1: X < Y ====\n";
    X_LT_Y.write(1);
    X_GT_Y.write(0);
    X_EQ_Y.write(0);
    sc_start(80, SC_NS);
    print_outputs(REG_X_SEL, REG_X_CLK, REG_X_LOAD, REG_X_RESET,
                  REG_Y_SEL, REG_Y_CLK, REG_Y_LOAD, REG_Y_RESET,
                  REG_O_SEL, REG_O_CLK, REG_O_LOAD, REG_O_RESET,
                  X_SELF_SUBTRACTION, Y_SELF_SUBTRACTION);

    // ========= TEST CASE 2: X > Y =========
    cout << "\n==== TEST CASE 2: X > Y ====\n";
    X_LT_Y.write(0);
    X_GT_Y.write(1);
    X_EQ_Y.write(0);
    sc_start(80, SC_NS);
    print_outputs(REG_X_SEL, REG_X_CLK, REG_X_LOAD, REG_X_RESET,
                  REG_Y_SEL, REG_Y_CLK, REG_Y_LOAD, REG_Y_RESET,
                  REG_O_SEL, REG_O_CLK, REG_O_LOAD, REG_O_RESET,
                  X_SELF_SUBTRACTION, Y_SELF_SUBTRACTION);

    // ========= TEST CASE 3: X == Y =========
    cout << "\n==== TEST CASE 3: X == Y ====\n";
    X_LT_Y.write(0);
    X_GT_Y.write(0);
    X_EQ_Y.write(1);
    sc_start(80, SC_NS);
    print_outputs(REG_X_SEL, REG_X_CLK, REG_X_LOAD, REG_X_RESET,
                  REG_Y_SEL, REG_Y_CLK, REG_Y_LOAD, REG_Y_RESET,
                  REG_O_SEL, REG_O_CLK, REG_O_LOAD, REG_O_RESET,
                  X_SELF_SUBTRACTION, Y_SELF_SUBTRACTION);

    // ========= TEST CASE 4: INVALID =========
    cout << "\n==== TEST CASE 4: INVALID ====\n";
    X_LT_Y.write(0);
    X_GT_Y.write(0);
    X_EQ_Y.write(0);
    sc_start(80, SC_NS);
    print_outputs(REG_X_SEL, REG_X_CLK, REG_X_LOAD, REG_X_RESET,
                  REG_Y_SEL, REG_Y_CLK, REG_Y_LOAD, REG_Y_RESET,
                  REG_O_SEL, REG_O_CLK, REG_O_LOAD, REG_O_RESET,
                  X_SELF_SUBTRACTION, Y_SELF_SUBTRACTION);

    cout << "Finished GCD_CTL simulation!\n";

    sc_close_vcd_trace_file(trace_file);
    return 0;
}
