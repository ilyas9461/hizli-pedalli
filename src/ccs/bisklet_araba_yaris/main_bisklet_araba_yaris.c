/* 
 * File:   main_bisklet_araba_yaris.c
 * Author: ilyas
 *
 * Created on 21 Þubat 2019 Perþembe, 17:05
 */
#include "main_bisklet_araba_yaris.h"
#include "tanimlamalar.h"
//#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "flex_lcd.c"
#include "timer.h"
#include "timer.c"
#include "dfPlayer.c"
#include "dfPlayer.h"

int8 jetonSay=0,calismaJetonSay=0;
BOOLEAN jetonAtildi=FALSE;
BOOLEAN b1=0, b2=0;
int o1_mod_say=MOD_SECILMEDI, o2_mod_say=MOD_SECILMEDI,o1_mod=SISTEM, o2_mod=SISTEM;

unsigned int32 time=0;          
unsigned int32 toplam_jeton_sayisi;           // Oyuncak çalýþmaya baþladýðý andan itibaren kullanýlan jeton sayýsý


unsigned int16 pwm_max_deger=400,v1_pwm=0,v2_pwm=0;
unsigned int16 akim_adc_deger1=0, akim_adc_deger2=0;

const float tekerlek_yari_cap =0.45f;  // 45 cm
//const float pi=3.14159f;

unsigned int16  rpm1=0, rpm2=0, rps1=0, rps2=0;	//  rotate per minute(rpm), rotate per second(rps))
//geçen zaman, devir/dak,  ölü zaman: sensorden gelen verinin son zamanýný okur ms
unsigned long long pevtime1=0, pevtime2=0, dtime1=0, dtime2=0;
unsigned long timetaken1=0, timetaken2=0;
float v1 = 0, v2 = 0;						//hýz
volatile BYTE rotation1=0,rotation2=0;
volatile unsigned int16 tekerlek_tur1 = 0, tekerlek_tur2 = 0;
unsigned int16 pist_tur1 = 0, pist_tur2=0;
int sira1=0, sira2=0;

float yol1 = 0, yol2 = 0;

unsigned long long time_mod_sec=0;
unsigned long long time_bis_sec=0;
unsigned long long time_oyun=90000LL;
unsigned long long time_oyun_sure_say = 0;

unsigned long bisiklet_secme_suresi=10000L;

unsigned int zmn_say1=0,zmn_say2=0;
unsigned int dfPlayer_ses=20, calisma_jeton_sayisi=0;

BOOLEAN oyun_basladi=FALSE, bis_sec_sure_bitti=FALSE, bis_sec_sure_baslat=FALSE;

unsigned long m1_pwm_deger = 0;
unsigned long m2_pwm_deger = 0;

struct bisikletVeri
{
	BYTE komut;
	BYTE veri_low;
	BYTE veri_high;
} panel;

void oyun_giris_islemler();
void oyun_islemler();
void o1_akim_olc();
void o2_akim_olc();
void o1_hiz_olcum();
void o2_hiz_olcum();
void ISR_RB0();
void ISR_RB1();
void ISR_RB2();
void ISR_RB_Change();
 void mp3_rastgele();

long map(long x, long in_min, long in_max, long out_min, long out_max);

