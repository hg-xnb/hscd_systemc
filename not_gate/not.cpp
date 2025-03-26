#include <systemc.h>

SC_MODULE(NOT_GATE){
    sc_in<bool> IN;                     /// input port
    sc_out<bool> OUT;                   /// output port

    SC_CTOR(NOT_GATE)
    {
        SC_THREAD(NOT_GATE_THREAD);     /// the thread
        sensitive << IN;
    }

    void NOT_GATE_THREAD(void) {
        while (true) {
            OUT.write(!IN.read());
            wait();
        }
    }
};

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> A, Y;   /// make signal A and Y

    NOT_GATE not1("NOT1");  /// make an instance if NOTGATE (`not1` is the object's name, `NOT1` is the name of instance)
    not1.IN(A); /// connect A to IN-pin of not1
    not1.OUT(Y);    /// connect Y to OUT-pin of not1

    sc_start(1, SC_NS); /// start simulation
    
    sc_trace_file *tf = sc_create_vcd_trace_file("waveform"); /// make vcd-writer-object to write to vcd file
    tf->set_time_unit(1, SC_NS);    /// the time unit

    sc_trace(tf, A, "INPUT");   /// set the signal to trace 
    sc_trace(tf, Y, "OUTPUT");  /// set the signal to trace

    A.write(0);
    sc_start(1, SC_NS);
    cout << "A = " << A.read() << ", Y = " << Y.read() << endl;

    A.write(1);
    sc_start(1, SC_NS);
    cout << "A = " << A.read() << ", Y = " << Y.read() << endl;

    /// Close trace file after sim
    sc_close_vcd_trace_file(tf);

    return 0;
}
