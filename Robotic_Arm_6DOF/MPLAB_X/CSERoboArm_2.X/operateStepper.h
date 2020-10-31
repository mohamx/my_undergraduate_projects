/* 
 * File:   operateStepper.h
 * Author: MHmed Hatem
 *
 * Created on September 29, 2017, 1:08 PM
 */

#ifndef OPERATESTEPPER_H
#define	OPERATESTEPPER_H

#define CW   1                  // Clockwise direction
#define CCW  2                  // Counter Clockwise
#define HOLD 0                  // No change 

#define IDLE 0                  // not exist in step exection
#define DEADTIME_EXECUTION 1    // Exit in dead time delay 
#define STEP_EXECUTION  2       // Exit in step exection 
#define SSLEEP   3               // Reach to requested angle and wait for new order from serial 
 
//#define OUTPUT(x,y,z) (if(x==0))
// Structure that implement the parameters for each motor
struct motorAttr{
    double currentAngle;        // Current angle of motor
    double requestedAngle;      // Requested angle which we need to reach it 
    unsigned int speed;         // Delay for each step of stepper motor (such 1.8 degree)
    unsigned char direction;    // Store direction of motor
    unsigned char state;        // Either idle = 0 or in step execution = 1
//    unsigned volatile char stopFlag;    // If timer overflow then stop motor or operate motor in case of deadtime
    unsigned char deadtime;
};
typedef struct motorAttr mAttr_t;

extern mAttr_t mAttr[3];

void initMotors(void);              // Prototype for motors output pins
unsigned char mCheck(mAttr_t *mPtr);
void rotateMotor(unsigned char mNumber , mAttr_t *mPtr);
void stopMotor(unsigned char mNumber);


#endif	/* OPERATESTEPPER_H */

