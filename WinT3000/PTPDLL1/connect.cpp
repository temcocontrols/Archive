//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include <vcl/comctrls.hpp>
#include <time.h>
#include "..\define.h"
#include "..\baseclas.h"
#include "connect.h"
#include "..\ptpdll.h"
//#include "color.hpp"
#include "ptp.h"
#include "..\router.h"

extern DUint  msgWM_PTP_CONNECTION;
extern DUint  msgWM_PTP_RECEPTION;
extern DUint  msgWM_PTP_TRANSMISSION;
extern DUint  msgWM_PTP_ESTABLISH_CONNECTION;
extern ROUTING_TABLE_T3000 Routing_table[MAX_Routing_table];
extern DInt NetworkAddress;
extern DInt Station_NUM;
extern DInt router(DInt service, DInt command, ROUTER_PARAMETERS *r=NULL, DInt	port_number=-1, DInt send=0);
extern HANDLE initCommPort(DInt comPort);
extern DInt closeCommPort(DInt comPort);

extern DInt pwait(DWORD timesleep, DUint msg, MSG& myMsg, DInt breakcond = 0);

extern bool refreshTimeFlag;
extern DInt refreshTime;
extern COMMINFO CommInfo[MAX_COMM_INFO];

int timevar;
DByte table_crc8[256] =
  {
	0x00,0xFE,0xFF,0x01,0xFD,0x03,0x02,0xFC,0xF9,0x07,0x06,0xF8,0x04,0xFA,0xFB,0x05,
	0xF1,0x0F,0x0E,0xF0,0x0C,0xF2,0xF3,0x0D,0x08,0xF6,0xF7,0x09,0xF5,0x0B,0x0A,0xF4,
	0xE1,0x1F,0x1E,0xE0,0x1C,0xE2,0xE3,0x1D,0x18,0xE6,0xE7,0x19,0xE5,0x1B,0x1A,0xE4,
	0x10,0xEE,0xEF,0x11,0xED,0x13,0x12,0xEC,0xE9,0x17,0x16,0xE8,0x14,0xEA,0xEB,0x15,
	0xC1,0x3F,0x3E,0xC0,0x3C,0xC2,0xC3,0x3D,0x38,0xC6,0xC7,0x39,0xC5,0x3B,0x3A,0xC4,
	0x30,0xCE,0xCF,0x31,0xCD,0x33,0x32,0xCC,0xC9,0x37,0x36,0xC8,0x34,0xCA,0xCB,0x35,
	0x20,0xDE,0xDF,0x21,0xDD,0x23,0x22,0xDC,0xD9,0x27,0x26,0xD8,0x24,0xDA,0xDB,0x25,
	0xD1,0x2F,0x2E,0xD0,0x2C,0xD2,0xD3,0x2D,0x28,0xD6,0xD7,0x29,0xD5,0x2B,0x2A,0xD4,
	0x81,0x7F,0x7E,0x80,0x7C,0x82,0x83,0x7D,0x78,0x86,0x87,0x79,0x85,0x7B,0x7A,0x84,
	0x70,0x8E,0x8F,0x71,0x8D,0x73,0x72,0x8C,0x89,0x77,0x76,0x88,0x74,0x8A,0x8B,0x75,
	0x60,0x9E,0x9F,0x61,0x9D,0x63,0x62,0x9C,0x99,0x67,0x66,0x98,0x64,0x9A,0x9B,0x65,
	0x91,0x6F,0x6E,0x90,0x6C,0x92,0x93,0x6D,0x68,0x96,0x97,0x69,0x95,0x6B,0x6A,0x94,
	0x40,0xBE,0xBF,0x41,0xBD,0x43,0x42,0xBC,0xB9,0x47,0x46,0xB8,0x44,0xBA,0xBB,0x45,
	0xB1,0x4F,0x4E,0xB0,0x4C,0xB2,0xB3,0x4D,0x48,0xB6,0xB7,0x49,0xB5,0x4B,0x4A,0xB4,
	0xA1,0x5F,0x5E,0xA0,0x5C,0xA2,0xA3,0x5D,0x58,0xA6,0xA7,0x59,0xA5,0x5B,0x5A,0xA4,
	0x50,0xAE,0xAF,0x51,0xAD,0x53,0x52,0xAC,0xA9,0x57,0x56,0xA8,0x54,0xAA,0xAB,0x55
  };

