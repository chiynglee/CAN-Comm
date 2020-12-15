#include "regc515c.h"     /* special function register declarations for 89C51XD2 */
#include "timer.h"

void Init_Timer()
{
	EAL = 0;

	TMOD |= 0x02;    // 8-bit timer (8-bit autoload)
	TH0 = 0x37;	// 200 microseconds
	TL0 = TH0;
	ET0 = 1;	

	EAL = 1;
	TR0 = 1;		// timer 0 start
}
