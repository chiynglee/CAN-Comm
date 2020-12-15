#include "AT89C51XD2.h"     	/* special function register declarations for C515C */
#include "canreg.h"	        /* can definition file */
#include "slip.h"
#include "crc.h"
#include "can.h"
#include "uart.h"
#include "timer.h"


void main()
{
	unsigned char xdata rx_buf[30];
	int  rx_len = 0, i;
	unsigned char rx_char;

	extern unsigned char RecvData_flag;
	extern unsigned int RecvData_len[2];
	extern unsigned char RecvData_rdata[2][30];

	extern unsigned char Error_Buf[5];
	extern int error_flag;

   	Init_UART();
	Init_Timer();
	Init_CAN();

	EX0 = 1;
	EA = 1;

	rx_char = 0x00;
	while(1)
	{
		if(error_flag == 1){
//			TransmitMessage(Error_Buf, 2);
			error_flag = 0;
		}
		if(RecvData_flag == 1){
			for(i=0; i<2; i++){	
				if(RecvData_len[i] > 0){
					TransmitMessage(RecvData_rdata[i], RecvData_len[i]);
					RecvData_len[i] = 0;
				}
			}
			RecvData_flag = 0;
		}
		if(RI){
			rx_char = ReadChar();
			rx_len = SLIP_Decode_Frame(rx_char, rx_buf);
		    
			if (rx_len > 0)
			{
			    if (check_crc(rx_buf, rx_len) == 1)
		     	{
					switch(rx_buf[0])
					{
					case CAN_REQUEST :
						Transmit_CAN_MSG(rx_buf, rx_len - 2);
						break;
					case CAN_RESPONSE : 
						Transmit_CAN_MSG(rx_buf, rx_len - 2);
						break;
					}
		    	}
			    rx_len = 0;
			}
		}
	}
}

