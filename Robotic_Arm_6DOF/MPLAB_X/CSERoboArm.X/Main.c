/*  Name     : main.c
 *  Purpose  : Main file for using UART of PIC16F877A.
 *  Author   : MHmed Hatem
 *  Date     : 20170724
 */

//  Pre-processor directives Section
#include "Includes.h"
// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)
// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)
// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)
// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)
// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

#include <stdint.h>

#define HIGH        (uint8_t)(0)
#define LOW         (uint8_t)(1)

#define INPUT       (1)
#define OUTPUT      (0)

#define SW1_DATA    PORTCbits.RC7
#define SW1_DIR     TRISC7

#define SW2_DATA    PORTCbits.RC6
#define SW2_DIR     TRISC6

#define SW3_DATA    PORTBbits.RB3
#define SW3_DIR     TRISB3

#define LED1_DATA    PORTAbits.RA1
#define LED1_DIR     TRISA1

#define LED2_DATA    PORTAbits.RA4
#define LED2_DIR     TRISA4

#define LED3_DATA    PORTAbits.RA5
#define LED3_DIR     TRISA5

#define LED4_DATA    PORTEbits.RE0
#define LED4_DIR     TRISE0

#define LED5_DATA    PORTEbits.RE1
#define LED5_DIR     TRISE1

#define LED6_DATA    PORTEbits.RE2
#define LED6_DIR    TRISE2

typedef enum BOOL_e{
    TRUE  = 1,
    FALSE = 0,
}BOOL_t;

typedef enum Seq_Direction_e{
    SEQ_DIR_FORWARD = 0,
    SEQ_DIR_BACKWARD = 1
}Seq_Direction_t;

void Seq_init(void);
void Seq_Operate(Seq_Direction_t Arg_eDirection);

// Main Function
void main(void)
{
    BOOL_t SW1_LastState = SW1_DATA;
    BOOL_t SW2_LastState = SW2_DATA;
    
    Seq_init();
    

//    TRISA = 0;
//    PORTA = 0xFF;
	while(1)
	{
//        PORTA = 0xFF;
//        __delay_ms(1000);
//        PORTA = 0x00;
//        __delay_ms(1000);
        if( (SW1_DATA == HIGH) && (SW1_LastState == LOW) )
        {
            Seq_Operate(SEQ_DIR_FORWARD);
            SW1_LastState = HIGH;
        }
        else if( SW1_DATA == LOW )
        {
            SW1_LastState = LOW;            
        }
        else
        {
            /* Nothing here */
        }
        
        if( (SW2_DATA == HIGH) && (SW2_LastState == LOW) )
        {
            Seq_Operate(SEQ_DIR_BACKWARD);
            SW2_LastState = HIGH;
        }
        else if( SW2_DATA == LOW )
        {
            SW2_LastState = LOW;            
        }
        else
        {
            /* Nothing here */
        }
        
	}
}


typedef struct Seq_Operate_States_s{
    uint8_t  boolLed1;
    uint8_t  boolLed2;
    uint8_t  boolLed3;
    
    struct Seq_Operate_States_s const * psForwardStatePtr;
    struct Seq_Operate_States_s const * psBackwardStatePtr;
    
} Seq_Operate_States_t;

const Seq_Operate_States_t Seq_Operate_State[3] = {
    
    { HIGH, LOW, LOW, &Seq_Operate_State[1], &Seq_Operate_State[0] },
    { LOW, HIGH, LOW, &Seq_Operate_State[2], &Seq_Operate_State[0] },
    { LOW, LOW, HIGH, &Seq_Operate_State[2], &Seq_Operate_State[1] },
    
};


typedef struct Seq_Operate_States_2_s{
    uint8_t  boolLed1;
    uint8_t  boolLed2;
    uint8_t  boolLed3;
    uint8_t  boolLed4;
    uint8_t  boolLed5;
    uint8_t  boolLed6;
    
    struct Seq_Operate_States_2_s const * psForwardStatePtr;
    struct Seq_Operate_States_2_s const * psBackwardStatePtr;
    
} Seq_Operate_States_2_t;

