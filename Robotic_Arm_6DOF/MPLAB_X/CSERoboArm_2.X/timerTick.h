/* 
 * File:   timerTick.h
 * Author: MHmed Hatem
 *
 * Created on October 19, 2017, 4:42 PM
 */

#ifndef TIMERTICK_H
#define	TIMERTICK_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Includes.h"
#define NO_OF_DELAYS    3
#define TICK_TIME   0.001       // Time in seconds
    
void initTimerTick(void);
void timerTick(void);
void startDelay(uint8_t delayNumber ,uint32_t noTicks);
void stopDelay(uint8_t delayNumber);
uint8_t stateOffDelay(uint8_t delayNumber);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMERTICK_H */

