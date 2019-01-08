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

// USBtestDlg.h : header file
//

#if !defined(AFX_USBTESTDLG_H__07909306_D75E_11D4_A644_93781B97CC41__INCLUDED_)
#define AFX_USBTESTDLG_H__07909306_D75E_11D4_A644_93781B97CC41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ftd2xx.h"



UINT ThreadProc1(LPVOID param);
const int ON = 100;
const int OFF = 101;


// CUSBtestDlg dialog
class CUSBtestDlg : public CDialog
{
// Construction
public:
	CUSBtestDlg(CWnd* pParent = NULL);	// standard constructor
	void CUSBtestDlg::buttons(int onoff);
	void CUSBtestDlg::Loadem();
	FT_STATUS Write(LPVOID, DWORD, LPDWORD);
	FT_STATUS CUSBtestDlg::OpenBy();


// Dialog Data
	//{{AFX_DATA(CUSBtestDlg)
	enum { IDD = IDD_USBTEST_DIALOG };
	CSliderCtrl	m_Slide;
	CListBox	m_Received;
	CString	m_PortStatus;
	CString	m_EchoRes;
	long	m_NumRecd;
	CString	m_128status;
	CString	m_NameNmbr;
	int		m_SerDesc;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSBtestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	HMODULE m_hmodule;
	FT_HANDLE m_ftHandle;
	void LoadDLL();

	typedef FT_STATUS (WINAPI *PtrToOpen)(PVOID, FT_HANDLE *); 
	PtrToOpen m_pOpen; 
	FT_STATUS Open(PVOID);

	typedef FT_STATUS (WINAPI *PtrToOpenEx)(PVOID, DWORD, FT_HANDLE *); 
	PtrToOpenEx m_pOpenEx; 
	FT_STATUS OpenEx(PVOID, DWORD);

	typedef FT_STATUS (WINAPI *PtrToListDevices)(PVOID, PVOID, DWORD);
	PtrToListDevices m_pListDevices; 
	FT_STATUS ListDevices(PVOID, PVOID, DWORD);

	typedef FT_STATUS (WINAPI *PtrToClose)(FT_HANDLE);
	PtrToClose m_pClose;
	FT_STATUS Close();

	typedef FT_STATUS (WINAPI *PtrToRead)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
	PtrToRead m_pRead;
	FT_STATUS Read(LPVOID, DWORD, LPDWORD);

	typedef FT_STATUS (WINAPI *PtrToWrite)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
	PtrToWrite m_pWrite;
	//FT_STATUS Write(LPVOID, DWORD, LPDWORD);

	typedef FT_STATUS (WINAPI *PtrToResetDevice)(FT_HANDLE);
	PtrToResetDevice m_pResetDevice;
	FT_STATUS ResetDevice();
	
	typedef FT_STATUS (WINAPI *PtrToPurge)(FT_HANDLE, ULONG);
	PtrToPurge m_pPurge;
	FT_STATUS Purge(ULONG);
	
	typedef FT_STATUS (WINAPI *PtrToSetTimeouts)(FT_HANDLE, ULONG, ULONG);
	PtrToSetTimeouts m_pSetTimeouts;
	FT_STATUS SetTimeouts(ULONG, ULONG);

	typedef FT_STATUS (WINAPI *PtrToGetQueueStatus)(FT_HANDLE, LPDWORD);
	PtrToGetQueueStatus m_pGetQueueStatus;
	FT_STATUS GetQueueStatus(LPDWORD);




	// Generated message map functions
	//{{AFX_MSG(CUSBtestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnReleasedcaptureSliderWriteRate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSend64Echo();
	afx_msg void OnButtonSendRecdis();
	afx_msg void OnButtonBlinkOn();
	afx_msg void OnButtonBlinkOff();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonClear();
	afx_msg void OnButtonHello();
	afx_msg void OnRadioNameNum();
	afx_msg void OnRadioSernum();
	afx_msg void OnChangeEditNameNumber();
	afx_msg void OnRadioDevno();
	afx_msg void OnButtonSearch();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBTESTDLG_H__07909306_D75E_11D4_A644_93781B97CC41__INCLUDED_)



