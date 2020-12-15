// SerialComDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialCom.h"
#include "SerialComDlg.h"
#include "SLIP.h"
#include "math.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 HWND hCommWnd;
/////////////////////////////////////////////////////////////////////////////
// CSerialComDlg dialog

CSerialComDlg::CSerialComDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialComDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialComDlg)
	m_iStopBit = 0;
	m_iSerialPort = 0;
	m_iParity = 0;
	m_iDataBit = 3;
	m_iBaudRate = 3;
	m_strControl = _T("");
	m_strSend = _T("");
	m_strReceive = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialComDlg)
	DDX_Control(pDX, IDC_RECEIVE, m_EditReceive);
	DDX_Control(pDX, IDC_SEND, m_EditSend);
	DDX_Control(pDX, IDC_CONTROL, m_EditControl);
	DDX_Control(pDX, IDC_STOP_BIT, m_cStopBit);
	DDX_Control(pDX, IDC_SERIAL_PORT, m_cSerialPort);
	DDX_Control(pDX, IDC_PARITY, m_cParity);
	DDX_Control(pDX, IDC_DATA_BIT, m_cDataBit);
	DDX_Control(pDX, IDC_BAUD_RATE, m_cBaudRate);
	DDX_CBIndex(pDX, IDC_STOP_BIT, m_iStopBit);
	DDX_CBIndex(pDX, IDC_SERIAL_PORT, m_iSerialPort);
	DDX_CBIndex(pDX, IDC_PARITY, m_iParity);
	DDX_CBIndex(pDX, IDC_DATA_BIT, m_iDataBit);
	DDX_CBIndex(pDX, IDC_BAUD_RATE, m_iBaudRate);
	DDX_Text(pDX, IDC_CONTROL, m_strControl);
	DDX_Text(pDX, IDC_SEND, m_strSend);
	DDX_Text(pDX, IDC_RECEIVE, m_strReceive);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialComDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialComDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_MESSAGE(WM_COMM_READ , OnCommunication) //추가
	ON_BN_CLICKED(IDC_PORT_OPEN, OnPortOpen)
	ON_BN_CLICKED(IDC_SEND_DATA, OnSendData)
	ON_BN_CLICKED(IDC_RECEIVE_DATA_CLEAR, OnReceiveDataClear)
	ON_BN_CLICKED(IDC_SEND_DATA_CLEAR, OnSendDataClear)
	ON_BN_CLICKED(IDC_PORT_CLOSE, OnPortClose)
	ON_BN_CLICKED(IDC_CONTROLBOXCLEAR, OnControlboxclear)
	ON_CBN_SELCHANGE(IDC_BAUD_RATE, OnSelchangeBaudRate)
	ON_CBN_SELCHANGE(IDC_SERIAL_PORT, OnSelchangeSerialPort)
	ON_CBN_SELCHANGE(IDC_DATA_BIT, OnSelchangeDataBit)
	ON_CBN_SELCHANGE(IDC_STOP_BIT, OnSelchangeStopBit)
	ON_CBN_SELCHANGE(IDC_PARITY, OnSelchangeParity)
	ON_BN_CLICKED(IDC_BTN_REMOTE_SEND, OnBtnRemoteSend)
	ON_BN_CLICKED(IDC_BTN_DATA_SEND, OnBtnDataSend)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialComDlg message handlers

BOOL CSerialComDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	hCommWnd = m_hWnd;

	int i;

	for(i=0; i<MAX_NODE_COUNT+1; i++){
		m_Connect[i] = 0;
	}

	m_Stop = 1;
	m_pntCount = 0;
	m_drawCount = 0;

	m_pos.x = -1;
	m_pos.y = -1;
	m_dist = -1;

	m_draw = 0;
	
	Recv_buf = new BYTE[MAX_SLIP_LENGTH];
	for(i=0; i<MAX_SLIP_LENGTH; i++)
		Recv_buf[i] = NULL;

	for(i=1; i<MAX_NODE_COUNT+1; i++){
		DrawStatus(i);
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialComDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialComDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void CSerialComDlg::OnQuit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_ComuPort.m_bConnected == TRUE)
		m_ComuPort.ClosePort();

	delete[] Recv_buf;

	CDialog::OnOK();//프로그램 종료
}