DUint table_crc16[256] = {
	0x0000,0x1189,0x2312,0x329b,0x4624,0x57ad,0x6536,0x74bf,0x8c48,0x9dc1,0xaf5a,0xbed3,0xca6c,0xdbe5,0xe97e,0xf8f7,
	0x1081,0x0108,0x3393,0x221a,0x56a5,0x472c,0x75b7,0x643e,0x9cc9,0x8d40,0xbfdb,0xae52,0xdaed,0xcb64,0xf9ff,0xe876,
	0x2102,0x308b,0x0210,0x1399,0x6726,0x76af,0x4434,0x55bd,0xad4a,0xbcc3,0x8e58,0x9fd1,0xeb6e,0xfae7,0xc87c,0xd9f5,
	0x3183,0x200a,0x1291,0x0318,0x77a7,0x662e,0x54b5,0x453c,0xbdcb,0xac42,0x9ed9,0x8f50,0xfbef,0xea66,0xd8fd,0xc974,
	0x4204,0x538d,0x6116,0x709f,0x0420,0x15a9,0x2732,0x36bb,0xce4c,0xdfc5,0xed5e,0xfcd7,0x8868,0x99e1,0xab7a,0xbaf3,
	0x5285,0x430c,0x7197,0x601e,0x14a1,0x0528,0x37b3,0x263a,0xdecd,0xcf44,0xfddf,0xec56,0x98e9,0x8960,0xbbfb,0xaa72,
	0x6306,0x728f,0x4014,0x519d,0x2522,0x34ab,0x0630,0x17b9,0xef4e,0xfec7,0xcc5c,0xddd5,0xa96a,0xb8e3,0x8a78,0x9bf1,
	0x7387,0x620e,0x5095,0x411c,0x35a3,0x242a,0x16b1,0x0738,0xffcf,0xee46,0xdcdd,0xcd54,0xb9eb,0xa862,0x9af9,0x8b70,
	0x8408,0x9581,0xa71a,0xb693,0xc22c,0xd3a5,0xe13e,0xf0b7,0x0840,0x19c9,0x2b52,0x3adb,0x4e64,0x5fed,0x6d76,0x7cff,
	0x9489,0x8500,0xb79b,0xa612,0xd2ad,0xc324,0xf1bf,0xe036,0x18c1,0x0948,0x3bd3,0x2a5a,0x5ee5,0x4f6c,0x7df7,0x6c7e,
	0xa50a,0xb483,0x8618,0x9791,0xe32e,0xf2a7,0xc03c,0xd1b5,0x2942,0x38cb,0x0a50,0x1bd9,0x6f66,0x7eef,0x4c74,0x5dfd,
	0xb58b,0xa402,0x9699,0x8710,0xf3af,0xe226,0xd0bd,0xc134,0x39c3,0x284a,0x1ad1,0x0b58,0x7fe7,0x6e6e,0x5cf5,0x4d7c,
	0xc60c,0xd785,0xe51e,0xf497,0x8028,0x91a1,0xa33a,0xb2b3,0x4a44,0x5bcd,0x6956,0x78df,0x0c60,0x1de9,0x2f72,0x3efb,
	0xd68d,0xc704,0xf59f,0xe416,0x90a9,0x8120,0xb3bb,0xa232,0x5ac5,0x4b4c,0x79d7,0x685e,0x1ce1,0x0d68,0x3ff3,0x2e7a,
	0xe70e,0xf687,0xc41c,0xd595,0xa12a,0xb0a3,0x8238,0x93b1,0x6b46,0x7acf,0x4854,0x59dd,0x2d62,0x3ceb,0x0e70,0x1ff9,
	0xf78f,0xe606,0xd49d,0xc514,0xb1ab,0xa022,0x92b9,0x8330,0x7bc7,0x6a4e,0x58d5,0x495c,0x3de3,0x2c6a,0x1ef1,0x0f78
  };

