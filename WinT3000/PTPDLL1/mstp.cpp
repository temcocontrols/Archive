//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
                                   
#include "mstp.h"
//---------------------------------------------------------------------------
#define RS485

#include <vcl/comctrls.hpp>
#include "..\define.h"
#include "..\baseclas.h"
#include "..\ptpdll.h"
#include "ptp.h"
#include "..\router.h"
//#include "netb_bac.h"
//#include "ipx.h"

#define READ  0
#define WRITE 1

extern DInt Station_NUM;
extern ROUTING_TABLE_T3000 Routing_table[MAX_Routing_table];
extern DInt NetworkAddress;
extern DInt router(DInt service, DInt command, ROUTER_PARAMETERS *r=NULL, DInt	port_number=-1, DInt send=0);

extern DByte VendorID;
extern DInt maxImages;
extern TImage **tImageRun;
extern TProgressBar *ProgressBar;
void runImage(TImage *tImage[], DInt maximage);


DInt _export networklayer( DInt service, DInt priority, DInt network, DInt destination, DInt source,
						DByte *asdu_npdu, DInt length_asdu_npdu, DByte *apci, DInt length_apci,
						DInt data_expecting_reply=BACnetDataNotExpectingReply, DInt clientserver=CLIENT, DInt port = -1);

const DInt FFFF = 0xFFFF;
int   indexcommImage;
//long data_const, data_length_const;
//DInt  LengthReceivedClientAPDU;
//DByte *PTRReceivedClientAPDU;
//HWND hwndPB;    // handle of progress bar
//DInt progress_length;
//AnsiString sirtext, progresstext = "ллллллллллллллллллллллллллллллллллллллллл";

DInt clientprocedure( ClientTSMStateEnum state, DByte command, DInt bank, DByte *asdu, DByte **data, DInt lmax, DUint *length, DUint *length_asdu, DUint *last_length, DInt str_size)
{
 DUint l;
 DInt str_new_size;
// DByte *ptr;
// DByte no_points;
// Bank_Type tbank;
// tbank = *((Bank_Type *)&bank);
 if(command > 100)
 {
  command -= 100;
 }
 else
 {
	return READ;
 }
	str_new_size = lmax;
	switch( command )            //command
	{
		case T3000_OUT+1:
		case T3000_IN+1:
		case VAR+1:
		case CON+1:
		case WR+1:
		case AR+1:
		case PRG+1:
		case GRP+1:
		case AY+1:
		{
//		  ptr = (DByte *)&ptr_panel->info[command-1];
//		  str_new_size = ((Info_Table *)ptr)->str_size;
		  str_new_size = str_size;
		  break;
		}
	}
 if( (*length) > lmax )
 {
	l = lmax/str_new_size;
	*length_asdu = l*str_new_size;
	*length -= *length_asdu;
 }
 else
 {
	*length_asdu = *length;
	*length = 0;
 }
 if(*data)
 {
  memcpy(asdu, *data, *length_asdu);
  *data += *length_asdu;
 }
 *last_length += *length_asdu;
 return WRITE;
}

// t=0 application,  t=1 context
DInt encodetag(DByte cl, DByte t, DByte *tag, DUint length)
{
  DInt n=0;
	if( length <= 4 )
  {
	tag[0]=(t<<4)+(cl<<3)+ length;
	n=1;
  }
  if( length >= 5 && length<=253 )
  {
	tag[0]=(t<<4)+(cl<<3)+0x05;
	tag[1]=length;
	n=2;
  }
//  if( length >= 254 && length<=65535 )
  if( length >= 254 )
  {
	tag[0]=(t<<4)+(cl<<3)+0x05;
	tag[1]=254;
	tag[2]=(length>>8);
	tag[3]=length&0x00FF;
	n=4;
  }
  return n;
}

DInt decodetag(DByte *tag, DUint *length)
{
  DUint n=0, l;
  l = tag[0]&0x07;
  if( l <= 4 )
  {
	n=1;
  }
	if( l==5 )
  {
	n=2;
	l = tag[1];
	if( l==254 )
	{
	 n=4;
	 l = ( ((DUint)tag[2]) << 8)+tag[3];
	}
	}
	*length = l;
	return n;
}

void sendmessage(void)
{
   DWORD            dwStopTime;
   MSG              myMsg;
   dwStopTime  = GetTickCount() + 800;
   while ( GetTickCount() < dwStopTime )
   {
   		if(PeekMessage(&myMsg, NULL, NULL, NULL, PM_REMOVE))
   		{
   			if( myMsg.message == WM_KEYDOWN && myMsg.wParam == VK_CANCEL )
   			{
   			 break;
   			}
   			TranslateMessage(&myMsg);
   			DispatchMessage (&myMsg);
   		}
   }
}

void runImage(TImage *tImage[], DInt maximages)
{
  if(tImage)
  {
    if( indexcommImage == (maximages-1) )
    {
     tImage[indexcommImage]->Visible = false;
     tImage[0]->Visible = true;
     indexcommImage=0;
    }
    else
    {
     tImage[indexcommImage]->Visible = false;
     tImage[indexcommImage+1]->Visible = true;
     ++indexcommImage;
    }
  }      
}

void ProcSetProgress(TProgressBar *ProgressBar)
{
// ProgressBar->Top = MainScreen->StatusBar->Top+2;
// ProgressBar->Left = 0;
 if( ProgressBar )
	 ProgressBar->Visible = true;
}

void ProcResetProgress(TProgressBar *ProgressBar)
{
 if( ProgressBar )
 {
  ProgressBar->Position = 0;
  ProgressBar->Visible = false;
 }
}

void ProcUpdateProgress(TProgressBar *ProgressBar, int pos)
{
 if( ProgressBar )
   ProgressBar->Position = pos;
}

__declspec(dllexport) int ClientTransactionStateMachine(
			 ServicePrimitive Event,  DInt Service, FlowControl flowcontrol, DInt network,
			 DInt destination, DInt source, DByte invokeID, DByte *data, DUint *data_length,
			 DByte command, DInt arg, DByte vendorID, DLong timeout = TIMEOUT_NETCALL,
			 DByte *returnargs=NULL, DInt *length_returnargs=NULL,
			 DByte *sendargs=NULL, DInt length_sendargs=0, DByte bytearg=0, DInt task = -1, DInt str_size=400
			);

