/*  Preprocessor directives section */
#include "timerTick.h"
/*  Prototypes section  */
void resetTimerTick(void);
void enableTimerTick(void);
void disableTimerTick(void);

typedef struct timerDelay{
    uint8_t flag;   // set when time is out 
    uint32_t ticks;     // No of ticks
    uint8_t state;  // 0 -> off , 1 -> on
}timerDelay_t;
static volatile timerDelay_t delays[NO_OF_DELAYS];


/*  functions section   */
// from 0 to 262140 microseconds
void initTimerTick(void){
    if(NO_OF_DELAYS){
        initTimer1();                           // Initialize timer 1
    }
}
void resetTimerTick(void){
    startTimer1((int)(65535 - (TICK_TIME * _XTAL_FREQ)/16));                   // Start timer1 
}
void enableTimerTick(void){
    enableTimer1();
}
void disableTimerTick(void){
    disableTimer1();
}
void timerTick(void){
    uint8_t state = 0;
    if(NO_OF_DELAYS)
        for(int i=0 ;i<NO_OF_DELAYS ;i++){
            if(delays[i].state){
                if(delays[i].ticks == 0 && delays[i].flag  == 0){
                    delays[i].state = 0;    // Turn off this delay
                    delays[i].flag  = 1;    // Set delay's flag to do action
                }else if(delays[i].ticks == 0 && delays[i].flag  == 1){
                }else{
                    delays[i].ticks--;
                }
            }
            state |= delays[i].state;
        }
    if(state)  resetTimerTick();
    else    disableTimerTick();
}
void startDelay(uint8_t delayNumber ,uint32_t noTicks){
    delays[delayNumber].ticks = noTicks;
    delays[delayNumber].flag  = 0;
    delays[delayNumber].state = 1;    // Turn on this delay
    if(!TMR1ON){
        //enableTimerTick();
        resetTimerTick();
    }
}
void stopDelay(uint8_t delayNumber){
    delays[delayNumber].state = 0;
    delays[delayNumber].flag = 0;
}
uint8_t stateOffDelay(uint8_t delayNumber){
    return  delays[delayNumber].flag;
}