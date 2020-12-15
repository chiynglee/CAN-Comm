/* 1st byte : functional group */
#define REGISTER_DISPLAY 0x00
#define REGISTER_SET 0x01
#define CANMSG_TRANSMIT 0x02
#define CANMSG_DATASET  0x03
#define CAN_RESET      0x04

#define CAN_REQUEST 0x10
#define CAN_RESPONSE 0x20

#define NODE_START 0xFF


/* 2nd byte : REGISTER ID     */
#define GENERAL_REGISTER 0x00
#define MSG_OBJECT1 0x01
#define MSG_OBJECT2 0x02
#define MSG_OBJECT3 0x03
#define MSG_OBJECT4 0x04
#define MSG_OBJECT5 0x05
#define MSG_OBJECT6 0x06
#define MSG_OBJECT7 0x07
#define MSG_OBJECT8 0x08
#define MSG_OBJECT9 0x09
#define MSG_OBJECTA 0x0A
#define MSG_OBJECTB 0x0B
#define MSG_OBJECTC 0x0C
#define MSG_OBJECTD 0x0D
#define MSG_OBJECTE 0x0E
#define MSG_OBJECTF 0x0F

/*3rd byte : show all or show single */
#define SHOW_ALL  0xFF


typedef struct{
	unsigned char flag;
	unsigned int len;
	unsigned char rdata[30];
} UART_MSG;

void Init_UART();
unsigned char ReadChar();
void WriteChar(unsigned char Data);
void TransmitMessage(unsigned char* msg, int len);void create_error_msg(unsigned char *msg);
