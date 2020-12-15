// CRC.h: interface for the CCRC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRC_H__B98049BA_54C6_4AF7_B2BB_84FCDFCF27FD__INCLUDED_)
#define AFX_CRC_H__B98049BA_54C6_4AF7_B2BB_84FCDFCF27FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCRC  
{
public:
	CCRC();
	virtual ~CCRC();

	unsigned short calc_crc(unsigned char *buf, int len);
	int check_crc(unsigned char *buf, int len);


};

#endif // !defined(AFX_CRC_H__B98049BA_54C6_4AF7_B2BB_84FCDFCF27FD__INCLUDED_)
