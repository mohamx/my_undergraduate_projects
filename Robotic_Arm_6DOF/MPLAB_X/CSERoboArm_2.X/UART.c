/*  Name    : UART
    Purpose : To provide UART functionality for PIC
*/

#include "Includes.h"

void InitUART(void)
{
	TRISC6 = 1;   					// TX Pin
	TRISC7 = 1;   					// RX Pin
	SPBRG = 25;
            //((_XTAL_FREQ/16)/baudrate) - 1;
   // TXSTA = 6;
	BRGH  = 1;                   	// Fast baud Rate
	SYNC  = 0;						// Asynchronous
   // RCSTA = 90;
	SPEN  = 1;						// Enable serial port pins
	CREN  = 1;						// Enable reception
	SREN  = 0;						// No effect
	TXIE  = 1;						// Enable tx interrupts
	RCIE  = 1;						// Enable rx interrupts
	TX9   = 0;						// 8-bit transmission
	RX9   = 0;						// 8-bit reception
	TXEN  = 0;						// Reset transmitter
//	TXEN  = 1;						// Enable the transmitter
}


// Name     : Send byte serially 
// Purpose  : Writes a character to the serial port
// Input    : Character 
// Output   : None
void SendByteSerially(unsigned char Byte)
{
	while(!TXIF);  // wait for previous transmission to finish
	TXREG = Byte;
}

// Name     : Receive byte serially 
// Purpose  : Reads a character from the serial port
// Input    : None
// Output   : Character 
unsigned char ReceiveByteSerially(void)  
{
	if(OERR)                        // If over run error, then reset the receiver
	{
		CREN = 0;
		CREN = 1;
	}
	
	while(!RCIF);                   // Wait for transmission to receive
	
	return RCREG;
}

// Name     : Send many bytes serially 
// Purpose  : Writes a string to the serial port
// Input    : pointer of string 
// Output   : None
void SendStringSerially(const unsigned char* st)
{
	while(*st)
		SendByteSerially(*st++);
}