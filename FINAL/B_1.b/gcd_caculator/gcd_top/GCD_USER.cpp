#include <systemc.h>
#include "GCD_USER.h"

#ifndef SC_TRACE
    #define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif

#ifndef rep
    #define rep(var, start, stop) for(int var = (start); var < (stop); --var)
#endif
#ifndef rev
    #define rev(var, start, stop) for(int var = (start); var > (stop); --var)
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
    // Create clock and signals
    sc_clock clk("clk", 10, SC_NS, 0.5); // 10ns period, 50% duty cycle
    sc_signal<sc_uint<4>> x_sig("x_sig"), y_sig("y_sig"), out_sig("out_sig");
    sc_signal<bool> en_sig("en_sig"), done_sig("done_sig"), stop_sig("stop_sig");

    // Instantiate GCD_USER
    GCD_USER user("user");
    user.clk(clk);
    user.x_sig(x_sig);
    user.y_sig(y_sig);
    user.en_sig(en_sig);
    user.done_sig(done_sig);
    user.out_sig(out_sig);
    user.stop_sig(stop_sig);

    // Set up trace file
    sc_trace_file* trace_file = sc_create_vcd_trace_file("wavetrace_output");
    trace_file->set_time_unit(1, SC_NS);

    // Trace signals using sc_set_trace macro
    sc_set_trace(clk);
    sc_set_trace(x_sig);
    sc_set_trace(y_sig);
    sc_set_trace(en_sig);
    sc_set_trace(done_sig);
    sc_set_trace(out_sig);
    sc_set_trace(stop_sig);

    // Print start message using var_print
    var_print("Start sim...\n");

    // Run simulation
    sc_start(); // Run until sc_stop() is called by the module

    // Print finish message and close trace file
    var_print("\nFinished!\n");
    sc_close_vcd_trace_file(trace_file);
    return 0;
}