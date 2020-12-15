#include "AT89C51XD2.h"
#include "canreg.h"
#include "can.h"
#include "uart.h"

extern unsigned char canreg[77];
unsigned int count = 0;

void timer0_isr() interrupt 1
{
	if(count == 5000)
	{
		count = 0;
	}
	else count++;
}


// CAN interrupt == Externel interrupt 0
void CAN_isr() interrupt 0
{
	extern unsigned char RecvData_flag;
	extern unsigned int RecvData_len[2];
	extern unsigned char RecvData_rdata[2][30];

	extern unsigned char Error_Buf[5];
	extern int error_flag;

	unsigned char rx_buf_id;
	unsigned int index;

	EA = 0;

	if(STATUS & 0x03){
		create_error_msg(Error_Buf);
		error_flag = 1;
	}

	if(STATUS_RX & 0x07){
		error_flag = 0;

		rx_buf_id = STATUS & 0x30;
		rx_buf_id >>= 4;

		index = 16 * (rx_buf_id + 1);
		if(RecvData_len[0] == 0){
			RecvData_len[0] = Get_Receive_Message(RecvData_rdata[0], index);
			RecvData_flag = 1;
		}
		else if(RecvData_len[0] > 0){
			RecvData_len[1] = Get_Receive_Message(RecvData_rdata[1], index);
			RecvData_flag = 1;
		}
	}

	SETUP_3 |= 0x20;	// RxClr=0, Reset=0, IntClr=1

	EA = 1;
}

