#include "Includes.h"

#define S5 &bi_fsm[0]              // Define states addresses
#define S6 &bi_fsm[1]              
#define S10 &bi_fsm[2] 
#define S9 &bi_fsm[3]

//#define S1 &uni_fsm[0]
//#define S2 &uni_fsm[1]
//#define S4 &uni_fsm[2]
//#define S8 &uni_fsm[3]

// Structure that implement states of motor (Finite State Machine Algorithm)
// States is clock wise , counter clock wise , hold .
struct State{
  unsigned short int Out;       
  const struct State *Next[3];
};
typedef const struct State StateType;

// Array of motor state structure
// Bipolar 
StateType bi_fsm[4] = {
    {0x55,{S5 ,S6 ,S9 }},
    {0x66,{S6 ,S10,S5 }},
    {0xAA,{S10,S9 ,S6 }},
    {0x99,{S9 ,S5 ,S10}}
};
// Array of motor state structure
// Unipolar
//StateType uni_fsm[4] = {
//    {1,{S1,S2,S8}},
//    {2,{S2,S4,S1}},
//    {4,{S4,S8,S2}},
//    {8,{S8,S1,S4}}
//};

StateType *mStatePtr[3];           // Array of pointers to hold current state for each motor
mAttr_t mAttr[3];                               // Array of motor parameters structure

// Name     : Motor Check 
// Purpose  : Check if we need to rotate motor or not 
// Input    : Pointer -> motor parameters  
// Output   : True or False
unsigned char mCheck(mAttr_t *mAttrPtr){
    double res = mAttrPtr->requestedAngle - mAttrPtr->currentAngle;
    if ( res > 1.8 ){
        mAttrPtr->direction = CW;
        return 1;
    }
    else if( res < -1.8 ){
        mAttrPtr->direction = CCW;
        return 1;
    }
    return 0;
}
// Name     : Initialize motors pins 
// Purpose  : Define pins for each motor
// Input    : None 
// Output   : None
void initMotors(void){
    // Motor 4 -> bipolar circuit 
    TRISB &= ~0x1E;                         // Make pins RB1 to RB4 output
    PORTB &= ~0x1E;                         // Clear pins RB1 to RB4
    mStatePtr[1] = S5;                      // Bipolar
    mAttr[1].deadtime = 0;                  // Deadtime problem
    
    // Motor 3 -> bipolar circuit 
    // We need 8 pins to control all mosfets of H-bridge
    // We use 8 bits instead of 4 bits to manipulate h-bridge deadtime problem 
    ADCON1|= 0x7;                          // Disable analog to digital converter on PortA,E
    TRISA &= ~0x2F;                         // Make pins RA0 to RA3,RA5 output
    CMCON |= 0x7;
    CVROE  = 0;                             // Disable comparator voltage reference output pin
    PORTA &= ~0x2F;                         // Clear pins RA0 to RA5
    TRISE &= ~0x7;                           // Make pins RE0 to RE2 output
    PORTE &= ~0x7;                          // Clear pins RE0 to RE2
    mStatePtr[0] = S5;                      // Bipolar
    mAttr[0].deadtime = 1;                  // Deadtime problem
    
    // Motor 5 -> bipolar circuit 
    TRISC &= ~0x7;                          // Make pins RC0 to RC2 output
    PORTC &= ~0x7;                          // Clear pins RC0 to RC2 output
    TRISD &= ~0x1;                          // Make pin RD0 output
    PORTD &= ~0x1;                          // Clear pin RD0
    mStatePtr[2] = S5;                      // Bipolar
    mAttr[2].deadtime = 0;                  // Deadtime problem

}

// Name     : Rotate motor
// Purpose  : Rotate any motor any direction
// Input    : Motor number , motor pointer refer to motor parameters structure
// Output   : None
void rotateMotor(unsigned char mNumber , mAttr_t *mAttrPtr){
    // Check state of motor if idle or in execution
    
    if (mAttrPtr->state == IDLE && mAttrPtr->deadtime)
    {   // Determine the address of next motor state based on motor direction
        mStatePtr[mNumber] = mStatePtr[mNumber]->Next[mAttrPtr->direction];   
        switch(mNumber)
        {
            case 0:
                // Upper half of bridge whose value rolling from high to low 
                // Upper half dawdle in rolling form high to low causing short circuit so 
                // We need delay after clearing this bits to avoid short circuit 
                PORTA &= ~(((mStatePtr[mNumber]->Out ^ PORTA)&PORTA)&0x0F);
                startDelay(0,4);
                break;                
/*            case 2:
                // Upper half of bridge whose value rolling from high to low 
                PORTD &= ~(((mStatePtr[mNumber]->Out ^ PORTD)&PORTD)&0x0F);
                startTimer1((int)(65535 - (0.00115*_XTAL_FREQ)/16));                   // Start timer1 1.15 milliseconds
                break;
            case 1:
                // Upper half of bridge whose value rolling from high to low 
                PORTB &= ~((((mStatePtr[mNumber]->Out<<1) ^ PORTB)&PORTB)&0x1E);
                startTimer3((int)(65535 - (0.00115*_XTAL_FREQ)/16));                   // Start timer3 1.15 milliseconds
                break; */
        }
        mAttrPtr->state = DEADTIME_EXECUTION;
    }
    else if(mAttrPtr->state == DEADTIME_EXECUTION || (mAttrPtr->state == IDLE && !mAttrPtr->deadtime))
    {
        if(!mAttrPtr->deadtime)
            mStatePtr[mNumber] = mStatePtr[mNumber]->Next[mAttrPtr->direction];   
        switch(mNumber){
            case 0:
                // Lower half of bridge & the rest of upper half
                PORTE = (PORTE & ~0x7)|((mStatePtr[mNumber]->Out>>1)&0x7);
                PORTA = (PORTA & ~0x2F)|((mStatePtr[mNumber]->Out)&0xF);
                RA5 = mStatePtr[mNumber]->Out&0x1;
                startDelay(0,mAttrPtr->speed);
                break;                
            case 1:
                PORTB = (PORTB&~0x1E)|((mStatePtr[mNumber]->Out<<1)&0x1E);
                startDelay(1,mAttrPtr->speed);
                break;
            case 2:
                PORTD = (PORTD&~0x1)|((mStatePtr[mNumber]->Out>>3)&0x1);
                PORTC = (PORTC&~0x7)|((mStatePtr[mNumber]->Out)&0x7);
                startDelay(2,mAttrPtr->speed);
                break;
        }
        mAttrPtr->state = STEP_EXECUTION;
    }
}
void stopMotor(unsigned char mNumber){
    if (mAttr[mNumber].state == DEADTIME_EXECUTION){
        rotateMotor(mNumber , &mAttr[mNumber]);
    }
    else    
    {
        mAttr[mNumber].state = IDLE;
        if (mAttr[mNumber].direction == CW)
            mAttr[mNumber].currentAngle += 1.8; 
        else 
            mAttr[mNumber].currentAngle -= 1.8;
    }
}