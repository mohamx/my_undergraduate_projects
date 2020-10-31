/* 
 * File:   Timers.h
 * Author: MHmed Hatem
 *
 * Created on July 25, 2017, 4:50 AM
 */

#ifndef TIMERS_H
#define	TIMERS_H

extern unsigned int msDelay;                    // Number of milliseconds
void initTimer0(void);                          // (1/_XTAL_FREQ) second for each count so 
                                                // if _XTAL_FREQ is 4 MHZ Time range form 0 To 0.26214 seconds
void startTimer0(unsigned int period);          // Period = 65535 - (Time*_XTAL_FREQ)/16
void initTimer1(void);                          // (1/_XTAL_FREQ) second for each count
void startTimer1(unsigned int period);          // Period = 65535 - (Time*_XTAL_FREQ)/16
void initTimer2(void);                          // (1/_XTAL_FREQ) second for each count so
                                                // if _XTAL_FREQ is 4 MHZ Time range form 0 To 0.00102 seconds
void startTimer2(unsigned char period);         // Period = (Time*_XTAL_FREQ)/16
void msTimer2(unsigned int MSDelay);            // milliseconds timer 2 
void initTimer3(void);                          // (1/_XTAL_FREQ) second for each count
void startTimer3(unsigned int period);          // Period = 65535 - (Time*_XTAL_FREQ)/16


#endif	/* TIMERS_H */

