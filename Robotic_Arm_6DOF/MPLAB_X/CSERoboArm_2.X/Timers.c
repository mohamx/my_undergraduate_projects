/*
 * Name     : Timers
 * Purpose  : Provides time delays
 * Data     : 20170725
 */


#include "Includes.h"

volatile unsigned int T0Delay;
volatile unsigned int T2Delay;

void initTimer0(void){
    // Stop timer 0
    // 8 bit mode 
    // Internal clock source
    // PSA prescaler 1:4
    OPTION_REG  = 0x1;
    TMR0IF = 0;    
    TMR0IE = 0;    
}

void startTimer0(unsigned char period){
    TMR0  = period;
    TMR0IF = 0;        
    TMR0IE = 1;                     // Enable timer 0 interrupt
}

void msTimer0(unsigned int MSDelay){
    T0Delay = MSDelay;
    startTimer0(5);
}

void initTimer1(void){
    // Stop timer 1
    // 16 bit mode
    // Internal clock source
    // PSA 1:4
    T1CON  = 0x20;
    TMR1IF = 0;
    TMR1IE = 1;
}

void startTimer1(unsigned int period){
    TMR1H  = period>>8;
    TMR1L  = period & 0xFF;
    TMR1ON = 1;
}
void enableTimer1(void){
    TMR1ON = 1;
}
void disableTimer1(void){
    TMR1ON = 0;
}
void initTimer2(void){
    // Stop timer 2
    // 8 bit mode
    // Internal clock source
    // PSA 1:4
    T2CON  = 0x1;
    TMR2IE = 1;
    TMR0IF = 0;    
}

void startTimer2(unsigned char period){
    PR2  = period;
    TMR2 = 0;
    TMR2ON = 1;
}

void msTimer2(unsigned int MSDelay){
    T2Delay = MSDelay;
    startTimer2(250);
}
