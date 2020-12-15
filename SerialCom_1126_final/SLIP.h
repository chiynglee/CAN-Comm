// SLIP.h: interface for the CSLIP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLIP_H__1A4CDC07_96F5_47A6_989F_8ED2BA465491__INCLUDED_)
#define AFX_SLIP_H__1A4CDC07_96F5_47A6_989F_8ED2BA465491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FEND		0xC0
#define FESC		0xDB
#define TFEND		0xDC
#define TFESC		0xDD

#define START_FLAG		0x0
#define DECODE_FLAG		0x1

#define MAX_SLIP_LENGTH		256

typedef struct
{
	int fescflag;	/* received FESC flag */
	int state;
	int rxlen;
	unsigned char *rxPtr;
} SLIP_STATUS;

class CSLIP  
{
public:
	CSLIP();
	virtual ~CSLIP();

	int SLIP_Encode_Frame(unsigned char *in, int len, unsigned char *out);
	int SLIP_Decode_Frame(unsigned char c, unsigned char *outbuf);

private :
	SLIP_STATUS slip;
};

#endif // !defined(AFX_SLIP_H__1A4CDC07_96F5_47A6_989F_8ED2BA465491__INCLUDED_)
