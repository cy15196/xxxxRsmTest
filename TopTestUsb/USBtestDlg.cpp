//---------------------------------------------------------------------------
//
// Copyright (C) 2001 Don Powrie, DLP Design, All Rights Reserved.
// 
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this source code to use it without restriction, including without 
// limitation the rights to use, copy, modify, merge, publish, distribute, 
// sublicense, and/or sell copies of the Software, and to permit persons to 
// whom the source code is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL DLP DESIGN BE LIABLE FOR ANY CLAIM, DAMAGES 
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
// OTHER DEALINGS IN THE SOFTWARE.
// 
//---------------------------------------------------------------------------

// USBtestDlg.cpp : implementation file
#include "stdafx.h"
#include "USBtest.h"
#include "USBtestDlg.h"
#include "ftd2xx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//globals
int slide_pos;
int running;
int board_present;
int bus_busy;


// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSBtestDlg dialog

CUSBtestDlg::CUSBtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUSBtestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUSBtestDlg)
	m_PortStatus = _T("");
	m_EchoRes = _T("");
	m_NumRecd = 0;
	m_128status = _T("");
	m_NameNmbr = _T("Enter device Description or Serial Number here.");
	m_SerDesc = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUSBtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUSBtestDlg)
	DDX_Control(pDX, IDC_SLIDER_WRITE_RATE, m_Slide);
	DDX_Control(pDX, IDC_LIST1, m_Received);
	DDX_Text(pDX, IDC_EDIT_PORT_STATUS, m_PortStatus);
	DDX_Text(pDX, IDC_EDIT_ECHO_RESULT, m_EchoRes);
	DDX_Text(pDX, IDC_EDIT_NUM_RECD, m_NumRecd);
	DDX_Text(pDX, IDC_EDIT_128_STATUS, m_128status);
	DDX_Text(pDX, IDC_EDIT_NAME_NUMBER, m_NameNmbr);
	DDX_Radio(pDX, IDC_RADIO_NAME_NUM, m_SerDesc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUSBtestDlg, CDialog)
	//{{AFX_MSG_MAP(CUSBtestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_WRITE_RATE, OnReleasedcaptureSliderWriteRate)
	ON_BN_CLICKED(IDC_BUTTON_SEND128_ECHO, OnButtonSend64Echo)
	ON_BN_CLICKED(IDC_BUTTON_SEND_RECDIS, OnButtonSendRecdis)
	ON_BN_CLICKED(IDC_BUTTON_BLINK_ON, OnButtonBlinkOn)
	ON_BN_CLICKED(IDC_BUTTON_BLINK_OFF, OnButtonBlinkOff)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_HELLO, OnButtonHello)
	ON_BN_CLICKED(IDC_RADIO_NAME_NUM, OnRadioNameNum)
	ON_BN_CLICKED(IDC_RADIO_SERNUM, OnRadioSernum)
	ON_EN_CHANGE(IDC_EDIT_NAME_NUMBER, OnChangeEditNameNumber)
	ON_BN_CLICKED(IDC_RADIO_DEVNO, OnRadioDevno)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSBtestDlg message handlers



void CUSBtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUSBtestDlg::OnPaint() 
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
HCURSOR CUSBtestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}




//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************
BOOL CUSBtestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	buttons(OFF);//turn em off till a port is active
	bus_busy=0;
	board_present=0;
	SetTimer(15, 100, NULL);//check for new USB data in the buffer every 100mS
	m_Slide.SetRange(0, 500, TRUE);

	//read the last settings from the registry
	m_SerDesc = AfxGetApp()->GetProfileInt("MyUSBTestAp", "SerDesc", 0);//default to Description

	if(m_SerDesc==0)
		m_NameNmbr = AfxGetApp()->GetProfileString("MyUSBTestAp", "DescString", "Enter device Description or Serial Number here.");
	if(m_SerDesc==1)
		m_NameNmbr = AfxGetApp()->GetProfileString("MyUSBTestAp", "SerialString", "Enter device Description or Serial Number here.");
	if(m_SerDesc==2)
		m_NameNmbr = AfxGetApp()->GetProfileString("MyUSBTestAp", "DevNmbr", "Enter device Description or Serial Number here.");

	UpdateData(FALSE);


	Loadem();//load DLL and test for presense of DLP-USB1 and DLP-USB2

	return TRUE;  // return TRUE  unless you set the focus to a control
}



