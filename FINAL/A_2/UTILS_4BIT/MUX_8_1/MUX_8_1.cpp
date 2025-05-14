#include <systemc.h>
#include "MUX_8_1.h"

#include <bits/stdc++.h>
using namespace std;

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

int sc_main(int argc, char* argv[]) {
    // Define signals
    sc_vector<sc_signal<sc_uint<4>>> in("in", 8);
    sc_signal<sc_uint<3>> sel("sel");
    sc_signal<bool> en("en");  // Changed from clk to en
    sc_signal<sc_uint<4>> out("out");

    // Instantiate MUX_8_1
    MUX_8_1 mux81("mux81");
    mux81.EN(en);
    mux81.SEL(sel);
    mux81.IN(in);
    mux81.OUT(out);

    // Set up VCD trace file
    sc_trace_file* trace_file = sc_create_vcd_trace_file("wavetrace_output");
    trace_file->set_time_unit(1, SC_NS);

    // Trace all signals
    for (int i = 0; i < 8; ++i) sc_set_trace(in[i]);
    sc_set_trace(sel);
    sc_set_trace(en);
    sc_set_trace(out);

    cout << "Start sim...\n";

    // Initialize signals
    en.write(0);
    sel.write(0);
    for (int i = 0; i < 8; ++i) in[i].write(0);
    sc_start(10, SC_NS);

    // Print table header
    cout << "+-------+---------+---------+\n";
    cout << "| SEL   | IN[SEL] | OUT     |\n";
    cout << "+-------+---------+---------+\n";

    // Test loop: cycle through SEL and input patterns
    for (int s = 0; s < 8; ++s) {  // SEL: 0 to 7
        sel.write(s);
        for (int n = 0; n < 16; ++n) {
            for (int i = 0; i < 8; ++i) in[i].write((n + i) % 16);

            en.write(1);
            sc_start(5, SC_NS);
            en.write(0);
            sc_start(5, SC_NS);

            cout << "| " << std::left << std::setw(5) << sel.read()
                 << " | " << std::setw(7) << in[s].read()
                 << " | " << std::setw(7) << out.read()
                 << " |\n";
        }
        cout << "\n";
    }

    // Print table footer
    cout << "+-------+---------+---------+\n";

    cout << "\nFinished!\n";
    sc_close_vcd_trace_file(trace_file);
    return 0;
}
