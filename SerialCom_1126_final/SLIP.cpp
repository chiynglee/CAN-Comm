// SLIP.cpp: implementation of the CSLIP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialCom.h"
#include "SLIP.h"
#include "CRC.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSLIP::CSLIP()
{
	slip.fescflag = 0;
	slip.state = START_FLAG;
	slip.rxlen = 0;
	slip.rxPtr = NULL;
}

CSLIP::~CSLIP()
{

}

int CSLIP::SLIP_Encode_Frame(unsigned char *in, int len, unsigned char *out)
{
	int size = 0;
	int i;
	unsigned short crc;
	unsigned char trans_crc;
	CCRC m_crc;
	
	
	crc = m_crc.calc_crc(in, len);

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

int CSLIP::SLIP_Decode_Frame(unsigned char c, unsigned char *outbuf)
{
	int rtn_val;
	
	if (slip.fescflag)
	{
		slip.fescflag = 0;
		
		if (c==TFEND) c = FEND;
		else if (c==TFESC) c = FESC;
		else slip.state = START_FLAG;
	}
	else if (c==FESC)
	{
		slip.fescflag = 1;
		return(0);
	}
	else if (c==FEND)
	{
		if (slip.state==START_FLAG)
		{
			slip.rxPtr = outbuf;
			slip.rxlen = 0;
			slip.state = DECODE_FLAG;
			return(0);
		}
		else
		{
			slip.state = START_FLAG;
			rtn_val = slip.rxlen;
			slip.rxPtr = outbuf;
			slip.rxlen = 0;
			return(rtn_val);
		}
	}
	
	if (slip.state==DECODE_FLAG)
	{
		if (slip.rxlen < MAX_SLIP_LENGTH)
		{
			*slip.rxPtr++ = c;
			slip.rxlen++;
		}
		else
		{
			slip.state = START_FLAG;
			rtn_val = slip.rxlen;
			slip.rxPtr = outbuf;
			slip.rxlen = 0;
			return(rtn_val);
		}
	}

	return(0);
}
