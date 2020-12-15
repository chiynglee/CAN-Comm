#include "regc515c.h"
#include "LED.h" 

// P4_6 = CAN TX, P4_7 = CAN RX
void Init_LED()
{
	P4_0 = 1;
	P4_1 = 1;
	P4_2 = 1;
	P4_3 = 1;
	P4_4 = 1;
	P4_5 = 1;
}

