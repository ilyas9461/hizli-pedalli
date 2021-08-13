/* 
 * File:   timer.h
 * Author: ilyas
 *
 * Created on 25 Aralýk 2018 Salý, 08:35
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

    unsigned int32 timerIntSay=0;
    
    #define millis() timerIntSay
    #define setMillis(x) timerIntSay=x

    void timer_kur();
    void Timer0_int();


#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