//****************************************************************************
void CUSBtestDlg::OnButtonBlinkOn() 
{
	if(!board_present)
		return;

	buttons(OFF);

	//turn the 'OFF' button and slider back on
	GetDlgItem(IDC_BUTTON_BLINK_OFF)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLIDER_WRITE_RATE)->EnableWindow(TRUE);

	//turn off the exit button
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	running=1;

	//start thread 
	AfxBeginThread(ThreadProc1, this, THREAD_PRIORITY_NORMAL);
}


//****************************************************************************
UINT ThreadProc1(LPVOID param)//count up thread
{
	//typecast the handle to the parent window
	CUSBtestDlg *pMyHndl = (CUSBtestDlg *)param;

	unsigned char tx[65], cnt=0;
	DWORD ret_bytes;
	FT_STATUS status;

	while(running)
	{
		//sleep for requested duration
		if(slide_pos <500)
		{
			Sleep(500-slide_pos);
			//send binary count to the LEDs
			tx[0]=0x40;
			tx[0] |= (cnt & 0x0f);
			
			status = pMyHndl->Write(tx, 1, &ret_bytes);			
			if((status != FT_OK) && (status != FT_IO_ERROR))//if error during write then terminate thread
				running=0;

			cnt++;	
		}
		else
		{
			cnt=0;

			for(int t=0; t<64; t++)
			{
				tx[t]=0x40;
				tx[t] |= (cnt++ & 0x0f);
			}

			status = pMyHndl->Write(tx, 64, &ret_bytes);
			if((status != FT_OK) && (status != FT_IO_ERROR))//if error during write (other than timeout) then terminate thread
				running=0;

			Sleep(1);
		}
	}

	return 0;
}





//***********************************************************************
void CUSBtestDlg::buttons(int onoff)
{
	if(onoff == OFF)
	{
		GetDlgItem(IDC_BUTTON_SEND_RECDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SEND128_ECHO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_BLINK_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_BLINK_OFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_WRITE_RATE)->EnableWindow(FALSE);
	}

	if(onoff == ON)
	{
		GetDlgItem(IDC_BUTTON_SEND_RECDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SEND128_ECHO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_BLINK_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_BLINK_OFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_WRITE_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_HELLO)->EnableWindow(TRUE);
	}
}



//**********************************************************************************
void CUSBtestDlg::OnOK() 
{
	//terminate the thread
	running=0;
	Close();
	FreeLibrary(m_hmodule);
	
	CDialog::OnOK();
}

//**********************************************************************************
void CUSBtestDlg::OnReleasedcaptureSliderWriteRate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	slide_pos = m_Slide.GetPos();

	*pResult = 0;
}



//**********************************************************************************
void CUSBtestDlg::OnButtonSend64Echo() 
{
	//send 64 bytes and verify each is echoed back correctly
	
	unsigned char tx[3], rx[3];
	int t;
	BOOL err;
	DWORD ret_bytes;

	if(!board_present)
		return;

	bus_busy=1;//keep the timer function from grabbing return data

	buttons(OFF);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	UpdateData(TRUE);
	m_EchoRes = _T("Testing...");
	UpdateData(FALSE);
	UpdateWindow();	

	//clear the ports of any data
	Purge(FT_PURGE_RX || FT_PURGE_TX);

	err=0;
	m_EchoRes = _T("--PASS--");
	for(t=0; t<64; t++)
	{
		//write it
		rx[0]=0;
		tx[0] = 0x80;
		tx[0] |= (t & 0x0f);
		Write(tx, 1, &ret_bytes);

		//read it
		Read(rx, 1, &ret_bytes);
		if(ret_bytes==0)
			Read(rx, 1, &ret_bytes);

		if(rx[0] != tx[0])
			{
				err=1;
				break;
			}		
	}
	
	if(err==1) m_EchoRes = _T("--FAIL--");

	UpdateData(FALSE);
	UpdateWindow();	

	bus_busy=0;

	buttons(ON);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}



