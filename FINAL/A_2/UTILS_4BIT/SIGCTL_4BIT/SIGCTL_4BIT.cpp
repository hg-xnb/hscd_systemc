#include <bits/stdc++.h>
using namespace std;

#include <systemc.h>
#include "SIGCTL_4BIT.h"

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
    // Define signals for SIGCTL_4BIT
    sc_signal<bool> rst("rst");
    sc_signal<sc_uint<4>> a("a");
    sc_signal<sc_uint<4>> b("b");
    sc_signal<sc_uint<4>> select("select");
    sc_signal<bool> fs_rst("fs_rst"), fa_rst("fa_rst"), and_rst("and_rst");
    sc_signal<bool> nand_rst("nand_rst"), or_rst("or_rst"), xor_rst("xor_rst");
    sc_signal<bool> fs_en("fs_en"), fa_en("fa_en"), and_en("and_en");
    sc_signal<bool> nand_en("nand_en"), or_en("or_en"), xor_en("xor_en");
    sc_signal<bool> out_mux_en("out_mux_en");
    sc_signal<bool> fs_b_in("fs_b_in"), fa_c_in("fa_c_in");
    sc_signal<sc_uint<4>> fs_a("fs_a"), fa_a("fa_a"), and_a("and_a");
    sc_signal<sc_uint<4>> nand_a("nand_a"), or_a("or_a"), xor_a("xor_a");
    sc_signal<sc_uint<4>> fs_b("fs_b"), fa_b("fa_b"), and_b("and_b");
    sc_signal<sc_uint<4>> nand_b("nand_b"), or_b("or_b"), xor_b("xor_b");
    sc_signal<sc_uint<3>> out_mux_sel("out_mux_sel");
    sc_signal<bool> en("en");  // Clock for triggering

    // Define signals for module outputs
    sc_signal<sc_uint<4>> fa_out("fa_out"), fs_out("fs_out"), and_out("and_out");
    sc_signal<sc_uint<4>> nand_out("nand_out"), or_out("or_out"), xor_out("xor_out");
    sc_signal<bool> fa_carry_out("fa_carry_out"), fs_borrow_out("fs_borrow_out");

    // Instantiate SIGCTL_4BIT
    SIGCTL_4BIT sigctl("sigctl");
    sigctl.RST(rst);
	sigctl.EN(en);
    sigctl.A(a);
    sigctl.B(b);
    sigctl.SELECT(select);
    sigctl.FS_RST(fs_rst);
    sigctl.FA_RST(fa_rst);
    sigctl.AND_RST(and_rst);
    sigctl.NAND_RST(nand_rst);
    sigctl.OR_RST(or_rst);
    sigctl.XOR_RST(xor_rst);
    sigctl.FS_EN(fs_en);
    sigctl.FA_EN(fa_en);
    sigctl.AND_EN(and_en);
    sigctl.NAND_EN(nand_en);
    sigctl.OR_EN(or_en);
    sigctl.XOR_EN(xor_en);
    sigctl.OUT_MUX_EN(out_mux_en);
    sigctl.FS_B_IN(fs_b_in);
    sigctl.FA_C_IN(fa_c_in);
    sigctl.FS_A(fs_a);
    sigctl.FA_A(fa_a);
    sigctl.AND_A(and_a);
    sigctl.NAND_A(nand_a);
    sigctl.OR_A(or_a);
    sigctl.XOR_A(xor_a);
    sigctl.FS_B(fs_b);
    sigctl.FA_B(fa_b);
    sigctl.AND_B(and_b);
    sigctl.NAND_B(nand_b);
    sigctl.OR_B(or_b);
    sigctl.XOR_B(xor_b);
    sigctl.OUT_MUX_SEL(out_mux_sel);

    // Set up VCD trace file
    sc_trace_file* trace_file = sc_create_vcd_trace_file("wavetrace_output");
    trace_file->set_time_unit(1, SC_NS);

    // Trace all signals
    sc_set_trace(rst);
    sc_set_trace(a);
    sc_set_trace(b);
    sc_set_trace(select);
    sc_set_trace(fs_rst);
    sc_set_trace(fa_rst);
    sc_set_trace(and_rst);
    sc_set_trace(nand_rst);
    sc_set_trace(or_rst);
    sc_set_trace(xor_rst);
    sc_set_trace(fs_en);
    sc_set_trace(fa_en);
    sc_set_trace(and_en);
    sc_set_trace(nand_en);
    sc_set_trace(or_en);
    sc_set_trace(xor_en);
    sc_set_trace(out_mux_en);
    sc_set_trace(fs_b_in);
    sc_set_trace(fa_c_in);
    sc_set_trace(fs_a);
    sc_set_trace(fa_a);
    sc_set_trace(and_a);
    sc_set_trace(nand_a);
    sc_set_trace(or_a);
    sc_set_trace(xor_a);
    sc_set_trace(fs_b);
    sc_set_trace(fa_b);
    sc_set_trace(and_b);
    sc_set_trace(nand_b);
    sc_set_trace(or_b);
    sc_set_trace(xor_b);
    sc_set_trace(out_mux_sel);
    sc_set_trace(en);
    sc_set_trace(fa_out);
    sc_set_trace(fa_carry_out);
    sc_set_trace(fs_out);
    sc_set_trace(fs_borrow_out);
    sc_set_trace(and_out);
    sc_set_trace(nand_out);
    sc_set_trace(or_out);
    sc_set_trace(xor_out);

    cout << "Start sim...\n";

    // Initialize signals
    rst.write(false);
    a.write(0);
    b.write(0);
    select.write(0);
    en.write(0);
    sc_start(10, SC_NS);

    // Seed random number generator
    srand(time(nullptr));

    // Test loop: SEL from 0 to 9, 5 random <A, B> pairs each
    int test_case = 1;
    for (int sel = 0; sel <= 12; ++sel) {
        select.write(sel);
        cout << "\nTesting SEL = " << bitset<4>(sel) << "\n";

        for (int i = 0; i < 5; ++i) {
            // Generate random A and B (0 to 15)
            sc_uint<4> rand_a = rand() % 16;
            sc_uint<4> rand_b = rand() % 16;
            a.write(rand_a);
            b.write(rand_b);

            // Simulate one clock cycle
            mono_pulse(en);

            // Print test case header
            cout << "+------------------------------------+\n";
            cout << "| Test Case " << std::left << std::setw(2) << test_case++ << ": SEL=" << bitset<4>(select.read())
                 << ", A=" << bitset<4>(a.read()) << ", B=" << bitset<4>(b.read()) << " |\n";
            cout << "+------------------------------------+\n";

            // Print all signals as rows in binary
            cout << "| SELECT      | " << std::setw(8) << bitset<4>(select.read()) << " |\n";
            cout << "| A           | " << std::setw(8) << bitset<4>(a.read()) << " |\n";
            cout << "| B           | " << std::setw(8) << bitset<4>(b.read()) << " |\n";
            cout << "| FS_RST      | " << std::setw(8) << bitset<1>(fs_rst.read()) << " |\n";
            cout << "| FA_RST      | " << std::setw(8) << bitset<1>(fa_rst.read()) << " |\n";
            cout << "| AND_RST     | " << std::setw(8) << bitset<1>(and_rst.read()) << " |\n";
            cout << "| NAND_RST    | " << std::setw(8) << bitset<1>(nand_rst.read()) << " |\n";
            cout << "| OR_RST      | " << std::setw(8) << bitset<1>(or_rst.read()) << " |\n";
            cout << "| XOR_RST     | " << std::setw(8) << bitset<1>(xor_rst.read()) << " |\n";
            cout << "| FS_EN      | " << std::setw(8) << bitset<1>(fs_en.read()) << " |\n";
            cout << "| FA_EN      | " << std::setw(8) << bitset<1>(fa_en.read()) << " |\n";
            cout << "| AND_EN     | " << std::setw(8) << bitset<1>(and_en.read()) << " |\n";
            cout << "| NAND_EN    | " << std::setw(8) << bitset<1>(nand_en.read()) << " |\n";
            cout << "| OR_EN      | " << std::setw(8) << bitset<1>(or_en.read()) << " |\n";
            cout << "| XOR_EN     | " << std::setw(8) << bitset<1>(xor_en.read()) << " |\n";
            cout << "| OUT_MUX_EN | " << std::setw(8) << bitset<1>(out_mux_en.read()) << " |\n";
            cout << "| FS_B_IN     | " << std::setw(8) << bitset<1>(fs_b_in.read()) << " |\n";
            cout << "| FA_C_IN     | " << std::setw(8) << bitset<1>(fa_c_in.read()) << " |\n";
            cout << "| FS_A        | " << std::setw(8) << bitset<4>(fs_a.read()) << " |\n";
            cout << "| FA_A        | " << std::setw(8) << bitset<4>(fa_a.read()) << " |\n";
            cout << "| AND_A       | " << std::setw(8) << bitset<4>(and_a.read()) << " |\n";
            cout << "| NAND_A      | " << std::setw(8) << bitset<4>(nand_a.read()) << " |\n";
            cout << "| OR_A        | " << std::setw(8) << bitset<4>(or_a.read()) << " |\n";
            cout << "| XOR_A       | " << std::setw(8) << bitset<4>(xor_a.read()) << " |\n";
            cout << "| FS_B        | " << std::setw(8) << bitset<4>(fs_b.read()) << " |\n";
            cout << "| FA_B        | " << std::setw(8) << bitset<4>(fa_b.read()) << " |\n";
            cout << "| AND_B       | " << std::setw(8) << bitset<4>(and_b.read()) << " |\n";
            cout << "| NAND_B      | " << std::setw(8) << bitset<4>(nand_b.read()) << " |\n";
            cout << "| OR_B        | " << std::setw(8) << bitset<4>(or_b.read()) << " |\n";
            cout << "| XOR_B       | " << std::setw(8) << bitset<4>(xor_b.read()) << " |\n";
            cout << "| OUT_MUX_SEL | " << std::setw(8) << bitset<3>(out_mux_sel.read()) << " |\n";
            cout << "| FA_OUT      | " << std::setw(8) << bitset<4>(fa_out.read()) << " |\n";
            cout << "| FA_C_OUT    | " << std::setw(8) << bitset<1>(fa_carry_out.read()) << " |\n";
            cout << "| FS_OUT      | " << std::setw(8) << bitset<4>(fs_out.read()) << " |\n";
            cout << "| FS_B_OUT    | " << std::setw(8) << bitset<1>(fs_borrow_out.read()) << " |\n";
            cout << "| AND_OUT     | " << std::setw(8) << bitset<4>(and_out.read()) << " |\n";
            cout << "| NAND_OUT    | " << std::setw(8) << bitset<4>(nand_out.read()) << " |\n";
            cout << "| OR_OUT      | " << std::setw(8) << bitset<4>(or_out.read()) << " |\n";
            cout << "| XOR_OUT     | " << std::setw(8) << bitset<4>(xor_out.read()) << " |\n";
            cout << "+------------------------------------+\n";
            cout << "\n";
        }
    }

    cout << "\nFinished!\n";
    sc_close_vcd_trace_file(trace_file);
    return 0;
}