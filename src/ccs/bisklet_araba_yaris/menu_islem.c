#include "tanimlamalar.h"

#define BUTON_SOL_PIN PIN_A4
#define BUTON_ONAY_PIN PIN_A5
#define BUTON_SAG_PIN PIN_E2

#define BUTON_SOL 's'
#define BUTON_ONAY 'o'
#define BUTON_SAG 'g'

#define MENU_SAYISI 8

#define BUTON_GECIKME 350


char btn_getir(){
    
    if(!input(BUTON_SOL_PIN)){
        delay_ms(BUTON_GECIKME);
        mp3_stop();delay_ms(100);
        mp3_play(mp3_buton);
        return BUTON_SOL;
    }
     if(!input(BUTON_ONAY_PIN)){
        delay_ms(BUTON_GECIKME);
        mp3_stop();delay_ms(100);
        mp3_play(mp3_buton);
        return BUTON_ONAY;
    }
    if(!input(BUTON_SAG_PIN)){
        delay_ms(BUTON_GECIKME);
        mp3_stop();delay_ms(100);
        mp3_play(mp3_buton);
        return BUTON_SAG;
    }
}

char uzunBasBtnGetir(unsigned long btn_pin, unsigned long sure){
    unsigned long x=0;
    //
    if(!input(btn_pin)){
        delay_ms(BUTON_GECIKME);
       //  printf(lcd_putc,"\f......"); 
        do{
            x++;
            if(x>sure){

                if(btn_pin==BUTON_SOL_PIN) return BUTON_SOL;
                if(btn_pin==BUTON_ONAY_PIN) return BUTON_ONAY;
                if(btn_pin==BUTON_SAG_PIN) return BUTON_SAG;
            }
           // printf(lcd_putc,"\n%lu",x); 
            delay_ms(1);
           //if(input(btn_pin))break;
        }while(!input(btn_pin));
        mp3_stop();delay_ms(100);
        mp3_play(mp3_buton);
    }
}

void lcd_menu_sec(int sec){
    
        switch(sec){
            case 0:
                printf(lcd_putc,"\f    < MENU >"); 
                printf(lcd_putc,"\n    << <> >>"); 
                break;
            case 1:
                printf(lcd_putc,"\f 1-OYUN Sure"); 
                printf(lcd_putc,"\n    << <> >>");
                 break;
            case 2:
                 printf(lcd_putc,"\f 2-Secme Suresi"); 
                 printf(lcd_putc,"\n    << <> >>");
                 break;
            case 3:
                 printf(lcd_putc,"\f 3-Motor TEST"); 
                 printf(lcd_putc,"\n    << <> >>");
                 break;

            case 4:
                 printf(lcd_putc,"\f 4-Sensor TEST"); 
                 printf(lcd_putc,"\n    << <> >>");
                 break;
             case 5:
                 printf(lcd_putc,"\f 5-SES"); 
                 printf(lcd_putc,"\n    << <> >>");
                 break;  
            case 6:
                 printf(lcd_putc,"\f 6-JETON Sayisi"); 
                 printf(lcd_putc,"\n    << <> >>");
                 break;
            case 7:
                 printf(lcd_putc,"\f 7-RESET"); 
                 printf(lcd_putc,"\n    << <> >>");
                 break;
            case 8:
                 printf(lcd_putc,"\f 8-CIKIS"); 
                 printf(lcd_putc,"\n    << <> >>");
                 break;
        }
}

int oyun_sure_kaydet(int n){
    float oyun_sure=1.5f;
    int btn=0;
    
    oyun_sure=read_eeprom(EE_ADR_OYUN_SURE)/10.0f;
    if(oyun_sure>10.0) oyun_sure=2.5f;
    
    printf(lcd_putc,"\fSure : %f dk.",oyun_sure); 
    printf(lcd_putc,"\n    << <> >>");
    do{
       btn=btn_getir();
       if(btn==BUTON_SOL){
           oyun_sure-=0.5f;
           if(oyun_sure<1.0f) oyun_sure=15.0f;
           
       }
       if(btn==BUTON_SAG){
           oyun_sure+=0.5f;
           if(oyun_sure>15.0f) oyun_sure=1.0f;
       }
       printf(lcd_putc,"\fSure : %f dk.",oyun_sure); 
       printf(lcd_putc,"\n    << <> >>");
       delay_ms(100);
       
    }while(btn!=BUTON_ONAY);
    write_eeprom(EE_ADR_OYUN_SURE,(unsigned int)(oyun_sure*10.0));
    time_oyun=(unsigned long long int)(oyun_sure*60.0);//_mul(oyun_sure,60);      //saniye
    time_oyun= (unsigned long long int)_mul(time_oyun,1000);    //mS
    printf(lcd_putc,"\f ! KAYDEDILDI !"); 
    printf(lcd_putc,"\nT:%lu",time_oyun); 
    delay_ms(1500);
    
    return n+1;
}