void CSerialComDlg::OnPortOpen() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString PortName;
	PortName.Format("OPEN PORT: %s\r\n",byIndexComPort(m_iSerialPort));
	if(m_ComuPort.m_bConnected == FALSE)//포트가 닫혀 있을 경우에만 포트를 열기 위해
	{
		if(m_ComuPort.OpenPort(byIndexComPort(m_iSerialPort), byIndexBaud(m_iBaudRate), byIndexData(m_iDataBit), byIndexStop(m_iStopBit), byIndexParity(m_iParity)) ==TRUE)
		{
			m_EditReceive.SetSel(-1,0);
			m_EditReceive.ReplaceSel(PortName);
		}
	}
	else
	{
		AfxMessageBox("Already Port open");
	}
}



void CSerialComDlg::OnPortClose() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString PortName;
	if(m_ComuPort.m_bConnected == TRUE)
	{	
		m_ComuPort.ClosePort();
		PortName.Format("CLOSE PORT: %s \r\n",byIndexComPort(m_iSerialPort));
		m_EditReceive.SetSel(-1,0);
		m_EditReceive.ReplaceSel(PortName);
	}
	else
	{
		PortName.Format("%s Port not yet open \r\n",byIndexComPort(m_iSerialPort));
		m_EditReceive.SetSel(-1,0);
		m_EditReceive.ReplaceSel(PortName);
	}

	KillTimer(0);
	KillTimer(1);
	KillTimer(2);
}



void CSerialComDlg::OnSelchangeSerialPort() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString change,str;
	m_iSerialPort = m_cSerialPort.GetCurSel();
	m_cSerialPort.GetLBText(m_iSerialPort,str);
	change.Format("ComPort change: %s \r\n",str);
	m_EditReceive.SetSel(-1,0);
	m_EditReceive.ReplaceSel(change);
	
}

void CSerialComDlg::OnSelchangeBaudRate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString change,str;
	m_iBaudRate = m_cBaudRate.GetCurSel();
	if(m_ComuPort.m_bConnected == TRUE)
	{
		m_ComuPort.ClosePort();
		m_ComuPort.OpenPort(byIndexComPort(m_iSerialPort), byIndexBaud(m_iBaudRate), byIndexData(m_iDataBit), byIndexStop(m_iStopBit), byIndexParity(m_iParity));
	}
	m_cBaudRate.GetLBText(m_iBaudRate,str);
	change.Format("BaudRate change: %s \r\n",str);
	m_EditReceive.SetSel(-1,0);
	m_EditReceive.ReplaceSel(change);
}

void CSerialComDlg::OnSelchangeDataBit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString change,str;
	m_iDataBit = m_cDataBit.GetCurSel();
	if(m_ComuPort.m_bConnected == TRUE)
	{
		m_ComuPort.ClosePort();
		m_ComuPort.OpenPort(byIndexComPort(m_iSerialPort), byIndexBaud(m_iBaudRate), byIndexData(m_iDataBit), byIndexStop(m_iStopBit), byIndexParity(m_iParity));
	}
	m_cDataBit.GetLBText(m_iDataBit,str);
	change.Format("DataBit change: %s \r\n",str);
	m_EditReceive.SetSel(-1,0);
	m_EditReceive.ReplaceSel(change);
	
	
}

void CSerialComDlg::OnSelchangeStopBit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString change, str;
	m_iStopBit= m_cStopBit.GetCurSel();
	if(m_ComuPort.m_bConnected == TRUE)
	{
		m_ComuPort.ClosePort();
		m_ComuPort.OpenPort(byIndexComPort(m_iSerialPort), byIndexBaud(m_iBaudRate), byIndexData(m_iDataBit), byIndexStop(m_iStopBit), byIndexParity(m_iParity));
	}
	m_cStopBit.GetLBText(m_iStopBit,str);
	change.Format("StopBit change: %s \r\n",str);
	m_EditReceive.SetSel(-1,0);
	m_EditReceive.ReplaceSel(change);
}

void CSerialComDlg::OnSelchangeParity() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString change,str;
	m_iParity = m_cParity.GetCurSel();
	if(m_ComuPort.m_bConnected == TRUE)
	{
		m_ComuPort.ClosePort();
		m_ComuPort.OpenPort(byIndexComPort(m_iSerialPort), byIndexBaud(m_iBaudRate), byIndexData(m_iDataBit), byIndexStop(m_iStopBit), byIndexParity(m_iParity));
	}
	m_cParity.GetLBText(m_iParity,str);
	change.Format("Parity change: %s \r\n",str);
	m_EditReceive.SetSel(-1,0);
	m_EditReceive.ReplaceSel(change);
	
}

