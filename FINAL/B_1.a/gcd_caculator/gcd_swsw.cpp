#include <systemc.h>
#include <iostream>

// Module 1: UserInput (asks for two numbers and sends them to GCDCalculator)
SC_MODULE(UserInput) {
    sc_in<bool> clk;                     // Clock input for synchronization
    sc_out<sc_uint<4>> x_out, y_out;     // Outputs for the two numbers
    sc_out<bool> req;                    // Request signal (handshake)
    sc_in<bool> ack;                     // Acknowledge signal (handshake)

    SC_CTOR(UserInput) {
        SC_THREAD(input_process);
        sensitive << clk.pos();
    }

    void input_process() {
        while (true) {
            // Wait for positive clock edge
            wait();

            // Prompt user for input
            int x, y;
            std::cout << "Enter two numbers (0-15) for GCD calculation:" << std::endl;
            std::cout << "First number: ";
            std::cin >> x;
            std::cout << "Second number: ";
            std::cin >> y;

            // Validate and clamp inputs to 4-bit range
            x = (x >= 0 && x <= 15) ? x : 0;
            y = (y >= 0 && y <= 15) ? y : 0;

            // Send numbers and assert request
            x_out.write(x);
            y_out.write(y);
            req.write(true);

            // Wait for acknowledge from GCDCalculator (numbers received)
            do {
                wait();
            } while (!ack.read());

            // Clear request after acknowledge
            req.write(false);

            // Wait for GCD computation to complete (second ack)
            do {
                wait();
            } while (!ack.read());

            // Wait for ack to go low (computation fully done)
            do {
                wait();
            } while (ack.read());

            // Ask if user wants to continue
            std::cout << "Continue with new numbers? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice != 'y' && choice != 'Y') {
                sc_stop(); // Stop simulation
            }
        }
    }
};

// Module 2: GCDCalculator (computes GCD and interfaces with UserInput)
SC_MODULE(GCDCalculator) {
    sc_in<bool> clk;                     // Clock input for synchronization
    sc_in<sc_uint<4>> x_in, y_in;        // Inputs for the two numbers
    sc_in<bool> req;                     // Request signal (handshake)
    sc_out<bool> ack;                    // Acknowledge signal (handshake)
    sc_out<sc_uint<4>> gcd_out;          // Output for GCD result

    SC_CTOR(GCDCalculator) {
        SC_THREAD(calc_process);
        sensitive << clk.pos();
    }

    void calc_process() {
        while (true) {
            // Wait for request from UserInput
            do {
                wait();
            } while (!req.read());

            // Read inputs and assert acknowledge
            sc_uint<4> x = x_in.read();
            sc_uint<4> y = y_in.read();
            ack.write(true);
            wait(); // Allow UserInput to see ack

            // Clear acknowledge to proceed with computation
            ack.write(false);

            // Compute GCD using Euclidean algorithm
            sc_uint<4> a = x, b = y;
            while (b != 0) {
                sc_uint<4> temp = b;
                b = a % b;
                a = temp;
                wait(); // Simulate cycle-by-cycle computation
            }
            sc_uint<4> result = (a > 0) ? a.to_uint() : (x == 0 && y == 0 ? 0 : 1); // Handle GCD(0,0)

            // Output result
            gcd_out.write(result);
            std::cout << "GCD of " << x << " and " << y << " is: " << result << std::endl;

            // Signal completion by asserting ack again
            ack.write(true);
            wait();

            // Clear ack to allow new computation
            ack.write(false);
        }
    }
};

// Main function to instantiate and run the simulation
int sc_main(int argc, char* argv[]) {
    // Create clock
    sc_clock clk("clk", 10, SC_NS, 0.5); // 10ns period, 50% duty cycle

    // Create signals
    sc_signal<sc_uint<4>> x_sig, y_sig, gcd_sig;
    sc_signal<bool> req_sig, ack_sig;

    // Instantiate modules
    UserInput user("user");
    user.clk(clk);
    user.x_out(x_sig);
    user.y_out(y_sig);
    user.req(req_sig);
    user.ack(ack_sig);

    GCDCalculator calc("calc");
    calc.clk(clk);
    calc.x_in(x_sig);
    calc.y_in(y_sig);
    calc.req(req_sig);
    calc.ack(ack_sig);
    calc.gcd_out(gcd_sig);

    // Set up trace file
    sc_trace_file* trace_file = sc_create_vcd_trace_file("gcd_sw_sw_trace");
    trace_file->set_time_unit(1, SC_NS);

    // Trace signals
    sc_trace(trace_file, clk, "clk");
    sc_trace(trace_file, x_sig, "x");
    sc_trace(trace_file, y_sig, "y");
    sc_trace(trace_file, req_sig, "req");
    sc_trace(trace_file, ack_sig, "ack");
    sc_trace(trace_file, gcd_sig, "gcd");

    // Print start message
    std::cout << "Starting GCD SW/SW simulation..." << std::endl;

    // Run simulation
    sc_start();

    // Close trace file
    std::cout << "Simulation finished!" << std::endl;
    sc_close_vcd_trace_file(trace_file);
    return 0;
}