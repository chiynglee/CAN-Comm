#include "regc515c.h"     	/* special function register declarations for C515C */
#include "led.h"
#include "slip.h"
#include "crc.h"
#include "canreg.h"
#include "uart.h"

UART_MSG RecvData;
unsigned char Error_Buf[30];
int error_flag = 0;
unsigned char alive_flag = 0;
unsigned char alive_buf[3];

void Init_UART()
{
	int i;

	EAL = 0;
	TMOD |= 0x20;    // 8-bit timer (8-bit autoload)

	SCON = 0x50;    // mode 1, TI = 0, RI = 0
	TH1 = 251;  	// baud rate = 19200, 10 MHz
	SBUF = 0;
	PCON = 0x80;	// SMOD = 1

	EAL = 1;
	TR1 = 1;		// timer 1 start

	RecvData.flag = 0;
	RecvData.len = 0;

	for(i=0; i<30; i++)
		Error_Buf[i] = 0x00;
}

void WriteChar(unsigned char Data)
{
	while(!TI) ;
	SBUF = Data;
	TI = 0;
}


unsigned char ReadChar()
{
	unsigned char ch;

	while(!RI) ;

	ch = SBUF;
	RI = 0;

	return ch;
}    

void TransmitMessage(unsigned char* msg, int len)
{
	unsigned char debug_msg[30];

	int i;
	int debug_len;

	debug_len = SLIP_Encode_Frame(msg, len, debug_msg);
    for (i = 0; i < debug_len; i++)
	{
		WriteChar(debug_msg[i]);
	}	
}

void create_error_msg(unsigned char *msg){
	msg[0] = NODE_ID;
	if(SR & 0x80)
		msg[1] = BUSOFF;
	else{
		switch(SR & 0x07){
		case 0x01 : msg[1] = STUFF_ERROR;
			break;
		case 0x02 : msg[1] = FORM_ERROR;
			break;
		case 0x03 : msg[1] = ACK_ERROR;
			break;
		case 0x06 : msg[1] = CRC_ERROR;
			break;
		}
	}
}


