/*  Name     : main.c
 *  Purpose  : Main file for CSERoboArm
 *  Author   : MHmed Hatem
 *  Date     : 20170929
 */ 
// Config statements
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
//  Pre-processor directives Section

#include "Includes.h"

#include <stdint.h>

//#define HIGH        (uint8_t)(0)
//#define LOW         (uint8_t)(1)
//
//#define INPUT       (1)
//#define OUTPUT      (0)
//
//#define SW1_DATA    PORTCbits.RC7
//#define SW1_DIR     TRISC7
//
//#define SW2_DATA    PORTCbits.RC6
//#define SW2_DIR     TRISC6
//
//#define LED1_DATA    PORTAbits.RA0
//#define LED1_DIR     TRISA0
//
//#define LED2_DATA    PORTAbits.RA1
//#define LED2_DIR     TRISA1
//
//#define LED3_DATA    PORTAbits.RA2
//#define LED3_DIR     TRISA2
//
//typedef enum BOOL_e{
//    TRUE  = 1,
//    FALSE = 0,
//}BOOL_t;
//
//typedef enum Seq_Direction_e{
//    SEQ_DIR_FORWARD = 0,
//    SEQ_DIR_BACKWARD = 1
//}Seq_Direction_t;
//
//void Seq_init(void);
//void Seq_Operate(Seq_Direction_t Arg_eDirection);
//
//// Main Function
//void main(void)
//{
//    BOOL_t SW1_LastState = SW1_DATA;
//    BOOL_t SW2_LastState = SW2_DATA;
//    
//    Seq_init();
//	while(1)
//	{
//        if( (SW1_DATA == HIGH) && (SW1_LastState == LOW) )
//        {
//            Seq_Operate(SEQ_DIR_FORWARD);
//            SW1_LastState = HIGH;
//        }
//        else if( SW1_DATA == LOW )
//        {
//            SW1_LastState = LOW;            
//        }
//        else
//        {
//            /* Nothing here */
//        }
//        
//        if( (SW2_DATA == HIGH) && (SW2_LastState == LOW) )
//        {
//            Seq_Operate(SEQ_DIR_BACKWARD);
//            SW2_LastState = HIGH;
//        }
//        else if( SW2_DATA == LOW )
//        {
//            SW2_LastState = LOW;            
//        }
//        else
//        {
//            /* Nothing here */
//        }
//        
//	}
//}
//
//
//
//typedef struct Seq_Operate_States_s{
//    uint8_t  boolLed1;
//    uint8_t  boolLed2;
//    uint8_t  boolLed3;
//    
//    struct Seq_Operate_States_s const * psForwardStatePtr;
//    struct Seq_Operate_States_s const * psBackwardStatePtr;
//    
//} Seq_Operate_States_t;
//
//const Seq_Operate_States_t Seq_Operate_State[3] = {
//    
//    { HIGH, LOW, LOW, &Seq_Operate_State[1], &Seq_Operate_State[0] },
//    { LOW, HIGH, LOW, &Seq_Operate_State[2], &Seq_Operate_State[0] },
//    { LOW, LOW, HIGH, &Seq_Operate_State[2], &Seq_Operate_State[1] },
//    
//};
//
//static Seq_Operate_States_t const * psCurrentStatePtr;
//
//void Seq_init(void)
//{
//    /* Initialize Inputs */
//    SW1_DIR = INPUT;
//    SW2_DIR = INPUT;
//    
//    /* Initialize Outputs */
//    LED1_DIR = OUTPUT;
//    LED2_DIR = OUTPUT;
//    LED3_DIR = OUTPUT;
//    LED1_DATA = HIGH;
//    LED2_DATA = LOW;
//    LED3_DATA = LOW;
//    
//    psCurrentStatePtr = Seq_Operate_State;
//    
//}
//
//
//
//
//void Seq_Operate(Seq_Direction_t Arg_eDirection){
//    
//    
//    if(Arg_eDirection == SEQ_DIR_FORWARD)
//    {
//        psCurrentStatePtr = psCurrentStatePtr->psForwardStatePtr;
//    }
//    else if(Arg_eDirection == SEQ_DIR_BACKWARD)
//    {
//        psCurrentStatePtr = psCurrentStatePtr->psBackwardStatePtr;
//    }
//    else
//    {
//        /* Nothing here */
//    }
//    
//    LED1_DATA = psCurrentStatePtr->boolLed1;
//    LED2_DATA = psCurrentStatePtr->boolLed2;
//    LED3_DATA = psCurrentStatePtr->boolLed3;
//}

