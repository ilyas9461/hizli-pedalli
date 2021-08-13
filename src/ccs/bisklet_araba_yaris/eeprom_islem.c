#define EE_ADR_TOP_JTN_SYS  0    //32 bit 0-3
#define EE_ADR_OYUN_SURE    4    // 8bit dk*10 þeklinde kaydediliyor.
#define EE_ADR_BIS_SEC_SURE 5    // 8 bit saniye 
#define EE_ADR_JTN_SYS      6    // 8 bit. RFID çalýþmayacaksa çalýþma jeton sayýsý
#define EE_ADR_MAX_SES      7    // 8 bit . DF player ses seviyesi
#define EE_ADR_MAX_PWM      8    // 16 bit 8-9. PWM max deðeri

//unsigned int32 toplam_jeton_sayisi=0;

void ee_ilk_baslama_degerleri_oku(){
    
    if(read_eeprom(EE_ADR_TOP_JTN_SYS)!=0xFF && read_eeprom(EE_ADR_TOP_JTN_SYS+1)!=0xFF &&
      read_eeprom(EE_ADR_TOP_JTN_SYS+2)!=0xFF && read_eeprom(EE_ADR_TOP_JTN_SYS+3)!=0xFF){
                            //MSB first 
        toplam_jeton_sayisi=make32(read_eeprom(EE_ADR_TOP_JTN_SYS+3),read_eeprom(EE_ADR_TOP_JTN_SYS+2),
                                   read_eeprom(EE_ADR_TOP_JTN_SYS+1),read_eeprom(EE_ADR_TOP_JTN_SYS)); //LSB
    }else{
       toplam_jeton_sayisi=0;
    }
    printf(lcd_putc,"\fTOPLAM JETON :");
    printf(lcd_putc,"\n%Lu", toplam_jeton_sayisi);
    delay_ms(1250);
    
    if(read_eeprom(EE_ADR_OYUN_SURE)!=0xFF){
        time_oyun=(read_eeprom(EE_ADR_OYUN_SURE));//*60*1000;
        time_oyun= _mul(time_oyun,60)/10;
        time_oyun= _mul(time_oyun,1000);
    }else{
        time_oyun=150000LL;  //2.5 dk
    }
    printf(lcd_putc,"\fOYUN SURE :");
    printf(lcd_putc,"\n%Lu", time_oyun);
    delay_ms(1250);
    
    if(read_eeprom(EE_ADR_BIS_SEC_SURE)!=0xFF){
        bisiklet_secme_suresi=_mul(read_eeprom(EE_ADR_BIS_SEC_SURE),1000);
    }else{
        bisiklet_secme_suresi=10000L;
    }
    printf(lcd_putc,"\fBIS. SECME SURE:");
    printf(lcd_putc,"\n%Lu", bisiklet_secme_suresi);
    delay_ms(1250);
    
    if(read_eeprom(EE_ADR_MAX_PWM)!=0xFF && read_eeprom(EE_ADR_MAX_PWM+1)!=0xFF){
       pwm_max_deger=make16(read_eeprom(EE_ADR_MAX_PWM+1),read_eeprom(EE_ADR_MAX_PWM));
    }else{
         pwm_max_deger=450;
    }
    
    printf(lcd_putc,"\fMAX HIZ:");
    printf(lcd_putc,"\n%Lu",pwm_max_deger);
    delay_ms(1250);
    
    if(read_eeprom(EE_ADR_MAX_SES)!=0xFF){
        dfPlayer_ses=read_eeprom(EE_ADR_MAX_SES);
    }else{
        dfPlayer_ses=20;
    }
    printf(lcd_putc,"\fMP3 Ses:");
    printf(lcd_putc,"\n%u", dfPlayer_ses);
    delay_ms(1250);
    
     if(read_eeprom(EE_ADR_JTN_SYS)!=0xFF){
        calisma_jeton_sayisi=read_eeprom(EE_ADR_JTN_SYS);
    }else{
        calisma_jeton_sayisi=0;
    }
    printf(lcd_putc,"\fCALISMA JETON:");
    printf(lcd_putc,"\n%u", calisma_jeton_sayisi);
    delay_ms(1250);
   
}

