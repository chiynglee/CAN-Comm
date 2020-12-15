#include "AT89C51XD2.h"
#include "canreg.h"
#include "can.h"

unsigned char canreg[77] _at_ 0x8000;

unsigned char error_count = 0;
unsigned char retry = 0;

void Init_CAN()
{

	SETUP_1 = 0x80;	// Disabled=1

	SETUP_3 = 0xE2;	// RxClr=1, Reset=1, IntClr=1, RSJ2=0, RSJ1=1, RSJ0=0

	// baud rate = 1 MHz -> fail
/*	SETUP_0 = 0x83;	// BTR0 = 1, BTR1 = 1, Txint = 1, Rxint = 1, Errint = 0, Overint = 0
	SETUP_2 = 0x42;	// time segment 2 = 4 pluse, time segment 1 = 11 pluse

	// baud rate = 500 kHz -> fail
	SETUP_0 = 0xC3;	// BTR0 = 1, BTR1 = 1, Txint = 1, Rxint = 1, Errint = 0, Overint = 0
	SETUP_2 = 0x11;	// time segment 2 = 4 pluse, time segment 1 = 11 pluse

	// baud rate = 250 kHz -> success
	SETUP_0 = 0xC3;	// BTR0 = 1, BTR1 = 1, Txint = 1, Rxint = 1, Errint = 0, Overint = 0
	SETUP_2 = 0x24;	// time segment 2 = 4 pluse, time segment 1 = 11 pluse
*/
	// baud rate = 125 kHz -> success
	SETUP_0 = 0xC3;	// BTR0 = 1, BTR1 = 1, Txint = 1, Rxint = 1, Errint = 0, Overint = 0
	SETUP_2 = 0x4A;	// time segment 2 = 4 pluse, time segment 1 = 11 pluse

	FILTER_AM_0 = 0x8F;	// mask
	FILTER_AM_1 = 0xE0;

	def_RX();

	SETUP_3 = 0x02;	// RxClr=0, Reset=0, IntClr=0

	SETUP_1 = 0;	// Disabled=0
}

void def_RX(){
	SETUP_1 |= 0x80;	// Disabled = 1

	RX1_ARB_0 = 0x00;
	RX1_ARB_1 = 0x20;

	RX2_ARB_0 = 0x00;
	RX2_ARB_1 = 0x20;

	RX3_ARB_0 = 0x80;
	RX3_ARB_1 = 0x20;

	SETUP_1 &= 0x7F;	// Disabled = 0
}

void canmsg_transmit(){
	SETUP_3 = 0x2A;	// Txreq = 1 (send), IntClr = 1
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
	
	// identifier
	TX_ARB_0 = (unsigned char)(msg_id >> 8);
	TX_ARB_1 = (unsigned char)msg_id;

	for(i=0; i<len && i<8; i++){
		canreg[20+ i] = msg[i+2];
	}

	SETUP_1 &= 0x80;	// data frame, standard, data lenth part clear	
	SETUP_1 |= (unsigned char)data_len;		// data length

	canmsg_transmit();		//CAN message send
}

int Get_Receive_Message(unsigned char *RecvData, int index){
	int i, len = 0;

	canreg[index + 12] &= 0x0F;
	len = canreg[index + 12];

	for(i=0; i<len && i<8; i++){
		RecvData[i] = canreg[index + 4 + i];
	}

	switch(index){
	case 32 : SETUP_RX = 0x01;
		break;
	case 48 : SETUP_RX = 0x02;
		break;
	case 64 : SETUP_RX = 0x04;
		break;
	}

	return len;
}