/*




Using the D2XX Direct Drivers
Introduction
An FTD2XX device is an FT8U232 or FT8U245 running with FTDI's direct driver FTD2XX.SYS. The
device has a programming interface exposed by the dynamic link library FTD2XX.DLL, and this document
describes that interface.
Overview
Before the device can be accessed, it must first be opened. FT_Open and FT_OpenEx return a handle
which is used by all functions in the programming interface to identifiy the device. When the device has
been opened successfully, I/O can be performed using FT_Read and FT_Write. When operations are
complete, the device is closed using FT_Close.
Functions are available to reset the device (FT_ResetDevice); purge receive and transmit buffers
(FT_Purge); set receive and transmit timeouts (FT_SetTimeouts); get receive queue status
(FT_GetQueueStatus); set and reset break condition (FT_SetBreakOn, FT_SetBreakOff).
FT_ListDevices returns information about the devices currently connected.
For FT8U232 devices, functions are available to set the baud rate (FT_SetBaudRate), and set a nonstandard
baud rate (FT_SetDivisor); set the data characteristics such as word length, stop bits and parity
(FT_SetDataCharacteristics); set hardware or software handshaking (FT_SetFlowControl); set modem
control signals (FT_SetDTR, FT_ClrDTR, FT_SetRTS, FT_ClrRTS); get modem status
(FT_GetModemStatus); set special characters such as event and error characters (FT_SetChars).
Reference
The functions which comprise the FTD2XX programming interface are described in this section. Excerpts
from the header file FTD2XX.H are included to explain any references in the descriptions of the functions
below.
FT_HANDLE
typedef DWORD FT_HANDLE
FT_STATUS
FT_OK = 0
FT_INVALID_HANDLE = 1
FT_DEVICE_NOT_FOUND = 2
FT_DEVICE_NOT_OPENED = 3
FT_IO_ERROR = 4
FT_INSUFFICIENT_RESOURCES = 5
FT_INVALID_PARAMETER = 6
Flags (see FT_OpenEx)
FT_OPEN_BY_SERIAL_NUMBER = 1
FT_OPEN_BY_DESCRIPTION = 2
Flags (see FT_ListDevices)
FT_LIST_NUMBER_ONLY = 0x80000000
FT_LIST_BY_INDEX = 0x40000000
FT_LIST_ALL = 0x20000000
Word Length (see FT_SetDataCharacteristics)
FT_BITS_8 = 8
FT_BITS_7 = 7
Stop Bits (see FT_SetDataCharacteristics)
FT_STOP_BITS_1 = 0
FT_STOP_BITS_2 = 2
Parity (see FT_SetDataCharacteristics)
FT_PARITY_NONE = 0
FT_PARITY_ODD = 1
FT_PARITY_EVEN = 2
FT_PARITY_MARK = 3
FT_PARITY_SPACE = 4
Flow Control (see FT_SetFlowControl)
FT_FLOW_NONE = 0x0000
FT_FLOW_RTS_CTS = 0x0100
FT_FLOW_DTR_DSR = 0x0200
FT_FLOW_XON_XOFF = 0x0400
Purge RX and TX buffers (see FT_Purge)
FT_PURGE_RX = 1
FT_PURGE_TX = 2
FT_ListDevices
Description
Get information concerning the devices currently connected. This function can return
such information as the number of devices connected, and device strings such as serial
number and product description.
Syntax FT_STATUS FT_ListDevices ( PVOID pvArg1,PVOID pvArg2, DWORD dwFlags )
Parameters
pvArg1 PVOID: Meaning depends on dwFlags
pvArg2 PVOID: Meaning depends on dwFlags
dwFlags DWORD: Determines format of returned information
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
Remarks
This function can be used in a number of ways to return different types of information.
In its simplest form, it can be used to return the number of devices currently connected.
If FT_LIST_NUMBER_ONLY bit is set in dwFlags, the parameter pvArg1 is interpreted
as a pointer to a DWORD location to store the number of devices currently connected.
It can be used to return device string information. If FT_OPEN_BY_SERIAL_NUMBER
bit is set in dwFlags, the serial number string will be returned from this function. If
FT_OPEN_BY_DESCRIPTION bit is set in dwFlags, the product description string will
be returned from this function. If neither of these bits is set, the serial number string will
be returned by default.
It can be used to return device string information for a single device. If
FT_LIST_BY_INDEX bit is set in dwFlags, the parameter pvArg1 is interpreted as the
index of the device, and the parameter pvArg2 is interpreted as a pointer to a buffer to
contain the appropriate string. Indexes are zero-based, and the error code
FT_DEVICE_NOT_FOUND is returned for an invalid index.
It can be used to return device string information for all connected devices. If
FT_LIST_ALL bit is set in dwFlags, the parameter pvArg1 is interpreted as a pointer to an
array of pointers to buffers to contain the appropriate strings, and the parameter pvArg2 is
interpreted as a pointer to a DWORD location to store the number of devices currently
connected. Note that, for pvArg1, the last entry in the array of pointers to buffers should
be a NULL pointer so the array will contain one more location than the number of
devices connected.
Examples
Sample code shows how to get the number of devices currently connected.
FT_STATUS ftStatus;
DWORD numDevs;
ftStatus = FT_ListDevices(&numDevs,NULL,FT_LIST_NUMBER_ONLY);
if (ftStatus == FT_OK) {
// FT_ListDevices OK, number of devices connected is in numDevs
}
else {
// FT_ListDevices failed
}
This sample shows how to get the serial number of the first device found. Note that
indexes are zero-based. If more than one device is connected, incrementing devIndex
will get the serial number of each connected device in turn.
FT_STATUS ftStatus;
DWORD devIndex = 0;
char Buffer[16];
ftStatus =
FT_ListDevices((PVOID)devIndex,Buffer,FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_N
UMBER);
if (FT_SUCCESS(ftStatus)) {
// FT_ListDevices OK, serial number is in Buffer
}
else {
// FT_ListDevices failed
}
This sample shows how to get the product descriptions of all the devices currently
connected.
FT_STATUS ftStatus;
char *BufPtrs[3]; // pointer to array of 3 pointers
char Buffer1[64]; // buffer for product description of first
device found
char Buffer2[64]; // buffer for product description of second
device
DWORD numDevs;
// initialize the array of pointers
BufPtrs[0] = Buffer1;
BufPtrs[1] = Buffer2;
BufPtrs[2] = NULL; // last entry should be NULL
ftStatus =
FT_ListDevices(BufPtrs,&numDevs,FT_LIST_ALL|FT_OPEN_BY_DESCRIPTION);
if (FT_SUCCESS(ftStatus)) {
// FT_ListDevices OK, product descriptions are in Buffer1 and Buffer2,
and
// numDevs contains the number of devices connected
}
else {
// FT_ListDevices failed
}
FT_Open
Description Open the device and return a handle which will be used for subsequent accesses.
Syntax FT_STATUS FT_Open ( PVOID pvDevice, FT_HANDLE *ftHandle )
Parameters
pvDevice ULONG: Must be 0 if only one device is attached. For multiple devices 1, 2 etc.
ftHandle FT_HANDLE *: Pointer to a variable of type FT_HANDLE where the handle will be
stored. This handle must be used to access the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
Remarks
Although this function can be used to open multiple devices by setting pvDevice to 0, 1, 2
etc. there is no ability to open a specific device. To open named devices, use the function
FT_OpenEx.
Example
This sample shows how to open a device.
FT_HANDLE ftHandle;
FT_STATUS ftStatus;
ftStatus = FT_Open(0,&ftHandle);
if (ftStatus == FT_OK) {
// FT_Open OK, use ftHandle to access device
}
else {
// FT_Open failed
}
FT_OpenEx
Description
Open the named device and return a handle which will be used for subsequent accesses.
The device name can be its serial number or device description.
Syntax FT_STATUS FT_OpenEx ( PVOID pvArg1, DWORD dwFlags, FT_HANDLE
*ftHandle )
Parameters
pvArg1 PVOID: Meaning depends on dwFlags, but it will normally be interpreted as a pointer to
a null terminated string.
dwFlags DWORD: FT_OPEN_BY_SERIAL_NUMBER or FT_OPEN_BY_DESCRIPTION.
ftHandle FT_HANDLE *: Pointer to a variable of type FT_HANDLE where the handle will be
stored. This handle must be used to access the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
Remarks
This function should be used to open multiple devices. Multiple devices can be opened at
the same time if they can be distinguished by serial number or device description.
Example
These samples show how to open two devices simultaneously.
Suppose one device has serial number "FT000001", and the other has serial number
"FT999999".
FT_STATUS ftStatus;
FT_HANDLE ftHandle1;
FT_HANDLE ftHandle2;
ftStatus = FT_OpenEx("FT000001",FT_OPEN_BY_SERIAL_NUMBER,&ftHandle1);
if (ftStatus == FT_OK) {
// FT_OpenEx OK, device with serial number "FT000001" is open
}
else {
// FT_OpenEx failed
}
ftStatus = FT_OpenEx("FT999999",FT_OPEN_BY_SERIAL_NUMBER,&ftHandle2);
if (ftStatus == FT_OK) {
// FT_OpenEx OK, device with serial number "FT999999" is open
}
else {
// FT_OpenEx failed
}
Suppose one device has description "USB HS SERIAL CONVERTER", and the other
has description "USB PUMP CONTROLLER".
FT_STATUS ftStatus;
FT_HANDLE ftHandle1;
FT_HANDLE ftHandle2;
ftStatus = FT_OpenEx("USB HS SERIAL
CONVERTER",FT_OPEN_BY_DESCRIPTION,&ftHandle1);
if (ftStatus == FT_OK) {
// FT_OpenEx OK, device with description "USB HS SERIAL CONVERTER"
is open
}
else {
// FT_OpenEx failed
}
ftStatus = FT_OpenEx("USB PUMP
CONTROLLER",FT_OPEN_BY_DESCRIPTION,&ftHandle2);
if (ftStatus == FT_OK) {
// FT_OpenEx OK, device with description "USB PUMP CONTROLLER" is
open
}
else {
// FT_OpenEx failed
}
FT_Close
Description Close an open device.
Syntax FT_STATUS FT_Close ( FT_HANDLE ftHandle )
Parameters
ftHandle FT_HANDLE: handle of the device to close.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_Read
Description Read data from the device.
Syntax FT_STATUS FT_Read ( FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD
dwBytesToRead, LPDWORD lpdwBytesReturned )
Parameters
ftHandle FT_HANDLE: handle of the device to read.
lpBuffer LPVOID: Pointer to the buffer that receives the data from the device.
dwBytesToRead DWORD: Number of bytes to be read from the device.
lpdwBytesReturned LPDWORD: Pointer to a variable of type DWORD which receives the number
of bytes read from the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_Write
Description Write data to the device.
Syntax FT_STATUS FT_Write ( FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD
dwBytesToWrite, LPDWORD lpdwBytesWritten )
Parameters
ftHandle FT_HANDLE: handle of the device to write.
lpBuffer LPVOID: Pointer to the buffer that contains the data to be written to the device.
dwBytesToWrite DWORD: Number of bytes to write to the device.
lpdwBytesWritten LPDWORD: Pointer to a variable of type DWORD which receives the number
of bytes written to the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_ResetDevice
Description This function sends a reset command to the device.
Syntax FT_STATUS FT_ResetDevice ( FT_HANDLE ftHandle )
Parameters
ftHandle FT_HANDLE: handle of the device to reset.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_SetBaudRate
Description This function sets the baud rate for the device.
Syntax FT_STATUS FT_SetBaudRate ( FT_HANDLE ftHandle, DWORD dwBaudRate )
Parameters
ftHandle FT_HANDLE: handle of the device.
dwBaudRate DWORD: Baud rate.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_SetDivisor
Description
This function sets the baud rate for the device. It is used to set non-standard baud rates.
Syntax FT_STATUS FT_SetDivisor ( FT_HANDLE ftHandle, USHORT usDivisor )
Parameters
ftHandle FT_HANDLE: handle of the device.
usDivisor USHORT: Divisor.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
Remarks
The application note "Setting Baud rates for the FT8U232AM", which is available on our
web site
www.ftdi.co.uk , describes how to calculate the divisor for a non standard baud rate.
Example
Suppose we want to set a baud rate of 5787 baud. A simple calculation suggests that a
divisor of 4206 should work.
FT_HANDLE ftHandle; // handle of device obtained from FT_Open or
FT_OpenEx
FT_STATUS ftStatus;
ftStatus = FT_SetDivisor(ftHandle,0x4206);
if (ftStatus == FT_OK) {
// FT_SetDivisor OK, baud rate has been set to 5787 baud
}
else {
// FT_SetDivisor failed
}
FT_SetDataCharacteristics
Description This function sets the data characteristics for the device.
Syntax FT_STATUS FT_SetDataCharacteristics ( FT_HANDLE ftHandle, UCHAR
uWordLength, UCHAR uStopBits,UCHAR uParity )
Parameters
ftHandle FT_HANDLE: handle of the device.
uWordLength UCHAR: Number of bits per word - must be FT_BITS_8 or FT_BITS_7.
uStopBits UCHAR: Number of stop bits - must be FT_STOP_BITS_1 or FT_STOP_BITS_2.
uParity UCHAR: FT_PATITY_NONE, FT_PARITY_ODD, FT_PARITY_EVEN,
FT_PARITY_MARK, FT_PARITY_SPACE.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_SetFlowControl
Description This function sets the flow control for the device.
Syntax FT_STATUS FT_SetFlowControl ( FT_HANDLE ftHandle, USHORT
usFlowControl, UCHAR uXon,UCHAR uXoff )
Parameters
ftHandle FT_HANDLE: handle of the device.
usFlowControl USHORT: Must be one of FT_FLOW_NONE, FT_FLOW_RTS_CTS,
FT_FLOW_DTR_DSR, FT_FLOW_XON_XOFF
uXon UCHAR: Character used to signal XON. Only used if flow control is
FT_FLOW_XON_XOFF.
uXoff UCHAR: Character used to signal XOFF. Only used if flow control is
FT_FLOW_XON_XOFF.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_SetDTR
Description This function sets DTR.
Syntax FT_STATUS FT_SetDTR ( FT_HANDLE ftHandle )
Parameters
ftHandle FT_HANDLE: handle of the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_ClrDTR
Description This function clears DTR.
Syntax FT_STATUS FT_ClrDTR ( FT_HANDLE ftHandle )
Parameters
ftHandle FT_HANDLE: handle of the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_SetRTS
Description This function sets RTS.
Syntax FT_STATUS FT_SetRTS ( FT_HANDLE ftHandle )
Parameters
ftHandle FT_HANDLE: handle of the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_ClrRTS
Description This function clears RTS.
Syntax FT_STATUS FT_ClrRTS ( FT_HANDLE ftHandle )
Parameters
ftHandle FT_HANDLE: handle of the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_GetModemStatus
Description Gets the modem status from the device.
Syntax FT_STATUS FT_GetModemStatus ( FT_HANDLE ftHandle, LPDWORD
lpdwModemStatus )
Parameters
ftHandle FT_HANDLE: handle of the device to read.
lpdwModemStatus LPDWORD: Pointer to a variable of type DWORD which receives the modem
status from the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_SetChars
Description This function sets the special characters for the device.
Syntax FT_STATUS FT_SetChars ( FT_HANDLE ftHandle, UCHAR uEventCh, UCHAR
uEventChEn, UCHAR uErrorCh, UCHAR uErrorChEn )
Parameters
ftHandle FT_HANDLE: handle of the device.
uEventCh UCHAR: Event character,
uEventChEn UCHAR: 0 if event character is disabled, non-zero otherwise.
uErrorCh UCHAR: Error character,
uErrorChEn UCHAR: 0 if error character is disabled, non-zero otherwise.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_Purge
Description This function purges the receive and transmit buffers in the device.
Syntax FT_STATUS FT_Purge ( FT_HANDLE ftHandle, DWORD dwMask )
Parameters
ftHandle FT_HANDLE: handle of the device.
dwMask DWORD: Any combination of FT_PURGE_RX and FT_PURGE_TX.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_SetTimeouts
Description This function sets the read and write timeouts for the device.
Syntax FT_STATUS FT_SetTimeouts ( FT_HANDLE ftHandle, DWORD dwReadTimeout,
DWORD dwWriteTimeout )
Parameters
ftHandle FT_HANDLE: handle of the device.
dwReadTimeout DWORD: Read timeout in milliseconds.
dwWriteTimeout DWORD: Write timeout in milliseconds.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_GetQueueStatus
Description Gets the number of characters in the receive queue.
Syntax FT_STATUS FT_GetQueueStatus ( FT_HANDLE ftHandle, LPDWORD
lpdwAmountInRxQueue )
Parameters
ftHandle FT_HANDLE: handle of the device to read.
lpdwAmountInRxQueue LPDWORD: Pointer to a variable of type DWORD which receives the
number of characters in the receive queue.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_SetBreakOn
Description Sets the BREAK condition for the device.
Syntax FT_STATUS FT_SetBreakOn ( FT_HANDLE ftHandle )
Parameters
ftHandle FT_HANDLE: handle of the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.
FT_SetBreakOff
Description Resets the BREAK condition for the device.
Syntax FT_STATUS FT_SetBreakOff ( FT_HANDLE ftHandle )
Parameters
ftHandle FT_HANDLE: handle of the device.
Return Value FT_STATUS: FT_OK if successful, otherwise the return value is an FT error code.


  */
