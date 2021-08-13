/* 
 * File:   main_bisklet_araba_yaris.h
 * Author: ilyas
 *
 * Created on 21 Þubat 2019 Perþembe, 17:05
 */

#ifndef MAIN_BISKLET_ARABA_YARIS_H
#define	MAIN_BISKLET_ARABA_YARIS_H

#ifdef	__cplusplus
extern "C" {
#endif
     #include <18F45K22.h>
    //#include <18F4520.h>
    #device adc=10
    #device HIGH_INTS=TRUE 
    #device PASS_STRINGS=IN_RAM  

    #FUSES NOWDT                    //No Watch Dog Timer
    #FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
    #FUSES INTRC_IO                 //Internal RC Osc, no CLKOUT
    #FUSES PLLEN                  //4X HW PLL disabled, 4X PLL enabled in software
    #FUSES NOPUT                      //Power Up Timer
    #FUSES NOBROWNOUT               //No brownout reset
    #FUSES WDT_SW                   //No Watch Dog Timer, enabled in Software
    #FUSES NOPBADEN                 //PORTB pins are configured as digital I/O on RESET
    #FUSES MCLR                   //Master Clear pin used for I/O
    #FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
    #FUSES PROTECT                  //Code protected from reads
    //#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
    #FUSES NOFCMEN
    #FUSES NOHFOFST     

    #use delay(internal=32MHz) // çalýþtý 4MHZ çaýþtý

    //#use rs232(baud=9600,parity=N,UART1,bits=8,stream=serial_usb)  //uart1
   // #use rs232(baud=9600,parity=N,UART2,bits=8,stream=serial_dfPlayer) 
    
    #use rs232(baud=9600,parity=N, xmit=PIN_D6, rcv=PIN_D7,bits=8,DISABLE_INTS,STREAM=dfPlayerSerial)   //seri port pin tanýmlama
    #use rs232(baud=9600,parity=N, xmit=PIN_C6, rcv=PIN_C7,bits=8,DISABLE_INTS,STREAM=usbSerial)   //seri port pin tanýmlama

    #use rs232(baud=4800,parity=N, xmit=PIN_C4, rcv=PIN_C5,bits=8,DISABLE_INTS, STREAM=Panel1Serial)

    #use rs232(baud=4800,parity=N, xmit=PIN_E1, rcv=PIN_E0,bits=8,DISABLE_INTS, STREAM=Panel2Serial)

    //#use rs232(uart1,baud =9600,DISABLE_INTS,STREAM=dfPlayerSerial) 
    //#use rs232(uart2,baud =9600,DISABLE_INTS,STREAM=softSerial)

    //spi modes 
    #define SPI_MODE_0  (SPI_L_TO_H | SPI_XMIT_L_TO_H) 
    #define SPI_MODE_1  (SPI_L_TO_H) 
    #define SPI_MODE_2  (SPI_H_TO_L) 
    #define SPI_MODE_3  (SPI_H_TO_L | SPI_XMIT_L_TO_H) 

    //#use spi(MASTER, SPI1, BAUD = 9600, MODE=0, BITS=8) 
                             //9600
    //#use spi(MASTER, SPI1, BAUD = 1000000, MODE=0, BITS=8, MSB_FIRST, STREAM=SPI_RC522) 
   // #use spi(MASTER, SPI1, FORCE_HW, BAUD = 115200, MODE=0, BITS=8, MSB_FIRST, STREAM=SPI_RC522)
   // #use spi(MASTER, SPI1, FORCE_HW, MODE=0, BITS=8, MSB_FIRST, STREAM=SPI_RC522)

    #use fast_io(A)
    //#use fast_io(B)
    #use fast_io(C)
    #use fast_io(E)

    extern unsigned int32 timerIntSay;
    
    //#define millis() timerIntSay
    //#define setMillis(x) timerIntSay=x



#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_BISKLET_ARABA_YARIS_H */

