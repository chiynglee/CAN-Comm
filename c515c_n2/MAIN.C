#include "regc515c.h"     	/* special function register declarations for C515C */
#include "canreg.h"	        /* can definition file */
#include "led.h"
#include "slip.h"
#include "crc.h"
#include "can.h"
#include "timer.h"


//recv_data[0] = REGISTER_DISPLAY , recv_data[1] = register_set ID  - from 0 to 15
void register_display(unsigned char* recv_data , int len)
{
	unsigned char tx_buf[30]; //tx_buf[0] = ID
	int tx_len = 0;
	int offset = 0;
	int i;
	if (len <2)  //2 is minimum length
	{
		return;
	}

	if (recv_data[1] > 15)  //general register: 0, message object : 1 ~ 15 
	{
		return;
	}

	switch(recv_data[1])  
	{
		case GENERAL_REGISTER:
			if (recv_data[2] == SHOW_ALL)
			{
				tx_buf[0] = GENERAL_REGISTER; //0
				for (i = 0; i< 16; i++)
				{
					tx_buf[i+1] = canreg[i];    
				}
				tx_len = 17;
			}
			else
			{
				if ((recv_data[2] >= 0)&& (recv_data[2] <= 0x0f))
				{
					offset = recv_data[2];
					tx_buf[0] = canreg[offset];
					tx_len = 1;
				}
			}
			break;
		default:
			tx_len = 17;
			offset = recv_data[1] * 16;
			tx_buf[0] = recv_data[1]; // reg ID
			for (i = 0; i < 16; i++)
			{
				tx_buf[i+1] = canreg[offset + i];
			}
			break;
	}

	if (tx_len > 0)
	{
		TransmitMessage(tx_buf, tx_len);
	}

}

// recv_data[0] : SET (1)   recv_data[1] : ID (0 ~ 15)  recv_data[2] = 0xFF  
// recv_data[0] : SET (1)   recv_data[1] : ID (0 ~ 15)  recv_data[2] = offset, recv_data[3][4] value
void register_set(unsigned char* recv_data, int len)
{
	int offset = 0;

	int i;
	
	if (len < 4)
	{
		return;
	}

	if (recv_data[1] > 15)
	{
		return;
	}
	
	offset = recv_data[1] * 16;

	if (recv_data[2] == 0xFF)  // send all
	{
		for (i = 0; i < 16; i++)
		{
			// data´Â recv_data[3]ºÎÅÍ Á¸À
			canreg[offset + i] = recv_data[3+i];
			if(offset == 0 && i == 4){
				canreg[offset] = 0x41;
				canreg[offset + i] = recv_data[3+i];
				i++;
				canreg[offset + i] = recv_data[3+i];
				canreg[offset] = recv_data[3];
			}
		}
	}
	// GMS0 and GMS1 set
	else
	{
		offset = offset + recv_data[2];     // general : mask set, message object : ID set
		canreg[offset] = recv_data[3];  
		canreg[offset + 1] = recv_data[4];
	}
}

//recv_data[0] = 0x03 , recv_data[1] = msg obj ID, recv_data[2] = mcfg recv_data[3...9] = data byte
void canmsg_data_set(unsigned char* recv_data , int len)
{
	int offset = 0;
	int data_offset = 0;
	int i;
	
	if ((recv_data[1] > 15)||(recv_data[1] == 0))
	{
		return;
	}
	if (len > 100)
	{
		return;
	}

	offset = recv_data[1] * 16;
	data_offset = offset + 7;


	canreg[offset] = 0x55;
	canreg[offset + 1] = 0x59;  //mcr1   don't send while updating data
	
	canreg[offset + 6] = recv_data[2];  //mcfg set

	for (i = 0; i < 8; i++)
	{
		canreg[data_offset + i] = recv_data[3 + i];
	}
		
	canreg[offset] = 0xA9;      //mcr0
	canreg[offset + 1] = 0xF6;  //mcr1
}
/*
void Create_Position(int *px, int *py){
	int x, y;
	
	if(m_pntCount < 6){
		x = 731 + (21 * m_pntCount);
		y = 305 + (21 * m_pntCount);
	}
	else if(m_pntCount >= 6 && m_pntCount < 12){
		x = 857 - (21 * (m_pntCount - 6));
		y = 431 + (21 * (m_pntCount - 6));
	}
	else if(m_pntCount >= 12 && m_pntCount < 18){
		x = 731 - (21 * (m_pntCount - 12));
		y = 557 - (21 * (m_pntCount - 12));
	}
	else if(m_pntCount >= 18 && m_pntCount < 24){
		x = 605 + (21 * (m_pntCount - 18));
		y = 431 - (21 * (m_pntCount - 18));
	}
	
	m_pntCount++;
	if(m_pntCount >= 24)
		m_pntCount = 0;
	
	if(x >= 565 && x <= 895){
		if(y >= 265 && y <= 595){
			p->x = x;
			p->y = y;
		}
	}
}
*/
void main()
{
	unsigned char xdata rx_buf[30];
	int  rx_len = 0, tx_uart_count = 0;
	extern unsigned char start;
	unsigned char rx_char;
	extern UART_MSG RecvData;
	extern unsigned char Error_Buf[5];
	extern int error_flag;
	extern unsigned char alive_flag;
	extern unsigned char alive_buf[3];

   	Init_UART();
	Init_Timer();
	Init_CAN();
	Init_LED();

	rx_char = 0x00;
	while(!start) ;
	while(1)
	{
		if(alive_flag == 1){
			if(tx_uart_count > 3){
				TR0 = 0;		// timer quit
				tx_uart_count = 0;
			}
			else{
				TransmitMessage(alive_buf, 2);
				alive_flag = 0;
				tx_uart_count++;
				P4_5 = ~P4_5;
			}
		}
		if(error_flag == 1){
//			TransmitMessage(Error_Buf, 2);
			P4_4 = ~P4_4;
			error_flag = 0;
		}
		if(RecvData.flag == 1){
			TransmitMessage(RecvData.rdata, RecvData.len);
			RecvData.flag = 0;
			RecvData.len = 0;
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
						case REGISTER_DISPLAY:
							register_display(rx_buf, rx_len);
							break;
						case REGISTER_SET: 
							register_set(rx_buf, rx_len);
							break;
						case CANMSG_TRANSMIT:
							canmsg_transmit(rx_buf[1]);
							break;
						case CANMSG_DATASET:
							canmsg_data_set(rx_buf, rx_len);
							break;
						case CAN_RESET :
							Init_CAN();
							break;
						case CAN_REQUEST :
							Transmit_CAN_MSG(rx_buf, rx_len - 2);
							if(TR0 == 0)
								TR0 = 1;
							break;
						case CAN_RESPONSE : 
							if(TR0 == 0)
								TR0 = 1;
							if(rx_buf[3] == 0x2F)
								tx_uart_count = 0;
							Transmit_CAN_MSG(rx_buf, rx_len - 2);
							break;
						case NODE_START : 
							if(TR0 == 0)
								TR0 = 1;
							tx_uart_count = 0;
						}
		    		}
			       rx_len = 0;
			}
		}
	}
}