//*******************************************************************
void CUSBtestDlg::OnTimer(UINT nIDEvent) 
{
	unsigned char rx[600];
	DWORD ret_bytes;
	DWORD bytes_in_buf;
	FT_STATUS status;

	if(!board_present)
		return;

	if(bus_busy)	//don't interfere with another test
		return;

	status = GetQueueStatus(&bytes_in_buf);
	if(status != FT_OK)
	{
		UpdateData(TRUE);
		m_PortStatus = _T("DLP-USB1 not open.");
		OnButtonBlinkOff();
		Close();
		board_present=0;
		UpdateData(FALSE);
		UpdateWindow();

		bytes_in_buf = 0;
	}

	if(bytes_in_buf)
	{
		status = Read(rx, bytes_in_buf<600 ? bytes_in_buf : 600, &ret_bytes);
		if((status == FT_OK) || ((status == FT_IO_ERROR) && (ret_bytes > 0)) /* Read timeout */)
		{
			if(ret_bytes)
			{
				CString str;
				UpdateData(TRUE);
				for(DWORD x=0; x<ret_bytes; x++)
				{
					m_NumRecd++;
					str.Format("%d  0x%.2X", rx[x], rx[x]);
					m_Received.AddString(str);		
				}
			}
		}

		UpdateData(FALSE);
		UpdateWindow();
	}

	CDialog::OnTimer(nIDEvent);
}





//****************************************************************************
void CUSBtestDlg::OnButtonBlinkOff() 
{
	running=0;
	buttons(ON);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}




//****************************************************************************
BOOL CUSBtestDlg::OnCommand(WPARAM wParam, LPARAM lParam) //disable the ESC key
{
	if(wParam==2) return FALSE;
	return CDialog::OnCommand(wParam, lParam);
}



//**********************************************************************************
void CUSBtestDlg::OnButtonSendRecdis() 
{
	unsigned char tx[130];
	DWORD ret_bytes;

	if(!board_present)
		return;

	UpdateData(TRUE);
	m_128status = _T("Sending...");
	UpdateData(FALSE);
	UpdateWindow();

	bus_busy=1;
	buttons(OFF);

	for(int x=0; x<128; x++)
	{
		tx[x] = 0x20;
		tx[x] |= (x & 0x0f);
	}

	Write(tx, 128, &ret_bytes);

	bus_busy=0;

	UpdateData(TRUE);
	m_128status = _T("Done");
	UpdateData(FALSE);
	UpdateWindow();

	buttons(ON);

}



//*******************************************************************
void CUSBtestDlg::OnButtonClear() 
{
		UpdateData(TRUE);
		m_NumRecd=0;
		m_Received.ResetContent();
		UpdateData(FALSE);
		UpdateWindow();
}