int ClientTransactionStateMachine(
			 ServicePrimitive Event,  DInt Service, FlowControl flowcontrol, DInt network,
			 DInt destination, DInt source, DByte invokeID, DByte *data, DUint *data_length,
			 DByte command, DInt arg, DByte vendorID, DLong timeout,
			 DByte *returnargs, DInt *length_returnargs,
			 DByte *sendargs, DInt length_sendargs, DByte bytearg, DInt task, DInt str_size
			)
{
 DByte apci[MAXAPCI], asdu[MAXAPDUSIZE];
 DByte next, win_size, retry=0, compressed=0;
 DByte PDUtype, seg, mor, nosequence, windowsize, srv;
 DInt sa,current, error, service, length_apci = 4;
 DUint n;
 DUint length, length_asdu, last_length, data_length_pack;
 struct CTSMTable *ptable;
 DWORD            dwStopTime,timeVar;
 MSG              myMsg;

 indexcommImage = 0;
 length_asdu = 0;
 length = *data_length;
 error = RS232_SUCCESS;
 next=1;
 if ((current=ClientTSMTable.newentry(task, network, destination, source, (DInt)invokeID)) == CTSM_ILLEGAL)
 {
	return ERROR_COMM;   //reject PDU
 }
 ptable = &ClientTSMTable.table[current];
 while(next)
 {
	switch ( CTSM_State ){
	 case CTSM_IDLE:
			if (Event == UNCONF_SERVrequest)
			{
//			  send_Unconf_Req PDU
				apci[0] = (BACnetUnconfirmedRequestPDU<<4);
				apci[1] = UnconfirmedPrivateTransfer;

				apci[2]=0x09;             // Context Tag 0( Unsigned, Length=1)
				apci[3]=VendorID;
				apci[4]=0x1A;             // Context Tag 1( Unsigned, Length=2)
				apci[5] = command;
				apci[6] = *(DByte*)&arg;
				n = 3+length_asdu+length_sendargs;      // 3 = arg low, arg high, res
																								 // 2 = length of sendags
				length_apci = 7 + encodetag(1, 2, &apci[7], n);
// parameters
				apci[length_apci++] = *(DByte*)&arg;      //extra low
				apci[length_apci++] = *((DByte*)&arg+1);  //extra high
				apci[length_apci++] = 0;                 //reserved

				if(command <= 100)
					 if(sendargs)
					 {
						length_asdu = (length_sendargs>MAXAPDUSIZE)?0:length_sendargs;
						memcpy(asdu, sendargs, length_asdu);
					 }
				networklayer( N_UNITDATArequest, NORMALmessage, network, destination, source, asdu, length_asdu, apci, length_apci, BACnetDataNotExpectingReply);
				CTSM_State=CTSM_IDLE;
				next=0;
			}
			if (Event == CONF_SERVrequest)
			{
			 if (Service==ConfirmedPrivateTransfer)
			 {

				win_size = 0;
				last_length = 0;
				nosequence = 1;
				while( ++win_size<=WINDOWSIZE )
				{
				 if(command > 100)       //  Write
				 {
					clientprocedure( CTSM_State, command, arg, &asdu[0], &data, MAXASDUSIZE, &length, &length_asdu, &last_length, str_size);
					if(length)
					{               //segmented message
					apci[0] = (BACnetConfirmedRequestPDU<<4)|0x0c ;
					CTSM_State=CTSM_SEGMENTING_REQUEST;
					mor=1;
					}
					else
					{
					apci[0] = (BACnetConfirmedRequestPDU<<4);
					CTSM_State=CTSM_AWAIT_CONFIRMATION;
					mor=0;
					}
				 }
				 else                   //read
				 {
					apci[0] = (BACnetConfirmedRequestPDU<<4)+(1<<1);
					CTSM_State=CTSM_AWAIT_CONFIRMATION;
					mor = 0;
				 }
				 apci[1] = 0x03;  //up to 480 bytes
				 apci[2] = invokeID;
				 if(mor)
				 {
					apci[3] = nosequence++;
					apci[4] = WINDOWSIZE;
					apci[5] = Service;
					length_apci = 6;
				 }
				 else
				 {
					apci[3] = Service;
					length_apci = 4;
				 }
				 apci[length_apci++] = 0x09;            // Context Tag 0( Unsigned, Length=1)
				 apci[length_apci++] = vendorID;
				 apci[length_apci++] = 0x1A;            // Context Tag 1( Unsigned, Length=2)
				 apci[length_apci++] = command;
				 apci[length_apci++] = *(DByte*)&arg;
				 if(command > 100)
					 n = 3+length_asdu;      // 3 = arg low, arg high, res
				 else
					 n = 3+length_asdu+length_sendargs;      // 3 = arg low, arg high, res
																		// 2 = length of sendags
				 if(mor)
					n += 2;  //  2 = data length
				 length_apci += encodetag(1, 2, &apci[length_apci], n);
// parameters
				 apci[length_apci++] = *(DByte*)&arg;      //extra low
				 apci[length_apci++] = *((DByte*)&arg+1);  //extra high
				 apci[length_apci++] = 0;                 //reserved
				 if(mor)
				 {
					memcpy(&apci[length_apci], data_length, 2);
					length_apci += 2;
				 }

				 if(command <= 100)
					 if(sendargs)
					 {
						length_asdu = (length_sendargs>MAXAPDUSIZE)?0:length_sendargs;
						memcpy(asdu, sendargs, length_asdu);
					 }

//				 PTRReceivedClientAPDU = NULL;
//				 LengthReceivedClientAPDU = 0;
				 networklayer( N_UNITDATArequest, NORMALmessage, network, destination, source, asdu, length_asdu, apci, length_apci, BACnetDataExpectingReply);
				 if(!mor) break;
				}
                if( command > 100 && mor )
                {
                  ProcSetProgress(ProgressBar);
                }
				win_size=0;
			 }
			}
			break;
	 case CTSM_SEGMENTING_REQUEST:
/*
			if (Event == SEGMENT_ACK.indication // with server==TRUE)
			{
			 if (not final APDU segment)
			 {
				send Conf_Req PDU with
					  segmented_msg = TRUE
						more_follows = TRUE
			 }
			 if (final APDU segment)
			 {
				send Conf_Req PDU with
					  segmented_msg = TRUE
					  more_follows = FALSE
			 }
			 if (no more APDU segments)
			 {
				 CTSM_State=CTSM_AWAIT_CONFIRMATION;
			 }
			}
*/
/*
      T3000   implementation
			error = RS232_ERROR;
			l = timestart;
			if( ptable->task  >= 0 )
			{
				 if( !ptable->state )
					 msleep(timeout*18.2);
			}
			else
			{
				while ( !ptable->state && timestart < l+timeout )
				{
				 if( bioskey_new(1) == CTRLBREAK )
				 {
					error = USER_ABORT;
					break;
				 }
				}
			}
      T3000   implementation
*/
			error = RS232_ERROR;
			dwStopTime  = GetTickCount() + timeout*1000;
//       wait for respons
            timeVar = GetTickCount() + 800;
			while ( !ptable->state && GetTickCount() < dwStopTime )
			{
			    if(GetTickCount() > timeVar)
			    {
			        timeVar = GetTickCount() + 800;
			        runImage(tImageRun,maxImages);
			    }
				if(PeekMessage(&myMsg, NULL, NULL, NULL, PM_REMOVE))
				{
					if( myMsg.message == WM_KEYDOWN && myMsg.wParam == VK_CANCEL )
					{
					 error = USER_ABORT;
					 break;
					}
					TranslateMessage(&myMsg);
					DispatchMessage (&myMsg);
        		}
			}
	        runImage(tImageRun,maxImages);

			if( ptable->state )
			{
//				invokeid = ptable->data[1];
				PDUtype  = ptable->data[0]>>4;
				srv      = (ptable->data[0]&0x01)&&1;
				memcpy(apci, ptable->data, MAXAPCI);
				if(PDUtype==BACnetRejectPDU)
					error = RS232_REJECT;
				else
					error = RS232_SUCCESS;
			}
			if( error == RS232_ERROR || (error == USER_ABORT || error == RS232_REJECT) || retry >=1 )
			{
			 apci[0] = (BACnetAbortPDU<<4);   //server=FALSE
			 apci[1] = invokeID;
			 apci[2] = 0;           //other reasons
			 length_apci = 3;
			 networklayer( N_UNITDATArequest, NORMALmessage, network, destination, source, NULL, 0, apci, length_apci, BACnetDataNotExpectingReply);
			 sendmessage();
			 CTSM_State=CTSM_IDLE;
			 next=0;
			 break;
			}
/*
// retry
			if(error == RS232_ERROR)	//timeout
			{
			 retry++;
			 length += last_length;
			 *data -= last_length;
//			 last_length = 0;
			 nosequence--;
			 win_size=0;
			 PDUtype = BACnetSegmentACKPDU;
			 CTSM_State=CTSM_IDLE;
			 next=0;
			 break;
			}
*/
			if (PDUtype == BACnetSegmentACKPDU)
			{
			 if(!win_size) last_length = 0;
			 while(++win_size<=WINDOWSIZE && CTSM_State==CTSM_SEGMENTING_REQUEST)
			 {
				if(mor)
				{
				 clientprocedure( CTSM_State, command, arg, asdu, &data, MAXASDUSIZE, &length, &length_asdu, &last_length, str_size);
				 if(length)
				 {               //segmented message
					apci[0] = (BACnetConfirmedRequestPDU<<4)|0x0c ;
					mor=1;
				 }
				 else
				 {
					apci[0] = (BACnetConfirmedRequestPDU<<4)|0x08;
					mor=0;
					CTSM_State=CTSM_AWAIT_CONFIRMATION;
				 }
				 apci[1] = 0x03;  //up to 480 bytes
				 apci[2] = invokeID;
				 apci[3] = nosequence++;
				 apci[4] = WINDOWSIZE;
				 apci[5] = Service;
				 apci[6] = 0x09;            // Context Tag 0( Unsigned, Length=1)
				 apci[7] = vendorID;
				 apci[8] = 0x1A;            // Context Tag 1( Unsigned, Length=2)
				 apci[9] = command;
				 apci[10] = *(byte*)&arg;
				 length_apci = 11+encodetag(1, 2, &apci[11], length_asdu);
				 ptable->state = 0;
				 networklayer( N_UNITDATArequest, NORMALmessage, network, destination, source, asdu, length_asdu, apci, length_apci, BACnetDataExpectingReply);
                 ProcUpdateProgress(ProgressBar,(((*data_length)-length)*100)/(*data_length));
//				 if(showsign) communication_sign(data_const+(*data_length)-length,data_length_const?data_length_const:*data_length);
				}
				else
				{
				 ptable->state = 0;
				 CTSM_State=CTSM_AWAIT_CONFIRMATION;
				}
			 }
			 win_size=0;
			 retry=0;
			 break;
			}
/*
			if (Event == ABORT.indication with server==TRUE)
			{
			send ABORT.indication
				CTSM_State=CTSM_IDLE;
				next=0;
			}
*/
			if (PDUtype == BACnetAbortPDU && srv)
			{
			 CTSM_State=CTSM_IDLE;
			 error = RS232_ERROR;
			 next=0;
			 break;
			}
/*			if (unexpected PDU from server)
			{
			send Abort PDU with server = FALSE
				CTSM_State=CTSM_IDLE;
				next=0;
			}
		 if (timeout waiting for PDU)
		 {
			if(retry count not exceed)
			{
				resend previous CONF_req PDU segment
				 CTSM_State=CTSM_SEGMENTING_REQUEST;
			}
			if(retry count exceeded)
			{
				send CONF_SERV.confirm(-)
				 CTSM_State=CTSM_IDLE;
			 next=0;
			}
		 }
			if (Event == ABORT.request)
			{
			send Abort PDU with server = FALSE
				CTSM_State=CTSM_IDLE;
				next=0;
			}
*/
			CTSM_State=CTSM_IDLE;
			error = RS232_ERROR;
			next=0;

/*
			if (Event == SEGMENT_ACK.indication // with server==TRUE)
			{
			 if (not final APDU segment)
			 {
				send Conf_Req PDU with
						segmented_msg = TRUE
						more_follows = TRUE
			 }
			 if (final APDU segment)
			 {
				send Conf_Req PDU with
						segmented_msg = TRUE
						more_follows = FALSE
			 }
			 if (no more APDU segments)
			 {
				 CTSM_State=CTSM_AWAIT_CONFIRMATION;
			 }
			}
*/			break;
	 case CTSM_AWAIT_CONFIRMATION:
			error = RS232_ERROR;
			dwStopTime  = GetTickCount() + timeout*1000;
//       wait for respons
            timeVar = GetTickCount() + 800;
			while ( !ptable->state && GetTickCount() < dwStopTime )
			{
			    if(GetTickCount() > timeVar)
			    {
			        timeVar = GetTickCount() + 800;
			        runImage(tImageRun,maxImages);
			    }
				if(PeekMessage(&myMsg, NULL, NULL, NULL, PM_REMOVE))
				{
					if( myMsg.message == WM_KEYDOWN && myMsg.wParam == VK_CANCEL )
					{
					 error = USER_ABORT;
					 break;
					}
					TranslateMessage(&myMsg);
					DispatchMessage (&myMsg);
        		}
			}

	        runImage(tImageRun,maxImages);
			if( ptable->state )
			{
//				invokeid = ptable->data[1];
				PDUtype  = ptable->data[0]>>4;
				srv      = (ptable->data[0]&0x01)&&1;
				memcpy(apci, ptable->data, MAXAPCI);
				if(PDUtype==BACnetRejectPDU)
					error = RS232_REJECT;
				else
					error = RS232_SUCCESS;
//				}
			}
			if(error!=SUCCESS)		//timeout
			{
			 apci[0] = (BACnetAbortPDU<<4);   //server=FALSE
			 apci[1] = invokeID;
			 apci[2] = 0;           //other reasons
			 length_apci = 3;
			 networklayer( N_UNITDATArequest, NORMALmessage, network, destination, source, NULL, 0, apci, length_apci, BACnetDataNotExpectingReply);
			 sendmessage();
			 CTSM_State=CTSM_IDLE;
			 next=0;
			 break;
			}
			if (PDUtype == BACnetSimpleACKPDU)
			{
//			  send CONF_SERV.cnf(+)
				CTSM_State=CTSM_IDLE;
				next=0;
				break;
			}
			if (PDUtype == BACnetComplexACKPDU)
			{
			 seg = (apci[0] & 0x08)&&1;
			 mor = (apci[0] & 0x04)&&1;
			 if(seg)
			 {
				nosequence = apci[2];
				windowsize = apci[3];
				service = apci[4];
				sa = 5;
				if( nosequence != 1 )
				{      //error
				 error = RS232_ERROR;
				 CTSM_State=CTSM_IDLE;
				 next=0;
				 break;
				}
			 }
			 else
			 {
				service = apci[2];
				sa = 3;
			 }
			 if (service==Service)
			 {
				sa += decodetag(&apci[sa], &n);  //vendorid
				if( vendorID == apci[sa] )
				{
				sa += n;
				sa += decodetag(&apci[sa], &n);  //service
				if( (command == apci[sa]) && ( command!=50 || (command==50 &&  *(DByte*)&arg==apci[sa+1])) )
				{
				sa += n;
				sa += decodetag(&apci[sa], (DUint*)&last_length);   //asdu length
				if(seg==TRUE && mor==TRUE)  // segmented_msg = TRUE and
													// more_follows = TRUE
				{
/*
				if(segmentation is to be done by AE)
				{
				 send SegmentACK PDU with server = FALSE
				 CTSM_State=CTSM_AE_SEGMENTED_CONFIRMATION;
				}
*/
//      2 bytes - total length of response data
                ProcSetProgress(ProgressBar);
				data_length_pack =  *((unsigned int *)(&ptable->data[sa]));
				sa += 2;
//      READPROGRAMCODE_T3000 : 4 byte : 2 bytes - programs pool, 2 bytes - programs pool used
//      READGROUPELEMENTS_T3000 4 byte : 2 bytes - max_elements, 2 bytes - cur_elements
				if( command == READPROGRAMCODE_T3000 || command == READGROUPELEMENTS_T3000 )
				{
					if( returnargs )
						memcpy(returnargs, &ptable->data[sa], 4);
					sa += 4;
				}
				ptable->LengthReceivedClientAPDU -= sa;
				if(data)
				{
				 if(!compressed)
				 {
					memcpy(data, &ptable->data[sa], ptable->LengthReceivedClientAPDU);
				 }
				 else
				 {
//					ptable->LengthReceivedClientAPDU=uncompress(data, data_length_pack,  &ptable->data[sa], ptable->LengthReceivedClientAPDU);
				 }
				 *data_length += ptable->LengthReceivedClientAPDU;
				}

				if( flowcontrol == AEFLOWCONTROL )
				{
				 if(data)
					data += ptable->LengthReceivedClientAPDU;
				 last_length = ptable->LengthReceivedClientAPDU;
				 ptable->state = 0;
				 if( ++win_size >= windowsize )
				 {
					apci[0] = (BACnetSegmentACKPDU<<4);
					apci[1] = invokeID;
					apci[2] = nosequence;
					apci[3] = WINDOWSIZE;
					length_apci = 4;
					networklayer( N_UNITDATArequest, NORMALmessage, network, destination, source, NULL, 0, apci, length_apci, BACnetDataExpectingReply);
					win_size = 0;
				 }
				 CTSM_State=CTSM_AE_SEGMENTED_CONFIRMATION;
				}
/*
				if(segmentation is to be done by AP)
				{
				 send CONF_SERV.cnf(+) with more_follows = TRUE
				 CTSM_State=CTSM_AP_SEGMENTED_CONFIRMATION;
				}
*/
				if( flowcontrol == APFLOWCONTROL )
				{
				 if(data)
				 {
					memcpy(data, &ptable->data[sa], ptable->LengthReceivedClientAPDU - sa);
					*data_length += ptable->LengthReceivedClientAPDU-sa;
				 }
				 CTSM_State=CTSM_AE_SEGMENTED_CONFIRMATION;
				 next = 0;
				}
				break;
			 }
			 else
			 {
/*        check vendorID and command from asdu respons
					0x21;            // Application Tag 2( Unsigned, Length=1)
					PTRtable->vendorID;
					0x21;            // Application Tag 2( Unsigned, Length=1)
					PTRtable->command;
*/
//      READPROGRAMCODE_T3000 : 4 byte : 2 bytes - programs pool, 2 bytes - programs pool used
//      READGROUPELEMENTS_T3000 4 byte : 2 bytes - max_elements, 2 bytes - cur_elements
				 if( command == READPROGRAMCODE_T3000 || command == READGROUPELEMENTS_T3000 )
				 {
					if( returnargs )
						memcpy(returnargs, &ptable->data[sa], 4);
					sa += 4;
				 }
				 ptable->LengthReceivedClientAPDU -= sa;
/*  no compression for the moment
					if( (PTRReceivedClientAPDU[3]&0x01) )     //compressed?
					{
					memcpy(&n, PTRReceivedClientAPDU+4, 2);
					LengthReceivedClientAPDU -= 2;
					LengthReceivedClientAPDU=uncompress(data, n,  PTRReceivedClientAPDU+6, LengthReceivedClientAPDU);
					}
					else
*/
				 {
					if(data)
						memcpy(data, &ptable->data[sa], ptable->LengthReceivedClientAPDU);
					}
					*data_length += ptable->LengthReceivedClientAPDU;
					CTSM_State=CTSM_IDLE;
					next=0;
					break;
				 }
				}
				}
			 }
			}
/*
			if (CTSM_Event == ErrorPDU)
			{
				send CONF_SERV.cnf(-)
				CTSM_State=CTSM_IDLE;
				next=0;
			}
*/
/*
			if (CTSM_Event == RejectPDU with server=TRUE)
			{
				send REJECT.indication
				CTSM_State=CTSM_IDLE;
				next=0;
			}
*/
/*
			if (CTSM_Event == AbortPDU with server=TRUE)
			{
				send ABORT.indication
				CTSM_State=CTSM_IDLE;
				next=0;
			}
*/
			if (PDUtype == BACnetAbortPDU && srv)
			{
			 error = RS232_ERROR;
			 next=0;
			 break;
			}
/*
			if (CTSM_Event == Unexpected PDU from server)
			{
				send AbortPDU with server=FALSE
				CTSM_State=CTSM_IDLE;
			  next=0;
			}
*/
/*
		 if (timeout waiting for PDU)
		 {
			if(retry count not exceed and
				APDU > max_APDU_length_supported)
			{
				 send Conf_Req PDU with
					  segmented_msg = TRUE
					  more_follows = TRUE
				 CTSM_State=CTSM_SEGMENTING_REQUEST;
				}
			if(retry count not exceed and
				APDU <= max_APDU_length_supported)
			{
				 send Conf_Req PDU with
					  segmented_msg = FALSE
			  }
			if(retry count exceeded)
			{
			  send CONF_SERV.confirm(-)
				 CTSM_State=CTSM_IDLE;
			 next=0;
			}
		 }
*/
/*
			if (Event == ABORT.request)
			{
//         send Abort PDU with server = FALSE
				CTSM_State=CTSM_IDLE;
				next=0;
			}
*/
			CTSM_State=CTSM_IDLE;
			error = RS232_ERROR;
			next=0;
			break;
	 case CTSM_AE_SEGMENTED_CONFIRMATION:

			error = RS232_ERROR;
			dwStopTime  = GetTickCount() + timeout*1000;
            timeVar = GetTickCount() + 800;
			while ( !ptable->state && GetTickCount() < dwStopTime )
			{
			    if(GetTickCount() > timeVar)
			    {
			        timeVar = GetTickCount() + 800;
			        runImage(tImageRun,maxImages);
			    }
				if(PeekMessage(&myMsg, NULL, NULL, NULL, PM_REMOVE))
				{
					if( myMsg.message == WM_KEYDOWN && myMsg.wParam == VK_CANCEL )
					{
					 error = USER_ABORT;
					 break;
					}
					TranslateMessage(&myMsg);
					DispatchMessage (&myMsg);
				}
			}

	        runImage(tImageRun,maxImages);
			if( ptable->state )
			{
//				invokeid = ptable->data[1];
				PDUtype  = ptable->data[0]>>4;
				srv      = (ptable->data[0]&0x01)&&1;
				memcpy(apci, ptable->data, MAXAPCI);
				error = RS232_SUCCESS;
			}
			if(error!=SUCCESS)		//timeout
			{
			 apci[0] = (BACnetAbortPDU<<4);   //server=FALSE
			 apci[1] = invokeID;
			 apci[2] = 0;           //other reasons
			 length_apci = 3;
			 networklayer( N_UNITDATArequest, NORMALmessage, network, destination, source, NULL, 0, apci, length_apci, BACnetDataNotExpectingReply);
			 sendmessage();
			 CTSM_State=CTSM_IDLE;
			 next=0;
			 break;
			}
/*
			if (CTSM_Event == Complex_ACK PDU with segmented_msg = TRUE
																more_follows = TRUE )
			{
				 send SegmentACK PDU with server = FALSE
				 if(buffer full or other reasons)
				 {
					send Abort PDU with server=FALSE
					send CONF_SERV.cnf(-)
					CTSM_State=CTSM_IDLE;
					next=0;
				 }
			}
			if (CTSM_Event == Complex_ACK PDU with segmented_msg = TRUE
																more_follows = FALSE )
			{
				send SegmentACK PDU with server = FALSE
				send CONF_SERV.cnf(+)
				CTSM_State=CTSM_IDLE;
				next=0;
			}
*/
			if (PDUtype == BACnetComplexACKPDU)
			{
			 seg = (apci[0] & 0x08)&&1;
			 mor = (apci[0] & 0x04)&&1;
//			 invokeid = apci[1];
			 if(seg)
			 {
//			  if( apci[2] < nosequence-(win_size?win_size-1:windowsize-1) || apci[2] > nosequence+windowsize-win_size)
				if( apci[2] < nosequence-(win_size?win_size-1:windowsize-1) || apci[2] > nosequence+1)
				{      //error
				 ptable->state = 0;
				 break;
//				 CTSM_State=CTSM_IDLE;
//				 error = RS232_ERROR;
//				 next=0;
//				 break;
				}
				if( apci[2] == nosequence-(win_size?win_size-1:windowsize-1 ) )   // server resent last segment
				{
				 if(data)
					data -= last_length;
				 *data_length -= last_length;
				 last_length = 0;
				 win_size = 0;
				}
				if( apci[2] == nosequence+1 && !win_size ) // next segment
				 last_length = 0;

				nosequence = apci[2];
				windowsize = apci[3];
				service = apci[4];
				sa=5;
			 }
			 else
			 {
				service = apci[2];
				sa = 3;
			 }
/*
				application octet string
				have to check vendorID and command from asdu respons
					0x21;            // Application Tag 2( Unsigned, Length=1)
					PTRtable->vendorID;
					0x21;            // Application Tag 2( Unsigned, Length=1)
					PTRtable->command;
*/
			 if (service==Service)
			 {
				sa += decodetag(&apci[sa], &n);  //vendorid
				if( vendorID == apci[sa] )
				{
				 sa += n;
				 sa += decodetag(&apci[sa], &n);  //service
				 if( (command == apci[sa]) && ( command!=50 || (command==50 &&  *(DByte*)&arg==apci[sa+1])) )
				 {
					sa += n;
					sa += decodetag(&apci[sa], (DUint*)&last_length);   //asdu length
					if(seg && nosequence==1)   // resent first segment
					{
//      2 bytes - total length of response data
					 data_length_pack = *((unsigned int *)(&ptable->data[sa]));
					 sa += 2;
//      READPROGRAMCODE_T3000 : 4 byte : 2 bytes - programs pool, 2 bytes - programs pool used
//      READGROUPELEMENTS_T3000 4 byte : 2 bytes - max_elements, 2 bytes - cur_elements
					 if( command == READPROGRAMCODE_T3000 || command == READGROUPELEMENTS_T3000 )
					 {
						if( returnargs )
							memcpy(returnargs, &ptable->data[sa], 4);
						sa += 4;
					 }
					}
					ptable->LengthReceivedClientAPDU -= sa;
					if(data)
					{
					 if(!compressed)
					 {
						memcpy(data, &ptable->data[sa], ptable->LengthReceivedClientAPDU);
					 }
					 else
                        ;
//						ptable->LengthReceivedClientAPDU=uncompress(data, data_length_pack-(*data_length),  &ptable->data[sa], ptable->LengthReceivedClientAPDU);
					 data += ptable->LengthReceivedClientAPDU;
					 last_length = ptable->LengthReceivedClientAPDU;
					 *data_length += ptable->LengthReceivedClientAPDU;
                     ProcUpdateProgress(ProgressBar,(*data_length*100)/data_length_pack);
					}
					if( seg==TRUE )
					{
					 ptable->state = 0;
					 if( ++win_size >= windowsize )
					 {
						apci[0] = (BACnetSegmentACKPDU<<4);
						apci[1] = invokeID;
						apci[2] = nosequence;
						apci[3] = WINDOWSIZE;
						length_apci = 4;
						networklayer( N_UNITDATArequest, NORMALmessage, network, destination, source, NULL, 0, apci, length_apci, mor==TRUE?BACnetDataExpectingReply:BACnetDataNotExpectingReply);
						win_size = 0;
					 }
					}
					if(mor==FALSE)
					{
                     sendmessage();
					 CTSM_State=CTSM_IDLE;
					 next=0;
					}
				 }
				 else
				 {
					CTSM_State=CTSM_IDLE;
					error = RS232_ERROR;
					next=0;
				 }
				}
				else
				{
				 CTSM_State=CTSM_IDLE;
				 error = RS232_ERROR;
				 next=0;
				}
			 }
			 else
			 {
				CTSM_State=CTSM_IDLE;
				error = RS232_ERROR;
				next=0;
			 }
			 break;
			}
			if (PDUtype == BACnetAbortPDU && srv)
			{
			 CTSM_State=CTSM_IDLE;
			 error = RS232_ERROR;
			 next=0;
			 break;
			}
/*
			if (CTSM_Event == AbortPDU with server==TRUE)
			{
//         send ABORT.indication
				CTSM_State=CTSM_IDLE;
				next=0;
			}
			if (unexpected PDU from server)
			{
//         send Abort PDU with server = FALSE
				CTSM_State=CTSM_IDLE;
				next=0;
			}
//       if (timeout waiting for PDU)
//       {
//           send CONF_SERV.confirm(-)
//			    CTSM_State=CTSM_IDLE;
// 			 next=0;
//       }
			if (Event == ABORT.request)
			{
//         send Abort PDU with server = FALSE
				CTSM_State=CTSM_IDLE;
				next=0;
			}
*/
			 CTSM_State=CTSM_IDLE;
			 error = RS232_ERROR;
			 next=0;
			 break;
	} // end switch
 }  // end while
 ClientTSMTable.free(current);
 ProcResetProgress(ProgressBar);
 return error;
}

