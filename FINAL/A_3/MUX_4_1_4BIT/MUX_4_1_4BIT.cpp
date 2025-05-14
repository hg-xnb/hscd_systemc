#include <systemc.h>
#include "MUX_4_1_4BIT.h"

#include <bits/stdc++.h>
using namespace std;

#ifndef SC_TRACE
    #define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif

#ifndef rep
    #define rep(var, start, stop) for(int var = (start); var < (stop); ++var)
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
    // Signals
    sc_signal<bool> en("en");
    sc_signal<sc_uint<2>> sel("sel");
    sc_vector<sc_signal<sc_uint<4>>> in("in", 4);
    sc_signal<sc_uint<4>> out("out");

    // DUT
    MUX_4_1_4BIT mux("mux4_1_4bit");
    mux.EN(en);
    mux.SEL(sel);
    mux.IN(in);
    mux.OUT(out);

    // Trace
    sc_trace_file* trace_file = sc_create_vcd_trace_file("wavetrace_output");
    trace_file->set_time_unit(1, SC_NS);
    for (int i = 0; i < 4; ++i) sc_set_trace(in[i]);
    sc_set_trace(en);
    sc_set_trace(sel);
    sc_set_trace(out);

    // Init
    en.write(0);
    sel.write(0);
    for (int i = 0; i < 4; ++i) in[i].write(0);
    sc_start(10, SC_NS);

    // Header
    cout << "+-------+---------+---------+\n";
    cout << "| SEL   | IN[SEL] | OUT     |\n";
    cout << "+-------+---------+---------+\n";

    // Test
    for (int s = 0; s < 4; ++s) {
        sel.write(s);
        for (int n = 0; n < 16; ++n) {
            for (int i = 0; i < 4; ++i)
                in[i].write((n + i) % 16); // Change input pattern
            sc_start(3, SC_NS);
            en.write(1);
            sc_start(5, SC_NS);
            
            cout << "| " << std::left << std::setw(5) << sel.read()
                 << " | " << std::setw(7) << in[s].read()
                 << " | " << std::setw(7) << out.read()
                 << " |\n";

            en.write(0);
            sc_start(2, SC_NS);

        }
        cout << "\n";
    }

    cout << "+-------+---------+---------+\n";
    cout << "Simulation done.\n";

    sc_close_vcd_trace_file(trace_file);
    return 0;
}