//****************************************************************************************
void CUSBtestDlg::LoadDLL()
{
	m_hmodule = LoadLibrary("Ftd2xx.dll");	
	if(m_hmodule == NULL)
	{
		AfxMessageBox("Error: Can't Load ft8u245.dll");
		return;
	}

	m_pWrite = (PtrToWrite)GetProcAddress(m_hmodule, "FT_Write");
	if (m_pWrite == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_Write");
		return;
	}

	m_pRead = (PtrToRead)GetProcAddress(m_hmodule, "FT_Read");
	if (m_pRead == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_Read");
		return;
	}

	m_pOpen = (PtrToOpen)GetProcAddress(m_hmodule, "FT_Open");
	if (m_pOpen == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_Open");
		return;
	}

	m_pOpenEx = (PtrToOpenEx)GetProcAddress(m_hmodule, "FT_OpenEx");
	if (m_pOpenEx == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_OpenEx");
		return;
	}

	m_pListDevices = (PtrToListDevices)GetProcAddress(m_hmodule, "FT_ListDevices");
	if(m_pListDevices == NULL)
		{
			AfxMessageBox("Error: Can't Find FT_ListDevices");
			return;
		}

	m_pClose = (PtrToClose)GetProcAddress(m_hmodule, "FT_Close");
	if (m_pClose == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_Close");
		return;
	}

	m_pResetDevice = (PtrToResetDevice)GetProcAddress(m_hmodule, "FT_ResetDevice");
	if (m_pResetDevice == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_ResetDevice");
		return;
	}

	m_pPurge = (PtrToPurge)GetProcAddress(m_hmodule, "FT_Purge");
	if (m_pPurge == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_Purge");
		return;
	}

	m_pSetTimeouts = (PtrToSetTimeouts)GetProcAddress(m_hmodule, "FT_SetTimeouts");
	if (m_pSetTimeouts == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_SetTimeouts");
		return;
	}

	m_pGetQueueStatus = (PtrToGetQueueStatus)GetProcAddress(m_hmodule, "FT_GetQueueStatus");
	if (m_pGetQueueStatus == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_GetQueueStatus");
		return;
	}
}	


