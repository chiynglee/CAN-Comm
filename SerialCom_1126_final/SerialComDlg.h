// SerialComDlg.h : header file
//

#if !defined(AFX_SERIALCOMDLG_H__4FB3E547_C3BA_4697_81B7_AB1991BEA61A__INCLUDED_)
#define AFX_SERIALCOMDLG_H__4FB3E547_C3BA_4697_81B7_AB1991BEA61A__INCLUDED_

#include "CommThread.h"	// Added by ClassView
#include "SLIP.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define REQUEST		0x10
#define RESPONSE	0x20

#define NODE_ID		0x01

#define REQUEST_TIME	0x10
#define REQUEST_POS		0x11
#define REQUEST_DIST	0x12
#define REQUEST_ALIVE	0x1F

#define RESPONSE_TIME	0x20
#define RESPONSE_POS	0x21
#define RESPONSE_DIST	0x22
#define RESPONSE_ALIVE	0x2F

#define MAX_NODE_COUNT	3

#define INTERVAL_ALIVE		21000	// 21 second
#define INTERVAL_REQUEST	6000	// 6 second
#define INTERVAL_REPAINTING	20000	// 20 sencond
/////////////////////////////////////////////////////////////////////////////
// CSerialComDlg dialog

class CSerialComDlg :  public CDialog
{
// Construction
public:
	CSerialComDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSerialComDlg)
	enum { IDD = IDD_SERIALCOM_DIALOG };
	CEdit	m_EditReceive;
	CEdit	m_EditSend;
	CEdit	m_EditControl;
	CComboBox	m_cStopBit;
	CComboBox	m_cSerialPort;
	CComboBox	m_cParity;
	CComboBox	m_cDataBit;
	CComboBox	m_cBaudRate;
	int		m_iStopBit;
	int		m_iSerialPort;
	int		m_iParity;
	int		m_iDataBit;
	int		m_iBaudRate;
	CString	m_strControl;
	CString	m_strSend;
	CString	m_strReceive;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialComDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSerialComDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnQuit();
	afx_msg long OnCommunication(WPARAM wParam, LPARAM lParam);// 추가 시킨 내용
	afx_msg void OnPortOpen();
	afx_msg void OnSendData();
	afx_msg void OnReceiveDataClear();
	afx_msg void OnSendDataClear();
	afx_msg void OnPortClose();
	afx_msg void OnControlboxclear();
	afx_msg void OnSelchangeBaudRate();
	afx_msg void OnSelchangeSerialPort();
	afx_msg void OnSelchangeDataBit();
	afx_msg void OnSelchangeStopBit();
	afx_msg void OnSelchangeParity();
	afx_msg void OnBtnRemoteSend();
	afx_msg void OnBtnDataSend();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	CCommThread m_ComuPort;
private:
	CSLIP slip;
	BYTE *Recv_buf;
	CPoint m_pos;
	int m_Stop;
	int m_pntCount;
	int m_drawCount;
	int m_dist;
	int m_Connect[MAX_NODE_COUNT + 1];
	int m_draw;
	COLORREF m_color[MAX_NODE_COUNT + 1];

	int nMakeHexData(CString strSend); //보낼 데이터 타입이 hex일 경우 hex데이터로 만들어서 전송
	BYTE byCode2AsciiValue(char cData);// 문자를 hex값으로 변경 0~F 까지는 그대로 그 외에 글자는 0으로 
	CString byIndexComPort(int xPort);// 포트이름 받기 
	DWORD byIndexBaud(int xBaud);// 전송률 받기
	BYTE byIndexData(int xData);//데이터 비트 받기
	BYTE byIndexStop(int xStop);// 스톱비트 받기 
	BYTE byIndexParity(int xParity);// 펠리티 받기

	void SendRquest(int target_node_id, int command);
	void Request_Display(BYTE *Send_buff, int slip_size);
	void Create_Position(CPoint *p);
	void Create_Distance(int *dist);
	CPoint Modify_Position(CPoint pos, int dist);
	void Display_Position(CPoint p);
	void DrawCross(CDC *pDC, CPoint point);
	void DrawBitmap(CDC *pDC, CPoint point);
	void DrawStatus(int node_id);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALCOMDLG_H__4FB3E547_C3BA_4697_81B7_AB1991BEA61A__INCLUDED_)