#include "eeprom_islem.c"
#include "menu_islem.c"

 main() {

   setup_oscillator(OSC_32MHZ);

   setup_timer_1(T1_DISABLED); 
   setup_timer_2(T2_DIV_BY_16, 124, 1);   // 32MHz fosc ile 4KHz yaklaþýk(div_16)
   //setup_timer_2(T2_DIV_BY_16, 199, 1);   // 32MHz fosc ile 2.5KHz yaklaþýk(div_16)
   
   setup_timer_4(T4_DISABLED,0,1); 
   setup_timer_5(T5_DISABLED);
   setup_timer_6(T6_DISABLED,0,1); 
   
   setup_ccp1(CCP_PWM ); 
   setup_ccp2(CCP_PWM ); 
   
   setup_ccp3(CCP_OFF); 
   setup_ccp4(CCP_OFF); 
   setup_ccp5(CCP_OFF);
 
   setup_dac(DAC_OFF); 
   setup_comparator(NC_NC_NC_NC); 
   setup_vref(FALSE); 
   //setup_adc(ADC_OFF | NO_ANALOGS); //setup_adc(ADC_OFF); 
   //setup_adc_ports(NO_ANALOGS); 
   set_tris_a(0b00111111);
   //set_tris_b(0b11111111);
   set_tris_c(0b10101000);
   set_tris_e(0b00001101);
   
   output_low(PIN_C4);
   output_low(PIN_E1);
   
   lcd_init();  // Always call this first. 
   
   setup_adc_ports(sAN0|sAN1);  //L298 motor çýkýþlarý akým ölçmek için.
   //setup_adc_ports(AN0_TO_AN1);   //18f4520
   setup_adc(ADC_CLOCK_INTERNAL);   //ADC_CLOCK_DIV_8);
   
    ext_int_edge(H_TO_L);      // init interrupt triggering for button press 
  // ext_int_edge( 0, L_TO_H); // Set up PIC18 EXT0
   ext_int_edge( 1, L_TO_H); // Set up PIC18 EXT1
   ext_int_edge( 2, L_TO_H); // Set up PIC18 EXT0
   
   enable_interrupts(INT_EXT);
   disable_interrupts(INT_EXT1);       //HIZ1
   disable_interrupts(INT_EXT2);
   disable_interrupts(INT_RB);
   //enable_interrupts(INT_RB);
 
   timer_kur();  
   enable_interrupts(GLOBAL);
// 
    //set_pwm1_duty(512);       //yarým peryot motoru döndürmediiii
   set_pwm1_duty(0);
   set_pwm2_duty(0);
   
   printf(lcd_putc,"\f    YOYUNCAK");
   printf(lcd_putc,"\n HIZLI & PEDALLI");
   delay_ms(1500);
   
   ee_ilk_baslama_degerleri_oku() ;

   mp3_baslangic(dfPlayer_ses);

   time=millis();

   //fprintf(usbSerial,"JETON..");
   fprintf(Panel1Serial,"01010101c");  //Kablodaki gurultu vb verileri atmak için preamp benzeri bilgi.....
   fprintf(Panel2Serial,"01010101c");  //Kablodaki gurultu vb verileri atmak için
   
   srand(450); 
   printf(lcd_putc,"\f    YOYUNCAK");
   printf(lcd_putc,"\n HIZLI & PEDALLI");
   delay_ms(1500);

    //fprintf(usbSerial,"JETON..");
   fprintf(Panel1Serial,"01010101c");  //Kablodaki gurultu vb verileri atmak için preamp benzeri bilgi.....
   fprintf(Panel2Serial,"01010101c");  //Kablodaki gurultu vb verileri atmak için
   
   pevtime1 = pevtime2=millis();
 
   while(TRUE){
       
       if(input(DF_BUSY_PIN)){
           mp3_rastgele();
       }
      
       if(jetonAtildi){
           
            mp3_stop(); delay_ms(100);
            mp3_play(wav_jeton);
            delay_ms(300);
            
            //printf(lcd_putc,"\fj:%u",jetonSay);
            
            panel1_komut(JETON);
            panel2_komut(JETON);
            
            jetonAtildi=FALSE;
            zmn_say1=zmn_say2=0;
            
            oyun_basladi=FALSE;
            bis_sec_sure_bitti=FALSE;
            bis_sec_sure_baslat=TRUE;
            
            toplam_jeton_sayisi++;
            write_eeprom(EE_ADR_TOP_JTN_SYS,make8(toplam_jeton_sayisi,0));//LSB
            write_eeprom(EE_ADR_TOP_JTN_SYS+1,make8(toplam_jeton_sayisi,1));
            write_eeprom(EE_ADR_TOP_JTN_SYS+2,make8(toplam_jeton_sayisi,2));
            write_eeprom(EE_ADR_TOP_JTN_SYS+3,make8(toplam_jeton_sayisi,3));//MSB
            
            printf(lcd_putc,"\fTOPLAM JETON :");
            printf(lcd_putc,"\n%Lu", toplam_jeton_sayisi);
            
            time_bis_sec=millis();

       }//jetonAtildi
       if(jetonSay>0){
           oyun_giris_islemler();
       }
       
//       if(btn_getir()==BUTON_ONAY){
//           // delay_ms(BUTON_GECIKME);
//             lcd_menu_sec(0);
//        }
       if(uzunBasBtnGetir(BUTON_ONAY_PIN,3000)==BUTON_ONAY){
           lcd_menu_islem();
       }
        
       
   }//while(TRUE)
   
}//main
 
 int mp3_sarki=19;
 int mp3_sarki_list[7];
 int mp3_index=0;
 
 BOOLEAN mp3_list_kontrol(){
    int x=0; 
    for(x=0;x<7;x++){
        if( mp3_sarki==mp3_sarki_list[x]){
            return FALSE;
        } 
    }
    return TRUE;
 }
 
 void mp3_rastgele(){

    mp3_stop();
    delay_ms(100);
    
     if(mp3_index>6){
        mp3_index=0;
        memset(mp3_sarki_list,0,sizeof(mp3_sarki_list));
    }
    while(!mp3_list_kontrol()){
         mp3_sarki=20+rand()%7;
    }
    mp3_sarki_list[mp3_index]=mp3_sarki;
    mp3_index++;

    if(mp3_sarki>25)mp3_sarki=25;
    //mp3_play(mp3_getlowve_araba);
    printf(lcd_putc,"\f HIZLI & PEDALLI");
    printf(lcd_putc,"\n   ::  %u  ::",mp3_sarki);
    mp3_play(mp3_sarki);
   // delay_ms(1000);
     
 }
 
 
 
 void oyun_giris_islemler(){
     
        if(!input(BIS_BTN1_PIN))      
        {
            //printf(lcd_putc,"\fBIS_BTN1");
            delay_ms(350);
            if(o1_mod==SISTEM) b1=1;
  
            if(o1_mod==OYUNCU1){
                o1_mod_say++;
                if(o1_mod_say>=3) o1_mod_say=0;
               // printf(lcd_putc,"\fo1Mod:%u", o1_mod_say);
               // delay_ms(200);
                panel1_komut(OYUNCU_MOD_SECIMI);
                
                zmn_say1=0;
            }

        }
       
        if(!input(BIS_BTN2_PIN))      
        {
            //printf(lcd_putc,"\fBIS_BTN2");
            delay_ms(350);
            if(o2_mod==SISTEM) b2=1;

            if(o2_mod==OYUNCU2){
                o2_mod_say++;
                if(o2_mod_say>=3) o2_mod_say=0;
                //printf(lcd_putc,"\fo2Mod:%u", o2_mod_say);
                 panel2_komut(OYUNCU_MOD_SECIMI);
              
               zmn_say2=0;
            }
        }
        if(jetonSay==1){
            
            if(b1==1 && b2==0 && o1_mod!=OYUNCU1){
                o1_mod=OYUNCU1;
                o2_mod=SISTEM;
                //printf(lcd_putc,"\f0_1");
                
                //delay_ms(500);
                b2=0; 
            }
            if(b1==0 && b2==1 && o2_mod!=OYUNCU2){
                o1_mod=SISTEM;
                o2_mod=OYUNCU2;
                //printf(lcd_putc,"\f0_2");
               // delay_ms(500);
                b1=0;
                
            }
            if(o1_mod==OYUNCU1 && o2_mod==SISTEM && o1_mod_say==MOD_SECILMEDI){
//                printf(lcd_putc,"\f0_1 Mod Sec");
//                printf(lcd_putc,"\n0_2 JETON AT");
                //delay_ms(200);
                panel1_komut(OYUNCU_BISIKLET_SECIMI);
                delay_ms(100);
                panel2_komut(JETON_AT);
                o1_mod_say=0;
            }
            if(o2_mod==OYUNCU2 && o1_mod==SISTEM && o2_mod_say==MOD_SECILMEDI){
//                printf(lcd_putc,"\f0_2 Mod Sec");
//                printf(lcd_putc,"\n0_1 JETON AT");
               // delay_ms(200);
                panel2_komut(OYUNCU_BISIKLET_SECIMI);
                delay_ms(100);
                panel1_komut(JETON_AT);
                o2_mod_say=0;
            }
        }
        if(jetonSay>=2){
            
            jetonSay=2;
            
            if(b1==0&& b2==0){b1=1;b2=1;}
            
            if(b1==1 && b2==1){
                
                if(o2_mod==SISTEM){
                    o2_mod=OYUNCU2;
                    panel2_komut(OYUNCU_BISIKLET_SECIMI);
                }
                if(o1_mod==SISTEM){
                    o1_mod=OYUNCU1;
                    panel1_komut(OYUNCU_BISIKLET_SECIMI);
                }
                b1=b2=0;
                
            }
        }
       
       if((millis()-time_mod_sec>1000) && !oyun_basladi){
           
           if(o1_mod==OYUNCU1){
               zmn_say1++;
               if(zmn_say1>=5) {
                panel1_komut(OYUNCU_MOD_SECIM_SONU);  
                } else{
                  fprintf(Panel1Serial,"Z%uc",zmn_say1);  
                }
   
           }
           if(o2_mod==OYUNCU2){
               zmn_say2++;

               if(zmn_say2>=5) {
                    panel2_komut(OYUNCU_MOD_SECIM_SONU);  
               }else{
                   fprintf(Panel2Serial,"Z%uc",zmn_say2);
               }
  
           }
           time_mod_sec=millis();
       }
       
       if(bis_sec_sure_bitti){

            if(o1_mod==SISTEM && o2_mod==OYUNCU2){
                panel1_komut(SISTEM); 
            }
            if(o2_mod==SISTEM && o1_mod==OYUNCU1){
                panel2_komut(SISTEM); 
            }
            
            if(o1_mod==SISTEM && o2_mod==SISTEM && jetonSay==1){
                o1_mod=OYUNCU1;
                o2_mod==SISTEM;
                panel1_komut(OYUNCU1_SISTEM);  
                panel2_komut(SISTEM); 
                bis_sec_sure_baslat=FALSE;
                bis_sec_sure_bitti=FALSE;
                oyun_basladi=TRUE;
               // break;
            }
            
            if(o1_mod==SISTEM && o2_mod==SISTEM && jetonSay==2){
                o1_mod=OYUNCU1;
                o2_mod==OYUNCU2;
                
                panel1_komut(OYUNCU_MOD_SECIM_SONU);   
                panel2_komut(OYUNCU_MOD_SECIM_SONU); 
                
                bis_sec_sure_baslat=FALSE;
                bis_sec_sure_bitti=FALSE;
                oyun_basladi=TRUE;
               // break;
            }

            if(zmn_say1>=5) {
                panel1_komut(OYUNCU_MOD_SECIM_SONU);  
            } 

            if(zmn_say2>=5) {
                panel2_komut(OYUNCU_MOD_SECIM_SONU);  
            }
            
            if(o1_mod==OYUNCU1 || o2_mod==OYUNCU2){
                oyun_basladi=TRUE; 
            }
            
            bis_sec_sure_baslat=FALSE;
            bis_sec_sure_bitti=FALSE;
            //break;
       }//if(bis_sec_sure_bitti)
       
       if( ((millis()-time_bis_sec>bisiklet_secme_suresi) && bis_sec_sure_baslat))// &&(zmn_say1>=5 || zmn_say2>=5))// && !oyun_basladi && (b1==1 || b2==1))
       {  
           bis_sec_sure_bitti=TRUE;
       }
       
       if(oyun_basladi){
           delay_ms(200);
           panel1_komut(OYUNCU_MOD_SECIM_SONU);  
           panel2_komut(OYUNCU_MOD_SECIM_SONU);  
           oyun_islemler();
       }
     
 }
 
 void oyun_islemler(){
    fprintf(Panel1Serial,"01010101c");  //Kablodaki gurultu vb verileri atmak için preamp benzeri bilgi.....
    fprintf(Panel2Serial,"01010101c");  //Kablodaki gurultu vb verileri atmak için 
    
    printf(lcd_putc,"\fOYUN ->"); 
    printf(lcd_putc,"\nm1:%c,m2:%c",o1_mod,o2_mod); 
	mp3_stop(); delay_ms(100);
	mp3_play(mp3_mars);
    delay_ms(1000);
    
    fprintf(Panel1Serial,"01010101c");  //Kablodaki gurultu vb verileri atmak için preamp benzeri bilgi.....
    fprintf(Panel2Serial,"01010101c");  //Kablodaki gurultu vb verileri atmak için 
    delay_ms(300);
    
    if(o2_mod==SISTEM ){analogWrite(M2_PWM, 250);v1=5;}
    if(o1_mod==SISTEM ){analogWrite(M1_PWM, 250);v2=5;}
    
    enable_interrupts(INT_EXT1);
    enable_interrupts(INT_EXT2);
    
    BYTE x_fill=0;
    char buf_str[50],m1_var_yok=0,m2_var_yok=0;
    //BOOLEAN akim_busy, panel_update_busy,oyun_zaman_busy;
    unsigned int sistem_mod_hiz_deg_say=0;
    unsigned long long time_tur_olc1=millis();
    unsigned long long time_tur_olc2=millis();
    unsigned long long time_veri_gonder=millis();
    time_oyun_sure_say = millis();                  //Oyununun süresini sayar.
    unsigned long long time_oynanan_sure=millis();
    
    do{
        if(input(DF_BUSY_PIN)){
           mp3_stop();
           delay_ms(100);
           mp3_play(mp3_getlowve_araba);
       }
        
        //if (millis() - time_oynanan_sure >(unsigned long long)(time_oyun / (unsigned long long)(64)))
        if (millis() - time_oynanan_sure >time_oyun/60)// 2^6  64 bölmek(time_oyun >> 6)
        {
//            disable_interrupts(INT_EXT1);       //HIZ1
//            disable_interrupts(INT_EXT2);
            
            x_fill++;
            
//            fprintf(Panel1Serial,"Z,%u,c",x_fill);
//            delay_ms(50);
//            fprintf(Panel2Serial,"Z,%u,c",x_fill);
//            delay_ms(50);
            
			time_oynanan_sure = millis();
            
            //printf(lcd_putc,"\fsure:%Lu",((millis()-time_oyun_sure_say))); 
            
//            enable_interrupts(INT_EXT1);       //HIZ1
//            enable_interrupts(INT_EXT2);
        }
        
        if ((millis() - dtime1)>3000) //no sens found for 1500ms
		{
			rpm1 = v1 = 0; // make rpm and velocity as zero
			dtime1 = millis();
		}
        if ((millis() - dtime2)>3000) //no sens found for 1500ms
		{
			rpm2 = v2 = 0; // make rpm and velocity as zero
			dtime2 = millis();
		}
        
         if(!input(PIN_B5))// && o1_mod==OYUNCU1)      //TUR1
        {
             time_tur_olc1=millis();
             do{
                 if(millis()- time_tur_olc1>350){
                     break;
                 }
                 
             }while(!input(PIN_B5));
            
             pist_tur1++;
        }
        
        if(!input(PIN_B4))// && o2_mod==OYUNCU2)      //TUR2
        {
            time_tur_olc2=millis();
            do{
                 if(millis()- time_tur_olc2>350){
                     break;
                 }
                 
             }while(!input(PIN_B4));
             
            pist_tur2++;
        }

        if (rotation1 >= 1 && o1_mod==OYUNCU1)
        {
            o1_hiz_olcum();
        } 
        if (rotation2 >= 1 && o2_mod==OYUNCU2)
        {
            o2_hiz_olcum();
        }
        
        m1_pwm_deger = (long)(map((long)(v1), 0, BISIKLET_MAX_HIZ, 0, pwm_max_deger)+100);
		m2_pwm_deger = (long)(map((long)(v2), 0, BISIKLET_MAX_HIZ, 0, pwm_max_deger)+100);
        
        if (v1 == 0) m1_pwm_deger = 0;
		if (v2 == 0) m2_pwm_deger = 0;
        
        if(o2_mod==SISTEM){
            
            m2_pwm_deger=450+(rand()%5)*11;
            v2= m2_pwm_deger/10;
            
            rps2=(v2/(tekerlek_yari_cap*0.37699f))/60.0f;   //hýza göre saniyedeki devir sayýsý
            
            if(sistem_mod_hiz_deg_say>3){
               
                sistem_mod_hiz_deg_say=0;
                //printf(lcd_putc,"\npwm:%lu",m2_pwm_deger); 
                analogWrite(M2_PWM, m2_pwm_deger);
            }
            
        }else{
            analogWrite(M2_PWM, m2_pwm_deger);
        }
        
        if(o1_mod==SISTEM){
            m1_pwm_deger=450+(rand()%5)*11;
            v1= m1_pwm_deger/10;
            
            rps1=(v1/(tekerlek_yari_cap*0.37699f))/60.0f;   //hýza göre saniyedeki devir sayýsý
            
            if(sistem_mod_hiz_deg_say>3){
               
                sistem_mod_hiz_deg_say=0;
               // printf(lcd_putc,"\npwm:%lu",m1_pwm_deger); 
                analogWrite(M1_PWM, m1_pwm_deger);
            }
            
        }else{
            analogWrite(M1_PWM, m1_pwm_deger);		//analogWrite(pin, value)
        }
        o1_akim_olc();
        o2_akim_olc();
        
        if(v1>0 && akim_adc_deger1<1){

//             if(millis()-time_akim_olc1>1500 ){
//                 //panel1_komut(MOTOR_YOK);
//                 printf(lcd_putc,"\fM1 yok !!!"); 
//                 printf(lcd_putc,"\nI1:%Lu",akim_adc_deger1); 
//                 
//                 time_akim_olc1=millis();
//             } 
             m1_var_yok=MOTOR_YOK;
            
        }else  m1_var_yok=MOTOR_VAR;
        
        if(v2>0 && akim_adc_deger2<1){
 
//              if(millis()-time_akim_olc2>1500 ){
//                  
//                   //panel2_komut(MOTOR_YOK);
//                   printf(lcd_putc,"\fM2 yok !!!");
//                   printf(lcd_putc,"\nI2:%Lu",akim_adc_deger2);
//                   time_akim_olc2=millis();
//              }
               m2_var_yok=MOTOR_YOK;
                
        } else  m2_var_yok=MOTOR_VAR;
        
        if(akim_adc_deger1>200 || akim_adc_deger2>200)//1000mV/4.88mv=~205 motordan geçen 1Amper akým deðeridir. aþýrýakým korumasý.
        {
            analogWrite(M1_PWM, 0);
            analogWrite(M2_PWM, 0);
           
            printf(lcd_putc,"\f! ASIRI AKIM !"); 
            printf(lcd_putc,"\n    << <> >>");
            
            while(akim_adc_deger1< 25 && akim_adc_deger2<25){
                analogWrite(M1_PWM, 0);
                analogWrite(M2_PWM, 0);
                o1_akim_olc();
                o2_akim_olc();
               // panel1_komut(MOTOR_YOK);
               // panel2_komut(MOTOR_YOK);
                delay_ms(100);
            }
            
        }
        
        if(yol1>yol2){ sira1=1; sira2=2;}
        if(yol1<yol2){ sira1=2; sira2=1;}
        if(yol1==yol2){ sira1=0; sira2=0;}
        
        if(millis()-time_veri_gonder>=1250){
            
            disable_interrupts(INT_EXT1);       //HIZ1
            disable_interrupts(INT_EXT2);
            
             if(o2_mod==SISTEM){
                yol2+=rps2*(2*pi*tekerlek_yari_cap); 
               // pist_tur2=(long)(yol2/8.51f);
             }
             if(o1_mod==SISTEM){
                yol1+=rps1*(2*pi*tekerlek_yari_cap); 
               // pist_tur1=(long)(yol1/8.51f);
             }
           // v1=100; tekerlek_tur1=500;pist_tur1=250;sira1=1;yol1=525;
                          // 1                            2        3            4           5        6     7        8
            sprintf(buf_str,"O,%lu,%lu,%lu,%lu,%u,%u,%c,c",(long)v1,tekerlek_tur1,pist_tur1,(long)yol1,sira1,x_fill, m1_var_yok);
            fprintf(Panel1Serial,"%s",buf_str);
          //  delay_ms(100);
            sprintf(buf_str,"O,%lu,%lu,%lu,%lu,%u,%u,%c,c",(long)v2,tekerlek_tur2,pist_tur2,(long)yol2,sira2,x_fill,m2_var_yok);
            fprintf(Panel2Serial,"%s",buf_str);
            
            sistem_mod_hiz_deg_say++;
            
            printf(lcd_putc,"\fV1:%2.1f V2:%2.1f",v1,v2);
           // printf(lcd_putc,"\nI1:%3lu I2:%3lu",akim_adc_deger1,akim_adc_deger2);
            printf(lcd_putc,"\fsure:%Lu",((millis()-time_oyun_sure_say))); 
            
            enable_interrupts(INT_EXT1);       //HIZ1
            enable_interrupts(INT_EXT2);
            
            //time_akim_olc1=millis();
            //time_akim_olc2=millis();
            time_veri_gonder=millis();
        }
         
     }while((millis()-time_oyun_sure_say) < time_oyun);
     
     ////       Oyun BITTI          /////
 
     disable_interrupts(INT_EXT1);       //HIZ1
     disable_interrupts(INT_EXT2);
     disable_interrupts(INT_RB);
     
     delay_ms(300);
     panel1_komut(OYUN_BITTI);
     delay_ms(300);
     panel2_komut(OYUN_BITTI);
     
     mp3_stop(); delay_ms(100);
	 mp3_play(wav_oyunBitti);
    // delay_ms(4000);
     
  
     BOOLEAN m1_dur = FALSE, m2_dur = FALSE;
	 unsigned long long time = millis();
	 unsigned long long time_m12_dur = millis();
	 m1_pwm_deger = m2_pwm_deger =pwm_max_deger;        //yoldan çýkmamasý için ayarlanan max deger.
	 analogWrite(M1_PWM, m1_pwm_deger);		//analogWrite(pin, value)
	 analogWrite(M2_PWM, m2_pwm_deger);
     
     do{

		if (!input(TUR1_SENS_PIN)){
			analogWrite(M1_PWM, 0);		//analogWrite(pin, value)
			m1_dur = TRUE;
		}
		if (!input(TUR2_SENS_PIN)){
			analogWrite(M2_PWM, 0);
			m2_dur = TRUE;
		}

		if (millis() - time_m12_dur > 25000){
			analogWrite(M1_PWM, 0);
			analogWrite(M2_PWM, 0);
			break;
		}
		if (m1_dur && m2_dur) break;

    } while (1);
     
     mp3_stop(); delay_ms(100);
	 mp3_play(wav_alkisSesi);
	 delay_ms(3000);
     
     panel1_komut(BOS_ANIM_EKRANI);
     panel2_komut(BOS_ANIM_EKRANI);
     
     b1=b2=0;
     o1_mod_say=MOD_SECILMEDI; o2_mod_say=MOD_SECILMEDI;o1_mod=SISTEM; o2_mod=SISTEM;
     jetonAtildi=jetonSay=zmn_say1=zmn_say2=0;
     oyun_basladi=FALSE;
     tekerlek_tur1= tekerlek_tur2 = yol1 = yol2 = 0;
     pist_tur1 = pist_tur2 = 0;
	 sira1=sira2=0;
     bis_sec_sure_bitti=FALSE;
     
    printf(lcd_putc,"\f    YOYUNCAK");
    printf(lcd_putc,"\n HIZLI & PEDALLI");

 }
 
 
 void o1_hiz_olcum(){
     
    tekerlek_tur1++;
    dtime1 = millis();
    timetaken1 = (unsigned long)(millis() - pevtime1); //time in millisec for two rotations

    if (timetaken1>0){
       //rps1 = (unsigned int16)(1000 /((unsigned int16) timetaken1));//*60; //calculate rps(rotate per second)=F(Hz)
       rpm1 = (unsigned int16)(1000 /((unsigned int16) timetaken1))*60;
        v1 = (float)(tekerlek_yari_cap * rpm1 * 0.37699f); //0.33 is the radius of the wheel in meter
        if(o1_mod_say==COCUK_MOD) v1=v1*2.0f;
        if(o1_mod_say==BAYAN_MOD) v1=v1*1.5f;
       // v1=(float)(2*pi*radius_of_wheel*3600)/((unsigned int16)timetaken1);//km/sa
        //printf(lcd_putc,"\ft:%Lu",timetaken1);
        yol1=tekerlek_tur1*(2*pi*tekerlek_yari_cap);
    }
    pevtime1 = millis();
    rotation1 = 0;
 }
 
 void o2_hiz_olcum(){
     
    tekerlek_tur2++;
    dtime2 = millis();
    timetaken2 = (unsigned long)(millis() - pevtime2); //time in millisec for two rotations

    if (timetaken2>0){
      // rps2 = (unsigned int16)(1000 /((unsigned int16) timetaken2));//*60; //calculate rps(rotate per second)=F(Hz)
       rpm2 = (unsigned int16)(1000 /((unsigned int16) timetaken2))*60;
       v2= (float)(tekerlek_yari_cap * rpm2 * 0.37699f); //0.33 is the radius of the wheel in meter
       if(o2_mod_say==COCUK_MOD) v2=v2*2.0f;
       if(o2_mod_say==BAYAN_MOD) v2=v2*1.5f;
       // v2=(float)(2*pi*radius_of_wheel*3600)/((unsigned int16)timetaken2);//km/sa
        //printf(lcd_putc,"\ft:%f",v2);
        yol2=tekerlek_tur2*(2*pi*tekerlek_yari_cap);
    }
    pevtime2 = millis();
    rotation2 = 0;
 }
 
 void o1_akim_olc(){
      int ornek=0;
      set_adc_channel(ADC_KANAL_M1);
      delay_us(20);
      
      for(ornek=0;ornek<10;ornek++){
           akim_adc_deger1+=read_adc();
           delay_us(25);
       }  
       akim_adc_deger1=akim_adc_deger1/10;  
 }
 void o2_akim_olc(){
      int ornek=0;
      set_adc_channel(ADC_KANAL_M2);
      delay_us(20);
      
      for(ornek=0;ornek<10;ornek++){
           akim_adc_deger2+=read_adc();
           delay_us(25);
       }  
       akim_adc_deger2=akim_adc_deger2/10;  
 }
 /********************************Kesme Alt Program****************************/