//****************************************************************************************
FT_STATUS CUSBtestDlg::Read(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytesRead)
{
	if (!m_pRead)
	{
		AfxMessageBox("FT_Read is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pRead)(m_ftHandle, lpvBuffer, dwBuffSize, lpdwBytesRead);
}	


//****************************************************************************************
FT_STATUS CUSBtestDlg::Write(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytes)
{
	if (!m_pWrite)
	{
		AfxMessageBox("FT_Write is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pWrite)(m_ftHandle, lpvBuffer, dwBuffSize, lpdwBytes);
}	





//****************************************************************************************
FT_STATUS CUSBtestDlg::Open(PVOID pvDevice)
{
	if (!m_pOpen)
	{
		AfxMessageBox("FT_Open is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pOpen)(pvDevice, &m_ftHandle );
}	

//****************************************************************************************
FT_STATUS CUSBtestDlg::OpenEx(PVOID pArg1, DWORD dwFlags)
{
	if (!m_pOpenEx)
	{
		AfxMessageBox("FT_OpenEx is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pOpenEx)(pArg1, dwFlags, &m_ftHandle);
}	


//****************************************************************************************
FT_STATUS CUSBtestDlg::ListDevices(PVOID pArg1, PVOID pArg2, DWORD dwFlags)
{
	if (!m_pListDevices)
	{
		AfxMessageBox("FT_ListDevices is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pListDevices)(pArg1, pArg2, dwFlags);
}	


//****************************************************************************************
FT_STATUS CUSBtestDlg::Close()
{
	if (!m_pClose)
	{
		AfxMessageBox("FT_Close is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pClose)(m_ftHandle);
}	



//****************************************************************************************
FT_STATUS CUSBtestDlg::ResetDevice()
{
	if (!m_pResetDevice)
	{
		AfxMessageBox("FT_ResetDevice is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pResetDevice)(m_ftHandle);
}	



//****************************************************************************************
FT_STATUS CUSBtestDlg::Purge(ULONG dwMask)
{
	if (!m_pPurge)
	{
		AfxMessageBox("FT_Purge is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pPurge)(m_ftHandle, dwMask);
}	



//****************************************************************************************
FT_STATUS CUSBtestDlg::SetTimeouts(ULONG dwReadTimeout, ULONG dwWriteTimeout)
{
	if (!m_pSetTimeouts)
	{
		AfxMessageBox("FT_SetTimeouts is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pSetTimeouts)(m_ftHandle, dwReadTimeout, dwWriteTimeout);
}	


//****************************************************************************************
FT_STATUS CUSBtestDlg::GetQueueStatus(LPDWORD lpdwAmountInRxQueue)
{
	if (!m_pGetQueueStatus)
	{
		AfxMessageBox("FT_GetQueueStatus is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pGetQueueStatus)(m_ftHandle, lpdwAmountInRxQueue);
}	






//****************************************************************************************
void CUSBtestDlg::OnRadioNameNum() 
{
	CWinApp* pApp = AfxGetApp();  
	//write the new setup to the registry
	pApp->WriteProfileInt("MyUSBTestAp", "SerDesc", 0);

	UpdateData(TRUE);
	m_NameNmbr = AfxGetApp()->GetProfileString("MyUSBTestAp", "DescString", "Enter device Description or Serial Number here.");
	UpdateData(FALSE);
	UpdateWindow();
}

//****************************************************************************************
void CUSBtestDlg::OnRadioSernum() 
{
	CWinApp* pApp = AfxGetApp();  
	//write the new setup to the registry
	pApp->WriteProfileInt("MyUSBTestAp", "SerDesc", 1);

	UpdateData(TRUE);
	m_NameNmbr = AfxGetApp()->GetProfileString("MyUSBTestAp", "SerialString", "Enter device Description or Serial Number here.");
	UpdateData(FALSE);
	UpdateWindow();
}

//****************************************************************************************
void CUSBtestDlg::OnRadioDevno() 
{
	CWinApp* pApp = AfxGetApp();  
	//write the new setup to the registry
	pApp->WriteProfileInt("MyUSBTestAp", "SerDesc", 2);

	UpdateData(TRUE);
	m_NameNmbr = AfxGetApp()->GetProfileString("MyUSBTestAp", "DevNmbr", "Enter device Description or Serial Number here.");
	UpdateData(FALSE);
	UpdateWindow();
}


//****************************************************************************************
void CUSBtestDlg::OnChangeEditNameNumber() 
{
	UpdateData(TRUE);

	//write the new setup to the registry
	if(m_SerDesc==0)
		AfxGetApp()->WriteProfileString("MyUSBTestAp", "DescString", m_NameNmbr);
	if(m_SerDesc==1)
		AfxGetApp()->WriteProfileString("MyUSBTestAp", "SerialString", m_NameNmbr);
	if(m_SerDesc==2)
		AfxGetApp()->WriteProfileString("MyUSBTestAp", "DevNmbr", m_NameNmbr);

	if(m_NameNmbr.GetLength() < 1)
		m_SerDesc = 2;
	UpdateData(FALSE);
	UpdateWindow();
}





//****************************************************************************************
void CUSBtestDlg::Loadem()
{
	unsigned char txbuf[25], rxbuf[25];
	DWORD ret_bytes;

	LoadDLL();


	UpdateData(TRUE);
	
	//open the device
	FT_STATUS status = OpenBy();
	if(status)
	{
		m_PortStatus = _T("USB1 not responding");
		board_present=0;
		GetDlgItem(IDC_BUTTON_HELLO)->EnableWindow(TRUE);
	}
	else
	{
		ResetDevice();
		SetTimeouts(3000, 3000);//extend timeout while board DLP-USB2 finishes reset
		Purge(FT_PURGE_RX || FT_PURGE_TX);
		
		//test for presence of board
		txbuf[0] = 0x80;
		Write(txbuf, 1, &ret_bytes);
		Read(rxbuf, 1, &ret_bytes);
		if(ret_bytes==0)//if no response maybe windows was busy... read it again
			Read(rxbuf, 1, &ret_bytes);
		if(rxbuf[0] != 0x80)
		{
			m_PortStatus = _T("No response from DLP-USB2");
			int c = (int)rxbuf[0];
			CString str;
			str.Format(" (0x%.2X)", c); 
			m_PortStatus += str;
			board_present=0;
			GetDlgItem(IDC_BUTTON_HELLO)->EnableWindow(TRUE);
		}	
		else
		{
			m_PortStatus = _T("DLP-USB2 ready.");
			buttons(ON);//turn em off till a port is active
			board_present=1;
			//GetDlgItem(IDC_BUTTON_HELLO)->EnableWindow(FALSE);
		}
	}

	SetTimeouts(300, 300);
	UpdateData(FALSE);
	UpdateWindow();
}



//****************************************************************************************
FT_STATUS CUSBtestDlg::OpenBy()
{
	UpdateData(TRUE);

	if(m_NameNmbr.Find("Enter device Description") > -1)
	{
		//highlight the descriptor/serial number to draw attention...
		CEdit *pEdt;
		pEdt = (CEdit *)GetDlgItem(IDC_EDIT_NAME_NUMBER);
		pEdt->SetFocus();
		pEdt->SetSel(0, -1, FALSE);
		AfxMessageBox("You must enter a Device Description or Serial Number.");
		return FT_DEVICE_NOT_OPENED;
	}

	FT_STATUS status;
	ULONG x=0;
	if(m_SerDesc==0)
		status = OpenEx((PVOID)(LPCTSTR)m_NameNmbr, FT_OPEN_BY_DESCRIPTION);
	if(m_SerDesc==1)
		status = OpenEx((PVOID)(LPCTSTR)m_NameNmbr, FT_OPEN_BY_SERIAL_NUMBER);
	if((m_SerDesc==2) || (m_SerDesc==-1))//if open by device OR no method was selected
	{
		if(m_NameNmbr.GetLength() < 1)//nothing entered - open default device 0
		{
			status = Open((PVOID)x);//load default device 0
		}
	   else
	   {
			if(m_NameNmbr.GetLength() > 2)//no Open() method selected...
			{
				AfxMessageBox("Select a method to open or enter a valid device number (0-64).");
				return FT_DEVICE_NOT_OPENED;
			}

			//convert string to decimal number and open(x)
			char str[3];
			strcpy(str, (LPCTSTR)m_NameNmbr);
			x = atoi(str);
			if((x<0) || (x>64))
			{
				AfxMessageBox("Select a method to open or enter a valid device number (0-64).");
				return FT_DEVICE_NOT_OPENED; 
			}
			status = Open((PVOID)x);
	   }
	}

	return status;
}


//****************************************************************************************
void CUSBtestDlg::OnButtonHello() //code for the "Open" button
{
	unsigned char txbuf[25], rxbuf[25];
	DWORD ret_bytes;
	

	UpdateData(TRUE);
	m_PortStatus = _T("-reset-");
	UpdateData(FALSE);
	UpdateWindow();

	Close();

	//open the device
	FT_STATUS status = OpenBy();
	if(status>0)
	{
		m_PortStatus = _T("Could not open DLP-USB1");
		board_present=0;
	}
	else
	{
		
		ResetDevice();
		Purge(FT_PURGE_RX || FT_PURGE_TX);
		ResetDevice();
		SetTimeouts(3000, 3000);//extend timeout while board finishes reset
		Sleep(150);

		//test for presence of board
		txbuf[0] = 0x80;
		Write(txbuf, 1, &ret_bytes);
		Read(rxbuf, 1, &ret_bytes);
		if(ret_bytes==0)
			Read(rxbuf, 1, &ret_bytes);
		if(rxbuf[0] != 0x80)
		{
			m_PortStatus = _T("DLP-USB2 not responding");
			int c = (int)rxbuf[0];
			CString str;
			str.Format(" (0x%.2X)", c); 
			//m_PortStatus += str;
			board_present=0;
			Close();
		}	
		else
		{
			m_PortStatus = _T("DLP-USB2 ready.");
			buttons(ON);//turn em off till a port is active
			board_present=1;
		}
	}

	SetTimeouts(300, 300);
	UpdateData(FALSE);
	UpdateWindow();
}


//****************************************************************************************
void CUSBtestDlg::OnSelchangeList1() 
{
	UpdateData(TRUE);
	
	int pos = m_Received.GetCurSel();
	m_Received.GetText(pos, m_NameNmbr);

	UpdateData(FALSE);
	UpdateWindow();

	if(m_SerDesc==0)
		AfxGetApp()->WriteProfileString("MyUSBTestAp", "DescString", m_NameNmbr);
	if(m_SerDesc==1)
		AfxGetApp()->WriteProfileString("MyUSBTestAp", "SerialString", m_NameNmbr);
	if(m_SerDesc==2)
		AfxGetApp()->WriteProfileString("MyUSBTestAp", "DevNmbr", m_NameNmbr);
}





//****************************************************************************************
void CUSBtestDlg::OnButtonSearch() 
{
	//search for Descriptions or Serial Numbers depending on the current mode
	FT_STATUS ftStatus;
	DWORD numDevs;

	Close();//must be closed to perform the ListDevices() function
	UpdateData(TRUE);
	m_PortStatus = _T("DLP-USB1 Closed.");
	m_NumRecd=0;
	m_NameNmbr = _T("");
	m_Received.ResetContent();
	UpdateData(FALSE);
	UpdateWindow();

	ftStatus = ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
	if(ftStatus == FT_OK) 
	{
		// FT_ListDevices OK, show number of devices connected in list box
		CString str;
		str.Format("%d device(s) attached:", (int)numDevs);
		m_Received.AddString(str);		


		//if current mode is open "by device #" then list device numbers
		if((m_SerDesc==2) && (numDevs>0))
		{
			for(DWORD d=0; d<numDevs; d++)
			{
				str.Format("%d", d);
				m_Received.AddString(str);		
			}

		}



		//if current mode is open "by description" then list descriptions of all connected devices
		if((m_SerDesc==0) && (numDevs>0))
		{
			ftStatus = ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
			if(ftStatus == FT_OK) 
			{
				char *BufPtrs[64]; // pointer to array of 64 pointers
				for(DWORD d=0; d<numDevs; d++)
					BufPtrs[d] = new char[64];
				BufPtrs[d] = NULL;

				ftStatus = ListDevices(BufPtrs, &numDevs, FT_LIST_ALL|FT_OPEN_BY_DESCRIPTION);
				if (FT_SUCCESS(ftStatus)) 
				{
					for(DWORD u=0; u<numDevs; u++)
					{
						str.Format("%s", BufPtrs[u]);
						m_Received.AddString(str);		
					}
				}
				else 
				{
					str.Format("ListDevices failed");
					m_Received.AddString(str);		
				}	

				//free ram to avoid memory leaks
				for(d=0; d<numDevs; d++)
				{
					delete BufPtrs[d];  
				}
			}		
		}

		//if current mode is open "by serial number" the list descriptions 
		//of all connected devices
		if((m_SerDesc==1) && (numDevs>0))
		{
			//AfxMessageBox("by serial"); 
			ftStatus = ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
			if(ftStatus == FT_OK) 
			{
				char *BufPtrs[64]; // pointer to array of 64 pointers
				for(DWORD d=0; d<numDevs; d++)
					BufPtrs[d] = new char[64];
				BufPtrs[d] = NULL;

				ftStatus = ListDevices(BufPtrs, &numDevs, FT_LIST_ALL|FT_OPEN_BY_SERIAL_NUMBER);
				if (FT_SUCCESS(ftStatus)) 
				{
					for(DWORD u=0; u<numDevs; u++)
					{
						str.Format("%s", BufPtrs[u]);
						m_Received.AddString(str);		
					}
				}
				else 
				{
					str.Format("ListDevices failed");
					m_Received.AddString(str);		
				}	

				//free ram to avoid memory leaks
				for(d=0; d<numDevs; d++)
					delete BufPtrs[d];  
			}
		}

	}
	else 
	{
		// FT_ListDevices failed
		AfxMessageBox("FT_ListDevices failed"); 
	}		
}


