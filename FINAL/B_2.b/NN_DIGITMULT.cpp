#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <systemc.h>
#include "NN_DIGITMULT.h"

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
        pulse.write(1);
        sc_start(10, SC_NS);
        pulse.write(0);
    };
#endif

int sc_main(int argc, char* argv[]) {
    std::srand(std::time(0));  // Seed random for reset noise

    // Define signals
    sc_clock CLK("CLK", 10, SC_NS);                // 10 ns clock
    sc_signal<bool> RST("RST");
    sc_signal<bool> EN("EN");
    sc_signal<sc_uint<32>> A("A");
    sc_signal<sc_uint<32>> B("B");
    sc_signal<bool> DONE("DONE");
    sc_vector<sc_signal<sc_uint<32>>> OUT("OUT", 2);

    // Instantiate NN_DIGITMULT
    NN_DIGITMULT mult("mult");
    mult.CLK(CLK);
    mult.RST(RST);
    mult.EN(EN);
    mult.A(A);
    mult.B(B);
    mult.DONE(DONE);
    mult.OUT(OUT);

    // Set up VCD trace file
    sc_trace_file* trace_file = sc_create_vcd_trace_file("wavetrace_output");
    trace_file->set_time_unit(1, SC_NS);

    sc_set_trace(CLK);
    sc_set_trace(RST);
    sc_set_trace(EN);
    sc_set_trace(A);
    sc_set_trace(B);
    sc_set_trace(DONE);
    sc_set_trace(OUT[0]);
    sc_set_trace(OUT[1]);

    std::cout << "Start sim...\n";

    // Reset sequence
    RST.write(1);
    EN.write(false);
    sc_start(10, SC_NS);
    RST.write(0);

    // Print table header
    std::cout << "+--------------+--------------+--------------+--------------+--------------+--------------+\n";
    std::cout << "| EN           | A            | B            | DONE         | OUT[0]       | OUT[1]       |\n";
    std::cout << "+--------------+--------------+--------------+--------------+--------------+--------------+\n";

    // Test loop
    rep(i, 0, 16) {
        rep(j, 0, 16) {
            A.write(rand()%UINT32_MAX);
            B.write(rand()%UINT32_MAX);
            mono_pulse(EN);  // One-clock-cycle enable

            // Wait for DONE to be asserted
            while (!DONE.read()) {
                sc_start(10, SC_NS);
            }

            // Print result
            std::cout << "| " << std::left << std::setw(12) << EN.read()
                      << " | " << std::setw(12) << A.read()
                      << " | " << std::setw(12) << B.read()
                      << " | " << std::setw(12) << DONE.read()
                      << " | " << std::setw(12) << OUT[0].read()
                      << " | " << std::setw(12) << OUT[1].read()
                      << " |\n";

            // Wait for DONE to de-assert (FSM returns to WAIT)
            while (DONE.read()) {
                sc_start(10, SC_NS);
            }
        }
    }
    std::cout << "+--------------+--------------+--------------+--------------+--------------+--------------+\n";

    std::cout << "\nFinished!\n";
    sc_close_vcd_trace_file(trace_file);
    sc_stop();
    return 0;
}
