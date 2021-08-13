#include "timer.h"
void timer_kur(){
    
    setup_timer_0(T0_INTERNAl | T0_8_BIT | T0_DIV_64);    // clock 48Mhz,32MHz
    //set_timer0(0x43);            //1000uS    clock 48Mhz
    set_timer0(0x82);            //1000uS 32MHZ
    //set_timer0(0x05);            //1000uS 64MHZ
//    setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
//    set_timer1(0xFB1D);   //64285 1000uS div_8 ile kesme   
//    setup_timer_3(T3_INTERNAL|T3_DIV_BY_8);
//    set_timer3(0xFB1D);
//    clear_interrupt(INT_TIMER3);
//    enable_interrupts(INT_TIMER3); 
//    disable_interrupts(INT_TIMER3);
//    enable_interrupts(PERIPH);
//    enable_interrupts(INT_TIMER1);
    
    enable_interrupts(INT_TIMER0);
   // enable_interrupts(GLOBAL);
     
}
long say=0;
#INT_TIMER0 //HIGH                    //Timer0 Kesmesi
void Timer0_int(){
   // set_timer0(0x43);            //1000uS 16Mhz
   set_timer0(0x82);            //1000uS 32MHZ
   // set_timer0(0x05);            //1000uS 64MHZ
   
   say++;
   if(say>=100){
       output_toggle(PIN_B3);
       say=0;
   }
    timerIntSay++;
    clear_interrupt(INT_TIMER0 ) ;
}
