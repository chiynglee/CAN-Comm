#include "uart.h"

// message identifier
#define MSG_ID_DATA_ALL 0x1000
#define MSG_ID_DATA_NODE1 0x1020
#define MSG_ID_DATA_NODE2 0x1040
#define MSG_ID_DATA_NODE3 0X1060


void Init_CAN();
void canmsg_transmit(int index);
void Transmit_CAN_MSG(unsigned char *msg, int len);
void Get_Receive_Message(UART_MSG *RecvData, int index);
