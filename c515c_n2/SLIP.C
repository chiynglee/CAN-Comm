#include "slip.h"
#include "crc.h"

#define FEND		0xC0
#define FESC		0xDB
#define TFEND		0xDC
#define TFESC		0xDD

#define START_FLAG		0x0
#define DECODE_FLAG		0x1

#define MAX_SLIP_LENGTH		256

static int fescflag = 0;	/* received FESC flag */
static int state = START_FLAG;
static int rxlen;
static unsigned char *rxPtr;

int SLIP_Encode_Frame(unsigned char *in, int len, unsigned char *out)
{
	int size = 0;
	int i;
	unsigned short crc;
	unsigned char trans_crc; 
	
	
	crc = calc_crc(in, len);

	*out++ = FEND;
	size = 1;

	for (i=0;i<len;i++)
	{
		switch (*in)
		{
			case FEND:
				*out++ = FESC;
				*out++ = TFEND;
				size++;
				break;
			case FESC :
				*out++ = FESC;
				*out++ = TFESC;
				size++;
				break;
			default :
				*out++ = *in;
				break;
		}
		size++;
		in++;
	}
	
	trans_crc = (unsigned char)(crc >> 8);
	switch (trans_crc)
	{
		case FEND:
			*out++ = FESC;
			*out++ = TFEND;
			size++;
			break;
		case FESC :
			*out++ = FESC;
			*out++ = TFESC;
			size++;
			break;
		default :
			*out++ = trans_crc;
			break;
	}

 	trans_crc = (unsigned char)(crc);
	switch (trans_crc)
	{
		case FEND:
			*out++ = FESC;
			*out++ = TFEND;
			size++;
			break;
		case FESC :
			*out++ = FESC;
			*out++ = TFESC;
			size++;
			break;
		default :
			*out++ = trans_crc;
			break;
	}

    *out++ = FEND;
	return size+3;
}

int SLIP_Decode_Frame(unsigned char c, unsigned char *outbuf)
{
	int rtn_val;

	if (fescflag)
	{
		fescflag = 0;
		if (c==TFEND) c = FEND;
		else if (c==TFESC) c = FESC;
		else state = START_FLAG;
	}
	else if (c==FESC)
	{
		fescflag = 1;
		return(0);
	}
	else if (c==FEND)  //C0
	{
		if (state==START_FLAG)
		{
			rxPtr = outbuf;
			rxlen = 0;
			state = DECODE_FLAG;
			return(0);
		}
		else
		{
			rtn_val = rxlen;
			state = START_FLAG;
			return rxlen;
		}
	}

	if (state==DECODE_FLAG)
	{
		*rxPtr++ = c;
		rxlen++;
	}

	return(0);
}

