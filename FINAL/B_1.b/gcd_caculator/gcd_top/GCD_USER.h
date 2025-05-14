#ifndef __GCD_USER_H__
#define __GCD_USER_H__

#include <systemc.h>
#include <iostream>
#include "../gcd_f/gcd.h"

SC_MODULE(GCD_USER) {
public:
    sc_in_clk clk;                  // Clock input
    sc_out<sc_uint<4>> x_sig;       // Output for input X
    sc_out<sc_uint<4>> y_sig;       // Output for input Y
    sc_out<bool> en_sig;            // Output for enable
    sc_out<bool> done_sig;          // Output for done
    sc_out<sc_uint<4>> out_sig;     // Output for GCD result
    sc_out<bool> stop_sig;          // Output for stop control

    SC_CTOR(GCD_USER) : 
    gcd_inst("gcd_inst"), clk("clk"), x_sig("x_sig"),
    en_sig("en_sig"), done_sig("done_sig"), out_sig("out_sig"),
    stop_sig("stop_sig")
    {
        // Bind GCD module ports to internal signals
        gcd_inst.CLK(clk);
        gcd_inst.EN(en_internal);
        gcd_inst.X(x_internal);
        gcd_inst.Y(y_internal);
        gcd_inst.DONE(done_internal);
        gcd_inst.OUT(out_internal);

        // Register interact method
        SC_METHOD(interact);
        sensitive << clk.pos();

        // Register copy method, sensitive to all internal signals
        SC_METHOD(copy);
        sensitive << x_internal << y_internal << en_internal << done_internal << out_internal << stop_internal;
    }

private:
    GCD gcd_inst;                   // Instance of GCD module
    sc_signal<sc_uint<4>> x_internal;    // Internal signal for X
    sc_signal<sc_uint<4>> y_internal;    // Internal signal for Y
    sc_signal<bool> en_internal;         // Internal signal for enable
    sc_signal<bool> done_internal;       // Internal signal for done
    sc_signal<sc_uint<4>> out_internal;  // Internal signal for GCD output
    sc_signal<bool> stop_internal;       // Internal signal for stop

    void interact();
    void copy();
};

void GCD_USER::interact() {
    static enum { IDLE, INPUT, COMPUTE, OUTPUT } state = IDLE;
    static sc_uint<4> x, y;

    switch (state) {
        case IDLE:
            std::cout << "Enter two numbers (0-15) for GCD calculation:" << std::endl;
            std::cout << "First number: ";
            int input_x;
            std::cin >> input_x;
            std::cout << "Second number: ";
            int input_y;
            std::cin >> input_y;

            // Validate and clamp inputs to 4-bit range
            x = (input_x >= 0 && input_x <= 15) ? input_x : 0;
            y = (input_y >= 0 && input_y <= 15) ? input_y : 0;

            x_internal.write(x);    // Write inputs to internal signals
            y_internal.write(y);
            en_internal.write(true); // Enable GCD computation
            stop_internal.write(false); // Initialize stop signal
            state = INPUT;
            break;

        case INPUT:
            // Wait one cycle to ensure inputs are latched
            state = COMPUTE;
            break;

        case COMPUTE:
            if (done_internal.read()) {
                // GCD computation is complete
                sc_uint<4> result = out_internal.read();
                std::cout << "GCD of " << x << " and " << y << " is: " << result << std::endl;
                en_internal.write(false); // Disable GCD
                state = OUTPUT;
            }
            break;

        case OUTPUT:
            std::cout << "Stop simulation? (y/n): ";
            char stop_choice;
            std::cin >> stop_choice;
            if (stop_choice == 'y' || stop_choice == 'Y') {
                stop_internal.write(true); // Signal to stop
                sc_stop(); // Stop simulation
            } else {
                stop_internal.write(false); // Continue
                state = IDLE; // Loop back for new inputs
            }
            break;
    }
}

void GCD_USER::copy() {
    x_sig.write(x_internal.read());
    y_sig.write(y_internal.read());
    en_sig.write(en_internal.read());
    done_sig.write(done_internal.read());
    out_sig.write(out_internal.read());
    stop_sig.write(stop_internal.read());
}

#endif