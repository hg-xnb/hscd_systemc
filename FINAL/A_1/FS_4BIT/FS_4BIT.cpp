#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <systemc.h>
#include "FS_4BIT.h"

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
    template<class T> void print(T what){std::cout << what;}
    template<class T, class... Args> void print(T what, Args... args){
        std::cout << what;
        print(args...);
    }
#endif

#ifndef _mono_pulse
    #define _mono_pulse
    void mono_pulse(sc_signal<bool> &pulse){
        pulse.write(0);
        sc_start(5, SC_NS);
        pulse.write(1);
        sc_start(5, SC_NS);
        pulse.write(0);
    };
#endif

int sc_main(int argc, char* argv[]) {
    std::srand(std::time(0));  // Seed random for reset noise

    // Define signals
    sc_signal<bool> en("en");
    sc_signal<bool> rst("rst");
    sc_signal<sc_uint<4>> a("a");
    sc_signal<sc_uint<4>> b("b");
    sc_signal<bool> borrow_in("borrow_in");
    sc_signal<sc_uint<4>> out("out");
    sc_signal<bool> borrow_out("borrow_out");

    // Instantiate FS_4BIT
    FS_4BIT fs("fs");
    fs.EN(en);  // Replaced CLK with EN
    fs.RST(rst);
    fs.A(a);
    fs.B(b);
    fs.BORROW_IN(borrow_in);
    fs.OUT(out);
    fs.BORROW_OUT(borrow_out);

    // Set up VCD trace file
    sc_trace_file* trace_file = sc_create_vcd_trace_file("wavetrace_output");
    trace_file->set_time_unit(1, SC_NS);

    sc_set_trace(en);
    sc_set_trace(rst);
    sc_set_trace(a);
    sc_set_trace(b);
    sc_set_trace(borrow_in);
    sc_set_trace(out);
    sc_set_trace(borrow_out);

    std::cout << "Start sim...\n";

    // Initialize signals
    rst.write(false);
    en.write(false);
    a.write(0);
    b.write(0);
    borrow_in.write(false);
    sc_start(10, SC_NS);

    // Print table header
    std::cout << "+-------+-------+-------+-------+-------+\n";
    std::cout << "| B_IN  | A     | B     | B_OUT | OUT   |\n";
    std::cout << "+-------+-------+-------+-------+-------+\n";

    // Test loop
    rep(bin, 0, 2){
        rep(i, 0, 16){
            rep(j, 0, 16){
                borrow_in.write(bin);
                a.write(i);
                b.write(j);

                // Occasionally trigger reset
                if (rand() % 1007 == 0) {
                    rst.write(true);
                    sc_start(5, SC_NS);
                    mono_pulse(en);
                    mono_pulse(en);
                    mono_pulse(en);
                    rst.write(false);
                }

                mono_pulse(en);  // Trigger enable (was clk)

                // Print output
                std::cout << "| " << std::left << std::setw(5) << borrow_in.read()
                          << " | " << std::setw(5) << a.read()
                          << " | " << std::setw(5) << b.read()
                          << " | " << std::setw(5) << borrow_out.read()
                          << " | " << std::setw(5) << out.read()
                          << " |\n";
            }
        }
    }

    std::cout << "+-------+-------+-------+-------+-------+\n";
    std::cout << "\nFinished!\n";
    sc_close_vcd_trace_file(trace_file);
    return 0;
}
