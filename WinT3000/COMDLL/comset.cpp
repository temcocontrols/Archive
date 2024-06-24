//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "comset.h"
//---------------------------------------------------------------------------
//----------------------------------------------------------------------------
//  Project wint3000
//
//  Copyright © 1996. All Rights Reserved.
//
//  SUBSYSTEM:    wint3000.apx Application
//  FILE:         sttngsdl.cpp
//  AUTHOR:       Adrian Boici
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of SettingsDlg (TDialog).
//
//----------------------------------------------------------------------------
#include "t3000def.h"
#include "rs232.h"
#include "modem.h"
#define MAXBAUDRATE    8
#define MAXCOMPORTS    4
#define MAXMODEMTYPE   5
#define MAXCONNECTION  4

#define TXQUEUE          0
#define RXQUEUE          1

/**************************************************************************
	 Module for adding serial interface parameters

**************************************************************************/

/*===========================< External variables >======================*/
//extern Byte    szAppTitle;
//extern int     nnCid;
//extern DInt    WriteFlag;
//extern HANDLE  hInst;
//extern HWND    Mainwindow;	// handle of window for timer messages

//extern    COMMINFO CommInfo[5];         /* Current line parameters */
//extern HANDLE   hInst;
//extern HANDLE   hwMain;
/*============================< Local variables >========================*/
//DCB       CommDCB;
//DCB FAR * lpDCB;
//COMSTAT   CommState;

//char      szMsgBoxTitle[]= "COMx:-Status";     // Error box title

//char      szDefSetting[] = "COM1:=96,n,8,1";   // Default values

char      szaCommPort[4][5]  = {
				 {"COM1"},
				 {"COM2"},
				 {"COM3"},
				 {"COM4"} };

/*
char      szaBaudRate[MAXBAUDRATE][7] = {
				{"1200"},
				{"2400"},
				{"4800"},
				{"9600"},
				{"19200"},
				{"38400"},
				{"57600"},
				{"115200"}
						 };
char      szaConnection[MAXCONNECTION][7]  = {
				 {"none"},
				 {"Serial"},
				 {"Modem"},
				 {"RS485"}
				 };

DLong     waBaudRate[MAXBAUDRATE]     = { 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
Byte      waByteSize[2]     = { 7, 8 };
Byte      waStopBits[3]     = { ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS };
Byte      waParity[3]       = { NOPARITY,   ODDPARITY,    EVENPARITY };

char      szErrorMsg[80];

ModemCapabilities ModemDataBase[MAXMODEMTYPE] = {
	{
			"User Defined",
		"AT &F&C1&D2V1",
		"AT S0=0",
		"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0\0",
		"",
		"",
		38400L,
		0,
		1
	},
	{  "Generic Modem",
		"AT &F&C1&D2V1",
		"AT S0=0",
		"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0\0",
		"",
		"",
		38400L,
		0,
		1
	},
	{ "Hayes Compatible",
		"AT &F B1E1Q0V1X4Y0 &C1&D2 S7=90",
		"\0",
		"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0",
		"",
		"",
		2400L,
		0,
		0
	},
	{ "Practical Peripherals 14400FX",
		"AT &F0 &C1 &D2 S95=44",
		"\0",
		"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0",
		"CLASS 5\0V.42BIS\0\0",
		"LAP-M\0ALT\0\0",
		57600L,
		1,
		1
	},
	{ "Intel SatisFAXtion 400e",
		"AT &F",
		"\0",
		"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0",
		"COMP\0\0",
		"LAPM\0MNP\0REL\0\0",
		57600L,
		1,
		1
	}
};
*/

__declspec(dllexport) HANDLE InitCommPort(COMMINFO *pCom);
__declspec(dllexport) bool WriteString(HANDLE h, LPSTR lpData, DWORD dwLength);

