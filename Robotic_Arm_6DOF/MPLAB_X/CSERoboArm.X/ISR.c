#include "Includes.h"
#define motorNum    (uartRxData[1]>>4)&0x7 //motorNum
//#define uartRxData         uartRxData
//#define sspRxData         sspRxData //sRD

unsigned char uartRxData[10];                     // Word
//unsigned char sspRxData[10];
unsigned char uIndex = 0;
//unsigned char sIndex = 0;
//unsigned char error = 0;                   // if 0 then still send message else still send character

void interrupt ISR(void)
{
	if(RCIF)                                // If UART Rx Interrupt
	{
        unsigned char received = RCREG;     // Store received character 
		if(OERR)                            // If over run error, then reset the receiver
		{
			CREN = 0;
			CREN = 1;
		}
        if(received == '\n' && uIndex != 0){
            uartRxData[uIndex] = received;
            uint8_t motorId = motorNum;
            if (uartRxData[0] == 'M' && uIndex == 6){
                if((motorId) < 3){
                    if (mAttr[motorId].state == SSLEEP)                    
                        mAttr[motorId].state = IDLE;                         // From sleep to idle 
                    mAttr[motorId].requestedAngle = ((((uartRxData[1]&0x1)<<8)|uartRxData[2]) + (uartRxData[3]%100)/100.0)*pow(-1,((uartRxData[1]>>3)&0x01));
                    if (motorId == 1){
                        mAttr[motorId].requestedAngle *= 5.051;                // Gear ratio for motor 1
                    }else if (motorId == 2){
                        mAttr[motorId].requestedAngle *= 5.412;                // Gear ratio for motor 2
                    }
                    if((mAttr[motorId].speed = ((uartRxData[4]&0x7F)<<8)|uartRxData[5])<30)    mAttr[motorId].speed=30;
//                    TXIE = 1;
                    ring_buffer_put_char(uart_rbd,'\n');
                    ring_buffer_put(uart_rbd,&uartRxData[0]);
                    ring_buffer_put_char(uart_rbd,(uartRxData[1]>>4)&0x7);
                    ring_buffer_put_char(uart_rbd,'Y');
                    ring_buffer_put_char(uart_rbd,'\n');
                }
                else{
                    ring_buffer_put_char(uart_rbd,'\n');
                    for(int i=0; i<7; i++)
                        ring_buffer_put(uart_rbd,&uartRxData[i]);
                        ring_buffer_put_char(uart_rbd,'\n');

                } 
                if(!TXEN)   TXEN = 1;
            }
            else if(uartRxData[0] == 'S' && uIndex == 2){
                ring_buffer_put_char(uart_rbd,0xA);
                ring_buffer_put(uart_rbd,&uartRxData[0]);
                ring_buffer_put(uart_rbd,&uartRxData[1]);               
                if(uartRxData[1] < 3){
                    if(mAttr[uartRxData[1]].state == SSLEEP)
                        ring_buffer_put_char(uart_rbd,'I'); // Sleep
                    else 
                        ring_buffer_put_char(uart_rbd,'B'); // Busy
                }
                ring_buffer_put_char(uart_rbd,0xA);
                if(!TXEN)   TXEN = 1;
            }
            uIndex = 0;
        }
        else if(received != '\n')
        {
            uartRxData[uIndex] = received;
            uIndex++;
            uIndex %= 10;
        }
	}
    if(TXIF){
        unsigned char data1;
        if(ring_buffer_get(uart_rbd,&data1) == 0)
            TXREG = data1;
        else
            TXEN = 0;
    }
//    if(SSPIF){
//        if(spiDataReady()){
//            unsigned char received = spiRead();
//            SSPIF = 0;
//            if(received == '\n' && sIndex != 0){
//                sspRxData[sIndex] = received;
//                if (sspRxData[0] == 'M' && sIndex == 3){
////                    TXIE = 1;
//                    ring_buffer_put_char(uart_rbd,0xA);
//                    for(int i=0; i<4; i++)
//                        ring_buffer_put(uart_rbd,&sspRxData[i]);
//                    if(!TXIE)   TXIE = 1;
//                }
//                sIndex = 0;
//            }
//            else if(received != '\n')
//            {
//                sspRxData[sIndex] = received;
//                sIndex++;
//                sIndex %= 10;
//            }
//        }
//        else    
//            SSPIF = 0;
//        unsigned char data2;
//        if(ring_buffer_peek(spi_rbd,&data2) == 0){
//            if(spiWrite(data2))
//                ring_buffer_dequeue(spi_rbd);
//            SSPIF = 1;
//        }  
//    }
    if(TMR0IF)
    {
        TMR0ON = 0;
        TMR0IF = 0;
//        stopMotor(1);
        mAttr[1].stopFlag = 1;
    }
    if(TMR1IF)
    {
        TMR1ON = 0;
        TMR1IF = 0;
//        stopMotor(2);
        mAttr[2].stopFlag = 1;
    }
//    if(TMR2IF)
//    {
//        msDelay--;
//        if(msDelay == 0){
//            TMR2ON = 0;
//            ring_buffer_put_char(uart_rbd,'\n');
//            ring_buffer_put_char(uart_rbd,'S');
//            unsigned char msg = 0;
//            for(int i=0 ; i<3 ; i++)
//                if(mAttr[i].state != SSLEEP)
//                    msg |= 1<<i;
//            ring_buffer_put(uart_rbd,&msg);
//            ring_buffer_put_char(uart_rbd,'\n');
//            if(!TXEN)   TXEN = 1;
//            msTimer2(10);
//        }
//        else
//            startTimer2(250);
//        TMR2IF = 0;
//    }
    /*
    else if(TMR2IF)
    {
        msDelay--;
        if(msDelay == 0){
            TMR2ON = 0;
            if (mParams[3].direction == CW)
                mParams[3].currentAngle += 1.8; 
            else 
                mParams[3].currentAngle -= 1.8;
            mParams[3].state = IDLE;
            // Check(0))
        }
        else
            startTimer2(250);
        TMR2IF = 0;
    }
    */
    if(TMR3IF)
    {
        TMR3ON = 0;
//        stopMotor(0);
        mAttr[0].stopFlag = 1;
        TMR3IF = 0;
    }
}