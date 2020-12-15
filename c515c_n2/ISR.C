#include "regc515c.h"
#include "canreg.h"
#include "can.h"
#include "LED.h"

extern unsigned char pdata canreg[256];
unsigned int count = 0;
extern unsigned char alive_flag;
extern unsigned char alive_buf[3];

void timer0_isr() interrupt 1
{
	if(count == 40000)		// count = 5000 : 1 second
	{
		alive_buf[0] = 0x01;
		alive_buf[1] = 0x1F;
		alive_flag = 1;
		
		P4_0 = ~P4_0;
		count = 0;
	}
	else count++;
}

void CAN_isr() interrupt 17
{
	extern unsigned char start;
	extern UART_MSG RecvData;

	extern unsigned char Error_Buf[5];
	extern int error_flag;

	unsigned char status;
	unsigned char interrupt_id;
	unsigned int index;

	interrupt_id = IR;
	status = SR;

	if(status & 0x80){
		create_error_msg(Error_Buf);
		error_flag = 1;
		P4_3 = ~P4_3;
	}
	if(interrupt_id >= 3){
		index = (interrupt_id - 2) * 16;
		error_flag = 0;
		if(status & 0x08){
			P4_1 = ~P4_1;
		}
		if(status & 0x10){
			if(canreg[index + 8] == 0x1F)
				start = 1;
			Get_Receive_Message(&RecvData, index);
			P4_2 = ~P4_2;
		}
		canreg[index] = 0xFD;	// INTPND reset
	}
}
