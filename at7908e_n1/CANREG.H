#define NODE_ID 0x01

// error
#define BUSOFF 0xFF
#define ERROR 0xF0

extern unsigned char canreg[77];

/****************************/
/* CAN Register Declaration */
/* CAN Control Registers    */   

#define		SETUP_0			canreg[0]
#define		SETUP_1			canreg[1]
#define		SETUP_2			canreg[2]
#define		SETUP_3			canreg[3]
#define		SETUP_RX		canreg[4]

#define		STATUS			canreg[5]
#define		STATUS_RX		canreg[6]

#define		FILTER_AM_0		canreg[7]
#define		FILTER_AM_1		canreg[8]

#define		ERR_COUNT_TX	canreg[9]
#define		ERR_COUNT_RX	canreg[10]

#define		TRIG_MATCH_0	canreg[11]
#define		TRIG_MATCH_1	canreg[12]

#define		TX_ARB_0		canreg[16]
#define		TX_ARB_1		canreg[17]
#define		TX_ARB_2		canreg[18]
#define		TX_ARB_3		canreg[19]
#define		TX_MESSAGE_0	canreg[20]
#define		TX_MESSAGE_1	canreg[21]
#define		TX_MESSAGE_2	canreg[22]
#define		TX_MESSAGE_3	canreg[23]
#define		TX_MESSAGE_4	canreg[24]
#define		TX_MESSAGE_5	canreg[25]
#define		TX_MESSAGE_6	canreg[26]
#define		TX_MESSAGE_7	canreg[27]

#define		RX1_ARB_0		canreg[32]
#define		RX1_ARB_1		canreg[33]
#define		RX1_ARB_2		canreg[34]
#define		RX1_ARB_3		canreg[35]
#define		RX1_MESSAGE_0	canreg[36]
#define		RX1_MESSAGE_1	canreg[37]
#define		RX1_MESSAGE_2	canreg[38]
#define		RX1_MESSAGE_3	canreg[39]
#define		RX1_MESSAGE_4	canreg[40]
#define		RX1_MESSAGE_5	canreg[41]
#define		RX1_MESSAGE_6	canreg[42]
#define		RX1_MESSAGE_7	canreg[43]
#define		RX1_STATUS		canreg[44]

#define		RX2_ARB_0		canreg[48]
#define		RX2_ARB_1		canreg[49]
#define		RX2_ARB_2		canreg[50]
#define		RX2_ARB_3		canreg[51]
#define		RX2_MESSAGE_0	canreg[52]
#define		RX2_MESSAGE_1	canreg[53]
#define		RX2_MESSAGE_2	canreg[54]
#define		RX2_MESSAGE_3	canreg[55]
#define		RX2_MESSAGE_4	canreg[56]
#define		RX2_MESSAGE_5	canreg[57]
#define		RX2_MESSAGE_6	canreg[58]
#define		RX2_MESSAGE_7	canreg[59]
#define		RX2_STATUS		canreg[60]

#define		RX3_ARB_0		canreg[64]
#define		RX3_ARB_1		canreg[65]
#define		RX3_ARB_2		canreg[66]
#define		RX3_ARB_3		canreg[67]
#define		RX3_MESSAGE_0	canreg[68]
#define		RX3_MESSAGE_1	canreg[69]
#define		RX3_MESSAGE_2	canreg[70]
#define		RX3_MESSAGE_3	canreg[71]
#define		RX3_MESSAGE_4	canreg[72]
#define		RX3_MESSAGE_5	canreg[73]
#define		RX3_MESSAGE_6	canreg[74]
#define		RX3_MESSAGE_7	canreg[75]
#define		RX3_STATUS		canreg[76]

