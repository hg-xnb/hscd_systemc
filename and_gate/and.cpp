#include "systemc.h"

SC_MODULE(AND_GATE){
    sc_in<bool> IN_A;                     /// input port
    sc_in<bool> IN_B;                     /// input port
    sc_out<bool> OUT;                     /// output port
    
    /// set-up init state
    SC_CTOR(AND_GATE){
        SC_THREAD(main_thread);
        sensitive << IN_A << IN_B;
    }

    /// set-up the behavior
    void main_thread(void){
        while(true){
            OUT.write(IN_A.read() && IN_B.read());
            wait();
        }
    }
};

int sc_main(int arg, char* argv[]){
    sc_signal<bool> A, B, Y;
    AND_GATE and0("AND 0");

    and0.IN_A(A);
    and0.IN_B(B);
    and0.OUT(Y);

    sc_trace_file* trace_file = sc_create_vcd_trace_file("and_waveform");
    trace_file->set_time_unit(1, SC_NS);
    sc_trace(trace_file, A, "A");
    sc_trace(trace_file, B, "B");
    sc_trace(trace_file, Y, "Y");

    
    cout << "Start sim...\n";
    
    for(uint8_t n = 0; n < 4; ++n){
        A.write(bool(n&0x1)); B.write(bool(n&0x2));
        sc_start(5, SC_NS);
        cout    << "A: " << A.read() << " "
                << "B: " << B.read() << " "
                << "Y: " << Y.read() << '\n';
    }

    cout << "\nFinished!\n";
    sc_close_vcd_trace_file(trace_file);

    return 0;
}