//int      head_Data, ps->tail_Data, ps->overflow;
//DInt     RxDelayTime=100;                   //, WriteFlag;
//Byte     ComData[MAXDATA];

__declspec(dllexport) void ProcCommTimerTimer(DInt comPort, DInt DelayTime);

void ProcCommTimerTimer(DInt comPort, DInt DelayTime)
{
   COMSTAT          CommState;
   DInt             port, insert;
   DWORD            lErrors, i;
   PORT_STATUS_variables *ps;
   PTP              *ptp;
   PTP_FRAME        sframe;
   DByte            c_lsr_mcr;
   int              complete_frame, nRead;           // Number of characters read
   HANDLE           hCom;
//   time_t           t;
//   char             ptext[30];

    hCom = CommInfo[comPort].Handle();
    if( hCom==INVALID_HANDLE_VALUE) return;
    port = CommInfo[comPort].conPort();
    if( port==-1 || ( port<0 && port>=MAX_Routing_table ) )
        return;
   	ptp = (class PTP *)Routing_table[port].ptr;
	if(!ptp) return;

    if ( ptp->transmission_state == TR_PENDING )
    {
	 if ( ptp->ResponseTimer > 0 ) ptp->ResponseTimer -= DelayTime;
     if( ptp->ResponseTimer <= 0 )
     {
//      ProcPostMessage(WM_PTP_TRANSMISSION,2);
		PostMessage(
			 ptp->Handle,	// handle of destination window
			 msgWM_PTP_TRANSMISSION,	// message to post
			 comPort,	// first message parameter
			 2 	// second message parameter
		);

     }
    }
/*
    timevar += RxDelayTime;
    if( timevar > 60000)
    {
      timevar = 0;
      time(&t);
      strcpy(ptext,ctime(&t));
      ptext[10] = 0;
      ptext[16] = 0;
      //      StatusBarMessage(SBTime, AnsiString(ptext) + "    " + AnsiString(&ptext[11]));
      if(Item)
       Item->Text = AnsiString(ptext) + "    " + AnsiString(&ptext[11]);
    }
//    if( refreshTimeFlag )
//    {
    if( refreshTime > 0 ) refreshTime -= RxDelayTime;
    if( refreshTime <= 0 && refreshTime > -RxDelayTime )
    {
//       ProcPostMessage(WM_REFRESHGRID, GridForm->Point_type);
		 PostMessage(
			 ptp->Handle,	// handle of destination window
			 WM_REFRESHGRID,	// message to post
			 0,   	// first message parameter
			 0 	// second message parameter
		 );
         refreshTime = -RxDelayTime;
    }
//    }
*/
	ps  = &Routing_table[port].port_status_vars;
	ps->SilenceTimer   += DelayTime;
	ps->HeartbeatTimer += DelayTime;
//	if( WriteFlag )
//	{
//	 return;
//	}
	if( ps->HeartbeatTimer >= T_HEARTBEAT )
	{
		if( ps->PTP_comm_status.reception_blocked == Q_NOT_BLOCKED )
			 sframe.FrameType = HEARTBEAT_XON;
		else
			 sframe.FrameType = HEARTBEAT_XOFF;
		sframe.Length = 0;
		ptp->PTP_SendFrame((PTP_FRAME *)&sframe);
		return;
	}

//	h = CommInfo[0].handle;

//		  PTP CONNECTION WITH RECEIVE FRAME
	complete_frame=0;
//   if ( ps->overflow || ps->tail_Data!=head_Data )
	do
	{
		ClearCommError(
				hCom,	// handle of communications device
				&lErrors,	// address of variable to receive error codes
				&CommState	// address of buffer for communications status
		);
		if( lErrors&&CE_FRAME || lErrors&&CE_IOE || lErrors&&CE_OVERRUN )
				ps->ReceiveError=TRUE;
		nRead = CommState.cbInQue;        // Number of characters in
		if(nRead)
		{
				if (!ps->overflow)
				{
				 if( ps->head_Data >= ps->tail_Data )
				 {
					if( nRead <= MAXDATA-ps->head_Data  )
					{
					 ReadFile(
						hCom,	// handle of file to read
						&ps->ComData[ps->head_Data],	// address of buffer that receives data
						nRead,	// number of bytes to read
						&i,	// address of number of bytes read
						NULL 	// address of structure for data
					 );
					}
					else
					{
					 ReadFile(
						hCom,	// handle of file to read
						&ps->ComData[ps->head_Data],	// address of buffer that receives data
						MAXDATA-ps->head_Data,	// number of bytes to read
						&i,	// address of number of bytes read
						NULL 	// address of structure for data
					 );
					}
					nRead -= i;
					ps->head_Data += (DInt)i;
					if( ps->head_Data == MAXDATA ) ps->head_Data = 0;
					if( ps->head_Data == ps->tail_Data ) ps->overflow = 1;
				 }
				 if( !ps->overflow && nRead && ps->head_Data < ps->tail_Data )
				 {
					if( nRead <= ps->tail_Data-ps->head_Data )
					{
					 ReadFile(
						hCom,	// handle of file to read
						&ps->ComData[ps->head_Data],	// address of buffer that receives data
						nRead,	// number of bytes to read
						&i,	// address of number of bytes read
						NULL 	// address of structure for data
					 );
					}
					else
					{
					 ReadFile(
						hCom,	// handle of file to read
						&ps->ComData[ps->head_Data],	// address of buffer that receives data
						ps->tail_Data-ps->head_Data,	// number of bytes to read
						&i,	// address of number of bytes read
						NULL 	// address of structure for data
					 );
	//       ps->overflow = 1;
					}
					nRead -= i;
					ps->head_Data += (DInt)i;
					if( ps->head_Data == ps->tail_Data ) ps->overflow = 1;
				 }
				}
		}

		if ( ps->overflow || ps->tail_Data!=ps->head_Data )
		{
		 do
		 {
			 insert=1;
			 c_lsr_mcr = ps->ComData[ps->tail_Data];
			 if( ps->connection != PTP_IDLE )
			 {
				 if( c_lsr_mcr == 0x10 )
				 {
					ptp->data.rx_control_char = 1;
					insert=0;
				 }
				 else
				 {
					if( ptp->data.rx_control_char )
					{
					 c_lsr_mcr &= (DByte)~0x80;
					 ptp->data.rx_control_char = 0;
					}
				 }
				 if( insert )
				 {
					if( ps->ReceiveError==TRUE )
					{
					 ps->ReceiveError=FALSE; ps->Preamble1=ps->Preamble2=0;
					 if( ptp->data.recframe )
					 {
						((struct PTP_ReceivedFrame *)ptp->data.recframe)->status = 0;
					 }
					 ps->ReceiveFrameStatus = RECEIVE_FRAME_IDLE;
					 ptp->data.rx_control_char = 0;
					}
					if( ps->Preamble1 && ps->SilenceTimer>PTP_Tframe_abort )
					{
					 ps->Preamble1=ps->Preamble2=0;
					 if( ptp->data.recframe )
					 {
						((struct PTP_ReceivedFrame *)ptp->data.recframe)->status = 0;
					 }
					 ps->ReceiveFrameStatus = RECEIVE_FRAME_IDLE;
					 ptp->data.rx_control_char = 0;
					}
					if( ps->Preamble2 )
					{
					 if( ps->ReceiveFrameStatus == RECEIVE_FRAME_IDLE )
					 {
						ps->HeaderCRC = table_crc8[ps->HeaderCRC^c_lsr_mcr];
						switch ( ps->HEADERState ) {
						 case HEADER_HeaderCRC:
							{
							 if( ps->HeaderCRC == 0x55 )
							 {
								if(!ps->Length)
								{
									insert=2;
									switch ( ps->FrameType ) {
									 case HEARTBEAT_XON:
									 {
										ps->PTP_comm_status.transmission_blocked = 0;
										break;
									 }
									 case HEARTBEAT_XOFF:
									 {
										ps->PTP_comm_status.transmission_blocked = 1;
										break;
									 }
									 case DATA_ACK_0_XON:
										if( ps->PTP_comm_status.TxSequence_number == 0 )
										{
										//	Ack0 XON
										ps->PTP_comm_status.ack0received = 1;
										ps->PTP_comm_status.transmission_blocked = 0;
										}
										else
										{
										//	Duplicate XON
										ps->PTP_comm_status.transmission_blocked = 0;
										}
										break;
									 case DATA_ACK_0_XOFF:
										if( ps->PTP_comm_status.	TxSequence_number == 0 )
										{
										//	Ack0 XOFF
										ps->PTP_comm_status.ack0received = 1;
										ps->PTP_comm_status.transmission_blocked = 1;
										}
										else
										{
										//	Duplicate XOFF
										ps->PTP_comm_status.transmission_blocked = 1;
										}
										break;
									 case DATA_ACK_1_XON:
										if( ps->PTP_comm_status.TxSequence_number == 0 )
										{
										//	Duplicate XON
										ps->PTP_comm_status.transmission_blocked = 0;
										}
										else
										{
										//	Ack1 XON
										ps->PTP_comm_status.ack1received = 1;
										ps->PTP_comm_status.transmission_blocked = 0;
										}
										break;
									 case DATA_ACK_1_XOFF:
										if( ps->PTP_comm_status.TxSequence_number == 0 )
										{
										//	Duplicate XOFF
										ps->PTP_comm_status.transmission_blocked = 1;
										}
										else
										{
										//	Ack1 XOFF
										ps->PTP_comm_status.ack1received = 1;
										ps->PTP_comm_status.transmission_blocked = 1;
										}
										break;
									 default:
										if( (ptp->data.recframe = ((class PTP *)Routing_table[port].ptr)->ReceivedFramePool.NextFreeEntry())!=0 )
										{
										((struct PTP_ReceivedFrame *)ptp->data.recframe)->Frame.Length = 0;
										((struct PTP_ReceivedFrame *)ptp->data.recframe)->Frame.FrameType = ps->FrameType;
										((struct PTP_ReceivedFrame *)ptp->data.recframe)->flags.ReceivedValidFrame = TRUE;
										((struct PTP_ReceivedFrame *)ptp->data.recframe)->status = 1;
										ptp->data.recframe = NULL;
										if( ps->FrameType==CONNECT_REQUEST || ps->FrameType==CONNECT_RESPONSE ||
											 ps->FrameType==DISCONNECT_REQUEST || ps->FrameType==DISCONNECT_RESPONSE )
										{
//										    ProcPostMessage(WM_PTP_CONNECTION,0);
											PostMessage(
												ptp->Handle,	// handle of destination window
												msgWM_PTP_CONNECTION,	// message to post
												comPort,	// first message parameter
												0 	// second message parameter
											);
										}
										else
										 if( ps->connection == PTP_CONNECTED )
										 {
//										    ProcPostMessage(WM_PTP_RECEPTION,0);
											PostMessage(
												ptp->Handle,	// handle of destination window
												msgWM_PTP_RECEPTION,	// message to post
												comPort,	// first message parameter
												0 	// second message parameter
											);
										 }
										}
										insert=0;
										break;
									}
									if (insert==2)
									{
									 ps->InactivityTimer = 0;
									 if( ptp->transmission_state == TR_PENDING )
									 {
											if( ps->FrameType!=HEARTBEAT_XOFF && ps->FrameType!=HEARTBEAT_XON )
											{
												if( ( ps->PTP_comm_status.ack0received && ps->PTP_comm_status.TxSequence_number == 0 ) ||
														 ( ps->PTP_comm_status.ack1received && ps->PTP_comm_status.TxSequence_number == 1 ) )
												{
													ps->PTP_comm_status.TxSequence_number = 1 - ps->PTP_comm_status.TxSequence_number;
													ps->PTP_comm_status.ack0received = 0;
													ps->PTP_comm_status.ack1received = 0;
													ptp->transmission_state = TR_READY;
//										            ProcPostMessage(WM_PTP_TRANSMISSION, 1);
													PostMessage(
													 ptp->Handle,	// handle of destination window
													 msgWM_PTP_TRANSMISSION,	// message to post
													 comPort,	// first message parameter
													 1 	// second message parameter
													 );
//
//-----------
//													insert=0;
//													ps->Preamble1=ps->Preamble2=0;
//													break;
//--------------
												}
//												else
//										            ProcPostMessage(WM_PTP_TRANSMISSION,0);
//
//													PostMessage(
//													 App->Handle,	// handle of destination window
//													 WM_PTP_TRANSMISSION,	// message to post
//													 0,	// first message parameter
//													 0 	// second message parameter
//													 );
//
											}
											insert=0;
											ps->Preamble1=ps->Preamble2=0;
											break;
									 }
									 else
									 {
//										 if( ps->FrameType!=HEARTBEAT_XOFF && ps->FrameType!=HEARTBEAT_XON )
										 {
//								            ProcPostMessage(WM_PTP_TRANSMISSION,0);
											PostMessage(
											 ptp->Handle,	// handle of destination window
											 msgWM_PTP_TRANSMISSION,	// message to post
											 comPort,	// first message parameter
											 0 	// second message parameter
											);
										 }
				                     }
									 insert=0;
									}
									ps->Preamble1=ps->Preamble2=0;
									complete_frame=1;
								}
								else    //DATA
								{
								 if( (ptp->data.recframe = ((class PTP *)Routing_table[port].ptr)->ReceivedFramePool.NextFreeEntry())!=0 )
								 {
									((struct PTP_ReceivedFrame *)ptp->data.recframe)->Frame.Length = ps->Length;
									((struct PTP_ReceivedFrame *)ptp->data.recframe)->Frame.FrameType = ps->FrameType;
									ptp->data.ptr = ((struct PTP_ReceivedFrame *)ptp->data.recframe)->Frame.Buffer;
									ptp->data.length = 0;
									ps->DataCRC=0xffff;
									ps->ReceiveFrameStatus = RECEIVE_FRAME_DATA;
								 }
								 else
								 {
									ps->Preamble1=ps->Preamble2=0;
//  		                  complete_frame=1;
								 }
								}
							 }
							 else
							 {
								ps->Preamble1=ps->Preamble2=0;
//      		           complete_frame=1;
							 }
							}
							break;
							case HEADER_Length2:
							{
							 ps->Length += c_lsr_mcr;
							 ps->HEADERState = HEADER_HeaderCRC;
							}
							break;
							case HEADER_Length1:
							{
							 ps->Length = (c_lsr_mcr<<8);
							 ps->HEADERState = HEADER_Length2;
							}
							break;
							case HEADER_FrameType:
							{
								ps->FrameType = c_lsr_mcr;
								ps->HEADERState = HEADER_Length1;

							}
							break;
							}
							}
							else  //RECEIVE_FRAME_DATA
							{
							ps->DataCRC= (ps->DataCRC >> 8 ) ^ table_crc16[ (ps->DataCRC&0xFF) ^ c_lsr_mcr ];
//						ps->DataCRC=CalcDataCRC(c_lsr_mcr, ps->DataCRC);
							if( ptp->data.length < ps->Length )
							{
							 ptp->data.length++;
							 *ptp->data.ptr++ = c_lsr_mcr;
							}
							else
							{
							 if( ptp->data.length++ == ps->Length+1 )
							 {
								if( ps->DataCRC==0x0f0b8 )
								 ((struct PTP_ReceivedFrame *)ptp->data.recframe)->flags.ReceivedValidFrame = TRUE;
								else
								 ((struct PTP_ReceivedFrame *)ptp->data.recframe)->flags.ReceivedInvalidFrame = TRUE;
								((struct PTP_ReceivedFrame *)ptp->data.recframe)->status = 1;
								ptp->data.recframe = NULL;
								if( ps->connection == PTP_CONNECTED )
								{
//							            ProcPostMessage(WM_PTP_RECEPTION, 0);
										 PostMessage(
				 							 ptp->Handle,	// handle of destination window
											 msgWM_PTP_RECEPTION,	// message to post
											 comPort,	// first message parameter
											 0 	// second message parameter
									     );
								}
								else
//								            ProcPostMessage(WM_PTP_CONNECTION, 0);
											PostMessage(
												ptp->Handle,	// handle of destination window
												msgWM_PTP_CONNECTION,	// message to post
												comPort,	// first message parameter
												0 	// second message parameter
											);
								ps->ReceiveFrameStatus = RECEIVE_FRAME_IDLE;
								ps->Preamble1=ps->Preamble2=0;
								complete_frame=1;
							 }
							}
							}
					}
					else
					{
					 if(ps->Preamble1)  //0x55
					 {
						if(c_lsr_mcr==0xff)
						{
							 ps->HeaderCRC=0xFF;
							 ps->HEADERState=HEADER_FrameType;
							 ps->Preamble2=0xFF;
						}
						else
							if(c_lsr_mcr!=0x55)
							{
							 ps->Preamble1=0;
							}
					 }
					 else
						if(c_lsr_mcr==0x55)
						{
							ps->Preamble1=0x55;
							ps->Preamble2=0;
						}
					}
				 }
				 ps->SilenceTimer=0;
			 }
			 if ( ++ps->tail_Data==MAXDATA ) ps->tail_Data = 0;
			 if (ps->overflow) ps->overflow=0;
			 if (complete_frame) return;
		 } while ( ps->tail_Data != ps->head_Data );
		}
		else
			 break;
	} while ( 1 );
}