int secme_sure_kaydet(int n)        //Bisiklet seçme süresi kaydet.
{
    int sec_sure=10;
    int btn=0;
    sec_sure=read_eeprom(EE_ADR_BIS_SEC_SURE);
    
    if(sec_sure>30) sec_sure=10;

    do{
       btn=btn_getir();
       if(btn==BUTON_SOL){
           sec_sure-=1;
           if(sec_sure<6) sec_sure=5;
           
       }
       if(btn==BUTON_SAG){
           sec_sure+=1;
           if(sec_sure>30) sec_sure=10;
       }
       printf(lcd_putc,"\fSure : %u S.",sec_sure); 
       printf(lcd_putc,"\n    << <> >>");
       delay_ms(100);
       
    }while(btn!=BUTON_ONAY);
    
    write_eeprom(EE_ADR_BIS_SEC_SURE, sec_sure);
    bisiklet_secme_suresi=_mul(sec_sure,1000);
    
    printf(lcd_putc,"\f ! KAYDEDILDI !"); 
    delay_ms(1500);
    return n+1;
}

int motor_pwm_kaydet(int n){
    unsigned int16 pwm=150;
    char btn=0;
    pwm=make16(read_eeprom(EE_ADR_MAX_PWM+1),read_eeprom(EE_ADR_MAX_PWM));
    if(pwm>500) pwm=450;
    unsigned long long time_akim_olc=millis();
    
    if(pwm>500) pwm=400;
    do{
        btn=btn_getir();
        if(btn==BUTON_SOL){
           pwm-=10;
           if( pwm<100) pwm=100;
        }
        if(btn==BUTON_SAG){
           pwm+=10;
           if( pwm>500) pwm=500;
        }
        
        analogWrite(M1_PWM,pwm);
        analogWrite(M2_PWM,pwm);
        
        o1_akim_olc();
        o2_akim_olc();
        
        if(akim_adc_deger1<5){
             printf(lcd_putc,"\fM1 yok !!!"); 
             printf(lcd_putc,"\nI1:%Lu",akim_adc_deger1); 
             
             if(millis()-time_akim_olc>1500 ){
                // panel1_komut(MOTOR_YOK);
                 mp3_stop();delay_ms(100);
                 mp3_play(mp3_beep_uyari);
                 time_akim_olc=millis();
             }       
            
        }
        if(akim_adc_deger2<5){
            printf(lcd_putc,"\fM2 yok !!!");
            printf(lcd_putc,"\nI2:%Lu",akim_adc_deger2);
             
              if(millis()-time_akim_olc>1500 ){
                  mp3_stop();delay_ms(100);
                  mp3_play(mp3_beep_uyari);
                  time_akim_olc=millis();
                 //  panel2_komut(MOTOR_YOK);
              }  
        }
        
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
                panel1_komut(MOTOR_YOK);
                panel2_komut(MOTOR_YOK);
                delay_ms(150);
            }
            
        }
        printf(lcd_putc,"\fHIZ : %Lu ",pwm); 
        printf(lcd_putc,"\nI1:%lu I2:%lu"akim_adc_deger1,akim_adc_deger2);
        delay_ms(250);
        
    }while(btn!=BUTON_ONAY);
    
    analogWrite(M1_PWM,0x0000);
    analogWrite(M2_PWM,0x0000);
    write_eeprom(EE_ADR_MAX_PWM, make8(pwm,0));     //LSB
    write_eeprom(EE_ADR_MAX_PWM+1, make8(pwm,1));
    pwm_max_deger=pwm;
    printf(lcd_putc,"\f ! KAYDEDILDI !"); 
    
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

		if (millis() - time_m12_dur > 10000){
			analogWrite(M1_PWM, 0);
			analogWrite(M2_PWM, 0);
			break;
		}
		if (m1_dur && m2_dur) break;

    } while (1);
    
    return n+1;
    
}

