#ifndef __ALU_4BIT_H__
#define __ALU_4BIT_H__

#include <systemc.h>

#include "../UTILS_4BIT/MUX_8_1/MUX_8_1.h"
#include "../UTILS_4BIT/SIGCTL_4BIT/SIGCTL_4BIT.h"

#include "../UTILS_4BIT/BITWISE_4BIT/AND_4BIT/AND_4BIT.h"
#include "../UTILS_4BIT/BITWISE_4BIT/NAND_4BIT/NAND_4BIT.h"
#include "../UTILS_4BIT/BITWISE_4BIT/OR_4BIT/OR_4BIT.h"
#include "../UTILS_4BIT/BITWISE_4BIT/XOR_4BIT/XOR_4BIT.h"

#include "../UTILS_4BIT/MATH_4BIT/FA_4BIT/FA_4BIT.h"
#include "../UTILS_4BIT/MATH_4BIT/FS_4BIT/FS_4BIT.h"

#ifndef WIDTH
#define WIDTH 4
#endif
#ifndef OUT_SEL_WIDTH
#define OUT_SEL_WIDTH 3
#endif
#ifndef rep
#define rep(var, start, stop) for (int var = (start); var < (stop); --var)
#endif
#ifndef rev
#define rev(var, start, stop) for (int var = (start); var > (stop); --var)
#endif

#ifndef sigRead
#define sigRead(SIG) ((SIG).read())
#endif
#ifndef sigWrite
#define sigWrite(SIG, VAL) (SIG).write(VAL)
#endif

#ifndef OUT_CODE
#define OUT_CODE
enum OUT_CODE
{
    FA = 0,
    FS,
    AND,
    NAND,
    OR,
    XOR,
    FULL_1,
    FULL_0
};
#endif

#ifndef ALU_CODE
#define ALU_CODE
enum ALU_CODE
{
    PASS_A = 0,
    INCR_A,
    ADDITION,
    ADD_W_CARRY,
    SUBTRACTION,
    SUB_W_BORROW,
    DECR_A,
    BW_AND,
    BW_NAND,
    BW_OR,
    BW_XOR,
    RESERVED
};
#endif