CString CSerialComDlg::byIndexComPort(int xPort)
{
	CString PortName;
	switch(xPort)
	{
		case 0:		PortName = "COM1"; 			break;
		case 1:		PortName = "COM2";			break;
		case 2:		PortName = "COM3"; 			break;
		case 3:		PortName = "COM4";			break;
		case 4:		PortName = "\\\\.\\COM5"; 	break;
		case 5:		PortName = "\\\\.\\COM6";	break;
		case 6:		PortName = "\\\\.\\COM7"; 	break;
		case 7:		PortName = "\\\\.\\COM8";	break;
		case 8:		PortName = "\\\\.\\COM9"; 	break;
		case 9:		PortName = "\\\\.\\COM10";	break;
	}
	
	return PortName;
}

DWORD CSerialComDlg::byIndexBaud(int xBaud)
{
	DWORD dwBaud;
	switch(xBaud)
	{
		case 0:		dwBaud = CBR_4800;		break;
		case 1:		dwBaud = CBR_9600;		break;
		case 2:		dwBaud = CBR_14400;		break;
		case 3:		dwBaud = CBR_19200;		break;
		case 4:		dwBaud = CBR_38400;		break;
		case 5:		dwBaud = CBR_56000;		break;
		case 6:		dwBaud = CBR_57600;		break;
		case 7:		dwBaud = CBR_115200;	break;
	}

	return dwBaud;
}

BYTE CSerialComDlg::byIndexData(int xData)
{
	BYTE byData;
	switch(xData)
	{
		case 0 :	byData = 5;			break;
		case 1 :	byData = 6;			break;
		case 2 :	byData = 7;			break;
		case 3 :	byData = 8;			break;
	}
	return byData;
}

BYTE CSerialComDlg::byIndexStop(int xStop)
{
	BYTE byStop;
	if(xStop == 0)
	{
		byStop = ONESTOPBIT;
	}
	else
	{
		byStop = TWOSTOPBITS;
	}
	return byStop;
}
BYTE CSerialComDlg::byIndexParity(int xParity)
{
	BYTE byParity;
	switch(xParity)
	{
	case 0 :	byParity = NOPARITY;	break;
	case 1 :	byParity = ODDPARITY;	break;
	case 2 :	byParity = EVENPARITY;	break;
	}

	return byParity;
}

BYTE CSerialComDlg::byCode2AsciiValue(char cData)
{
	//이 함수는 char문자를 hex값으로 변경해 주는 함수 입니다.
	BYTE byAsciiValue;
	if( ( '0' <= cData ) && ( cData <='9' ) )
	{
		byAsciiValue = cData - '0';
	}
	else if( ( 'A' <= cData ) && ( cData <= 'F' ) )
	{
		byAsciiValue = (cData - 'A') + 10;
	}
	else if( ( 'a' <= cData ) && ( cData <= 'f' ) )
	{
		byAsciiValue = (cData - 'a') + 10;
	}
	else
	{
		byAsciiValue = 0;
	}
	return byAsciiValue;
}

void CSerialComDlg::OnSendData() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int iSize;

	iSize = nMakeHexData(m_strSend); //데이터를  hex로 보낼 경우
}

