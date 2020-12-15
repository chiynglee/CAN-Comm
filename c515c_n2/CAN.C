#include "regc515c.h"
#include "canreg.h"
#include "can.h"
#include "LED.h"

unsigned char pdata canreg[256] _at_ 0xF700;
unsigned char error_count = 0;
unsigned char retry = 0;
unsigned char start = 0;

void Init_CAN()
{
	int i, index;

	EAL = 0;

	P4 |= 0x40;			// output port
	SYSCON &= 0xFC;                // XMAP0=0, XMAP1=0
	XPAGE = 0xF7;			// 8 bit addressing of CAN

	CR = 0x41;			// set init and CCE (enable access baudrate)
//	BTR0 = 0x40;  BTR1 = 0x43;		// baud rate = 1 MHz
//	BTR0 = 0x40;  BTR1 = 0x4D;		// baud rate = 500 kHz
//	BTR0 = 0x41;  BTR1 = 0x4D;		// baud rate = 250 kHz
	BTR0 = 0x43;  BTR1 = 0x4D;		// baud rate = 125 kHz
	CR = 0x01;			// reset CCE (disable access baudrate)

	GMS0 = 0x0F;      GMS1 = 0xFF;	// global mask short
	UGML0 = 0x1F;  UGML1 = 0x1F; 	// global mask long
	LGML0 = 0x1F;  LGML1 = 0x1F;		
	UMLM0 = 0x1F;  UMLM1 = 0x1F;  	// global mask short
	LMLM0 = 0x1F;  LMLM1 = 0x1F;		

	SR = 0xE7;			// clear TXOK and RXOK

	for(i=1; i<15; i++){
		index = 16 * i;
		canreg[index] = 0xA9;
		canreg[index + 1] = 0x55;

		// message object 1 ~ 3 : transmit buffer
		if(i <= 3)
			canreg[index + 6] = 0x08;
		else if(i == 14){
			canreg[index + 2] = 0x00;
			canreg[index + 3] = 0x00;
			canreg[index + 6] = 0x00;
		}
		else{
			canreg[index + 2] = 0x00;
			canreg[index + 3] = 0x40;
			canreg[index + 6] = 0x00;
		}
	}

	IEN2 = 0x02;		// enable CAN interrupt

	CR = 0x06;		// reset INIT, CAN interrupt generation

	EAL = 1;
}

void canmsg_transmit(int index)
{
	if(SR & 0x18)
		SR = 0;

	canreg[index] = 0xA9;     //MCR low
	canreg[index+1] = 0xE7;   //MCR high
}

void Transmit_CAN_MSG(unsigned char *msg, int len){
	int i, index = 16, data_len;
	unsigned short msg_id;

	data_len = len - 2;

	switch(msg[1]){
	case 0x01 : 
		msg_id = MSG_ID_DATA_NODE1;
		break;
	case 0x02 : 
		msg_id = MSG_ID_DATA_NODE2;
		break;
	case 0x03 : 
		msg_id = MSG_ID_DATA_NODE3;
		break;
	default :
		msg_id = MSG_ID_DATA_ALL;
		break;
	}

	// select transmit buffer
	for(i=1; i<=3; i++){
		index = 16 * i;
		if((canreg[index + 1] & 0x02) != 0x02){
				break;
		}
	}

	canreg[index + 1] = 0xDB;		// CPUUPD=1 
	
	// identifier
	canreg[index + 2] = (unsigned char)(msg_id >> 8);
	canreg[index + 3] = (unsigned char)msg_id;
	
	canreg[index + 6] = (unsigned char)(data_len << 4) + 0x08;//0x88;		// data length code = 8, transmit object, standard

	for(i=0; i<len && i<8; i++){
		canreg[index + 7 + i] = msg[i+2];
	}

	canreg[index] = 0xA9;		// MSGVAL=1, TXIE=1, RXIE=1, INTPND=0
	canreg[index + 1] = 0x56;		// RMTPND=0, TXRQ=0, CPUUPD=0, NEWDAT=1

	canmsg_transmit(index);		//CAN message send
}

void Get_Receive_Message(UART_MSG *RecvData, int index){
	int i, len = 0;

	len = canreg[index + 6];
	len >>= 4;

	for(i=0; i<len && i<8; i++){
		RecvData->rdata[i] = canreg[index + 7 + i];
	}

	canreg[index + 1] = 0x55;

	RecvData->flag = 1;
	RecvData->len = len;
}