SC_MODULE(ALU_4BIT)
{
    sc_in<bool> EN, RST;
    sc_in<sc_uint<WIDTH>> A, B, SEL;
    sc_out<bool> C_OUT, B_OUT;
    sc_out<sc_uint<WIDTH>> OUT;

    FA_4BIT *fa_4bit;
    FS_4BIT *fs_4bit;
    AND_4BIT *and_4bit;
    NAND_4BIT *nand_4bit;
    OR_4BIT *or_4bit;
    XOR_4BIT *xor_4bit;
    MUX_8_1 *mux_8_1;
    SIGCTL_4BIT *sigctl;

    sc_signal<bool> FS_RST, FA_RST, AND_RST, NAND_RST, OR_RST, XOR_RST;
    // Clock outputs
    sc_signal<bool> FS_EN, FA_EN, AND_EN, NAND_EN, OR_EN, XOR_EN, OUT_MUX_EN;
    // Borrow/Carry input outputs
    sc_signal<bool> FS_B_IN, FA_C_IN, FA_C_OUT, FS_B_OUT;
    // Data A outputs
    sc_signal<sc_uint<WIDTH>> FS_A, FA_A, AND_A, NAND_A, OR_A, XOR_A;
    // Data B outputs
    sc_signal<sc_uint<WIDTH>> FS_B, FA_B, AND_B, NAND_B, OR_B, XOR_B;
    // Output selects
    sc_signal<sc_uint<OUT_SEL_WIDTH>> OUT_MUX_SEL;
    // Output results
    sc_signal<sc_uint<WIDTH>> FS_OUT, FA_OUT, AND_OUT, NAND_OUT, OR_OUT, XOR_OUT,
        MUX_OUT, FULL_1, FULL_0;
    //

    void output_update() {
        OUT.write(MUX_OUT.read());
        C_OUT.write(FA_C_OUT.read());
        B_OUT.write(FS_B_OUT.read());
    }

    SC_CTOR(ALU_4BIT) : // Ports
                        EN("EN"), RST("RST"), SEL("SEL"), A("A"),
                        B("B"), OUT("OUT"), C_OUT("C_OUT"),
                        B_OUT("B_OUT"), FULL_1("FULL_1"), FULL_0("FULL_0"),

                        // RST signals
                        FS_RST("FS_RST"), FA_RST("FA_RST"), AND_RST("AND_RST"),
                        NAND_RST("NAND_RST"), OR_RST("OR_RST"), XOR_RST("XOR_RST"),

                        // EN signals
                        FS_EN("FS_EN"), FA_EN("FA_EN"), AND_EN("AND_EN"),
                        NAND_EN("NAND_EN"), OR_EN("OR_EN"), XOR_EN("XOR_EN"),
                        OUT_MUX_EN("OUT_MUX_EN"),

                        // Carry/Borrow
                        FS_B_IN("FS_B_IN"), FA_C_IN("FA_C_IN"),

                        // A-side data signals
                        FS_A("FS_A"), FA_A("FA_A"), AND_A("AND_A"),
                        NAND_A("NAND_A"), OR_A("OR_A"), XOR_A("XOR_A"),

                        // B-side data signals
                        FS_B("FS_B"), FA_B("FA_B"), AND_B("AND_B"),
                        NAND_B("NAND_B"), OR_B("OR_B"), XOR_B("XOR_B"),

                        // Output select
                        OUT_MUX_SEL("OUT_MUX_SEL"),

                        // OTHER INPUTS/OUTPUTS
                        FS_OUT("FS_OUT"), FA_OUT("FA_OUT"), AND_OUT("AND_OUT"), NAND_OUT("NAND_OUT"), OR_OUT("OR_OUT"), XOR_OUT("XOR_OUT"), MUX_OUT("MUX_OUT"), FA_C_OUT("FA_C_OUT")
    {
        /// set consts signal
        FULL_1.write(0xF);
        FULL_0.write(0x0);

        /// Instantiate modules
        fa_4bit = new FA_4BIT("FA_4BIT");
        fs_4bit = new FS_4BIT("FS_4BIT");
        and_4bit = new AND_4BIT("AND_4BIT");
        nand_4bit = new NAND_4BIT("NAND_4BIT");
        or_4bit = new OR_4BIT("OR_4BIT");
        xor_4bit = new XOR_4BIT("XOR_4BIT");
        mux_8_1 = new MUX_8_1("MUX_8_1");
        sigctl = new SIGCTL_4BIT("SIGCTL_4BIT");

        // Connect signals and components here...
        fa_4bit->EN(FA_EN);
        fa_4bit->RST(FA_RST);
        fa_4bit->A(FA_A);
        fa_4bit->B(FA_B);
        fa_4bit->CARRY_IN(FA_C_IN);
        fa_4bit->CARRY_OUT(FA_C_OUT);
        fa_4bit->OUT(FA_OUT);

        fs_4bit->EN(FS_EN);
        fs_4bit->RST(FS_RST);
        fs_4bit->A(FS_A);
        fs_4bit->B(FS_B);
        fs_4bit->BORROW_IN(FS_B_IN);
        fs_4bit->BORROW_OUT(FS_B_OUT);
        fs_4bit->OUT(FS_OUT);

        and_4bit->EN(AND_EN);
        and_4bit->RST(AND_RST);
        and_4bit->A(AND_A);
        and_4bit->B(AND_B);
        and_4bit->OUT(AND_OUT);

        nand_4bit->EN(NAND_EN);
        nand_4bit->RST(NAND_RST);
        nand_4bit->A(NAND_A);
        nand_4bit->B(NAND_B);
        nand_4bit->OUT(NAND_OUT);

        or_4bit->EN(OR_EN);
        or_4bit->RST(OR_RST);
        or_4bit->A(OR_A);
        or_4bit->B(OR_B);
        or_4bit->OUT(OR_OUT);

        xor_4bit->EN(XOR_EN);
        xor_4bit->RST(XOR_RST);
        xor_4bit->A(XOR_A);
        xor_4bit->B(XOR_B);
        xor_4bit->OUT(XOR_OUT);

        mux_8_1->IN[0](FA_OUT);
        mux_8_1->IN[1](FS_OUT);
        mux_8_1->IN[2](AND_OUT);
        mux_8_1->IN[3](NAND_OUT);
        mux_8_1->IN[4](OR_OUT);
        mux_8_1->IN[5](XOR_OUT);
        mux_8_1->IN[6](FULL_1);
        mux_8_1->IN[7](FULL_0);
        mux_8_1->SEL(OUT_MUX_SEL);
        mux_8_1->EN(OUT_MUX_EN);
        mux_8_1->OUT(MUX_OUT);

        sigctl->EN(EN);
        sigctl->RST(RST);
        sigctl->A(A);
        sigctl->B(B);
        sigctl->SELECT(SEL);
        sigctl->FS_RST(FS_RST);
        sigctl->FA_RST(FA_RST);
        sigctl->AND_RST(AND_RST);
        sigctl->NAND_RST(NAND_RST);
        sigctl->OR_RST(OR_RST);
        sigctl->XOR_RST(XOR_RST);
        sigctl->FS_EN(FS_EN);
        sigctl->FA_EN(FA_EN);
        sigctl->AND_EN(AND_EN);
        sigctl->NAND_EN(NAND_EN);
        sigctl->OR_EN(OR_EN);
        sigctl->XOR_EN(XOR_EN);
        sigctl->OUT_MUX_EN(OUT_MUX_EN);
        sigctl->FS_B_IN(FS_B_IN);
        sigctl->FA_C_IN(FA_C_IN);
        sigctl->FS_A(FS_A);
        sigctl->FA_A(FA_A);
        sigctl->AND_A(AND_A);
        sigctl->NAND_A(NAND_A);
        sigctl->OR_A(OR_A);
        sigctl->XOR_A(XOR_A);
        sigctl->FS_B(FS_B);
        sigctl->FA_B(FA_B);
        sigctl->AND_B(AND_B);
        sigctl->NAND_B(NAND_B);
        sigctl->OR_B(OR_B);
        sigctl->XOR_B(XOR_B);
        sigctl->OUT_MUX_SEL(OUT_MUX_SEL);

        SC_METHOD(output_update);
        sensitive << MUX_OUT << FA_C_OUT << FS_B_OUT;
    }

    ~ALU_4BIT(){
        delete fa_4bit;
        delete fs_4bit;
        delete and_4bit;
        delete nand_4bit;
        delete or_4bit;
        delete xor_4bit;
        delete mux_8_1;
        delete sigctl;
    }
    

};

#endif