int CSerialComDlg::nMakeHexData(CString strSend)
{
	int bufPos = 0;
	int datasize,bufsize, i,j, slip_size;
	BYTE *Send_buff, byHigh, byLow, *slip_Sendbuf;

	CString byGetData = strSend; //컨트롤 맴버 변수를 받는다.
	byGetData.Replace(" ","");// 공백 없애기 
	byGetData.Replace("\r\n","");//엔터 없애기
	datasize = byGetData.GetLength(); // 공백을 없앤 문자열 길이 얻기 
	
	// 문자 길이를 %2로 나눈 값이 0이 아니면 홀수 이기 때문에 마지막 문자를 처리 해줘야 함
	if(datasize %2 == 0)
	{
		bufsize = datasize;
	}
	else
	{
		bufsize = datasize -1; 
	}

	Send_buff = new BYTE[bufsize];
	
	for( i = 0; i < bufsize ; i+=2)
	{
		byHigh = byCode2AsciiValue(byGetData[i]);
		byLow  = byCode2AsciiValue(byGetData[i+1]);
		Send_buff[bufPos++] = (byHigh <<4) | byLow;
	}
	//마지막 문자가 1자리수 일때 처리 하기 위해  예) 1 -> 01
	if(datasize %2 !=0)
	{	
		Send_buff[bufPos++] = byCode2AsciiValue(byGetData[datasize-1]);
	}

	slip_Sendbuf = new BYTE[bufsize+4];

	slip_size = slip.SLIP_Encode_Frame(Send_buff, bufPos, slip_Sendbuf);

	int etc = slip_size % 8;
	//포트에 데이터를 8개씩 쓰기 위해
	//데이터의 길이가 8의 배수가 아니면 나머지 데이터는 따로 보내줌
	for(j =0; j < slip_size - etc; j+= 8)//8의 배수 보냄
	{
		m_ComuPort.WriteComm(&slip_Sendbuf[j], 8) ;
	}
	if(etc != 0)//나머지 데이터 전송
	{
		m_ComuPort.WriteComm(&slip_Sendbuf[slip_size -etc], etc) ;// 포트에 데이터 쓰기 
	}

//----------request display----------------
	Request_Display(Send_buff, slip_size);
//------------------------------------------

	delete [] Send_buff;
	delete [] slip_Sendbuf;
	return bufPos;
}

long CSerialComDlg::OnCommunication(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);//받는 데이터 타입을 알기 위해
	CString str = "";
	CString result;
	BYTE aByte; //데이터를 저장할 변수 
	int iSize =  (m_ComuPort.m_QueueRead).GetSize(); //포트로 들어온 데이터 갯수
									//SetCommState에 설정된 내용 때문에 거의 8개씩 들어옴
	int slip_size = 0, count = 0;

	for(int i = 0 ; i < iSize; i++)//들어온 갯수 만큼 데이터를 읽어 와 화면에 보여줌
	{
		(m_ComuPort.m_QueueRead).GetByte(&aByte);//큐에서 데이터 한개를 읽어옴
		slip_size = slip.SLIP_Decode_Frame(aByte, Recv_buf);
	}

	if(slip_size < 2)
		return 1;