__declspec(dllexport) DInt installPort(HWND Handle, DInt comPort);

DInt installPort(HWND Handle, DInt comPort)
{
   DInt ret = SUCCESS_REQUEST;
   int j,conPort;
   if( CommInfo[comPort].Connection()== 0 )
       return ERROR_COM_PORT_NOT_CONFIGURED;
   for(j=0;j<MAX_Routing_table;j++)
   {
     if( Routing_table[j].status == PORT_FREE ) break;
   }
   if( j<MAX_Routing_table )
   {
    if( initCommPort(comPort)==INVALID_HANDLE_VALUE )
       return ERROR_OPEN_COM_PORT;

    conPort = j;
    Routing_table[conPort].port_status_vars.RxDelayTime = 100;
	Routing_table[conPort].port_status_vars.connection = PTP_IDLE;
	Routing_table[conPort].ptr = (class PTP *) new PTP(Handle,comPort,conPort);
	if ( Routing_table[conPort].ptr == NULL )
	{
//				delete (class PTP *)Routing_table[conPort].ptr;
//				Routing_table[conPort].ptr=NULL;
                closeCommPort(comPort);
				return ERROR_INSTALL_PORT;
	}
    Routing_table[conPort].port_status_vars.head_Data=0;
    Routing_table[conPort].port_status_vars.tail_Data=0;
    Routing_table[conPort].port_status_vars.overflow=0;
	Routing_table[conPort].status = PTP_INSTALLED;
	((class PTP *)Routing_table[conPort].ptr)->init_idle_state(0);
    ((class PTP *)Routing_table[conPort].ptr)->panel_info1.network = NetworkAddress;
    ((class PTP *)Routing_table[conPort].ptr)->panel_info1.panel_number = Station_NUM;
    CommInfo[comPort].conPort(conPort);
   }
   else
    ret = NOT_AVAILABLE_PORT;
   return ret;
}

