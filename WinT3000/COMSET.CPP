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
#include "ptpdll.h"

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
DInt COMMINFO::objectcount=0;

class CommINFO _export CommInfo[MAX_COMM_INFO];
/*
={
                     {COM1,4,1,{38400}, {  "User Defined",
											"AT &F&C1&D2V1",
											"AT S0=0",
											"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0\0",
											"",
											"",
											38400L,
											0,
											1
										},0,5,1,INVALID_HANDLE_VALUE,-1},
                     {COM2,3,1,{38400}, {  "User Defined",
											"AT &F&C1&D2V1",
											"AT S0=0",
											"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0\0",
											"",
											"",
											38400L,
											0,
											1
										},0,5,1,INVALID_HANDLE_VALUE,-1},
                     {COM3,4,1,{38400}, {  "User Defined",
											"AT &F&C1&D2V1",
											"AT S0=0",
											"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0\0",
											"",
											"",
											38400L,
											0,
											1
										},0,5,1,INVALID_HANDLE_VALUE,-1},
                     {COM4,3,1,{38400},{  "User Defined",
											"AT &F&C1&D2V1",
											"AT S0=0",
											"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0\0",
											"",
											"",
											38400L,
											0,
											1
										},0,5,1,INVALID_HANDLE_VALUE,-1},
					};
*/
//DCB       CommDCB;
//DCB FAR * lpDCB;
//COMSTAT   CommState;

//char      szMsgBoxTitle[]= "COMx:-Status";     // Error box title

//char      szDefSetting[] = "COM1:=96,n,8,1";   // Default values

