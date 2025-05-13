#include <bits/stdc++.h>
using namespace std;

// Include your ALU_4BIT module here
#include "ALU_4BIT.h"

#ifndef SC_TRACE
    #define sc_set_trace(obj) sc_trace(trace_file, obj, obj.name());
#endif

#ifndef rev
    #define rev(i, n) for(int i = n-1; i >= 0; i--)
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
	sc_clock en("EN", 10, SC_NS);
    // sc_signal<bool> en("EN");
    sc_signal<bool> rst("RST");
    sc_signal<sc_uint<4>> a("A"), b("B");
    sc_signal<sc_uint<4>> select("SEL");
    sc_signal<bool> carry_out("C_OUT"), borrow_out("B_OUT");
    sc_signal<sc_uint<4>> out("OUT");


    // Instantiate ALU
    ALU_4BIT alu("ALU_4BIT");
    alu.EN(en);
    alu.RST(rst);
    alu.A(a);
    alu.B(b);
    alu.SEL(select);
    alu.C_OUT(carry_out);
    alu.B_OUT(borrow_out);
    alu.OUT(out);

    // Trace file
    sc_trace_file* trace_file = sc_create_vcd_trace_file("wavetrace_output");
    trace_file->set_time_unit(1, SC_NS);
    sc_set_trace(en); sc_set_trace(rst); sc_set_trace(a); sc_set_trace(b); sc_set_trace(select);
    sc_set_trace(carry_out); sc_set_trace(borrow_out); sc_set_trace(out);

    cout << "Start sim...\n";

    // Init
    rst.write(false);
    a.write(0);
    b.write(0);
    select.write(0);
    // en.write(0);
    sc_start(10, SC_NS);
    // Random seed
    srand(time(nullptr));
    int test_case = 1;
	sc_start(5, SC_NS);

    // Test loop
    for (int sel = 0; sel < 12; ++sel) {
        select.write(sel);
        cout << "\nTesting SEL = " << bitset<4>(sel) << "\n";

        for (int i = 0; i < 5; ++i) {
            sc_uint<4> rand_a = rand() % 16;
            sc_uint<4> rand_b = rand() % 16;
            a.write(rand_a);
            b.write(rand_b);
            // mono_pulse(en);
			sc_start(10, SC_NS);

            cout << "+------------------------------------+\n";
            cout << "| Test Case " << std::left << std::setw(2) << test_case++ << ": SEL=" << bitset<4>(select.read())
                 << ", A=" << bitset<4>(a.read()) << ", B=" << bitset<4>(b.read()) << " |\n";
            cout << "+------------------------------------+\n";
            cout << "| SELECT   | " << bitset<4>(select.read()) << " |\n";
            cout << "| A        | " << bitset<4>(a.read()) << " |\n";
            cout << "| B        | " << bitset<4>(b.read()) << " |\n";
            cout << "| OUT      | " << bitset<4>(out.read()) << " |\n";
            cout << "| C_OUT    | " << carry_out.read() << " |\n";
            cout << "| B_OUT    | " << borrow_out.read() << " |\n";
            cout << "+------------------------------------+\n\n";
        }
    }

    cout << "\nFinished!\n";
    sc_close_vcd_trace_file(trace_file);
    return 0;
}
