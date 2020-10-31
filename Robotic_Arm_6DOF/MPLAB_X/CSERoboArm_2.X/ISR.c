#include "Includes.h"
#define motorNum    ((uartRxData[1]>>4)&0x7)-3   // Motor 3,4,5 -> 0,1,2
//#define spiRxData         spiRxData   // sRD

void endTX(void);
unsigned char uartRxData[10];                     // Word
unsigned char uIndex = 0;
//static unsigned char data2;

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
            uIndex = 0;
		}
        else
        {
            if((received == 0xA || received == 254) && uIndex != 0){
                uartRxData[uIndex] = received;
                if (uartRxData[0] == 'M' && uIndex == 6){
                    uint8_t motorId = motorNum;
                    if (mAttr[motorId].state == SSLEEP)    mAttr[motorNum].state = IDLE;                         // From sleep to idle 
    //              mParams[motorNum].direction = ((Data[1]>>3)&0x1) + 1;   // Either CW Or CCW
                    mAttr[motorId].requestedAngle = ((((uartRxData[1]&0x1)<<8)|uartRxData[2]) + (uartRxData[3]%100)/100.0)*pow(-1.0,((uartRxData[1]>>3)&0x1));
                    mAttr[motorId].speed = ((uartRxData[4]&0x7F)<<8)|uartRxData[5];
                    ring_buffer_put_char(uart_rbd,0xA);
                    ring_buffer_put(uart_rbd,&uartRxData[0]);
                    ring_buffer_put_char(uart_rbd,(uartRxData[1]>>4)&0x7);
                    ring_buffer_put_char(uart_rbd,0x59);
                    ring_buffer_put_char(uart_rbd,0xA);
                    if(!TXEN)   TXEN = 1;
                }
                else if((uartRxData[0] == 'M' || uartRxData[0] == 'S') && uIndex == 3){
                    ring_buffer_put_char(uart_rbd,0xA);
                    for(int i=0; i<3; i++)
                        ring_buffer_put(uart_rbd,&uartRxData[i]);
                    endTX();
                }
                else if(uartRxData[0] == 'S' && uIndex == 2){
                    ring_buffer_put_char(uart_rbd,0xA);
                    ring_buffer_put(uart_rbd,&uartRxData[0]);
                    ring_buffer_put(uart_rbd,&uartRxData[1]);
                    if(mAttr[uartRxData[1]-3].state == SSLEEP)
                        ring_buffer_put_char(uart_rbd,'I'); // Sleep
                    else 
                        ring_buffer_put_char(uart_rbd,'B'); // Busy
                    endTX();
                }
//                else if(uartRxData[0] == 'S' && uIndex == 2){
//                    for(int i=0; i<3; i++)
//                        if(mAttr[i].state != SSLEEP)
//                            uartRxData[1] |= 1<<(i+3);
//                    ring_buffer_put_char(uart_rbd,'S');
//                    ring_buffer_put_char(uart_rbd,uartRxData[1]);
////                    ring_buffer_put_char(uart_rbd,0xA);
//                }
//                ring_buffer_put_char(uart_rbd,0xA);
//                if(!TXEN)   TXEN = 1;
                uIndex = 0;
            }
            else if(received != 0xA && received != 254 )
            {
                if(uartRxData[0] == 'S' && received == 0 && uIndex >1){
                }else{
                    uartRxData[uIndex++] = received;
                }
                //uIndex++;
                uIndex %= 10;
            }
        }
	}
    if(TXIF){
        unsigned char data1;
        if((ring_buffer_get(uart_rbd,&data1)) == 0)
            TXREG = data1;
            //SendByteSerially(data1);
        else
            TXEN = 0;
    }
//    if(SSPIF){
//        if(spiDataReady()){
//            unsigned char received = spiRead();
//            SSPIF = 0;
//            if(received == '\n' && sIndex != 0){
//                spiRxData[sIndex] = received;
//                if (spiRxData[0] == 'M' && sIndex == 6){
//                    mAttr[motorNum].state = IDLE;                         // From sleep to idle 
////                    mParams[(Data[1]>>4)&0x7].direction = ((Data[1]>>3)&0x1) + 1;   // Either CW Or CCW
//                    mAttr[motorNum].requestedAngle = ((((spiRxData[1]&0x1)<<8)|spiRxData[2]) + (spiRxData[3]%100)/100.0)*pow(-1.0,((spiRxData[1]>>3)&0x1));
//                    mAttr[motorNum].speed = ((spiRxData[4]&0x7F)<<8)|spiRxData[5];
//                    ring_buffer_put_char(0,0xA);
//                    ring_buffer_put_char(0,'M');
//                    ring_buffer_put_char(0,(spiRxData[1]>>4)&0x7);
//                    ring_buffer_put_char(spi_rbd,'Y');
//                    ring_buffer_put_char(spi_rbd,0xA);
//                    SSPIF = 1;
//                }
//                sIndex = 0;
//            }
//            else if(received != '\n')
//            {
//                spiRxData[sIndex] = received;
//                sIndex++;
//                sIndex %= 10;
//            }
//        }
//        else
//            SSPIF = 0;
//        int res = ring_buffer_peek(spi_rbd,&data2);
//        if(res == 0){
//            if(spiWrite(data2))
//                ring_buffer_dequeue(spi_rbd);
//            SSPIF = 1;
//        }
//    }
//    if(TMR0IF && TMR0IE && (TMR0 == 0xFF||TMR0 == 0))
//    {
//        if(T0Delay == 0){
////            stopMotor(2);
//            mAttr[2].stopFlag = 1;
//            TMR0IE = 0;                     // Disable timer 0 interrupt            
//        }
//        else
//            TMR0 = 5;
//        T0Delay--;
//        TMR0IF = 0;
//    }
    if(TMR1IF)
    {
        disableTimer1();
        TMR1IF = 0;
        timerTick();
    }
//    if(TMR2IF)
//    {
//        if(T2Delay == 0){
//            TMR2ON = 0;
////            stopMotor(1);
//            mAttr[1].stopFlag = 1;
//        }
//        else
//            TMR2 = 5;
//        T2Delay--;
//        TMR2IF = 0;
//    }
}
void endTX(void){
    ring_buffer_put_char(uart_rbd,0xA);
    if(!TXEN)   TXEN = 1;
}