//------------------receive data display--------------
	for(i = 0; i < slip_size - 2; i++){
		str.Format("%02X ", Recv_buf[i]);
		result += str;
	}
	result += _T("\r\n");

	m_EditControl.SetSel(-1,0);
	m_EditControl.ReplaceSel(result);//화면에 받은 데이터 보여줌

	CString operation;
	operation.Empty();
	
	switch(Recv_buf[0]){
	case 0x00 : operation += _T("All Node : ");
		break;
	case 0x01 : operation += _T("Node 1 : ");
		break;
	case 0x02 : operation += _T("Node 2 : ");
		break;
	case 0x03 : operation += _T("Node 3 : ");
		break;
	}

	CString str_send, rdata, sdata;
	CPoint pos;

	switch(Recv_buf[1]){
	case 0x1F : 
		m_Connect[Recv_buf[0]] = 1;
		str_send.Format("%02X %02X %02X %02X", RESPONSE, Recv_buf[0], NODE_ID, RESPONSE_ALIVE);
		nMakeHexData(str_send);
		break;
	case 0x11 : 
		operation += _T("Request Position \r\n");
		m_EditReceive.SetSel(-1,0);
		m_EditReceive.ReplaceSel(operation);//화면에 받은 데이터 보여줌
		
		int x, y;
		Create_Position(&pos);
		x = pos.x - 605;
		y = pos.y - 305;
		str_send.Format("%02X %02X %02X %02X %02X %02X", 
			RESPONSE, Recv_buf[0], NODE_ID, RESPONSE_POS, x, y);
		nMakeHexData(str_send);
		break;
	case 0x12 : 
		operation += _T("Request Distance \r\n");
		m_EditReceive.SetSel(-1,0);
		m_EditReceive.ReplaceSel(operation);//화면에 받은 데이터 보여줌

		int dist;
		Create_Distance(&dist);
		str_send.Format("%02X %02X %02X %02X %02X", 
			RESPONSE, Recv_buf[0], NODE_ID, RESPONSE_DIST, dist);
		nMakeHexData(str_send);
		break;
	case 0x2f : 
		if(m_Connect[Recv_buf[0]] != 1){
			m_Connect[Recv_buf[0]] = 1;
			DrawStatus(Recv_buf[0]);
			SetTimer(Recv_buf[0], INTERVAL_ALIVE, NULL);
		}
		else if(m_Connect[Recv_buf[0]] == 1){
			KillTimer(Recv_buf[0]);
			SetTimer(Recv_buf[0], INTERVAL_ALIVE, NULL);
		}
		// modify
		rdata.Empty();
		rdata.Format("<----- Node %d is Alive ----->\r\n", Recv_buf[0]);
		operation += rdata;
		m_EditReceive.SetSel(-1,0);
		m_EditReceive.ReplaceSel(operation);//화면에 받은 데이터 보여줌
		break;
	case 0x21 : 
		rdata.Empty();
		rdata.Format("Response Position x 좌표 : %d, y 좌표 : %d \r\n", Recv_buf[2], Recv_buf[3]);
		operation += rdata;
		m_EditReceive.SetSel(-1,0);
		m_EditReceive.ReplaceSel(operation);//화면에 받은 데이터 보여줌

		m_pos.x = Recv_buf[2] + 605;
		m_pos.y = Recv_buf[3] + 305;
		Display_Position(m_pos);
		if(m_Connect[Recv_buf[0] + 1] != 0)
			SendRquest(Recv_buf[0] + 1, REQUEST_DIST);
		m_draw = 1;
		break;
	case 0x22 : 
		rdata.Empty();
		rdata.Format("Response Distance 중심으로부터의 거리 : %d \r\n", Recv_buf[2]);
		operation += rdata;
		m_EditReceive.SetSel(-1,0);
		m_EditReceive.ReplaceSel(operation);//화면에 받은 데이터 보여줌
		
		CPoint position;
		m_dist = Recv_buf[2];
		position = Modify_Position(m_pos, m_dist);
		Display_Position(position);	
		m_draw = 0;
		break;
	}

	for(i=0; i<slip_size; i++)
		Recv_buf[i] = NULL;

	return 1;
}



void CSerialComDlg::OnReceiveDataClear() 
{
	// TODO: Add your control notification handler code here
	//이런곳은 굳이 주석을 붙일 필요가 없겠죠?
	m_EditReceive.SetSel(0, -1);
	m_EditReceive.Clear();
	m_strReceive.Empty();
}

void CSerialComDlg::OnSendDataClear() 
{
	// TODO: Add your control notification handler code here
	m_EditSend.SetSel(0,-1);
	m_EditSend.Clear();
	m_strSend.Empty();
	m_EditSend.SetFocus();	
}

void CSerialComDlg::OnControlboxclear() 
{
	// TODO: Add your control notification handler code here
	m_EditControl.SetSel(0,-1);
	m_EditControl.Clear();
	m_strControl.Empty();
}

// Communication start : function name과 관련 없음.
void CSerialComDlg::OnBtnRemoteSend() 
{
	// TODO: Add your control notification handler code here
	int count = 100;
	m_Stop = 0;
	
	for(int i=0; i<MAX_NODE_COUNT; i++)
		m_Connect[i] = 0;
	SendRquest(0x00, REQUEST_ALIVE);

	SetTimer(0, INTERVAL_REQUEST, NULL);
	SetTimer(1, INTERVAL_ALIVE, NULL);
	SetTimer(2, INTERVAL_ALIVE, NULL);
}

void CSerialComDlg::SendRquest(int target_node_id, int command){
	CString byGetData;
	
	byGetData.Format("10 %02X %02X %02X", target_node_id, NODE_ID, command);
	nMakeHexData(byGetData);
}

// Communication stop : function name과 관련 없음.
void CSerialComDlg::OnBtnDataSend() 
{
	// TODO: Add your control notification handler code here
	m_Stop = 1;

	KillTimer(0);
	KillTimer(1);
	KillTimer(2);
}

