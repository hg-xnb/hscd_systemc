#ifndef __MUX_4_1_4BIT_H__
#define __MUX_4_1_4BIT_H__

#include <systemc.h>
#include "../MUX_4_1/mux_4_1.h"

#ifndef WIDTH
#define WIDTH 4
#endif
#ifndef rep
#define rep(var, start, stop) for (int var = (start); var < (stop); ++var)
#endif

#define DEBUG false

SC_MODULE(MUX_4_1_4BIT)
{
    sc_in<bool> EN;
    sc_in<sc_uint<2>> SEL;
    sc_vector<sc_in<sc_uint<WIDTH>>> IN;
    sc_out<sc_uint<WIDTH>> OUT;

    sc_signal<bool> _en;
    sc_signal<sc_uint<2>> _sel;
    sc_vector<sc_signal<bool>> _bit_in[WIDTH];
    sc_vector<sc_signal<bool>> _bit_out;

    MUX_4_1 *mux[WIDTH];

    void combine_bit_in()
    {
        rep(bit, 0, WIDTH)
        {
            #if DEBUG == true
                cout << "Inside MUX: _bit_in[" << bit << "] = ";
            #endif
            rep(i, 0, 4)
            {
                _bit_in[bit][i].write(IN[i].read()[bit]);
                #if DEBUG == true
                    cout << _bit_in[bit][i].read();
                #endif
            }
            #if DEBUG == true
                cout << '\n';
            #endif
        }
    }

    void combine_bit_out()
    {
        sc_uint<WIDTH> result = 0;
        rep(bit, 0, WIDTH)
        {
            result[bit] = _bit_out[bit].read();
        }
        #if DEBUG == true
            cout << "Inside MUX: OUT: " << result << '\n';
        #endif
        OUT.write(result);
    }

    void process_inputs()
    {
        #if DEBUG == true
            cout << "Inside MUX: EN: " << EN.read() << " SEL: " << SEL.read() << '\n';
        #endif
        _sel.write(SEL.read());
        _en.write(EN.read());
        combine_bit_in();
    }

    void process_outputs()
    {
        combine_bit_out();
        #if DEBUG == true
            cout << '\n';
        #endif
    }

    SC_CTOR(MUX_4_1_4BIT)
        : IN("IN", 4), EN("EN"), SEL("SEL"), OUT("OUT"),
          _bit_out("_bit_out", WIDTH), _en("_en"), _sel("_sel")
    {
        rep(bit, 0, WIDTH)
        {
            _bit_in[bit].init(4);
            std::string name = "mux_" + std::to_string(bit);
            mux[bit] = new MUX_4_1(name.c_str());
            rep(i, 0, 4)
            {
                mux[bit]->IN[i](_bit_in[bit][i]);
            }
            mux[bit]->EN(_en);
            mux[bit]->SEL(_sel);
            mux[bit]->OUT(_bit_out[bit]);
        }

        SC_METHOD(process_inputs);
        sensitive << SEL << EN;
        rep(i, 0, 4) sensitive << IN[i];

        SC_METHOD(process_outputs);
        rep(bit, 0, WIDTH) sensitive << _bit_out[bit];
    }

    ~MUX_4_1_4BIT()
    {
        for (int i = 0; i < WIDTH; ++i)
        {
            delete mux[i];
        }
    }
};

#endif