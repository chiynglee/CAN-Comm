// message identifier
#define MSG_ID_DATA_ALL 0x1000
#define MSG_ID_DATA_NODE1 0x1020
#define MSG_ID_DATA_NODE2 0x1040
#define MSG_ID_DATA_NODE3 0X1060

void Init_CAN();
void def_RX();
void canmsg_transmit();
void Transmit_CAN_MSG(unsigned char *msg, int len);
int Get_Receive_Message(unsigned char *RecvData, int index);

