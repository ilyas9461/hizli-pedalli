/* 
 * File:   tanimlamalar.h
 * Author: ilyas
 *
 * Created on 22 Þubat 2019 Cuma, 08:43
 */

#ifndef TANIMLAMALAR_H
#define	TANIMLAMALAR_H

#ifdef	__cplusplus
extern "C" {
#endif
    #define led0 PIN_B3
    #define JTN_PIN PIN_B0
    #define JTN2_PIN PIN_B7

    #define HIZ1_SENS_PIN PIN_B1
    #define HIZ2_SENS_PIN PIN_B2

    #define TUR1_SENS_PIN PIN_B5
    #define TUR2_SENS_PIN PIN_B4

    #define BTN1_PIN PIN_A4
    #define BTN2_PIN PIN_A5
    #define BTN3_PIN PIN_E2

    #define BIS_BTN1_PIN PIN_A2
    #define BIS_BTN2_PIN PIN_A3

    #define ADC_KANAL_M2 0  //SENS_B
    #define ADC_KANAL_M1 1  //SENS_A

    #define PWM_DUTY_MAX_4_KHZ      500
    #define PWM_DUTY_MAX_2B5_KHZ    800

    #define F_MAX 10   //saniyedeki max tur sayýsý, buþekildse max 100Hz olur
    //#define DELAY_OLCUM ((1/F_MAX)*1000)/10  // mS/10  F_MAX=100Hz iken delay=1mS
    #define DELAY_OLCUM 10

    
    #define panel1_komut(komut)fprintf(Panel1Serial,"%cc",komut)
    #define panel2_komut(komut)fprintf(Panel2Serial,"%cc",komut)

    #define M1_PWM  10
    #define M2_PWM  11

    #define analogWrite(_pwm_pin, _pwm_deger)                           \
                        if(_pwm_pin==M1_PWM) set_pwm1_duty(_pwm_deger); \
                        if(_pwm_pin==M2_PWM) set_pwm2_duty(_pwm_deger); \

    #define jetonKontrol(pin) if(!input(pin)){                    \
                                                                  \
                            delay_ms(500);                        \
                            calismaJetonSay++;                    \
                           }                                      \
                                                                  \
                           if((calisma_jeton_sayisi>0 && calismaJetonSay>=calisma_jeton_sayisi)) \
                           {                                            \
                                jetonAtildi=TRUE;                       \
                                calismaJetonSay=0;                      \
                                jetonSay++;                             \
                           }                                            \
                           else{                                        \
                                jetonAtildi=TRUE;                       \
                                jetonSay++;                             \
                                }                                       \

    #define JETON 'J'

    #define OYUNCU_MOD_SECIMI 'M'
	#define OYUNCU_MOD_SECIM_SONU 'm'

	#define OYUNCU_HIZ_VERI 'V'
	#define OYUNCU2_HIZ_VERI 'v'

	#define OYUNCU_TEKERLEK_DONME_SAYISI 'D'
	#define OYUNCU2_TEKERLEK_DONME_SAYISI 'd'

	#define OYUNCU_TUR_SAYISI 'T'

	#define OYUNCU_YARIS_SIRA1 '1'
	#define OYUNCU_YARIS_SIRA2 '2'
	#define OYUNCU_YARIS_SIRA0 '0'

	#define OYUNCU1 '3'
	#define OYUNCU2 '4'
	#define SISTEM '5'

	#define OYUNCU_BISIKLET_SECIMI 'B'
    #define OYUNCU1_SISTEM 'X'
    #define OYUNCU1_OYUNCU2'x'

	#define JETON_CHAR 'J'
	#define JETON_AT 'A'

	#define OYUN_MOD_SECME_SURESI 5000
    //#define BISIKLET_SECME_SURESI 10000
    #define MOD_SECILMEDI (-1)
	#define BAY_MOD 0
	#define BAYAN_MOD 1
	#define COCUK_MOD 2

	#define OYUN_OYNANAN_SURE 'O'
	#define OYUN_BITTI 'E'

	#define BOS_ANIM_EKRANI 'K'

    #define ZAMAN_SAY 'Z'
    #define MOTOR_YOK 'Q'
    #define MOTOR_VAR 'q'


	#define BISIKLET_MAX_HIZ    71
	//#define PWM_MAX_DEGER       400
    #define PIST_UZUNLUK        8.51f
                           
/// mp3-wav index
	#define wav_atKisnemesi     1
	#define wav_atKosmasi       2
	#define wav_suDamlasi       3
	#define wav_heySesi         4
	#define wav_alkisSesi       5
	#define wav_oyunBitti       6
	#define wav_getReady        7
	#define wav_whoosh          8
	#define wav_jeton           9
	#define wav_remix           10
	#define wav_remix2          11
	#define wav_waterDrop       12
	#define mp3_adele_Rolling   14
	#define wav_duduk_basla     15
	#define wav_ding            16
	#define mp3_mars            17
	#define mp3_getlowve_araba	18
    #define mp3_beep_uyari      19

     //// bosta rastgele mp3 müzik
    
    #define mp3_adele_Rolling   20
    #define mp3_anitta          21
    #define mp3_love_lo         22
    #define mp3_remix           23
    #define mp3_dejavu          24
    #define mp3_rain            25
    #define mp3_aweli           26
    /////
    #define mp3_buton           27
    #define mp3_buton2          28
    
	#define wav_remix_sure 200000 //(3*60+20)*1000  //3:20 mS degeri....
	#define wav_atKisnemesi_sure  1500
	#define wav_atKosmasi_sure    2000
	#define wav_whoosh_sure  300//350


#ifdef	__cplusplus
}
#endif

#endif	/* TANIMLAMALAR_H */

