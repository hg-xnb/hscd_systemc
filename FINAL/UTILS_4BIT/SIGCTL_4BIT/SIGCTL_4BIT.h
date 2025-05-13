#ifndef __SIGCTL_4BIT_H__
#define __SIGCTL_4BIT_H__

#include <systemc.h>

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

SC_MODULE(SIGCTL_4BIT)
{
    // Inputs
    sc_in<bool> EN;
    sc_in<bool> RST;
    sc_in<sc_uint<WIDTH>> A;
    sc_in<sc_uint<WIDTH>> B;
    sc_in<sc_uint<WIDTH>> SELECT;

    // Reset outputs
    sc_out<bool> FS_RST;
    sc_out<bool> FA_RST;
    sc_out<bool> AND_RST;
    sc_out<bool> NAND_RST;
    sc_out<bool> OR_RST;
    sc_out<bool> XOR_RST;

    // Clock outputs
    sc_out<bool> FS_EN;
    sc_out<bool> FA_EN;
    sc_out<bool> AND_EN;
    sc_out<bool> NAND_EN;
    sc_out<bool> OR_EN;
    sc_out<bool> XOR_EN;
    sc_out<bool> OUT_MUX_EN;

    // Borrow/Carry input outputs
    sc_out<bool> FS_B_IN;
    sc_out<bool> FA_C_IN;

    // Data A outputs
    sc_out<sc_uint<WIDTH>> FS_A;
    sc_out<sc_uint<WIDTH>> FA_A;
    sc_out<sc_uint<WIDTH>> AND_A;
    sc_out<sc_uint<WIDTH>> NAND_A;
    sc_out<sc_uint<WIDTH>> OR_A;
    sc_out<sc_uint<WIDTH>> XOR_A;

    // Data B outputs
    sc_out<sc_uint<WIDTH>> FS_B;
    sc_out<sc_uint<WIDTH>> FA_B;
    sc_out<sc_uint<WIDTH>> AND_B;
    sc_out<sc_uint<WIDTH>> NAND_B;
    sc_out<sc_uint<WIDTH>> OR_B;
    sc_out<sc_uint<WIDTH>> XOR_B;

    // Output selects
    /// 0...15 : FA FS AND NAND OR XOR 4'b1111 4'b0000
    sc_out<sc_uint<OUT_SEL_WIDTH>> OUT_MUX_SEL;

    void low_all_en()
    {
        sigWrite(FS_EN, 0x0);
        sigWrite(FA_EN, 0x0);
        sigWrite(AND_EN, 0x0);
        sigWrite(NAND_EN, 0x0);
        sigWrite(OR_EN, 0x0);
        sigWrite(XOR_EN, 0x0);
        sigWrite(OUT_MUX_EN, 0x0);
    }
    void pass_to_output(sc_uint<OUT_SEL_WIDTH> output_code)
    {
        low_all_en();
        sigWrite(OUT_MUX_SEL, output_code);
        switch (output_code)
        {
        case OUT_CODE::FA:
            sigWrite(FA_EN, 0x1);
            break;
        case OUT_CODE::FS:
            sigWrite(FS_EN, 0x1);
            break;
        case OUT_CODE::AND:
            sigWrite(AND_EN, 0x1);
            break;
        case OUT_CODE::NAND:
            sigWrite(NAND_EN, 0x1);
            break;
        case OUT_CODE::OR:
            sigWrite(OR_EN, 0x1);
            break;
        case OUT_CODE::XOR:
            sigWrite(XOR_EN, 0x1);
            break;
        }
        sigWrite(OUT_MUX_EN, 1);
    }
    void copy_all_rst_signals()
    {
        sigWrite(FS_RST, sigRead(RST));
        sigWrite(FA_RST, sigRead(RST));
        sigWrite(AND_RST, sigRead(RST));
        sigWrite(NAND_RST, sigRead(RST));
        sigWrite(OR_RST, sigRead(RST));
        sigWrite(XOR_RST, sigRead(RST));
    }

    void pass_a()
    {
        sigWrite(FA_A, sigRead(A));
        sigWrite(FA_B, 0x0);
        sigWrite(FA_C_IN, 0);
        pass_to_output(OUT_CODE::FA);
    }
    void incr_a()
    {
        sigWrite(FA_A, sigRead(A));
        sigWrite(FA_B, 0x0);
        sigWrite(FA_C_IN, 0x1);
        pass_to_output(OUT_CODE::FA);
    }
    void add()
    {
        sigWrite(FA_A, sigRead(A));
        sigWrite(FA_B, sigRead(B));
        sigWrite(FA_C_IN, 0x0);
        pass_to_output(OUT_CODE::FA);
    }
    void carry_add()
    {
        sigWrite(FA_A, sigRead(A));
        sigWrite(FA_B, sigRead(B));
        sigWrite(FA_C_IN, 0x1);
        pass_to_output(OUT_CODE::FA);
    }
    void sub()
    {
        sigWrite(FS_A, sigRead(A));
        sigWrite(FS_B, sigRead(B));
        sigWrite(FS_B_IN, 0x0);
        pass_to_output(OUT_CODE::FS);
    }
    void borrow_sub()
    {
        sigWrite(FS_A, sigRead(A));
        sigWrite(FS_B, sigRead(B));
        sigWrite(FS_B_IN, 0X1);
        pass_to_output(OUT_CODE::FS);
    }
    void decr_a()
    {
        sigWrite(FS_A, sigRead(A));
        sigWrite(FS_B, 0x0);
        sigWrite(FS_B_IN, 0X1);
        pass_to_output(OUT_CODE::FS);
    }
    void bw_and()
    {
        sigWrite(AND_A, sigRead(A));
        sigWrite(AND_B, sigRead(B));
        pass_to_output(OUT_CODE::AND);
    }
    void bw_nand()
    {
        sigWrite(NAND_A, sigRead(A));
        sigWrite(NAND_B, sigRead(B));
        pass_to_output(OUT_CODE::NAND);
    }
    void bw_or()
    {
        sigWrite(OR_A, sigRead(A));
        sigWrite(OR_B, sigRead(B));
        pass_to_output(OUT_CODE::OR);
    }
    void bw_xor()
    {
        sigWrite(XOR_A, sigRead(A));
        sigWrite(XOR_B, sigRead(B));
        pass_to_output(OUT_CODE::XOR);
    }
    void reserved() {
        low_all_en();
    }

    void process()
    {
        copy_all_rst_signals();

        if (sigRead(RST))
        {
            sigWrite(OUT_MUX_SEL, 0x7);
            return;
        }
        if (sigRead(EN))
        {
            switch (sigRead(SELECT))
            {
            case ALU_CODE::PASS_A:
                pass_a();
                break;
            case ALU_CODE::INCR_A:
                incr_a();
                break;
            case ALU_CODE::ADDITION:
                add();
                break;
            case ALU_CODE::ADD_W_CARRY:
                carry_add();
                break;

            case ALU_CODE::SUBTRACTION:
                sub();
                break;
            case ALU_CODE::SUB_W_BORROW:
                borrow_sub();
                break;
            case ALU_CODE::DECR_A:
                decr_a();
                break;
            case ALU_CODE::BW_AND:
                bw_and();
                break;
            case ALU_CODE::BW_NAND:
                bw_nand();
                break;
            case ALU_CODE::BW_OR:
                bw_or();
                break;
            case ALU_CODE::BW_XOR:
                bw_xor();
                break;

            default:
                reserved();
                break;
            }
        }
    }

    // Constructor with compact initialization list
    SC_CTOR(SIGCTL_4BIT) : EN("EN"), RST("RST"), A("A"), B("B"), SELECT("SELECT"),
                           FS_RST("FS_RST"), FA_RST("FA_RST"), AND_RST("AND_RST"), NAND_RST("NAND_RST"),
                           OR_RST("OR_RST"), XOR_RST("XOR_RST"),
                           FS_EN("FS_EN"), FA_EN("FA_EN"), AND_EN("AND_EN"), NAND_EN("NAND_EN"),
                           OR_EN("OR_EN"), XOR_EN("XOR_EN"), OUT_MUX_EN("OUT_MUX_EN"),
                           FS_B_IN("FS_B_IN"), FA_C_IN("FA_C_IN"),
                           FS_A("FS_A"), FA_A("FA_A"), AND_A("AND_A"), NAND_A("NAND_A"), OR_A("OR_A"),
                           XOR_A("XOR_A"),
                           FS_B("FS_B"), FA_B("FA_B"), AND_B("AND_B"), NAND_B("NAND_B"), OR_B("OR_B"),
                           XOR_B("XOR_B"),
                           OUT_MUX_SEL("OUT_MUX_SEL")
    {
        SC_METHOD(process);
        sensitive << EN << A << B << RST << SELECT;
    }
};

#endif