int sensor_test(int n){
    int btn=0;
    disable_interrupts(GLOBAL);
    do{
        btn=btn_getir();
        
        if(!input(JTN_PIN)){
           mp3_stop();delay_ms(100);
           mp3_play(wav_ding);
           printf(lcd_putc,"\fSens: JETON"); 
           printf(lcd_putc,"\n    << <> >>");
           delay_ms(1000);
        }
        if(!input(JTN2_PIN)){
           mp3_stop();delay_ms(100);
           mp3_play(wav_ding);
           printf(lcd_putc,"\fSens: JETON2"); 
           printf(lcd_putc,"\n    << <> >>");
           delay_ms(1000);
        }
        if(!input(HIZ1_SENS_PIN)){ 
           mp3_stop();delay_ms(100);
           mp3_play(wav_ding);
           printf(lcd_putc,"\fSens: HIZ1 "); 
           printf(lcd_putc,"\n    << <> >>");
           delay_ms(1000);
        }
        
        if(!input(HIZ2_SENS_PIN)){ 
           mp3_stop();delay_ms(100);
           mp3_play(wav_ding);
           printf(lcd_putc,"\fSens: HIZ2 "); 
           printf(lcd_putc,"\n    << <> >>");
           delay_ms(1000);
        }
        if(!input(TUR1_SENS_PIN)){ 
           mp3_stop();delay_ms(100);
           mp3_play(wav_ding);
           printf(lcd_putc,"\fSens: TUR1 "); 
           printf(lcd_putc,"\n    << <> >>");
           delay_ms(1000);
        }
        if(!input(TUR2_SENS_PIN)){ 
           mp3_stop();delay_ms(100);
           mp3_play(wav_ding);
           printf(lcd_putc,"\fSens: TUR2 "); 
           printf(lcd_putc,"\n    << <> >>");
           delay_ms(1000);
        }
        if(!input(BIS_BTN1_PIN)){ 
           mp3_stop();delay_ms(100);
           mp3_play(wav_ding);
           printf(lcd_putc,"\fSens: BIS BTN1 "); 
           printf(lcd_putc,"\n    << <> >>");
           delay_ms(1000);
        }
        if(!input(BIS_BTN2_PIN)){ 
           mp3_stop();delay_ms(100);
           mp3_play(wav_ding);
           printf(lcd_putc,"\fSens: BIS BTN2 "); 
           printf(lcd_putc,"\n    << <> >>");
           delay_ms(1000);
        }

//        printf(lcd_putc,"\fSens: ?",); 
//        printf(lcd_putc,"\n    << <> >>");
        delay_ms(100);
        
    }while(btn!=BUTON_ONAY);
    
    
    enable_interrupts(GLOBAL);
}

int dfPlayer_ses_kaydet(int n){
    unsigned int ses=15;
    char btn=0;
    
    mp3_stop();
    delay_ms(100);
    mp3_play(mp3_adele_Rolling);
    ses=read_eeprom(EE_ADR_MAX_SES);
    if(ses>30) ses=30;
    do{
         btn=btn_getir();
        if(btn==BUTON_SOL){
          ses--;
          if( ses<1) ses=1;
          mp3_set_volume(ses);
          delay_ms(100);
        }
        if(btn==BUTON_SAG){
          ses++;
          if( ses>30) ses=30;
          mp3_set_volume(ses);
          delay_ms(100);
        }
        printf(lcd_putc,"\fSES: %u ",ses); 
        printf(lcd_putc,"\n    << <> >>");
        delay_ms(100);
        
     }while(btn!=BUTON_ONAY);
     
    dfPlayer_ses=ses;
    write_eeprom(EE_ADR_MAX_SES,ses);
    printf(lcd_putc,"\f ! KAYDEDILDI !"); 
    delay_ms(1500);
    return n+1;
}
int jeton_calisma_sayi_kaydet(int n)
{
    signed int jtn=0;
    char btn=0;
    jtn=read_eeprom(EE_ADR_JTN_SYS);
    if(jtn>20) jtn=0;
     printf(lcd_putc,"\fDIKKAT !!!"); 
     printf(lcd_putc,"\nKartli sis. de,");
     delay_ms(2500);
     printf(lcd_putc,"\njeton=0 olmali..");
     delay_ms(2500);
     
     do{
        btn=btn_getir();
        if(btn==BUTON_SOL){
            jtn--;
            if(jtn<0) jtn=0;
        }
         if(btn==BUTON_SAG){
            jtn++;
            if(jtn>20) jtn=20;
        }
        printf(lcd_putc,"\fJeton: %d ",jtn); 
        printf(lcd_putc,"\n    << <> >>");
        delay_ms(100);
         
     }while(btn!=BUTON_ONAY);
     
     calisma_jeton_sayisi=jtn;
     write_eeprom(EE_ADR_JTN_SYS,jtn);
     
     printf(lcd_putc,"\f ! KAYDEDILDI !"); 
     delay_ms(1500);
     return n+1;
}