__declspec(dllexport) DInt desinstallPort(DInt comPort);

DInt desinstallPort(DInt comPort)
{
   DInt ret = SUCCESS_REQUEST;
   if( comPort<0 || comPort>MAX_COMM_INFO ) return ERROR_REQUEST;
   closeCommPort(comPort);
   if( CommInfo[comPort].conPort()!=-1 )
   {
    if( Routing_table[CommInfo[comPort].conPort()].ptr )
    {
		 delete (class PTP *)Routing_table[CommInfo[comPort].conPort()].ptr;
		 Routing_table[CommInfo[comPort].conPort()].ptr=NULL;
    }
    Routing_table[CommInfo[comPort].conPort()].status=PORT_FREE;
    CommInfo[comPort].conPort(-1);
   }
   else
    ret=ERROR_REQUEST;
   return ret;
}

DInt checkConnection(DInt conPort, DInt *networkaddress, DInt *station_num)
{
		DInt result;
		result=ERROR_ESTABLISH_CONNECTION;
		if( Routing_table[conPort].status==PTP_ACTIVE )
		{
		  if( Routing_table[conPort].Port.network )
		  {
            	*networkaddress = Routing_table[conPort].Port.network;
				*station_num = Routing_table[conPort].Port.address;
				result=SUCCESS_REQUEST;
		  }
          else
		    result=SAME_NETWORK_NUMBER;
		}
		return result;
}

