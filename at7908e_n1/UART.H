/* 1st byte : functional group */
#define REGISTER_DISPLAY 	0x00
#define REGISTER_SET 		0x01
#define CANMSG_TRANSMIT 	0x02
#define CANMSG_DATASET  	0x03
#define CAN_RESET      		0x04

#define CAN_REQUEST 		0x10
#define CAN_RESPONSE 		0x20


void Init_UART();
unsigned char ReadChar();
void WriteChar(unsigned char Data);
void TransmitMessage(unsigned char* msg, int len);
void create_error_msg(unsigned char *msg);

