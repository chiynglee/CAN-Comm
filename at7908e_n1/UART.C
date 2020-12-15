#include "AT89C51XD2.h"     /* special function register declarations for 89C51XD2 */
#include "canreg.h"	        /* can definition file */
#include "slip.h"
#include "crc.h"
#include "can.h"
#include "uart.h"

unsigned char RecvData_flag;
unsigned int RecvData_len[2];
unsigned char RecvData_rdata[2][30];
unsigned char Error_Buf[10];
int error_flag = 0;

void Init_UART()
{
	int i;
	
	TMOD |= 0x20;    // 8-bit timer (8-bit autoload)

	SCON = 0x52;    // mode 1, TI = 0, RI = 0
	TH1 = 0xFD;		// BAUDRATE = 19200 - 11.0592
	SBUF = 0;
	PCON = 0x80;	// SMOD = 1

	TR1 = 1;		// timer 1 start

	RecvData_flag = 0;
	RecvData_len[0] = 0;
	RecvData_len[1] = 0;

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
	if(STATUS & 0x01)
		msg[1] = BUSOFF;
	else if(STATUS & 0x02)
		msg[1] = ERROR;
}