void ServerTransactionStateMachine(
		 DInt port, DInt network, DInt destination, DInt source, DByte *apdu, DInt length_apdu, DInt entrytimeout=-1, DInt destport=-1)
{
// DByte apci[MAXAPCI];
// DByte s1,s,next,win_size, postponed;
// DByte *asdu;
// DInt  n, length_apci, current, STSM_Event, STSM_PDUtype;
   DInt  STSM_Event;
   DByte STSM_NoSeq=1, STSM_SRV, STSM_InvokeID, STSM_WinSize;
// DUint length_asdu, l;
// DByte STSM_SEG, STSM_MOR, STSM_SA, STSM_MaxResp;
// DByte STSM_Service, STSM_SRV, STSM_REASON;
// signed DByte STSM_State;
// struct TSMTable *PTRtable;
// Serial *sptr;
// sptr = (Serial *)Routing_table[port].ptr;

// postponed=0;
 if(entrytimeout<0)
 {
  STSM_Event = apdu[0]>>4;
  switch ( STSM_Event ) {
	  case BACnetUnconfirmedRequestPDU:
/*
			 STSM_Service = apdu[1];
			 STSM_InvokeID = 255;
			 n=2;
*/
			 break;
	  case BACnetConfirmedRequestPDU:
/*
			 STSM_SEG      = (apdu[0]&0x08)&&1;
			 STSM_MOR      = (apdu[0]&0x04)&&1;
			 STSM_SA       = (apdu[0]&0x02)&&1;
			 STSM_MaxResp  = apdu[1];
			 STSM_InvokeID = apdu[2];
			 if( STSM_SEG )  STSM_NoSeq   = apdu[3];
			 if( STSM_SEG )  STSM_WinSize = apdu[4];
			 if( STSM_SEG ) {STSM_Service = apdu[5]; n=6;}
			 else           {STSM_Service = apdu[3]; n=4;};
*/
			 break;
	  case BACnetSegmentACKPDU:
			 STSM_SRV      = (apdu[0]&0x01)&&1;
			 STSM_InvokeID = apdu[1];
			 STSM_NoSeq    = apdu[2];
			 STSM_WinSize  = apdu[3];
//			 n=4;
			 if( STSM_SRV == TRUE )
			 {
				ClientTSMTable.received(destination, source, apdu, length_apdu);
				return;
			 }
			 break;
		case BACnetAbortPDU:
/*
			 STSM_SRV      = (apdu[0]&0x01)&&1;
			 STSM_InvokeID = apdu[1];
			 STSM_REASON   = apdu[2];
			 n =3;
*/
			 break;
		default:
			 ClientTSMTable.received(destination, source, apdu, length_apdu);
			 return;
  }

 }
}
/*
DInt networklayer( DInt service, DInt priority, DInt network, DInt destination, DInt source,
						DByte *asdu_npdu, DInt length_asdu_npdu, DByte *apci, DInt length_apci,
						DInt data_expecting_reply, DInt clientserver, DInt port )
{
 DSignedChar n;
 DByte npci[32];
 DInt i, j, length_npci, nextentry;
// FRAME *pframe;
 DInt DNET, SNET, MessageType, dnet, snet;
 DByte Control, DLEN, SLEN, VendorID, DADR[6], SADR[6];
 DSignedChar HopCount;
 ROUTER_PARAMETERS r;

 snet=dnet=DNET=SNET=0;

 if( service==N_UNITDATArequest )
 {
	npci[0]=0x01;
	if ( network == NetworkAddress )
	{    // local network
	 npci[1]=0x00;     //DNET, DADR, SNET, SADR absent
	 length_npci=2;
	}
	else
	{
	 if ( network == BROADCAST )
	 {
		npci[1]=0x70;
		npci[2]=0xFF;
		npci[3]=0xFF;
		npci[4]=0;
		memcpy(&npci[5], &NetworkAddress, 2);
		npci[7]=Station_NUM;
		npci[8]=5;      //HopCount
		length_npci=9;
	 }
	 else
	 {
		npci[1]=0x50;
		memcpy( &npci[2], &network, 2);
		npci[4]=destination;
		memcpy( &npci[5], &NetworkAddress, 2);
		npci[7]=Station_NUM;
		npci[8]=5;
		length_npci=9;
	 }
	}

	if ( network != BROADCAST )
	{
// find routing entry
		snet = 0; dnet = MAX_Routing_table;
		if( port >= 0 ) {snet = port; dnet = port+1;}
		for(i=snet; i<dnet; i++)
		{
		 j=-1;
		 if( (Routing_table[i].status&PORT_ACTIVE)==PORT_ACTIVE )
			if( Routing_table[i].Port.network==network )
			{
				 break;
			}
			else
			{
				for(j=0; j<MAX_reachable_networks; j++)
				 if( Routing_table[i].Port.networks_list[j].status == REACHABIL &&
						Routing_table[i].Port.networks_list[j].network==network)
				{
					 destination=Routing_table[i].Port.networks_list[j].router_address;
					 break;
				}
				if(j<MAX_reachable_networks) break;
			}
		}
//  check result
		if (i<dnet)
		{
		 if(!(Routing_table[i].status^PTP_ACTIVE))
		 {
			((class PTP *)Routing_table[i].ptr)->sendframe(length_npci, npci, length_asdu_npdu, asdu_npdu, length_apci, apci);
		 }
		 if( (Routing_table[i].status&RS485_ACTIVE)==RS485_ACTIVE )
		 {
		 }
		}
	}
	else
	{

	}
 }
 if( service==DL_UNITDATAindication )
 {
	memcpy(npci, asdu_npdu, 30);
	Control = npci[1];
	switch (Control&0x60) {
	 case 0x00: dnet = ABSENT;   //this panel
					n = 2;
					break;
	 case 0x40: memcpy(&DNET, &npci[2], 2);   //DNET, HopCount, DADR one octet
					DADR[0] = npci[4];
					DLEN = 1;
					n = 5;
					break;
	 case 0x60: memcpy(&DNET, &npci[2], 2);   //DNET, DLEN, HopCount
					DLEN = npci[4];               // DLEN=0 broadcast MAC DADR
					if (!DLEN) DADR[0]=0xFF;      // DLEN>0 length DADR
					else memcpy(DADR, &npci[5], DLEN);
					n = 5+DLEN;
					break;
	}
	switch (Control&0x18) {
	 case 0x00: snet = ABSENT;
					break;
	 case 0x10: memcpy(&SNET, &npci[n], 2);
					SADR[0] = npci[n+2];
					SLEN = 1;
					n += 3;
					break;
	 case 0x18: memcpy(&SNET, &npci[n], 2);
					SLEN = npci[n+2];
					memcpy(&SADR, &npci[n+3], SLEN);
					n += 3+SLEN;
					break;
	}
	if ( dnet!=ABSENT ) HopCount = npci[n++];
	if ( snet==ABSENT )
	{
		SNET = Routing_table[port].Port.network;
		SADR[0] = source;
	}
	if ( Control&0x80 )
	{
	 MessageType=npci[n++];
	 if ( MessageType>=0x80 && MessageType<=0xFF ) VendorID = npci[n++];
//	 direct = 0;
	 if( dnet==ABSENT || DNET==FFFF )           //DNET==0xFFFF
	 {  //global broadcast
		//interpret si send
		r.SNET = SNET;
		r.SLEN = SLEN;
		memcpy(r.SADR, SADR, 6);
		r.len = length_asdu_npdu-n;
		r.data = &asdu_npdu[n];
		router(DL_UNITDATAindication, MessageType, &r, port);
	 }
	 --HopCount;
	 if( dnet!=ABSENT )
	 {
		for(i=0; i<MAX_Routing_table; i++)
		{
		 if( (Routing_table[i].status&PORT_ACTIVE)==PORT_ACTIVE)
		 {
			if( DNET == BROADCAST )
			{
			}
			else
			if( Routing_table[i].Port.network==DNET )
			{
			 if(!(Routing_table[i].status^PTP_ACTIVE))
			 {
				npci[0]=0x01;
				npci[1]=0x90;
				memcpy(&npci[2], &SNET, 2);
				npci[4]=SADR[0];
				npci[5]=MessageType;
				((class PTP *)Routing_table[i].ptr)->sendframe(6, npci, length_asdu_npdu-n, &asdu_npdu[n]);
			 }
			 break;
			}
			else
			{
			 if(HopCount)
				for(j=0; j<MAX_reachable_networks; j++)
				 if( Routing_table[i].Port.networks_list[j].status == REACHABIL &&
						Routing_table[i].Port.networks_list[j].network==DNET)
				 {
					 if(!(Routing_table[i].status^PTP_ACTIVE))
					 {
						 npci[1]=0xD0;
						 memcpy(&npci[5], &SNET, 2);
						 npci[7]=SADR[0];
						 npci[8]=HopCount;
						 npci[9]=MessageType;
						 ((class PTP *)Routing_table[i].ptr)->sendframe(10, npci, length_asdu_npdu-n, &asdu_npdu[n]);
					 }
					 break;
				 }
			}
		 }
		}
	}
	else
	{
		npci[1]=0x90;
		memcpy(&npci[2], &SNET, 2);
		npci[4]=SADR[0];
		npci[5]=HopCount;
		npci[6]=MessageType;
		for(j=0; j<MAX_Routing_table; j++)
		{
		 if( !(Routing_table[i].status^PTP_ACTIVE) && j!=port)
			((class PTP *)Routing_table[j].ptr)->sendframe(7, npci, length_asdu_npdu-n, &asdu_npdu[n]);
		}
	}
	}
	else
	{
	i=0;
// request on local panel
	if ( dnet==ABSENT || (dnet!=ABSENT && DNET == NetworkAddress && (DADR[0] == Station_NUM || DADR[0] == 0xFF) ) )
	{
	 if( clientserver == CLIENT )
	 {
			ClientTSMTable.received(destination, SADR[0], &asdu_npdu[n], length_asdu_npdu-n);
	 }
	 else
	 {
		if ( dnet==ABSENT ) DADR[0] = destination;
		network = SNET;
		ServerTransactionStateMachine(port, network, DADR[0], SADR[0],
						&asdu_npdu[n], length_asdu_npdu-n );
	 }
	 i=1;
	}
// request on remote panel
	if ( !i && dnet!=ABSENT )
	{
		for(i=0; i<MAX_Routing_table; i++)
		{
		 if( (Routing_table[i].status&PORT_ACTIVE)==PORT_ACTIVE)
		 {
			if( Routing_table[i].Port.network==DNET )
			{
// connected direct to DNET
				if( (Routing_table[i].status&RS485_ACTIVE)==RS485_ACTIVE )
				{
				if ( DADR[0] == Station_NUM || DADR[0] == 0xFF )
				{
// more then one RS485 installed on this panel
				 if( clientserver == CLIENT )
				 {
						ClientTSMTable.received(DADR[0], SADR[0], &asdu_npdu[n], length_asdu_npdu-n);
				 }
				 else
				 {
				  network = SNET;
				  ServerTransactionStateMachine(port, network, DADR[0], SADR[0],
						&asdu_npdu[n], length_asdu_npdu-n, -1, i );
				 }
             return 1;
				}
				else
				{
// send further on network
				 npci[0]=0x01;
				 npci[1]=0x10;
				 memcpy(&npci[2], &SNET, 2);
				 npci[4]=SADR[0];
				 length_npci = 5;
				}
			  }
			  else
			  {
//
				if( DNET == NetworkAddress )
				{
// request on local network
				 npci[0]=0x01;
				 npci[1]=0x10;
				 memcpy(&npci[2], &SNET, 2);
				 npci[4]=SADR[0];
				 length_npci = 5;
				}
				else
				{
// request on other network
				 npci[1]=0x50;
				 memcpy(&npci[2], &DNET, 2);
				 npci[4]=DADR[0];
				 memcpy(&npci[5], &SNET, 2);
				 npci[7]=SADR[0];
				 npci[8]=HopCount;
				 length_npci = 9;
				}
			  }
			  if(!(Routing_table[i].status^PTP_ACTIVE))
			  {
				 ((class PTP *)Routing_table[i].ptr)->sendframe(length_npci, npci, length_asdu_npdu-n, &asdu_npdu[n]);
			  }
				if( (Routing_table[i].status&RS485_ACTIVE)==RS485_ACTIVE )
			  {
			  }
			  break;
			}
			else
			{
			 if(HopCount)
			  for(j=0; j<MAX_reachable_networks; j++)
				 if( Routing_table[i].Port.networks_list[j].status == REACHABIL &&
					  Routing_table[i].Port.networks_list[j].network==DNET)
				 {
					 if(!(Routing_table[i].status^PTP_ACTIVE))
					 {
						 npci[1]=0x50;
						 memcpy(&npci[2], &DNET, 2);
						 npci[4]=DADR[0];
						 memcpy(&npci[5], &SNET, 2);
						 npci[7]=SADR[0];
						 npci[8]=HopCount;
						 ((class PTP *)Routing_table[i].ptr)->sendframe(9, npci, length_asdu_npdu-n, &asdu_npdu[n]);
					 }
					 break;
				 }
			}
		 }
	  }
	  if( (Routing_table[port].status&RS485_ACTIVE)==RS485_ACTIVE )
	  {
				// build frame response postpond
	  }
	}
  }
 }
}
*/