__declspec(dllexport) void disconnectRequest(DInt comPort);

void disconnectRequest(DInt comPort)
{
  if( CommInfo[comPort].conPort() == -1 ) return;
  ((class PTP *)Routing_table[CommInfo[comPort].conPort()].ptr)->NL_parameters.primitive = DL_DISCONNECT_REQUEST;
   SendMessage(
		  ((class PTP *)Routing_table[CommInfo[comPort].conPort()].ptr)->Handle,	// handle of destination window
		  msgWM_PTP_CONNECTION,	// message to post
		  comPort,	// first message parameter
		  0             	// second message parameter
	 );
}

__declspec(dllexport) DInt establishConnection(DInt comPort, DInt *networkaddress, DInt *station_num);

DInt establishConnection(DInt comPort, DInt *networkaddress, DInt *station_num)
{
    MSG myMsg;
    DInt TimeOut;
	DInt result;
	ROUTER_PARAMETERS r;

	result = ERROR_ESTABLISH_CONNECTION;
    if( CommInfo[comPort].conPort()==-1 )
      return PORT_NOT_INSTALLED;
    Routing_table[CommInfo[comPort].conPort()].port_status_vars.head_Data=0;
    Routing_table[CommInfo[comPort].conPort()].port_status_vars.tail_Data=0;
    Routing_table[CommInfo[comPort].conPort()].port_status_vars.overflow=0;
	r.termination_time_value = 50;
 	router(N_UNITDATArequest, Establish_Connection_To_Network, &r, CommInfo[comPort].conPort());
	TimeOut = pwait(30000, WM_PTP_ESTABLISH_CONNECTION, myMsg, 1);
	if(!TimeOut)
	{
	  if( (result=checkConnection(CommInfo[comPort].conPort(), networkaddress, station_num)) != SUCCESS_REQUEST )
      {
       disconnectRequest(CommInfo[comPort].conPort());
      }
    }
    return result;
}

