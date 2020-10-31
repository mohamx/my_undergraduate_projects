/*
 * Name     : Timers
 * Purpose  : Provides time delays
 * Data     : 20170725
 */


#include "Includes.h"

unsigned int msDelay;

void initTimer0(void){
    // Stop timer 0
    // 16 bit mode 
    // Internal clock source
    // PSA prescaler 1:4
    T0CON  = 0x01;
    TMR0IE = 1;                     // Enable timer 1 interrupt
}

void startTimer0(unsigned int period){
    TMR0H  = period>>8;
    TMR0L  = period & 0xFF;
    TMR0ON = 1;
}

void initTimer1(void){
    // Stop timer 1
    // 16 bit mode
    // Internal clock source
    // PSA 1:4
    T1CON  = 0x24;
    TMR1IE = 1;
}

void startTimer1(unsigned int period){
    TMR1H  = period>>8;
    TMR1L  = period & 0xFF;
    TMR1ON = 1;
}

void initTimer2(void){
    // Stop timer 2
    // 8 bit mode
    // Internal clock source
    // PSA 1:4
    T2CON  = 0x01;
    TMR2IE = 1;
}

void startTimer2(unsigned char period){
    PR2  = period;
    TMR2 = 0;
    TMR2ON = 1;
}

void msTimer2(unsigned int MSDelay){
    msDelay = MSDelay;
    startTimer2(250);
}

void initTimer3(void){
    // Stop timer 3
    // 16 bit mode
    // Internal clock source
    // PSA 1:4
    T3CON  = 0x24;
    TMR3IE = 1;
}

void startTimer3(unsigned int period){
    TMR3H  = period>>8;
    TMR3L  = period & 0xFF;
    TMR3ON = 1;
}