#INT_EXT HIGH //NOCLEAR //      //Set external interrupt on pin RB0 
void ISR_RB0() 
{ 
    // printf(lcd_putc,"\fINT_EXT0-JTN1");
     jetonKontrol(JTN_PIN);
     clear_interrupt(INT_EXT);
} 

#INT_EXT1 NOCLEAR //HIGH      //HIZ1
void ISR_RB1() 
{ 
//    if(!input(HIZ1_SENS_PIN)){
//        printf(lcd_putc,"\fINT_EXT1-H1"); 
//        delay_ms(1500);
//    }
    disable_interrupts(INT_EXT1);
    delay_ms(DELAY_OLCUM);// bekleyip ölçüm yapýlmalý.
                // max saniyede 10 tur yani 10 Hz olursa
    if(!input(HIZ1_SENS_PIN)) rotation1++;
    
    clear_interrupt(INT_EXT1);
    enable_interrupts(INT_EXT1);
   
}
#INT_EXT2 NOCLEAR   //HIGH      //HIZ2
void ISR_RB2() 
{ 
//    if(!input(HIZ2_SENS_PIN)){
//        printf(lcd_putc,"\fINT_EXT2-H2");
//        delay_ms(1500);
//    }
    
    disable_interrupts(INT_EXT2);
    delay_ms(DELAY_OLCUM);// bekleyip ölçüm yapýlmalý.
                // max saniyede 10 tur yani 10 Hz olursa
    if(!input(HIZ2_SENS_PIN)) rotation2++; 
    clear_interrupt(INT_EXT2);
    enable_interrupts(INT_EXT2);
}

#INT_RB //NOCLEAR //Port B any change on B4-B7
void ISR_RB_Change() 
{ 
    delay_ms(10);
    
    if(!input(PIN_B4))      //TUR2
    {
      pist_tur2++;
     // printf(lcd_putc,"\fB4_T2:%lu",pist_tur2);
          //delay_ms(1500);
       
    }
    if(!input(PIN_B5))      //TUR1
    {
        pist_tur1++;
       // printf(lcd_putc,"\fB5-T1:%lu"pist_tur1);
        //delay_ms(1500);
        
    }
    if(!input(PIN_B7))
    {
        //printf(lcd_putc,"\fB7-J2");
        //jetonKontrol(JTN2_PIN);
    }
     
    clear_interrupt(INT_RB);
  
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
 	    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