char      szaCommPort[MAX_COMM_INFO][5]  = {
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
*/

//extern int head_Data, tail_Data, overflow;

//__declspec(dllexport) HANDLE initCommPort(COMMINFO *pCom);
//__declspec(dllexport) bool WriteString(HANDLE h, LPSTR lpData, DWORD dwLength);
//__declspec(dllexport) void initCommPort(DInt comPort=-1);
//__declspec(dllexport) void closeCommPort(DInt comPort=-1);

void SETTINGS::Adjust( DLong baudRate,
				 DInt parity,
				 DInt wordLength,
				 DInt stopBits,
				 DInt dtr,
				 DInt rts,
				 DInt xon_xoff,
				 DInt rts_cts,
				 DInt dtr_dsr )
{
    if ( baudRate != UNCHANGED )
		  BaudRate = baudRate;
    if ( parity != UNCHANGED )
	   Parity = parity;
    if ( wordLength != UNCHANGED )
	   WordLength = wordLength;
    if ( stopBits != UNCHANGED )
	   StopBits = stopBits;
    if ( dtr != UNCHANGED )
	   Dtr = dtr;
    if ( rts != UNCHANGED )
	   Rts = rts;
    if ( xon_xoff != UNCHANGED )
	   XonXoff = xon_xoff;
    if ( rts_cts != UNCHANGED )
	   RtsCts = rts_cts;
    if ( dtr_dsr != UNCHANGED )
	   DtrDsr = dtr_dsr;
}

CommINFO::CommINFO(void)
{
  ModemCapabilities modemcapabilities= {  "User Defined",
										  "AT &F&C1&D2V1",
										  "AT S0=0",
										  "NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0\0",
										  "",
										  "",
										  38400L,
										  0,
										  1
										};
  PortName = objectcount++;
  Connection(1);   // 1-Serial connection
  Set( 38400, NOPARITY, 8, ONESTOPBIT);
  setmodemCapabilities(&modemcapabilities);
  modemTypeIndex(1);  //  set Modemtypeindex
  setRings(0);
  Handle(INVALID_HANDLE_VALUE);
  conPort(-1);
}

void COMMINFO::Set( DLong baudRate,
				 DInt parity,
				 DInt wordLength,
				 DInt stopBits,
				 DInt dtr,
				 DInt rts,
				 DInt xon_xoff,
				 DInt rts_cts,
				 DInt dtr_dsr )
{
   Settings.Adjust( baudRate ,
				    parity,
				    wordLength,
				    stopBits,
				    dtr,
				    rts,
				    xon_xoff,
				    rts_cts,
				    dtr_dsr );
   if ( baudRate != UNCHANGED )
   {
     switch( baudRate ) {
        case 1200:
           Baudrateindex=0;
           break;
        case 2400:
           Baudrateindex=1;
           break;
        case 4800:
           Baudrateindex=2;
           break;
        case 9600:
           Baudrateindex=3;
           break;
        case 19200:
           Baudrateindex=4;
           break;
        case 38400:
           Baudrateindex=5;
           break;
        case 57600:
           Baudrateindex=6;
           break;
        case 115200:
           Baudrateindex=7;
           break;
     }
   }
}

void COMMINFO::setRings( DByte r )
{
 rings = r;
}

DByte CommINFO::Connection( void )
{
 return connection;
};

void CommINFO::Connection( DByte c )
{
 connection=c;
};

DInt CommINFO::conPort(void)                 // get connection port
{
  return conport;
}

DByte CommINFO::baudRateIndex(void)
{
 return Baudrateindex;
};

DLong CommINFO::baudRate(void)
{
 return Settings.BaudRate;
}

DByte CommINFO::modemTypeIndex(void)
{
 return Modemtypeindex;
};

void  CommINFO::modemTypeIndex(DByte mti)
{
 Modemtypeindex=mti;
};

void CommINFO::setmodemCapabilities(ModemCapabilities *mc)    // get modemcapabilities
{
 modemcapabilities = *mc;
};

void CommINFO::getmodemCapabilities(ModemCapabilities &mc)    // get modemcapabilities
{
 mc = modemcapabilities;
};

DByte *CommINFO::firstInitString(void)
{
  return &modemcapabilities.first_init_string[0];
}

void  CommINFO::firstInitString(DByte *first)
{
  strcpy(modemcapabilities.first_init_string, first);
}

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
HANDLE initCommPort(COMMINFO *pCom)
{
 DCB dcb;
 HANDLE hCom;
 DWORD dwError;
 BOOL fSuccess;
 COMMTIMEOUTS  ctmo;	// address of communications time-out structure

 hCom = CreateFile(
    szaCommPort[pCom->portName()],
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
    pCom->Handle( hCom );
    return hCom;
 }
/*
 * Omit the call to SetupComm to use the default queue sizes.
 * Get the current configuration.
 */

 fSuccess = GetCommState(hCom, &dcb);

 if (!fSuccess) {
    // Handle the error.
    dwError = GetLastError();
	CloseHandle(hCom);// handle of object to close
    hCom = INVALID_HANDLE_VALUE;
    pCom->Handle(hCom);
    return hCom;
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
   dcb.ByteSize  = pCom->Settings.WordLength;
   dcb.Parity    = pCom->Settings.Parity;
   dcb.StopBits  = pCom->Settings.StopBits;
   dcb.StopBits  = pCom->Settings.StopBits;
   dcb.fDtrControl       = pCom->Settings.Dtr;
   dcb.fRtsControl       = pCom->Settings.Rts;
   dcb.fTXContinueOnXoff = pCom->Settings.XonXoff;
   dcb.fOutxCtsFlow      = pCom->Settings.RtsCts;
   dcb.fOutxDsrFlow      = pCom->Settings.DtrDsr;

   fSuccess = SetCommState(hCom, &dcb);

   if (!fSuccess) {
    // Handle the error.
    dwError = GetLastError();
	CloseHandle(hCom);// handle of object to close
    hCom = INVALID_HANDLE_VALUE;
    pCom->Handle(hCom);
    return hCom;
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
 pCom->Handle(hCom);
 return  hCom;
}

HANDLE initCommPort(DInt comPort)
{
  int j;
  HANDLE h=INVALID_HANDLE_VALUE;
/*
  if(comPort==-1)
  {
   for(j=0; j<MAXCOMPORTS; j++)
		initCommPort(&CommInfo[j]);
  }
  else
*/
  if(comPort>=0 && comPort<4)
    h=initCommPort(&CommInfo[comPort]);
  return h;
}

DInt closeCommPort(DInt comPort)
{
  DInt r = 0;
  if(comPort==-1)
  {
	for(int j=0; j<MAXCOMPORTS; j++)
	{
		if( CommInfo[j].Handle()!=INVALID_HANDLE_VALUE )
			 CloseHandle(CommInfo[j].Handle());// handle of object to close
	};
  }
  else
  {
    if(comPort>=0 && comPort<4)
	 if( CommInfo[comPort].Handle()!=INVALID_HANDLE_VALUE )
	 {
       r=CloseHandle(CommInfo[comPort].Handle());// handle of object to close
       CommInfo[comPort].Handle(INVALID_HANDLE_VALUE);
     }
  }
  return r;      // 0 - error  1 - success
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

bool WriteString(DInt Cid, DByte *lpData, DWORD dwLength)
{
	 DWORD    i;
	 HANDLE   h;
	 h = CommInfo[Cid-1].Handle();

	 PurgeComm(
	 		h,	// handle of communications resource
	 		PURGE_TXCLEAR	// action to perform
	 );
	 WriteFile(
	 	h,	// handle of file to write to
	 	(LPCVOID)lpData,	// address of data to write to file
	 	dwLength,	// number of bytes to write
	 	(LPDWORD)&i,	// address of number of bytes written
	 	NULL 	// addr. of structure needed for overlapped I/O
	  );
     return true;
} // WriteString