/*************************************************************************
 I n i t C o m m P o r t ()
 ==========================

 This function opens the serial interface specified by <CommPortId> and
 initializes the interface with the values given by the corresponding
 DCB structure <CommBCD[CommPortID]>.

 Parameters:
	 int CommPortId     0=COM1:, ... 3=COM4:

 Return values:
	 The return value appears whether the function was successful or not. If
	 a positive value is returned, initialization was successful. Otherwise,
	 -1 is returned. The CID is placed in the Id field of the appropriate DCB
	 structure, and indicates the correct initialization.
*************************************************************************/
HANDLE InitCommPort(COMMINFO *pCom)
{
 DCB dcb;
 HANDLE hCom;
 DWORD dwError;
 BOOL fSuccess;
 COMMTIMEOUTS  ctmo;	// address of communications time-out structure

 hCom = CreateFile(
    szaCommPort[pCom->PortName],
    GENERIC_READ | GENERIC_WRITE,
    0,    /* comm devices must be opened w/exclusive-access */
    NULL, /* no security attrs */
    OPEN_EXISTING, /* comm devices must use OPEN_EXISTING */
    0,    /* not overlapped I/O */
    NULL  /* hTemplate must be NULL for comm devices */
    );

 if (hCom == INVALID_HANDLE_VALUE) {
    dwError = GetLastError();
    /* handle error */
    pCom->handle = hCom;
    return hCom;
 }
/*
 * Omit the call to SetupComm to use the default queue sizes.
 * Get the current configuration.
 */

 fSuccess = GetCommState(hCom, &dcb);

 if (!fSuccess) {
    // Handle the error.
 }
 else
 {
   SetupComm(
   		hCom,	// handle of communications device
   		2048,	// size of input buffer
   		2048  	// size of output buffer
   );

/* Fill in the DCB: baud=9600, 8 data bits, no parity, 1 stop bit. */
//   CommDCB.DCBlength = sizeof(DCB);           // sizeof(DCB)
   dcb.BaudRate  = pCom->Settings.BaudRate;
   dcb.ByteSize  = 8;
   dcb.Parity    = NOPARITY;
   dcb.StopBits  = ONESTOPBIT;
//   dcb.fDtrControl = pCom->Settings.Dtr;

   fSuccess = SetCommState(hCom, &dcb);

   if (!fSuccess) {
    // Handle the error.
   }
//-----< Control display of current parameters >----------
   ctmo.ReadIntervalTimeout = MAXDWORD;
   ctmo.ReadTotalTimeoutMultiplier=0;
   ctmo.ReadTotalTimeoutConstant=0;
   ctmo.WriteTotalTimeoutMultiplier=1;
   ctmo.WriteTotalTimeoutConstant=100;
   SetCommTimeouts(
			 hCom,	// handle of communications device
			 &ctmo	// address of communications time-out structure
   );
//   GetCommState(hCom, &CommDCB);
 }
 pCom->handle = hCom;
 return  hCom;
}

/*****************************************************************************
  W r i t e C h a r ()
  ====================

  WriteChar sends a character to the serial interface.

  Parameters:

    unsigned int ComNr:         Serial interface (1-4).
    int          SendChar:      Character to be sent.

  Return values: None.
*****************************************************************************/
//void WriteChar (DInt ComNr, char SendChar)
//{
/*
  char szData[2];
  HANDLE   h;
  DWORD    i;
  h = CommInfo[ComNr-1].handle;
  szData[0] = SendChar;
  szData[1] = '\0';
  WriteFile(
    h,	// handle of file to write to
    (LPCVOID)szData,	// address of data to write to file
    1,	// number of bytes to write
    (LPDWORD)&i,	// address of number of bytes written
    NULL 	// addr. of structure needed for overlapped I/O
   );
//  WriteComm(ComNr, (LPSTR)szData, 1);
*/
//} // WriteChar

/*****************************************************************************
  W r i t e S t r i n g ()
  ========================

  WriteString sends a string ended by '\0' to the serial interface.

  Parameters:

  unsigned int Cid:      Serial interface (1-4)
  LPSTR        lpData:   String to be sent

  Return values:         TRUE if the string can be sent. Otherwise, FALSE.
  *****************************************************************************/

//bool WriteString(DInt Cid, LPSTR lpData, DWORD dwLength)
bool WriteString(HANDLE h, LPSTR lpData, DWORD dwLength)
{
	 COMSTAT          CommState;
	 DInt              nOffset, nWritten;
	 DWORD            dwStopTime;
	 MSG              myMsg;
	 BOOL             bool_var;

	 DWORD    j;
//	 HANDLE   h;
//	 h = CommInfo[Cid-1].handle;

	 bool_var=PurgeComm(
				h,	// handle of communications resource
				PURGE_TXCLEAR	// action to perform
	 );
//	 WriteFlag=1;
	 bool_var=WriteFile(
				h,	// handle of file to write to
				(LPCVOID)lpData,	// address of data to write to file
				dwLength,	// number of bytes to write
				(LPDWORD)&i,	// address of number of bytes written
				NULL 	// addr. of structure needed for overlapped I/O
	  );
//	WriteFlag=0;
     return true;
} // WriteString

