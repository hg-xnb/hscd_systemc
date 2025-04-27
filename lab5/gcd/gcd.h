#ifndef __GCD_H__
#define __GCD_H__
#include <systemc.h>

#include "../mux84/mux84.h"
#include "../reg4b/reg4b.h"
#include "../cmp4b/cmp4b.h"
#include "../sub4b/sub4b.h"
#include "../gcd_ctl/gcd_ctl.h"

#define WIDTH 4
#ifndef HIGH
    #define HIGH 0x1
#endif
#ifndef LOW
    #define LOW 0x0
#endif

SC_MODULE(GCD)
{
    /// Inputs
    sc_in<sc_uint<WIDTH>> X, Y;
    sc_in<bool> EN, CLK;
    /// Outputs
    sc_out<bool> DONE;
    sc_out<sc_uint<WIDTH>> OUT;
    /// outputs debug
    sc_out<bool> _X_LT_Y, _X_GT_Y, _X_EQ_Y,
    _REG_X_SEL, _REG_X_CLK, _REG_X_LOAD, _REG_X_RESET, 
    _REG_Y_SEL, _REG_Y_CLK, _REG_Y_LOAD, _REG_Y_RESET, 
    _REG_O_SEL, _REG_O_CLK, _REG_O_LOAD, _REG_O_RESET, 
    _X_SELF_SUBTRACTION, _Y_SELF_SUBTRACTION;
    sc_out<sc_uint<WIDTH>> _internalX, _internalY;
    /// Internal Signals (for control)
    sc_signal<bool> X_LT_Y, X_GT_Y, X_EQ_Y,
        REG_X_SEL, REG_X_CLK, REG_X_LOAD, REG_X_RESET, 
        REG_Y_SEL, REG_Y_CLK, REG_Y_LOAD, REG_Y_RESET, 
        REG_O_SEL, REG_O_CLK, REG_O_LOAD, REG_O_RESET, 
        X_SELF_SUBTRACTION, Y_SELF_SUBTRACTION;
    /// Internal Signals (for data exchange)
    sc_signal<sc_uint<WIDTH>> subForX_OUT, 
        subForY_OUT, internalX, internalY,
        muxForX_OUT, muxForY_OUT, cmp_OUT;

    /// Components
    MUX84 * muxForX, * muxForY;
    REG4B * regForX, * regForY, * regForOut;
    CMP4B * cmp;
    SUB4B * subForX, * subForY;
    GCD_CTL * gcd_ctl;

    void copySignal(){
        DONE.write(REG_O_LOAD.read());
    };

    void copySignalForDebug(){
        _X_LT_Y.write(X_LT_Y.read());
        _X_GT_Y.write(X_GT_Y.read());
        _X_EQ_Y.write(X_EQ_Y.read());
    
        _REG_X_SEL.write(REG_X_SEL.read());
        _REG_X_CLK.write(REG_X_CLK.read());
        _REG_X_LOAD.write(REG_X_LOAD.read());
        _REG_X_RESET.write(REG_X_RESET.read());
    
        _REG_Y_SEL.write(REG_Y_SEL.read());
        _REG_Y_CLK.write(REG_Y_CLK.read());
        _REG_Y_LOAD.write(REG_Y_LOAD.read());
        _REG_Y_RESET.write(REG_Y_RESET.read());
    
        _REG_O_SEL.write(REG_O_SEL.read());
        _REG_O_CLK.write(REG_O_CLK.read());
        _REG_O_LOAD.write(REG_O_LOAD.read());
        _REG_O_RESET.write(REG_O_RESET.read());
    
        _X_SELF_SUBTRACTION.write(X_SELF_SUBTRACTION.read());
        _Y_SELF_SUBTRACTION.write(Y_SELF_SUBTRACTION.read());

        _internalX.write(internalX.read());
        _internalY.write(internalY.read());
    }

    SC_CTOR(GCD):
    CLK("CLK"), X("X"), Y("Y"), EN("EN"), DONE("DONE"),
    _X_EQ_Y("X_EQ_Y"), _X_GT_Y("X_GT_Y"), _X_LT_Y("X_LT_Y"),
    _REG_X_SEL("REG_X_SEL"), _REG_X_CLK("REG_X_CLK"), _REG_X_LOAD("REG_X_LOAD"), _REG_X_RESET("REG_X_RESET"),
    _REG_Y_SEL("REG_Y_SEL"), _REG_Y_CLK("REG_Y_CLK"), _REG_Y_LOAD("REG_Y_LOAD"), _REG_Y_RESET("REG_Y_RESET"),
    _REG_O_SEL("REG_O_SEL"), _REG_O_CLK("REG_O_CLK"), _REG_O_LOAD("REG_O_LOAD"), _REG_O_RESET("REG_O_RESET"),
    _X_SELF_SUBTRACTION("X_SELF_SUBTRACTION"), _Y_SELF_SUBTRACTION("Y_SELF_SUBTRACTION"), _internalX("_internalX"), _internalY("_internalY")
    {
        muxForX     = new MUX84("muxForX");
        muxForY     = new MUX84("muxForY");
        regForX     = new REG4B("regForX");
        regForY     = new REG4B("regForY");
        regForOut   = new REG4B("regForOut");
        subForX     = new SUB4B("subForX");
        subForY     = new SUB4B("subForY");
        cmp         = new CMP4B("cmp");
        gcd_ctl     = new GCD_CTL("gcd_ctl");

        /// muxForX
        muxForX->SEL(REG_X_SEL);
        muxForX->IN0(X);
        muxForX->IN1(subForX_OUT);
        muxForX->OUT(muxForX_OUT);
        /// muxForY
        muxForY->IN0(Y);
        muxForY->SEL(REG_Y_SEL);
        muxForY->IN1(subForY_OUT);
        muxForY->OUT(muxForY_OUT);
        /// regX
        regForX->IN(muxForX_OUT);
        regForX->CLK(REG_X_CLK);
        regForX->LOAD(REG_X_LOAD);
        regForX->OUT(internalX);
        regForX->RESET(REG_X_RESET);
        /// regY
        regForY->IN(muxForY_OUT);
        regForY->CLK(REG_Y_CLK);
        regForY->LOAD(REG_Y_LOAD);
        regForY->OUT(internalY);
        regForY->RESET(REG_Y_RESET);
        /// cmp
        cmp->X(internalX);
        cmp->Y(internalY);
        cmp->X_GT_Y(X_GT_Y);
        cmp->X_LT_Y(X_LT_Y);
        cmp->X_EQ_Y(X_EQ_Y);
        /// subX
        subForX->X(internalX);
        subForX->Y(internalY);
        subForX->OUT(subForX_OUT);
        subForX->CLK(X_SELF_SUBTRACTION);
        /// subY
        subForY->X(internalX);
        subForY->Y(internalY);
        subForY->OUT(subForY_OUT);
        subForY->CLK(Y_SELF_SUBTRACTION);
        /// regOut
        regForOut->IN(internalY);
        regForOut->CLK(REG_O_CLK);
        regForOut->LOAD(REG_O_LOAD);
        regForOut->RESET(REG_O_RESET);
        regForOut->OUT(OUT);
        /// controller
        gcd_ctl->EN(EN);
        gcd_ctl->CLK(CLK);
        gcd_ctl->X_LT_Y(X_LT_Y);
        gcd_ctl->X_GT_Y(X_GT_Y);
        gcd_ctl->X_EQ_Y(X_EQ_Y);
        gcd_ctl->REG_X_SEL(REG_X_SEL);
        gcd_ctl->REG_X_CLK(REG_X_CLK);
        gcd_ctl->REG_X_LOAD(REG_X_LOAD);
        gcd_ctl->REG_X_RESET(REG_X_RESET);
        gcd_ctl->REG_Y_SEL(REG_Y_SEL);
        gcd_ctl->REG_Y_CLK(REG_Y_CLK);
        gcd_ctl->REG_Y_LOAD(REG_Y_LOAD);
        gcd_ctl->REG_Y_RESET(REG_Y_RESET);
        gcd_ctl->REG_O_SEL(REG_O_SEL);
        gcd_ctl->REG_O_CLK(REG_O_CLK);
        gcd_ctl->REG_O_LOAD(REG_O_LOAD);
        gcd_ctl->REG_O_RESET(REG_O_RESET);
        gcd_ctl->X_SELF_SUBTRACTION(X_SELF_SUBTRACTION);
        gcd_ctl->Y_SELF_SUBTRACTION(Y_SELF_SUBTRACTION);

        SC_METHOD(copySignal);
        sensitive << CLK << X << Y << EN << REG_O_LOAD;

        SC_METHOD(copySignalForDebug);
        sensitive 
        << _X_EQ_Y << _X_GT_Y << _X_LT_Y
        << _REG_X_SEL << _REG_X_CLK << _REG_X_LOAD << _REG_X_RESET
        << _REG_Y_SEL << _REG_Y_CLK << _REG_Y_LOAD << _REG_Y_RESET
        << _REG_O_SEL << _REG_O_CLK << _REG_O_LOAD << _REG_O_RESET
        << _X_SELF_SUBTRACTION << _Y_SELF_SUBTRACTION
        << _internalX << _internalY << X_LT_Y << X_GT_Y << X_EQ_Y <<
        REG_X_SEL << REG_X_CLK << REG_X_LOAD << REG_X_RESET << 
        REG_Y_SEL << REG_Y_CLK << REG_Y_LOAD << REG_Y_RESET << 
        REG_O_SEL << REG_O_CLK << REG_O_LOAD << REG_O_RESET << 
        X_SELF_SUBTRACTION << Y_SELF_SUBTRACTION;
    }
};

#endif