DInt networklayer( DInt service, DInt priority, DInt network, DInt destination, DInt source,
						DByte *asdu_npdu, DInt length_asdu_npdu, DByte *apci, DInt length_apci,
						DInt data_expecting_reply, DInt clientserver, DInt port )
{
 DSignedChar n;
 DByte npci[32];
 DInt i, j, length_npci, nextentry, router_address;
 FRAME *pframe;
 DInt DNET, SNET, MessageType, dnet, snet;
 DByte Control, DLEN, SLEN, VendorID, DADR[6], SADR[6], direct;
 DSignedChar HopCount;
 ROUTER_PARAMETERS r;
 class ConnectionData *cdata;

 snet=dnet=DNET=SNET=0;

 if( service==N_UNITDATArequest )
 {
	router_address = destination;
	npci[0]=0x01;
	if ( network != BROADCAST )
	{
// find routing entry
		snet = 0; dnet = MAX_Routing_table;
		if( port >= 0 ) {snet = port; dnet = port+1;}
		for(i=snet; i<dnet; i++)
		{
		 j=-1;
		 if( (Routing_table[i].status&PORT_ACTIVE)==PORT_ACTIVE )
			if( Routing_table[i].Port.network==network )
			{
				 break;
			}
			else
			{
				for(j=0; j<MAX_reachable_networks; j++)
				 if( Routing_table[i].Port.networks_list[j].status == REACHABIL &&
						Routing_table[i].Port.networks_list[j].network==network)
				 {
					 router_address=Routing_table[i].Port.networks_list[j].router_address;
					 break;
				 }
				if(j<MAX_reachable_networks) break;
			}
		}
//  check result
		if (i<dnet)
		{
		 npci[0]=0x01;
		 if( network == Routing_table[i].Port.network &&
				 ( ((Routing_table[i].status&RS485_INSTALLED)==RS485_INSTALLED)  ||
					 ((Routing_table[i].status&TCPIP_INSTALLED)==TCPIP_INSTALLED)  ||
					 ((Routing_table[i].status&IPX_INSTALLED)==IPX_INSTALLED) ) )
		 {    // local network
			 npci[1]=0x00;     //DNET, DADR, SNET, SADR absent
			 length_npci=2;
		 }
		 else
		 {
			 cdata = (class ConnectionData *)Routing_table[i].ptr;
			 npci[1]=0x50;
			 memcpy( &npci[2], &network, 2);
			 npci[4]=destination;
			 memcpy( &npci[5], &cdata->panel_info1.network, 2);
			 npci[7]=Station_NUM;
			 npci[8]=5;
			 length_npci=9;
		 }

		 if(!(Routing_table[i].status^PTP_ACTIVE))
		 {
			((class PTP *)Routing_table[i].ptr)->sendframe(length_npci, npci, length_asdu_npdu, asdu_npdu, length_apci, apci);
		 }
		 if( (Routing_table[i].status&RS485_ACTIVE)==RS485_ACTIVE )
		 {
/*
// build frame
			if(replyflag==1)
			{
			 pframe = (FRAME *)&ServerBuffer;
			}
			else
			{
			 nextentry = ((class MSTP *)Routing_table[i].ptr)->SendFramePool.NextFreeEntry();
			 if( nextentry < 0 ) return 0;
			 pframe = &((class MSTP *)Routing_table[i].ptr)->SendFramePool.Frame[nextentry];
			}
//	if( (nextentry = SendFramePool.NextFreeEntry()) >=0 )
//	{
//	 pframe = &SendFramePool.Frame[nextentry];
			pframe->FrameType = data_expecting_reply;
			pframe->Destination = router_address;
			pframe->Source = source;
			pframe->Length = length_npci+length_apci+length_asdu_npdu;
			memcpy(pframe->Buffer, npci, length_npci);
			memcpy(&pframe->Buffer[length_npci], apci, length_apci);
			memcpy(&pframe->Buffer[length_npci+length_apci], asdu_npdu, length_asdu_npdu);

			if(replyflag!=1)
			{
//			((class MSTP *)Routing_table[i].ptr)->SendFramePool.Unlockhead();
			 ((class MSTP *)Routing_table[i].ptr)->SendFramePool.status[nextentry]=1;
			}
			else
			 replyflag=2;
//	}
*/
		 }
#ifdef NET_BAC_COMM
		 if( (Routing_table[i].status&IPX_ACTIVE)==IPX_ACTIVE
				|| (Routing_table[i].status&TCPIP_ACTIVE)==TCPIP_ACTIVE )
		 {
// build frame
			if(replyflag==1)
			{
			 pframe = (FRAME *)&ServerBuffer;
			}
			else
			{
				pframe = (FRAME *)((class NET_BAC *)Routing_table[i].ptr)->SendFramePool.NextFreeEntry(0);
				if( pframe == NULL )
					return 0;
			}
			pframe->FrameType = data_expecting_reply;
			pframe->Destination = router_address;
			pframe->Source = source;
			pframe->Length = length_npci+length_apci+length_asdu_npdu;
			memcpy(pframe->Buffer, npci, length_npci);
			memcpy(&pframe->Buffer[length_npci], apci, length_apci);
			memcpy(&pframe->Buffer[length_npci+length_apci], asdu_npdu, length_asdu_npdu);

			((class NET_BAC *)Routing_table[i].ptr)->SendFramePool.Unlock_frame(
																									 (SEND_FRAME_ENTRY*)pframe );
			resume( ((class NET_BAC *)Routing_table[i].ptr)->task_number );
		 }
#endif // NET_BAC_COMM
		}
	}
	else
	{

	}
 }
 if( service==DL_UNITDATAindication )
 {
	memcpy(npci, asdu_npdu, 30);
	Control = npci[1];
	switch (Control&0x60) {
	 case 0x00: dnet = ABSENT;   //this panel
					n = 2;
					break;
	 case 0x40: memcpy(&DNET, &npci[2], 2);   //DNET, HopCount, DADR one octet
					DADR[0] = npci[4];
					DLEN = 1;
					n = 5;
					break;
	 case 0x60: memcpy(&DNET, &npci[2], 2);   //DNET, DLEN, HopCount
					DLEN = npci[4];               // DLEN=0 broadcast MAC DADR
					if (!DLEN) DADR[0]=0xFF;      // DLEN>0 length DADR
					else memcpy(DADR, &npci[5], DLEN);
					n = 5+DLEN;
					break;
	}
	switch (Control&0x18) {
	 case 0x00: snet = ABSENT;
					break;
	 case 0x10: memcpy(&SNET, &npci[n], 2);
					SADR[0] = npci[n+2];
					SLEN = 1;
					n += 3;
					break;
	 case 0x18: memcpy(&SNET, &npci[n], 2);
					SLEN = npci[n+2];
					memcpy(&SADR, &npci[n+3], SLEN);
					n += 3+SLEN;
					break;
	}
	if ( dnet!=ABSENT ) HopCount = npci[n++];
	if ( snet==ABSENT )
	{
		SNET = Routing_table[port].Port.network;
		SADR[0] = source;
	}
	if ( Control&0x80 )
	{
	 MessageType=npci[n++];
	 if ( MessageType>=0x80 && MessageType<=0xFF ) VendorID = npci[n++];
	 direct = 0;
	 if( dnet==ABSENT || DNET==FFFF )
	 {  //global broadcast
		//interpret si send
		r.SNET = SNET;
		r.SLEN = SLEN;
		memcpy(r.SADR, SADR, 6);
		r.len = length_asdu_npdu-n;
		r.data = &asdu_npdu[n];
/*
						rr2++;
						sprintf( rrr_buf, "%d", rr2 );
						mxyputs(10,12,"NU_indication");
						mxyputs(30,12,rrr_buf );
*/

		router(DL_UNITDATAindication, MessageType, &r, port);
	 }
	 --HopCount;
	 if( dnet!=ABSENT )
	 {
		for(i=0; i<MAX_Routing_table; i++)
		{
		 if( (Routing_table[i].status&PORT_ACTIVE)==PORT_ACTIVE)
		 {
			if( DNET == BROADCAST )
			{
			}
			else
			if( Routing_table[i].Port.network==DNET )
			{
			 if(!(Routing_table[i].status^PTP_ACTIVE))
			 {
				npci[0]=0x01;
				npci[1]=0x90;
				memcpy(&npci[2], &SNET, 2);
				npci[4]=SADR[0];
				npci[5]=MessageType;
				((class PTP *)Routing_table[i].ptr)->sendframe(6, npci, length_asdu_npdu-n, &asdu_npdu[n]);
			 }
			 break;
			}
			else
			{
			 if(HopCount)
				for(j=0; j<MAX_reachable_networks; j++)
				 if( Routing_table[i].Port.networks_list[j].status == REACHABIL &&
						Routing_table[i].Port.networks_list[j].network==DNET)
				 {
					 if(!(Routing_table[i].status^PTP_ACTIVE))
					 {
						 npci[1]=0xD0;
						 memcpy(&npci[5], &SNET, 2);
						 npci[7]=SADR[0];
						 npci[8]=HopCount;
						 npci[9]=MessageType;
						 ((class PTP *)Routing_table[i].ptr)->sendframe(10, npci, length_asdu_npdu-n, &asdu_npdu[n]);
					 }
					 break;
				 }
			}
		 }
		}
/*
		if( i >= MAX_Routing_table )
		{
		// Who_Is_Router_To_Network
		 if( DNET != GLOBAL )
		 {
			npci[1]=0x80;
			npci[2]=Who_Is_Router_To_Network;
			memcpy(&npci[3], DNET, 2);
			for(int j=0; j<MAX_Routing_table; j++)
			{
			if(Routing_table[i].status&PTP_ACTIVE && i!=port)
			 ((class PTP *)Routing_table[i].ptr)->sendframe(4, npci);
			}
		 }
		}
*/
	 }
	 else
	 {
		npci[1]=0x90;
		memcpy(&npci[2], &SNET, 2);
		npci[4]=SADR[0];
		npci[5]=HopCount;
		npci[6]=MessageType;
		for(j=0; j<MAX_Routing_table; j++)
		{
		 if( !(Routing_table[i].status^PTP_ACTIVE) && j!=port)
			((class PTP *)Routing_table[j].ptr)->sendframe(7, npci, length_asdu_npdu-n, &asdu_npdu[n]);
		}
	 }
	}
	else
	{
	 i=0;
// request on local panel
	 if(port>=0)
	 {
/*
		if( (Routing_table[port].status&PTP_INSTALLED)==PTP_INSTALLED )
		 sptr = (class PTP *)Routing_table[port].ptr;
		else
		 sptr = (class MSTP *)Routing_table[port].ptr;
*/
		cdata = ( class ConnectionData *)Routing_table[port].ptr;
//		j = comm_info[cdata->com_port].NetworkAddress;
		j = cdata->panel_info1.network;
	 }
	 else
		j = NetworkAddress;

	 if ( dnet==ABSENT || (dnet!=ABSENT && DNET == j && (DADR[0] == Station_NUM || DADR[0] == 0xFF) ) )
	 {
		if( clientserver == CLIENT )
		{
			ClientTSMTable.received(destination, SADR[0], &asdu_npdu[n], length_asdu_npdu-n);
		}
		else
		{
		 if ( dnet==ABSENT ) DADR[0] = destination;
		 network = SNET;
		 ServerTransactionStateMachine(port, network, DADR[0], SADR[0],
						&asdu_npdu[n], length_asdu_npdu-n );
		}
		i=1;
	 }
// request on remote panel
	 if ( !i && dnet!=ABSENT )
	 {
		for(i=0; i<MAX_Routing_table; i++)
		{
		 if( (Routing_table[i].status&PORT_ACTIVE)==PORT_ACTIVE)
		 {
			if( Routing_table[i].Port.network==DNET )
			{
// connected direct to DNET
			 if( (Routing_table[i].status&RS485_ACTIVE)==RS485_ACTIVE ||
					 (Routing_table[i].status & IPX_ACTIVE) == IPX_ACTIVE ||
					 (Routing_table[i].status & TCPIP_ACTIVE) == TCPIP_ACTIVE )
			 {
				// request directed to a network ( RS485/IPX/TCPIP) port
				if ( DADR[0] == Station_NUM || DADR[0] == 0xFF )
				{
				 // more then one network (RS485/IPX/TCPIP) installed on this panel
				 if( clientserver == CLIENT )
				 {
//					LengthReceivedClientAPDU = length_asdu_npdu-n;
//					PTRReceivedClientAPDU = &asdu_npdu[n];
						ClientTSMTable.received(DADR[0], SADR[0], &asdu_npdu[n], length_asdu_npdu-n);
				 }
				 else
				 {
					network = SNET;
					ServerTransactionStateMachine(port, network, DADR[0], SADR[0],
						&asdu_npdu[n], length_asdu_npdu-n, -1, i );
				 }
						 return 1;
				}
				else
				{
// send further on network
				 npci[0]=0x01;
				 npci[1]=0x10;
				 memcpy(&npci[2], &SNET, 2);
				 npci[4]=SADR[0];
				 length_npci = 5;
				}
			 }
			 else
			 {
//
//------------				if( DNET == NetworkAddress )
				if( DNET == j )
				{
				 // request on local network
				 npci[0]=0x01;
				 npci[1]=0x10;
				 memcpy(&npci[2], &SNET, 2);
				 npci[4]=SADR[0];
				 length_npci = 5;
				}
				else
				{
				 // request on other network
				 npci[1]=0x50;
				 memcpy(&npci[2], &DNET, 2);
				 npci[4]=DADR[0];
				 memcpy(&npci[5], &SNET, 2);
				 npci[7]=SADR[0];
				 npci[8]=HopCount;
				 length_npci = 9;
				}
			 }
			 if(!(Routing_table[i].status^PTP_ACTIVE))
			 {
				 ((class PTP *)Routing_table[i].ptr)->sendframe(length_npci, npci, length_asdu_npdu-n, &asdu_npdu[n]);
			 }
			 if( (Routing_table[i].status&RS485_ACTIVE)==RS485_ACTIVE )
			 {
/*
					// build frame
					nextentry = ((class MSTP *)Routing_table[i].ptr)->SendFramePool.NextFreeEntry();
					if( nextentry >= 0 )
					{
					pframe = &((class MSTP *)Routing_table[i].ptr)->SendFramePool.Frame[nextentry];
					if( !data_expecting_reply )
						pframe->FrameType = (DADR[0]==255?BACnetDataNotExpectingReply:BACnetDataExpectingReply);
					else
						pframe->FrameType = data_expecting_reply;
					pframe->Destination = DADR[0];
					pframe->Source = TS;           //SADR[0];
					pframe->Length = length_npci+length_asdu_npdu-n;
					memcpy(pframe->Buffer, npci, length_npci);
					memcpy(&pframe->Buffer[length_npci], &asdu_npdu[n],  length_asdu_npdu-n);
//					((class MSTP *)Routing_table[i].ptr)->SendFramePool.Unlockhead();
					((class MSTP *)Routing_table[i].ptr)->SendFramePool.status[nextentry]=1;
					}
*/
			 }
#ifdef NET_BAC_COMM
			 if( (Routing_table[i].status & IPX_ACTIVE) == IPX_ACTIVE ||
					 (Routing_table[i].status & TCPIP_ACTIVE) == TCPIP_ACTIVE )
			 {
					// build frame
					pframe = (FRAME *)((class NET_BAC *)Routing_table[i].ptr)->SendFramePool.NextFreeEntry(0);
					if( pframe != NULL )
					{
						if( !data_expecting_reply )
							pframe->FrameType = (DADR[0]==255?BACnetDataNotExpectingReply:BACnetDataExpectingReply);
						else
							pframe->FrameType = data_expecting_reply;
						pframe->Destination = DADR[0];
						pframe->Source = Station_NUM;           //SADR[0];
						pframe->Length = length_npci+length_asdu_npdu-n;
						memcpy(pframe->Buffer, npci, length_npci);
						memcpy(&pframe->Buffer[length_npci], &asdu_npdu[n],  length_asdu_npdu-n);
						((class NET_BAC *)Routing_table[i].ptr)->SendFramePool.Unlock_frame(
																									 (SEND_FRAME_ENTRY*)pframe );
						resume( ((class NET_BAC *)Routing_table[i].ptr)->task_number );
					}
			 }
#endif
			 break;
			}
			else
			{
			 if(HopCount)
				for(j=0; j<MAX_reachable_networks; j++)
				 if( Routing_table[i].Port.networks_list[j].status == REACHABIL &&
						Routing_table[i].Port.networks_list[j].network==DNET)
				 {
					 if(!(Routing_table[i].status^PTP_ACTIVE))
					 {
						 npci[1]=0x50;
						 memcpy(&npci[2], &DNET, 2);
						 npci[4]=DADR[0];
						 memcpy(&npci[5], &SNET, 2);
						 npci[7]=SADR[0];
						 npci[8]=HopCount;
						 ((class PTP *)Routing_table[i].ptr)->sendframe(9, npci, length_asdu_npdu-n, &asdu_npdu[n]);
					 }
					 break;
				 }
			}
		 }
		}
		if( (Routing_table[port].status&RS485_ACTIVE)==RS485_ACTIVE )
		{
/*        
				// build frame response postpond
				 if(replyflag==1)
				 {
					pframe = (FRAME *)&ServerBuffer;
					pframe->FrameType = ReplyPostponed;
					pframe->Destination = source;
					pframe->Source = TS;
					pframe->Length = 0;
					replyflag=2;
				 }
*/
		}
	 }
	}
 }
}






char *ltrim(char *text)
{
	DInt n,i;
	n=strlen(text);
	for (i=0;i<n;i++)
			if (text[i]!=' ')
					break;
	strcpy(text,text+i);
	return text;
}


