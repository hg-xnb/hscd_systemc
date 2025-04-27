#ifndef GCD_CTL_H
#define GCD_CTL_H

#include <systemc.h>

#define LOW 0x0
#define HIGH 0x1
#define MAX_CYCLE_COUNT 0x11
#define WIDTH 4

SC_MODULE(GCD_CTL)
{
    enum RegSelection
    {
        FROM_INPUT,
        FROM_SUBTRACTOR
    };
    enum RegOutputSelection
    {
        FROM_X,
        FROM_Y
    };

    /// Inputs
    sc_in<bool> CLK;
    sc_in<bool> EN;

    /// Condition
    sc_in<bool> X_LT_Y, X_GT_Y, X_EQ_Y;
    /*
    X_LT_Y, X_GT_Y, X_EQ_Y
    0       0       0          ->   INVALID     -> IDLE
    0       0       1          ->   BUSY
    0       1       0          ->   BUSY
    0       1       1          ->   INVALID     -> IDLE
    1       0       0          ->   BUSY
    1       0       1          ->   INVALID     -> IDLE
    1       1       0          ->   INVALID     -> IDLE
    1       1       0          ->   INVALID     -> IDLE
    */

    /// Control signals for the registers
    sc_out<bool> REG_X_SEL, REG_X_CLK, REG_X_LOAD, REG_X_RESET;
    sc_out<bool> REG_Y_SEL, REG_Y_CLK, REG_Y_LOAD, REG_Y_RESET;
    /// Output register controls
    sc_out<bool> REG_O_SEL, REG_O_CLK, REG_O_LOAD, REG_O_RESET;
    /// Ouput EN/DIS-able SUBTRACTOR module
    sc_out<bool> X_SELF_SUBTRACTION, Y_SELF_SUBTRACTION;

    /// Internal state
    sc_uint<WIDTH+1> _cycle_count;
    
    void resetState()
    {
        REG_X_SEL.write(LOW);
        REG_X_CLK.write(LOW);
        REG_X_LOAD.write(LOW);
        REG_X_RESET.write(HIGH);

        REG_Y_SEL.write(LOW);
        REG_Y_CLK.write(LOW);
        REG_Y_LOAD.write(LOW);
        REG_Y_RESET.write(HIGH);

        REG_O_SEL.write(LOW);
        REG_O_CLK.write(LOW);
        REG_O_LOAD.write(LOW);
        REG_X_RESET.write(HIGH);

        X_SELF_SUBTRACTION.write(LOW);
        Y_SELF_SUBTRACTION.write(LOW);

        _cycle_count = 0;
    }

    void process()
    {
        /// Default outputs
        resetState();
        _cycle_count = 0;
        /// Main loop
        while (true)
        {
            wait();
            if (EN.read() == HIGH)
            {
                REG_X_RESET.write(LOW);
                REG_Y_RESET.write(LOW);
                /// read X & Y
                REG_X_SEL.write(FROM_INPUT);
                REG_Y_SEL.write(FROM_INPUT);
                REG_X_LOAD.write(HIGH);
                REG_Y_LOAD.write(HIGH);
                REG_X_CLK.write(HIGH);
                REG_Y_CLK.write(HIGH);
                wait();
                REG_X_SEL.write(FROM_SUBTRACTOR);
                REG_Y_SEL.write(FROM_SUBTRACTOR);
                REG_X_LOAD.write(LOW);
                REG_Y_LOAD.write(LOW);
                REG_X_CLK.write(LOW);
                REG_Y_CLK.write(LOW);

                /// Find GCD by Self-Subtraction
                while (0x1)
                {
                    if(++_cycle_count >= (MAX_CYCLE_COUNT - 1)){
                        resetState();
                        wait();
                        REG_O_LOAD.write(HIGH);
                        REG_O_CLK.write(HIGH);
                        wait();
                        REG_O_LOAD.write(LOW);
                        REG_O_CLK.write(LOW);
                        break;
                    }
                    /// Read the comparison result between X and Y
                    auto gt = X_GT_Y.read();
                    auto lt = X_LT_Y.read();
                    auto eq = X_EQ_Y.read();
                    /// CASE-0: X > Y
                    if ((gt == HIGH) && (lt == LOW) && (eq == LOW))
                    {
                        X_SELF_SUBTRACTION.write(HIGH);
                        wait();
                        X_SELF_SUBTRACTION.write(LOW);
                        REG_X_LOAD.write(HIGH);
                        REG_X_CLK.write(HIGH);
                        wait();
                        REG_X_LOAD.write(LOW);
                        REG_X_CLK.write(LOW);
                    }
                    else
                        /// CASE-1: X < Y
                        if ((gt == LOW) && (lt == HIGH) && (eq == LOW))
                        {
                            /// processing for CASE-1
                            Y_SELF_SUBTRACTION.write(HIGH);
                            wait();
                            Y_SELF_SUBTRACTION.write(LOW);
                            REG_Y_LOAD.write(HIGH);
                            REG_Y_CLK.write(HIGH);
                            wait();
                            REG_Y_LOAD.write(LOW);
                            REG_Y_CLK.write(LOW);
                        }
                        else
                            /// CASE-2: X == Y
                            if ((gt == LOW) && (lt == LOW) && (eq == HIGH))
                            {
                                REG_O_LOAD.write(HIGH);
                                REG_O_CLK.write(HIGH);
                                REG_X_SEL.write(FROM_INPUT);
                                REG_Y_SEL.write(FROM_INPUT);
                                wait();
                                REG_O_LOAD.write(LOW);
                                REG_O_CLK.write(LOW);
                                _cycle_count=0;
                                break;
                            }
                    wait();
                }
            }
            else
            {
                resetState();
            }
        }
    }

    SC_CTOR(GCD_CTL) : CLK("CLK"),
                       X_LT_Y("X_LT_Y"), X_GT_Y("X_GT_Y"), X_EQ_Y("X_EQ_Y"),
                       EN("EN"),
                       REG_X_SEL("REG_X_SEL"), REG_Y_SEL("REG_Y_SEL"),
                       REG_X_CLK("REG_X_CLK"), REG_X_LOAD("REG_X_LOAD"),
                       REG_Y_CLK("REG_Y_CLK"), REG_Y_LOAD("REG_Y_LOAD"),
                       REG_O_CLK("REG_O_CLK"), REG_O_LOAD("REG_O_LOAD"),
                       REG_X_RESET("REG_X_RESET"), REG_Y_RESET("REG_Y_RESET"),
                       REG_O_RESET("REG_O_RESET"),
                       X_SELF_SUBTRACTION("X_SELF_SUBTRACTION"),
                       Y_SELF_SUBTRACTION("Y_SELF_SUBTRACTION")
    {
        SC_CTHREAD(process, CLK.pos());
    }
};

#endif // GCD_CTL_H
