/* 
 * File:   Timers.h
 * Author: MHmed Hatem
 *
 * Created on July 25, 2017, 4:50 AM
 */

#ifndef TIMERS_H
#define	TIMERS_H

extern volatile unsigned int T0Delay;                    // Number of milliseconds
extern volatile unsigned int T2Delay;

void initTimer0(void);                          // (1/_XTAL_FREQ) second for each count so 
                                                // if _XTAL_FREQ is 4 MHZ Time range form 0 To 0.26214 seconds
void startTimer0(unsigned char period);          // Period = 255 - (Time*_XTAL_FREQ)/16
void msTimer0(unsigned int MSDelay);            // milliseconds timer 1

void initTimer1(void);                          // (1/_XTAL_FREQ) second for each count
void startTimer1(unsigned int period);          // Period = 65535 - (Time*_XTAL_FREQ)/16
void enableTimer1(void);
void disableTimer1(void);

void initTimer2(void);                          // (1/_XTAL_FREQ) second for each count so
                                                // if _XTAL_FREQ is 4 MHZ Time range form 0 To 0.00102 seconds
void startTimer2(unsigned char period);         // Period = (Time*_XTAL_FREQ)/16
void msTimer2(unsigned int MSDelay);            // milliseconds timer 2 

#endif	/* TIMERS_H */