int ee_reset(int n){
    char btn=0;
    printf(lcd_putc,"\fRESET ?"); 
    printf(lcd_putc,"\n   <E> <> <H>");
    do{
        btn=btn_getir();
        if(btn==BUTON_SOL){
            
            toplam_jeton_sayisi=0;
            write_eeprom(EE_ADR_TOP_JTN_SYS,make8(0xFF,0));//LSB
            write_eeprom(EE_ADR_TOP_JTN_SYS+1,make8(0xFF,1));
            write_eeprom(EE_ADR_TOP_JTN_SYS+2,make8(0xFF,2));
            write_eeprom(EE_ADR_TOP_JTN_SYS+3,make8(0xFF,3));//MSB
            printf(lcd_putc,"\fTOP.JetonSayisi:"); 
            printf(lcd_putc,"\n   0");
            delay_ms(1000);
            
            write_eeprom(EE_ADR_OYUN_SURE,(unsigned int)(2.5*10.0));   // default 2.5 dk
            printf(lcd_putc,"\fOYUN SURE:"); 
            printf(lcd_putc,"\n   2.5 dk.");
            delay_ms(1000);
            
            write_eeprom(EE_ADR_BIS_SEC_SURE, 0x05);
            printf(lcd_putc,"\fBIS. SECME SURE:");
            printf(lcd_putc,"\n   5 S.",);
            delay_ms(1000);
            
            write_eeprom(EE_ADR_MAX_PWM, make8(0x90,0));     //LSB 400 -> 0x0190
            write_eeprom(EE_ADR_MAX_PWM+1, make8(0x01,1));
            printf(lcd_putc,"\fHIZ :",); 
            printf(lcd_putc,"\n   400",);
            delay_ms(1000);
            
            write_eeprom(EE_ADR_JTN_SYS,0x00);
            printf(lcd_putc,"\fCALISMA JETON:");
            printf(lcd_putc,"\n   0");
            delay_ms(1000);
            
            break;
             
        }
    } while(btn!=BUTON_ONAY || btn!=BUTON_SAG);
}

void lcd_menu_islem(){
    
    int btn=0;
    signed int alt_menu=0;
    int1 cikis=0;
    mp3_stop();
    delay_ms(100);
    do{
        btn=btn_getir();
        
        if(btn==BUTON_SOL){
            alt_menu--;
            if(alt_menu<0) alt_menu=MENU_SAYISI;
        }

        if(btn==BUTON_SAG){
            alt_menu++;
            if(alt_menu>MENU_SAYISI) alt_menu=0;
        }
        lcd_menu_sec(alt_menu);
                
        if(btn==BUTON_ONAY){
            
            switch(alt_menu){
                case 1:
                    alt_menu=oyun_sure_kaydet(alt_menu);
                    break;
                case 2:
                    alt_menu=secme_sure_kaydet(alt_menu);
                    break;
                case 3:    
                    alt_menu=motor_pwm_kaydet(alt_menu);
                    break;
                case 4:    
                    alt_menu=sensor_test(alt_menu);
                    break;
                case 5:
                    alt_menu=dfPlayer_ses_kaydet(alt_menu);
                    break;
                case 6:
                    alt_menu=jeton_calisma_sayi_kaydet(alt_menu);
                    break;
                case 7:
                    alt_menu= ee_reset(alt_menu);
                    break;
                case 8:
                    cikis=1;
                    break;
            }
        }
        delay_ms(100);
    }while(!cikis);
    
    printf(lcd_putc,"\f    YOYUNCAK");
    printf(lcd_putc,"\n HIZLI & PEDALLI");
    mp3_stop();
    delay_ms(100);
    //mp3_play(mp3_getlowve_araba);
    
    
}