void CSerialComDlg::Request_Display(BYTE *Send_buff, int slip_size){
	int i;
	CString operation, tdata, result;

	operation.Empty();
	
	switch(Send_buff[2]){
	case 0x00 : operation += _T("All Node : ");
		break;
	case 0x01 : operation += _T("Node 1 : ");
		break;
	case 0x02 : operation += _T("Node 2 : ");
		break;
	case 0x03 : operation += _T("Node 3 : ");
		break;
	}

	if(Send_buff[3] & 0x10){
		operation += _T("Request ");
	}
	else if(Send_buff[3] & 0x20){
		operation += _T("Response ");
	}

	switch(Send_buff[3] & 0x0F){
	case 0x00 : operation += _T("Time ");
		break;
	case 0x01 : operation += _T("Position ");
		break;
	case 0x02 : operation += _T("Distance ");
		break;
	case 0x0F : operation += _T("Alive ");
		break;
	}

	switch(Send_buff[1]){
	case 0x00 : operation += _T("( to All Node ) \r\n");
		break;
	case 0x01 : operation += _T("( to Node 1 ) \r\n");
		break;
	case 0x02 : operation += _T("( to Node 2 ) \r\n");
		break;
	case 0x03 : operation += _T("( to Node 3 ) \r\n");
		break;
	}
	
	tdata.Empty();
	for(i = 0; i < slip_size - 2; i++){
		tdata.Format("%02X ", Send_buff[i]);
		result += tdata;

		Send_buff[i] = NULL;
	}
	result += _T("\r\n");

	m_EditReceive.SetSel(-1,0);
	m_EditReceive.ReplaceSel(operation);//화면에 받은 데이터 보여줌
	
	m_EditControl.SetSel(-1,0);
	m_EditControl.ReplaceSel(result);//화면에 받은 데이터 보여줌
}

void CSerialComDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0){
		m_drawCount++;
		if(m_Stop == 0){
			if(m_Connect[2] != 0)
				SendRquest(0x02, REQUEST_POS);
			else if(m_Connect[3] != 0)
				SendRquest(0x03, REQUEST_DIST);
		}
		if(m_drawCount >= 3){	// test
			for(int i=1; i<MAX_NODE_COUNT+1; i++)
				DrawStatus(i);
		}
	}
	else{
		m_Connect[nIDEvent] = 0;
		DrawStatus(nIDEvent + 1);
		KillTimer(nIDEvent);
	}

	CDialog::OnTimer(nIDEvent);
}

void CSerialComDlg::Create_Position(CPoint *p){
	int x, y;
	
	if(m_pntCount < 6){
		x = 731 + (21 * m_pntCount);
		y = 305 + (21 * m_pntCount);
	}
	else if(m_pntCount >= 6 && m_pntCount < 12){
		x = 857 - (21 * (m_pntCount - 6));
		y = 431 + (21 * (m_pntCount - 6));
	}
	else if(m_pntCount >= 12 && m_pntCount < 18){
		x = 731 - (21 * (m_pntCount - 12));
		y = 557 - (21 * (m_pntCount - 12));
	}
	else if(m_pntCount >= 18 && m_pntCount < 24){
		x = 605 + (21 * (m_pntCount - 18));
		y = 431 - (21 * (m_pntCount - 18));
	}
	
	m_pntCount++;
	if(m_pntCount >= 24)
		m_pntCount = 0;
	
	if(x >= 565 && x <= 895){
		if(y >= 265 && y <= 595){
			p->x = x;
			p->y = y;
		}
	}
}

void CSerialComDlg::Create_Distance(int *dist){
	CPoint center;

	center.x = (857 - 605) / 2;
	center.y = (557 - 305) / 2;

	*dist = center.x - 605;		// application의 좌표계의 중심으로부터의 거리
}

CPoint CSerialComDlg::Modify_Position(CPoint pos, int dist){
	CPoint position, center;
	int now_dist;
	int dsub = 0;

	center.x = (857 - 605) / 2 + 605;
	center.y = (557 - 305) / 2 + 305;
	
	now_dist = (int)(sqrt(pow((pos.x - center.x), 2.0) + pow((pos.y - center.y), 2.0)));

	dsub = abs(dist - now_dist);

	if(pos.x >= 731 && pos.y <=431){
		position.x = pos.x + (int)(dsub / sqrt(2));
		position.y = pos.y - (int)(dsub / sqrt(2));
	}
	else if(pos.x >= 731 && pos.y >= 431){
		position.x = pos.x + (int)(dsub / sqrt(2));
		position.y = pos.y + (int)(dsub / sqrt(2));
	}
	else if(pos.x <= 731 && pos.y >= 431){
		position.x = pos.x - (int)(dsub / sqrt(2));
		position.y = pos.y + (int)(dsub / sqrt(2));
	}
	else if(pos.x <= 731 && pos.y <= 431){
		position.x = pos.x - (int)(dsub / sqrt(2));
		position.y = pos.y - (int)(dsub / sqrt(2));
	}

	return position;
}

