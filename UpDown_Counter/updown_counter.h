/**
 * @author: Nguyen Thanh Phu
 * @brief UP/DOWN dataER (4 bits)
 * - CLK    : clock signal, Low2High active
 * - PE     : '1' -> enabled, '0' -> disabled/load from P<3:0>
 * - UD     : '1' -> UP mode
 * - BD     : '1' -> Binary mode (4'b0000->4'b1111)
 * - P<3:0> : for pre-programming the dataer
 * - Q<3:0> : 4-bit output
 */

 #ifndef __UPDOWN_dataER__
 #define __UPDOWN_dataER__
 
 #include <systemc.h>
 
 #define BIT_WIDTH 4
 
 SC_MODULE(UPDOWN_COUNTER){
     sc_in<bool> CLK, PE, UD, BD;
     sc_in<sc_uint<BIT_WIDTH>> P;
     sc_out<sc_uint<BIT_WIDTH>> Q;
 
     void main_thread(){
         sc_uint<BIT_WIDTH> data = 0;
         while (true) {
             wait();
             if (PE.read() == 0) {
                 data = P.read();
             } else {
                 if(CLK.read() == 1) {
                     if (UD.read() == 1) {
                         if(BD.read() == 1){
                             data = (data >= 15)?0:(data+1);
                         }else{
                             data = (data >= 9)?0:(data+1);
                         }
                     } else {
                         if(BD.read() == 1){
                             data = (data == 0) ? 15 : (data-1) ;
                         }else{
                             data = ((data == 0) || (data > 9)) ? 9 : (data-1);
                         }
                     }
                 }
             }
             Q.write(data);
         }
     }
 
     SC_CTOR(UPDOWN_COUNTER):
     CLK("CLK"), PE("PE"), UD("UD"), BD("BD"), P("P"), Q("Q")
     {
         SC_THREAD(main_thread);
         sensitive << CLK.pos() << PE;
     }
 };
 
 
 #endif