const Seq_Operate_States_2_t Seq_Operate_State_2[6] = {
    
    { HIGH, LOW, LOW, LOW, LOW, LOW, &Seq_Operate_State_2[1], &Seq_Operate_State_2[0] },
    { LOW, HIGH, LOW, LOW, LOW, LOW, &Seq_Operate_State_2[2], &Seq_Operate_State_2[0] },
    { LOW, LOW, HIGH, LOW, LOW, LOW, &Seq_Operate_State_2[3], &Seq_Operate_State_2[1] },
    { LOW, LOW, LOW, HIGH, LOW, LOW, &Seq_Operate_State_2[4], &Seq_Operate_State_2[2] },
    { LOW, LOW, LOW, LOW, HIGH, LOW, &Seq_Operate_State_2[5], &Seq_Operate_State_2[3] },
    { LOW, LOW, LOW, LOW, LOW, HIGH, &Seq_Operate_State_2[5], &Seq_Operate_State_2[4] },    
    
};

static Seq_Operate_States_t const * psCurrentStatePtr;

static Seq_Operate_States_2_t const * psCurrentStatePtr_2;


void Seq_init(void)
{
    /* Initialize Inputs */
    SW1_DIR = INPUT;
    SW2_DIR = INPUT;
    SW3_DIR = INPUT;
    
    /* Initialize Outputs */
    ADCON1|= 0x0F;                          // Disable analog to digital converter on PortA,E
    TRISA &= ~0x3F;                         // Make pins RA0 to RA5 output
    CVROE  = 0;                             // Disable comparator voltage reference output pin
    LED1_DIR = OUTPUT;
    LED2_DIR = OUTPUT;
    LED3_DIR = OUTPUT;
    LED4_DIR = OUTPUT;
    LED5_DIR = OUTPUT;
    LED6_DIR = OUTPUT;
    
    LED1_DATA = HIGH;
    LED2_DATA = LOW;
    LED3_DATA = LOW;
    LED4_DATA = LOW;
    LED5_DATA = LOW;
    LED6_DATA = LOW;
    
    psCurrentStatePtr = Seq_Operate_State;
    psCurrentStatePtr_2 = Seq_Operate_State_2;
}




void Seq_Operate(Seq_Direction_t Arg_eDirection){
    
    if(SW3_DATA == HIGH){
        if(Arg_eDirection == SEQ_DIR_FORWARD)
        {
            psCurrentStatePtr = psCurrentStatePtr->psForwardStatePtr;
        }
        else if(Arg_eDirection == SEQ_DIR_BACKWARD)
        {
            psCurrentStatePtr = psCurrentStatePtr->psBackwardStatePtr;
        }
        else
        {
            /* Nothing here */
        }

        LED1_DATA = psCurrentStatePtr->boolLed1;
        LED2_DATA = psCurrentStatePtr->boolLed2;
        LED3_DATA = psCurrentStatePtr->boolLed3;
        LED4_DATA = LOW;
        LED5_DATA = LOW;
        LED6_DATA = LOW;
    }
    else{
        if(Arg_eDirection == SEQ_DIR_FORWARD)
        {
            psCurrentStatePtr_2 = psCurrentStatePtr_2->psForwardStatePtr;
        }
        else if(Arg_eDirection == SEQ_DIR_BACKWARD)
        {
            psCurrentStatePtr_2 = psCurrentStatePtr_2->psBackwardStatePtr;
        }
        else
        {
            /* Nothing here */
        }

        LED1_DATA = psCurrentStatePtr_2->boolLed1;
        LED2_DATA = psCurrentStatePtr_2->boolLed2;
        LED3_DATA = psCurrentStatePtr_2->boolLed3;
        LED4_DATA = psCurrentStatePtr_2->boolLed4;
        LED5_DATA = psCurrentStatePtr_2->boolLed5;
        LED6_DATA = psCurrentStatePtr_2->boolLed6;
    
    }
}