void CSerialComDlg::Display_Position(CPoint p){
	CString pos;
	CClientDC cdc(this);

	pos.Format("< 현재 좌표 : x = %d / y = %d >\r\n", p.x, p.y);

	m_EditReceive.SetSel(-1,0);
	m_EditReceive.ReplaceSel(pos);//화면에 받은 데이터 보여줌

	if(m_draw == 0)
		DrawCross(&cdc, p);
	else
		DrawBitmap(&cdc, p);
}

void CSerialComDlg::DrawBitmap(CDC *pDC, CPoint point){
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);

	CBitmap bitmap, *oldbitmap;
	bitmap.LoadBitmap(IDB_BMP_SAT);
	oldbitmap = memdc.SelectObject(&bitmap);

	pDC->BitBlt(point.x, point.y, 450, 85, &memdc, 0, 0, SRCCOPY);

	memdc.SelectObject(oldbitmap);
}

//십자가를 그려주는 합수
void CSerialComDlg::DrawCross(CDC *pDC, CPoint point)
{
	pDC->MoveTo(point.x-5,point.y-5);
	pDC->LineTo(point.x+5,point.y+5);
	pDC->MoveTo(point.x-5,point.y+5);
	pDC->LineTo(point.x+5,point.y-5);
}

void CSerialComDlg::DrawStatus(int node_id){
	CRect rectView, pre_rectView;
	COLORREF color = RGB(255, 255, 255);
	
	if(node_id != NODE_ID){
		if(m_Connect[node_id] == 1){
			switch(node_id){
			case 1 : 
				GetDlgItem(IDC_DEAD_NODE1)->GetWindowRect(&pre_rectView);
				ScreenToClient(&pre_rectView);
				InvalidateRect(&pre_rectView);

				GetDlgItem(IDC_ALIVE_NODE1)->GetWindowRect(&rectView);
				break;
			case 2 : 
				GetDlgItem(IDC_DEAD_NODE2)->GetWindowRect(&pre_rectView);
				ScreenToClient(&pre_rectView);
				InvalidateRect(&pre_rectView);

				GetDlgItem(IDC_ALIVE_NODE2)->GetWindowRect(&rectView);
				break;
			case 3 : 
				GetDlgItem(IDC_DEAD_NODE3)->GetWindowRect(&pre_rectView);
				ScreenToClient(&pre_rectView);
				InvalidateRect(&pre_rectView);

				GetDlgItem(IDC_ALIVE_NODE3)->GetWindowRect(&rectView);
				break;
			}
		}
		else{
			switch(node_id){
			case 1 : 
				GetDlgItem(IDC_ALIVE_NODE1)->GetWindowRect(&pre_rectView);
				ScreenToClient(&pre_rectView);
				InvalidateRect(&pre_rectView);

				GetDlgItem(IDC_DEAD_NODE1)->GetWindowRect(&rectView);
				break;
			case 2 : 
				GetDlgItem(IDC_ALIVE_NODE2)->GetWindowRect(&pre_rectView);
				ScreenToClient(&pre_rectView);
				InvalidateRect(&pre_rectView);

				GetDlgItem(IDC_DEAD_NODE2)->GetWindowRect(&rectView);
				break;
			case 3 : 
				GetDlgItem(IDC_ALIVE_NODE3)->GetWindowRect(&pre_rectView);
				ScreenToClient(&pre_rectView);
				InvalidateRect(&pre_rectView);

				GetDlgItem(IDC_DEAD_NODE3)->GetWindowRect(&rectView);
				break;
			}
		}
		ScreenToClient(&rectView);

		CClientDC dc(this);
		CBrush newBrush, *oldBrush;

		m_color[node_id] = RGB(0, 255, 0);

		newBrush.CreateSolidBrush(m_color[node_id]);
		oldBrush = dc.SelectObject(&newBrush);

		dc.Rectangle(&rectView);

		dc.SelectObject(&oldBrush);
		newBrush.DeleteObject();
	}
}
