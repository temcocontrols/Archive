//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------

#include <vcl/comctrls.hpp>
#include <dos.h>
#include "..\define.h"
#include "..\baseclas.h"
#include "..\ptpdll.h"
#include "ptp.h"
#include "..\router.h"

extern DUint  msgWM_PTP_CONNECTION;
extern DUint  msgWM_PTP_RECEPTION;
extern DUint  msgWM_PTP_TRANSMISSION;
extern DUint  msgWM_PTP_ESTABLISH_CONNECTION;

DByte CalcHeaderCRC(DByte dataValue, DByte crcValue);
DUint CalcDataCRC(DByte dataValue, DUint crcValue);
extern DInt  networklayer( DInt service, DInt priority, DInt network, DInt destination, DInt source,
						Byte *asdu_npdu, DInt length_asdu_npdu, Byte *apci, DInt length_apci,
						DInt data_expecting_reply, DInt clientserver, DInt port );
extern BOOL WriteString (DInt Cid, DByte *lpData, DWORD dwLength);
extern COMMINFO CommInfo[MAX_COMM_INFO];
extern ROUTING_TABLE_T3000 Routing_table[MAX_Routing_table];
extern TImage **tImageRun;
extern DInt maxImages;
extern TProgressBar *ProgressBar;
//extern DInt NetworkAddress;
extern void runImage(TImage *tImage[], DInt maximages);
extern DInt router(DInt service, DInt command, ROUTER_PARAMETERS *r=NULL, DInt	port_number=-1, DInt send=0);

DInt pwait(DWORD timesleep, DUint msg, MSG& myMsg, DInt breakcond = 0);
void delay(DUint timedelay);

DByte trigger_sequence[] = "BACnet\x0d";
DByte ring_sequence[] = "RING";
PTP_FRAME sendframedata;

__declspec(dllexport) void ProcEvConnection(DInt comPort);
__declspec(dllexport) void ProcEvReception(DInt comPort);
__declspec(dllexport) void ProcEvTransmission(DInt comPort, DInt param);

void ProcEvConnection(DInt comPort)
{
	  struct PTP_ReceivedFrame rec_frame;
      DInt    next, TimeOut;
      PTP_FRAME frame;

      MSG myMsg;
	  PTP *ptp;
      PORT_STATUS_variables *ps;
	  ptp = (class PTP *)Routing_table[CommInfo[comPort].conPort()].ptr;
	  ps = &Routing_table[ptp->port_number].port_status_vars;

	  next = 1;
//	if( ptp->port_status == INSTALLED )
	  while( next )
      {
		 switch( ptp->connection_state )
		 {
			case PTP_IDLE:
				break;
			case PTP_DISCONNECTED:
				if( ptp->NL_parameters.primitive == DL_CONNECT_REQUEST )
				{
					ptp->connection_state=OUTBOUND;
					ps->connection = PTP_SEND_TRIGGER_SEQ;
					ptp->NL_parameters.primitive = DL_IDLE;
	                WriteString((DInt)1, trigger_sequence, (DWORD)strlen((const char *)trigger_sequence));
					ptp->RetryCount=0;
					break;
				}
				Routing_table[ptp->port_number].status = PTP_INSTALLED;
				if( ps->physical_connection_state==CONNECTION_INACTIVE )
				{
					ptp->init_idle_state();
				}
                next = 0;
				break;
			case OUTBOUND:
				/* Connect Request Received*/
				TimeOut=FALSE;
				pwait(T_CONN_REQ, WM_PTP_CONNECTION, myMsg, 1);
				if(ptp->ReceivedFramePool.RemoveEntry(&rec_frame)<0) TimeOut=TRUE;
//              Timeout    ResponseTimer >= Tconn_rqst
				if( !TimeOut && rec_frame.flags.ReceivedInvalidFrame ) TimeOut=TRUE;
				if( TimeOut )
				{
                    next = 0;
					if( ps->physical_connection_state!=CONNECTION_INACTIVE && ptp->RetryCount < N_RETRIES )
					{
					/*	Connect Request Timeout	*/
/*
						ptp->FlushTXbuffer();
						ptp->port->Write( trigger_sequence, strlen(trigger_sequence), 1000 );
						while( ptp->port->TXSpaceUsed() );
						ptp->RetryCount++;
*/
						break;
					}
					else
					{
					/*	Connect Request Failure	*/
					/* signal network layer	*/
/*						N_UNITDATA_parameters.primitive = DL_UNITDATA_INDICATION;*/
						ptp->NL_parameters.primitive = DL_CONNECT_ATTEMPT_FAILED;
						ptp->init_idle_state();
//                        ProcPostMessage(WM_KEYDOWN,VK_CANCEL);
					    PostMessage(
					     ptp->Handle,	// handle of destination window
					     WM_KEYDOWN,	// message to post
					     VK_CANCEL,	// first message parameter
					     0  	// second message parameter
					    );
						break;
					}
				}

				if( rec_frame.flags.ReceivedValidFrame )
					if( rec_frame.Frame.FrameType == CONNECT_REQUEST )
					{
//						  ptp->ReceivedFramePool.Clear();
						  ptp->SendFramePool.Clear();

						  ps->PTP_comm_status.TxSequence_number = 0;
						  ps->PTP_comm_status.RxSequence_number = 0;
						  ptp->NL_parameters.primitive = DL_CONNECT_INDICATION;
						  ptp->connection_state = PTP_CONNECTED;
						  Routing_table[ptp->port_number].status=PTP_ACTIVE;
						  Routing_table[ptp->port_number].Port.network=0;             //0xFFFF;
						  ps->InactivityTimer = 0;
						  ps->connection = PTP_CONNECTED;
						  frame.FrameType = CONNECT_RESPONSE;
						  frame.Length = 0;
						  ptp->PTP_SendFrame((PTP_FRAME *)&frame);

//                          ProcSendMessage(WM_PTP_RECEPTION,0);
						  SendMessage(
						    ptp->Handle,	// handle of destination window
						    msgWM_PTP_RECEPTION,	// message to post
						    comPort,	// first message parameter
						    0  	// second message parameter
						   );
//						  resume(ptp->task+PTP_reception);

//                          ProcSendMessage(WM_PTP_TRANSMISSION,0);
						  SendMessage(
						    ptp->Handle,	// handle of destination window
						    msgWM_PTP_TRANSMISSION,	// message to post
						    comPort,	// first message parameter
						    0  	// second message parameter
						   );
//						  resume(ptp->task+PTP_transmission);
                          delay(500);
						  router(N_UNITDATArequest, I_Am_Router_To_Network, NULL, ptp->port_number);
                          delay(500);
//						  delay(4000);
//						  router(N_UNITDATArequest, I_Am_Router_To_Network_Prop, NULL, ptp->port_number);
//						  delay(3000);
//						  router(N_UNITDATArequest, I_Am_Router_To_Network, NULL, ptp->port_number);
/*
					     if(!pwait(50000, WM_PTP_ESTABLISH_CONNECTION))
							  SendMessage(
							    MainWindow->HWindow,	// handle of destination window
									WM_PTP_ESTABLISH_CONNECTION,	// message to post
							    0,	// first message parameter
							    0  	// second message parameter
							   );
*/
                          next = 0;
						  break;
					}
					break;
			case INBOUND:
				break;
			case PTP_CONNECTED:
			/*	The connection procedure has been completed and the two devices
			are exchanging BACnet PDUs. The data link remains in this
			connection_state until termination	*/
//				TimeOut=FALSE;
//				suspend(ptp->task);
				/*	Connection Lost	*/
				if( ps->physical_connection_state!=CONNECTION_INACTIVE )
				/*	Network disconnect	*/
				{
				 if( ptp->NL_parameters.primitive == DL_DISCONNECT_REQUEST )
				 {
					ptp->NL_parameters.primitive = DL_IDLE;
					frame.FrameType = DISCONNECT_REQUEST;
					frame.Length = 0;
					ptp->PTP_SendFrame((PTP_FRAME *)&frame);
//					ptp->ResponseTimer = 0;
					ptp->connection_state = PTP_DISCONNECTING;
					break;
				 }
				 if(ptp->ReceivedFramePool.RemoveEntry(&rec_frame)>=0)
				 {
					if( rec_frame.flags.ReceivedValidFrame )
					{
				 /*	Disconnect request received	*/
					if( rec_frame.Frame.FrameType == DISCONNECT_REQUEST )
					{
					 frame.FrameType = DISCONNECT_RESPONSE;
					 frame.Length = 0;
					 ptp->PTP_SendFrame((PTP_FRAME *)&frame);
//					 delay(500);
					}
					if( rec_frame.Frame.FrameType == CONNECT_REQUEST )
					{
					 frame.FrameType = CONNECT_RESPONSE;
					 frame.Length = 0;
					 ptp->PTP_SendFrame((PTP_FRAME *)&frame);
					 ptp->connection_state = PTP_CONNECTED;
					 break;
					}
				  }
				 }
				}
				ptp->init_idle_state();
			    router(N_UNITDATArequest, I_Am_Router_To_Network_Prop, NULL, ptp->port_number,1);
				break;
			case PTP_DISCONNECTING:
			/*	The network layer has requested termination of the data link. The
			device is waiting for a Disconnect Response frame from the peer device*/
				/* Disconnect Response Received */
//				TimeOut=FALSE;
//				msleep(T_CONN_RSP);
				TimeOut = pwait(T_CONN_RSP, WM_PTP_CONNECTION, myMsg);
				if(ptp->ReceivedFramePool.RemoveEntry(&rec_frame)<0) TimeOut=TRUE;
//          Timeout
//			   if( ResponseTimer >= Tconn_rsp )
				if( !TimeOut && rec_frame.flags.ReceivedInvalidFrame ) TimeOut=TRUE;
				if( TimeOut )
				{
					if( ps->physical_connection_state!=CONNECTION_INACTIVE && ptp->RetryCount < N_RETRIES )
					{
						/*	Disconnect response timeout	*/
						frame.FrameType = DISCONNECT_REQUEST;
						frame.Length = 0;
						ptp->PTP_SendFrame((PTP_FRAME *)&frame, 1);
						ptp->RetryCount++;
//						ptp->ResponseTimer=0;
						break;
					}
				}
				else
				if( rec_frame.flags.ReceivedValidFrame )
				{
					/*	Disconnect request received */
					if( rec_frame.Frame.FrameType == DISCONNECT_REQUEST )
					{
							frame.FrameType = DISCONNECT_RESPONSE;
							frame.Length = 0;
							ptp->PTP_SendFrame((PTP_FRAME *)&frame);
//							delay(500);
					}
//					TextGadget->SetText("Disconnected");
//					Tapp->MainWindow->UpdateWindow();
				}
				ptp->init_idle_state();
			    router(N_UNITDATArequest, I_Am_Router_To_Network_Prop, NULL, ptp->port_number,1);
				break;
		 }
	  }
}

void ProcEvReception(DInt comPort)
{
 DInt next;
 struct PTP_ReceivedFrame rec_frame;
 PTP_FRAME sframe;
 PORT_STATUS_variables *ps;
 PTP *ptp;
 ptp = (class PTP *)Routing_table[CommInfo[comPort].conPort()].ptr;
 ps = &Routing_table[ptp->port_number].port_status_vars;

//	if( ptp->port_status == INSTALLED && ptp->connection_state == CONNECTED )
   next = 1;
	while( next )
	{
		switch( ptp->reception_state )
		{
			case REC_IDLE:
			/*	The receiver is waiting for the data link to be established
				between the local device and the peer device. The receiver waits
				to be notified that a peer device is ready to communicate	*/
				/*	Connection Established	*/
				if( ptp->connection_state == PTP_CONNECTED )
				{
					 ps->PTP_comm_status.RxSequence_number = 0;
					 ps->PTP_comm_status.reception_blocked = Q_NOT_BLOCKED;
					 ptp->reception_state = REC_READY;
					 break;
				}
//				suspend(ptp->task+PTP_reception);
						next = 0;
				break;
/*			case REC_NO_READY: - only in version II */
			case REC_READY:
			/*	The device is ready to receive frames from the peer device	*/
				if(ptp->ReceivedFramePool.RemoveEntry(&rec_frame)>=0)
				{
				 if( rec_frame.flags.ReceivedValidFrame )
				 {
					ps->InactivityTimer = 0;
					if( rec_frame.Frame.FrameType == DATA_0 || rec_frame.Frame.FrameType == DATA_1 )
					{
						ptp->reception_state = DATA;
						break;
					}
					if( DATA_ACK_0_XOFF <= rec_frame.Frame.FrameType &&
						  rec_frame.Frame.FrameType <= DATA_ACK_1_XON )
					{
						ptp->reception_state = DATA_ACK;
						break;
					}
					if( DATA_NAK_0_XOFF <= rec_frame.Frame.FrameType &&
						  rec_frame.Frame.FrameType <= DATA_NAK_1_XON )
					{
						ptp->reception_state = DATA_NAK;
						break;
					}
					if( rec_frame.Frame.FrameType == TEST_REQUEST )
					{
/*
						if( ptp->SendFramePool.NextFreeEntry() )
						{
							frame = &ptp->SendFramePool.Entry[SendFramePool.HeadFrame];
							frame->FrameType = TEST_RESPONSE;
							frame->Length = 0;
							PTP_send_frame();
							PTP_comm_status.received_valid_frame = 0;
							ps->InactivityTimer = 0;
							reception_state = REC_READY;
							break;
						}
*/
					}
					if( rec_frame.Frame.FrameType == TEST_RESPONSE )
					{
/*
						NL_parameters.primitive = DL_UNITDATA_INDICATION;
						NL_parameters.npdu = rec_frame->Buffer;
						NL_parameters.length_npdu = rec_frame->Length;
						NL_parameters.AtoN = 0;
						NL_parameters.DtoN = 1;
						networklayer();
						PTP_comm_status.received_valid_frame = 0;
						ps->InactivityTimer = 0;
						reception_state = REC_READY;
						break;
*/
					}
					break;
				 }
				 if( rec_frame.flags.ReceivedInvalidFrame )
				 {
						/*	Bad Data0 / Full buffers	*/
						ps->InactivityTimer = 0;
						if( rec_frame.Frame.FrameType == DATA_0 )
						{
							/* Discard frame */
							/* Clear( &rec_queue );*/
							if( ps->PTP_comm_status.reception_blocked == Q_BLOCKED )
									sframe.FrameType = DATA_NAK_0_XOFF;
							else
									sframe.FrameType = DATA_NAK_0_XON;
							sframe.Length = 0;
							ptp->PTP_SendFrame((PTP_FRAME *)&sframe);
							break;
						}
						/*	Bad Data1 / Full buffers	*/
						if( rec_frame.Frame.FrameType == DATA_1 )
						{
							/* Discard Frame */
							/*Clear( &rec_queue );*/
							if( ps->PTP_comm_status.reception_blocked == Q_BLOCKED )
									sframe.FrameType = DATA_NAK_1_XOFF;
							else
									sframe.FrameType = DATA_NAK_1_XON;
							sframe.Length = 0;
							ptp->PTP_SendFrame((PTP_FRAME *)&sframe);
							break;
						}
						/* Discard Frame */
						/*	Clear( &rec_queue );	*/
						break;
				 }
				}
				if( ptp->connection_state == PTP_DISCONNECTED )
				{
						ptp->reception_state = REC_IDLE;
						break;
				}
//				suspend(ptp->task+PTP_reception);
            next = 0;
				break;
			case DATA:
			/*	In this state the device has received a Data frame for processing */
				if( rec_frame.Frame.FrameType == DATA_0 )
				{
					if( ps->PTP_comm_status.RxSequence_number == 1 )
					{
							if( ps->PTP_comm_status.reception_blocked == Q_BLOCKED )
//								Duplicate0_FullBuffers
								sframe.FrameType = DATA_ACK_0_XOFF;
							else
//								Duplicate0
								sframe.FrameType = DATA_ACK_0_XON;
//								Discard frame
					}
					else
					{
							if( ps->PTP_comm_status.reception_blocked == Q_BLOCKED )
							{
//									Data0_FullBuffers
//									Clear( &rec_queue );
								sframe.FrameType = DATA_NAK_0_XOFF;
							}
							else
							{
								//	DL_UNITDATA.indication
								ptp->NL_parameters.primitive = DL_UNITDATA_INDICATION;
								ptp->NL_parameters.AtoN = 0;
								ptp->NL_parameters.DtoN = 1;
								ptp->NL_parameters.npdu = rec_frame.Frame.Buffer;
								ptp->NL_parameters.length_npdu = rec_frame.Frame.Length;

								ps->PTP_comm_status.RxSequence_number = 1;
								if( ps->PTP_comm_status.reception_blocked == Q_NOT_BLOCKED )
								//	New Data0
									sframe.FrameType = DATA_ACK_0_XON;
								if( ps->PTP_comm_status.reception_blocked == Q_ALMOST_BLOCKED )
								//	Last Data0
									sframe.FrameType = DATA_ACK_0_XOFF;
								sframe.Length = 0;
								ptp->PTP_SendFrame((PTP_FRAME *)&sframe);

//								dwStopTime  = GetTickCount() + 1000;
//								while(GetTickCount() < dwStopTime);
								networklayer( DL_UNITDATAindication, NORMALmessage, 0, 0, 0,
									 rec_frame.Frame.Buffer, rec_frame.Frame.Length, NULL, 0, 0, SERVER, ptp->port_number);
								 ptp->reception_state = REC_READY;
								 break;
							}
						}
				}
				if( rec_frame.Frame.FrameType == DATA_1 )
				{
					if( ps->PTP_comm_status.RxSequence_number == 0 )
					{
							if( ps->PTP_comm_status.reception_blocked == Q_BLOCKED )
//								Duplicate0_FullBuffers
								sframe.FrameType = DATA_ACK_1_XOFF;
							else
//								Duplicate0
								sframe.FrameType = DATA_ACK_1_XON;
//								Discard frame
					}
					else
					{
							if( ps->PTP_comm_status.reception_blocked == Q_BLOCKED )
							{
//									Data0_FullBuffers
//									Clear( &rec_queue );
								sframe.FrameType = DATA_NAK_1_XOFF;
							}
							else
							{
								//	DL_UNITDATA.indication
								ptp->NL_parameters.primitive = DL_UNITDATA_INDICATION;
								ptp->NL_parameters.AtoN = 0;
								ptp->NL_parameters.DtoN = 1;
								ptp->NL_parameters.npdu = rec_frame.Frame.Buffer;
								ptp->NL_parameters.length_npdu = rec_frame.Frame.Length;

								ps->PTP_comm_status.RxSequence_number = 0;
								if( ps->PTP_comm_status.reception_blocked == Q_NOT_BLOCKED )
								//	New Data0
									sframe.FrameType = DATA_ACK_1_XON;
								if( ps->PTP_comm_status.reception_blocked == Q_ALMOST_BLOCKED )
								//	Last Data0
									sframe.FrameType = DATA_ACK_1_XOFF;
								sframe.Length = 0;
								ptp->PTP_SendFrame((PTP_FRAME *)&sframe);

	 /*
	The following loop determines the status of the serial
	interface, which specifies whether the application will act
	as sender or receiver.

	This loop gets the system time and waits for about 4 timer
	clicks. During this time, the application releases the CPU for
	message processing, and for getting important WM_TIMER
	messages for PollRxQueue.
	 */
//								dwStopTime  = GetTickCount() + 1000;
//								while(GetTickCount() < dwStopTime);
								networklayer( DL_UNITDATAindication, NORMALmessage, 0, 0, 0,
									 rec_frame.Frame.Buffer, rec_frame.Frame.Length, NULL, 0, 0, SERVER, ptp->port_number);
								ptp->reception_state = REC_READY;
								break;
							}
					}
				}
				sframe.Length = 0;
				ptp->PTP_SendFrame((PTP_FRAME *)&sframe);
				ptp->reception_state = REC_READY;
				break;
			case DATA_ACK:
				ptp->reception_state = REC_READY;  //DATA_ACK tratat in ISR_PC8250
				break;
			case DATA_NAK:
				/*	In this state the device has received a Data Nak
																					frame for processing */
				if( ps->PTP_comm_status.TxSequence_number == 0 )
				{
					if( rec_frame.Frame.FrameType == DATA_NAK_1_XON )
					{
						/*	Duplicate XON	*/
						ps->PTP_comm_status.transmission_blocked = 0;
					}
					if( rec_frame.Frame.FrameType == DATA_NAK_1_XOFF )
					{
						/*	Duplicate XOFF	*/
						ps->PTP_comm_status.transmission_blocked = 1;
					}
					if( rec_frame.Frame.FrameType == DATA_NAK_0_XON )
					{
						/*	Ack0 XON	*/
						ps->PTP_comm_status.nak0received = 1;
						ps->PTP_comm_status.transmission_blocked = 0;
					}
					if( rec_frame.Frame.FrameType == DATA_NAK_0_XOFF )
					{
						/*	Ack0 XOFF	*/
						ps->PTP_comm_status.nak0received = 1;
						ps->PTP_comm_status.transmission_blocked = 1;
					}
					ptp->reception_state = REC_READY;
					break;
				}
				if( ps->PTP_comm_status.TxSequence_number == 1 )
				{
					if( rec_frame.Frame.FrameType == DATA_NAK_0_XON )
					{
						/*	Duplicate XON	*/
						ps->PTP_comm_status.transmission_blocked = 0;
					}
					if( rec_frame.Frame.FrameType == DATA_NAK_0_XOFF )
					{
						/*	Duplicate XOFF	*/
						ps->PTP_comm_status.transmission_blocked = 1;
					}
					if( rec_frame.Frame.FrameType == DATA_NAK_1_XON )
					{
						/*	Ack1 XON	*/
						ps->PTP_comm_status.nak1received = 1;
						ps->PTP_comm_status.transmission_blocked = 0;
					}
					if( rec_frame.Frame.FrameType == DATA_NAK_1_XOFF )
					{
						/*	Ack1 XOFF	*/
						ps->PTP_comm_status.nak1received = 1;
						ps->PTP_comm_status.transmission_blocked = 1;
					}
					ptp->reception_state = REC_READY;
					break;
				}
		}
	}
}

//LRESULT TWint3000App::EvTransmission(WPARAM wparam, LPARAM lparam)
void ProcEvTransmission(DInt comPort, DInt param)
{
// PTP_FRAME pframe;
 DInt next;
 PTP_FRAME sframe;

 PORT_STATUS_variables *ps;
 PTP *ptp;
 ptp = (class PTP *)Routing_table[CommInfo[comPort].conPort()].ptr;
 ps = &Routing_table[ptp->port_number].port_status_vars;
 next = 1;
 while( next )
 {
//	if( ser_ptp->port_status == INSTALLED && connection_state == CONNECTED )
		switch( ptp->transmission_state )
		{
			case TR_IDLE:
				/*	In this state the transmitter is waiting for the data link to be
				established between the local device and the peer device. The
				transmitter waits to be notified that a peer device is ready to
				communicate	*/
				if( ptp->connection_state == PTP_CONNECTED )
				{
						if( ps->PTP_comm_status.reception_blocked == Q_NOT_BLOCKED )
							sframe.FrameType = HEARTBEAT_XON;
						else
							sframe.FrameType = HEARTBEAT_XOFF;
						sframe.Length = 0;
						ptp->PTP_SendFrame((PTP_FRAME *)&sframe);
						ps->PTP_comm_status.TxSequence_number = 0;
						ptp->transmission_state = TR_BLOCKED;
//						msleep(4);
						break;
				}
//				suspend(ptp->task+PTP_transmission);
            next = 0;
				break;
			case TR_BLOCKED:
			/*	In this state the peer device has indicated that it is not ready
			to receive data frames. The local device may have data ready to transmit.
			The local device periodically transmits a Heartbeat frame to maintain
			the data link, and waits for the peer device to become ready to receive
			data or for the termination of the data link.	*/
				/*	Send Request	*/
/*
				if( ptp->NL_parameters.primitive = DL_UNITDATA_REQUEST )
				{
					break;
				}
*/
				/*	Peer Receiver Ready	*/
				if( !ps->PTP_comm_status.transmission_blocked )
				{
					ptp->transmission_state = TR_READY;
					break;
				}
				if( ptp->connection_state == PTP_DISCONNECTED )
				{
					ptp->transmission_state = TR_IDLE;
               next = 0;
					break;
				}
/*
				if( Routing_table[ptp->port_number].port_status_vars.HeartbeatTimer >= T_HEARTBEAT )
				{
				  if( ps->PTP_comm_status.reception_blocked == Q_NOT_BLOCKED )
					sframe.FrameType = HEARTBEAT_XON;
				  else
					sframe.FrameType = HEARTBEAT_XOFF;
				  sframe.Length = 0;
				  ptp->PTP_SendFrame((PTP_FRAME *)&sframe);
            }
*/
            next = 0;
				break;
			case TR_READY:
			/*	The peer device has indicated its readiness to receive data frames
			but the local device has no data ready to transmit. The local device
			periodically transmits a Heartbeat frame to maintain the data link, and
			waits for a local request to transmit data or for the termination of
			the data link	*/
				if( !ps->PTP_comm_status.transmission_blocked )
				{
				 if ( ptp->SendFramePool.RemoveEntry(&sendframedata) >= 0 )
				 {
					if( ps->PTP_comm_status.reception_blocked == Q_NOT_BLOCKED )
					{
					 if( sendframedata.FrameType == DATA_0 )
						sendframedata.FrameType += (DByte)ps->PTP_comm_status.TxSequence_number;
					 ptp->PTP_SendFrame(&sendframedata);
					 ptp->RetryCount = 0;
//					 ptp->AcknowledgementTimer = 0;
					 ptp->transmission_state = TR_PENDING;
					 ptp->ResponseTimer = T_RESPONSE;
//----
                     next = 0;
					 break;
					}
				 }
				}
				else
				{
					ptp->transmission_state = TR_BLOCKED;
					break;
				}
				if( ptp->connection_state == PTP_DISCONNECTED )
				{
					ptp->transmission_state = TR_IDLE;
                    next = 0;
					break;
				}
				/*	Send Request	*/
/*
				if( Routing_table[ptp->port_number].port_status_vars.HeartbeatTimer >= T_HEARTBEAT )
				{
				 if( ps->PTP_comm_status.reception_blocked == Q_NOT_BLOCKED )
					sframe.FrameType = HEARTBEAT_XON;
				 else
					sframe.FrameType = HEARTBEAT_XOFF;
				 sframe.Length = 0;
				 ptp->PTP_SendFrame((PTP_FRAME *)&sframe);
				}
*/
				next = 0;
				break;
			case TR_PENDING:
//				TimeOut = pwait(T_RESPONSE, WM_PTP_TRANSMISSION, myMsg);

				/*	Send Request	*/
				/*	Disconnected	*/
				if( ptp->connection_state == PTP_DISCONNECTED )
				{
					ptp->transmission_state = TR_IDLE;
					next = 0;
					break;
				}
//				if( TimeOut )
				if( param == 2 )   // time out
				{
					if( ptp->RetryCount < N_RETRIES && !ps->PTP_comm_status.transmission_blocked )
					{
						/*	Retry	*/
						ptp->RetryCount++;
						ptp->ResponseTimer = T_RESPONSE;
						ptp->PTP_SendFrame(&sendframedata, 1);
//------
					    next = 0;
						break;
					}
					else
					{
						/*	Retries failed	*/
						ptp->RetryCount = 0;
//						response_timer = 0;
						ptp->transmission_state = TR_READY;
						break;
					}
				}
				if( param==0 )
				{
				 /*	Receive Acknowledgement	*/
				 if( ( ps->PTP_comm_status.ack0received && ps->PTP_comm_status.TxSequence_number == 0 ) ||
					 ( ps->PTP_comm_status.ack1received && ps->PTP_comm_status.TxSequence_number == 1 ) )
				 {
//					ptp->SendFramePool.AdvanceTail();
					ps->PTP_comm_status.TxSequence_number = (1 - ps->PTP_comm_status.TxSequence_number);
					ps->PTP_comm_status.ack0received = 0;
					ps->PTP_comm_status.ack1received = 0;
					ptp->transmission_state = TR_READY;
					break;
				 }
				}
                else
					if( param==1 )
					{
                     break;
        	        }

//				if( TimeOut )
//				if( !ptp->ResponseTimer )
//				&& ( ( ptp->PTP_comm_status.reject0received && ptp->PTP_comm_status.TxSequence_number == 0 ) ||
//						( ptp->PTP_comm_status.reject1received && ptp->PTP_comm_status.TxSequence_number == 1 ) ) )
//						(	ptp->response_timer > T_RESPONSE ) )
//--------
                next = 0;
				break;
		}
 }
}

PTP::PTP(HWND h, DInt com, DInt port)
{
    Handle = h;
	port_number = port;
    com_port = com;
	ResponseTimer = T_RESPONSE;
	frame_send_timer = 0;
	password_needed = 0;
	password_OK = 0;

	reception_state = 0;
	transmission_state = 0;
	sendframe_flag = 0;
	connection_state = PTP_DISCONNECTED;
	port_status=INSTALLED;

	modem_obj     = NULL;
	modem_present = 0;
	modem_active  = 0;
    modem_status  = 0;
/*
    if( CommInfo[com_port].Connection() == MODEM )
    {
		if( ( modem_obj = new Modem( DInt com, DInt port, this ) ) == NULL )
		{
			 DisplayMessage(20, 7, 60, 11, NULL, " Unable to install the modem ! ",NULL, Blue,NULL,1500);
			 modem_obj = NULL;
			 modem_present = 0;
		}
		else
		{
			modem_present = 1;
			if( ( modem_status = modem_obj->Initialize() ) != MODEM_SUCCESS )
			{
				 modem_active = 0;
             port_status = NOT_INSTALLED;
			}
			else
			{
				modem_active = 1;
			}
		}
    }
*/
}

DInt PTP::init_idle_state(DInt m)
{
 PORT_STATUS_variables *ps;
 ps = &Routing_table[0].port_status_vars;
 memset( &ps->PTP_comm_status, 0, 2 );
 ps->PTP_comm_status.reception_blocked = Q_NOT_BLOCKED;
 ps->PTP_comm_status.transmission_blocked = 1;
 ps->physical_connection_state = SERIAL_ACTIVE;
 reception_state = REC_IDLE;
 transmission_state = TR_IDLE;
// activity = FREE;
 connection_state = PTP_DISCONNECTED;
 ps->connection = PTP_DISCONNECTED;
// if( media==MODEM_LINK )
// {
//   ps->physical_connection_state = CONNECTION_INACTIVE;
//	connection_state = PTP_IDLE;
//	ps->connection = PTP_IDLE;
//	if(m && Read_mode()==SLAVE/
//		modem_obj->Initialize();
//	FlushRXbuffer();
//	FlushTXbuffer();
// }
 ReceivedFramePool.Clear();
 SendFramePool.Clear();
 return 0;
}

DInt PTP::sendframe(DInt length_npci, Byte *npci, DInt length_asdu_npdu, Byte * asdu_npdu, DInt length_apci, Byte *apci, DInt send)
{
 DInt nextentry;
 PTP_FRAME *pframe;

	 if( (nextentry = SendFramePool.NextFreeEntry()) < 0 ) return 0;
	 pframe = &SendFramePool.Frame[nextentry];
//	 pframe->Length = length_npci+length_apci+length_asdu_npdu;
	 pframe->FrameType = DATA_0;
	 pframe->Length = (DUint)(length_npci+length_apci+length_asdu_npdu);
	 memcpy(pframe->Buffer, npci, length_npci);
	 if( length_apci )
	  memcpy(&pframe->Buffer[length_npci], apci, length_apci);
	 if( length_asdu_npdu )
	  memcpy(&pframe->Buffer[length_npci+length_apci], asdu_npdu, length_asdu_npdu);
	 SendFramePool.Unlockhead();
//	 resume(task+PTP_transmission);

    if(send)
//	    ProcSendMessage(WM_PTP_TRANSMISSION,0);
  	  SendMessage(
	    Handle,	// handle of destination window
	    msgWM_PTP_TRANSMISSION,	// message to post
	    com_port,	// first message parameter
	    0  	// second message parameter
	  );
    else
//	    ProcPostMessage(WM_PTP_TRANSMISSION,0);
  	  PostMessage(
	    Handle,	// handle of destination window
	    msgWM_PTP_TRANSMISSION,	// message to post
	    com_port,	// first message parameter
	    0  	// second message parameter
	  );
	 return 1;
}

DInt SENDFRAMEPOOL::NextFreeEntry(void)
{
	DInt h,temp_head;
//	DInt r;
	if( access && lockedhead )
	{
	 return -1;
	}
	access=1;
	h = temp_head = HeadFrame;
	if( overflow )
	{
	h = -1;
	}
	else
	{
	if ( ++temp_head >= MSTP_NMAXSENDFRAMEPOOL )
	 temp_head = 0;
	lockedhead=1;
	if ( temp_head == TailFrame )
	 overflow=1;
	HeadFrame = temp_head;
	}
	access=0;
	return h;
}

DInt PTP_SENDFRAMEPOOL::RemoveEntry(PTP_FRAME *frame)
{
	DInt t,r;
	if( access ) return -1;
	access=1;
	if ( HeadFrame == TailFrame && !overflow )
	{
	 r = -1;
	}
	else
	{
	 t=TailFrame+1;
	 if ( t >= PTP_NMAXSENDFRAMEPOOL )
		t = 0;
	 if( t==HeadFrame && lockedhead )
	 {
		r = -1;
	 }
	 else
	 {
		*frame = Frame[TailFrame];
		TailFrame++;
		if ( TailFrame >= PTP_NMAXSENDFRAMEPOOL )
			 TailFrame = 0;
		r = 1;
	 }
	}
	access=0;
	return r;
}

void PTP_SENDFRAMEPOOL::AdvanceTail(void)
{
	if ( overflow ) {overflow=0;};
	TailFrame++;
	if ( TailFrame >= PTP_NMAXSENDFRAMEPOOL )
		TailFrame = 0;
}


PTP_RECEIVEDFRAMEPOOL::PTP_RECEIVEDFRAMEPOOL(void)
{
	memset(ReceivedFrame,0,sizeof(ReceivedFrame));
}

void PTP_RECEIVEDFRAMEPOOL::Clear(void)
{
  memset(ReceivedFrame,0,sizeof(ReceivedFrame));
}

void *PTP_RECEIVEDFRAMEPOOL::NextFreeEntry( void )
{
 DInt i;
 for(i=0; i<PTP_NMAXRECEIVEDFRAMEPOOL; i++)
 {
	if( !ReceivedFrame[i].status )
	{
		ReceivedFrame[i].status=2;
		break;
	}
 }
 if ( i >= PTP_NMAXRECEIVEDFRAMEPOOL ) return 0;
 return (void *)&ReceivedFrame[i];
}

DInt PTP_RECEIVEDFRAMEPOOL::RemoveEntry(void *f)
{
 DInt i;
// int j;
 PTP_ReceivedFrame *frame;
 frame = (PTP_ReceivedFrame *)f;
 frame->flags.ReceivedValidFrame = frame->flags.ReceivedInvalidFrame = FALSE;
 for(i=0; i<PTP_NMAXRECEIVEDFRAMEPOOL; i++)
 {
	if( ReceivedFrame[i].status==1 ) break;
 }
 if ( i >= PTP_NMAXRECEIVEDFRAMEPOOL ) return -1;
 memcpy(frame, &ReceivedFrame[i], sizeof(PTP_ReceivedFrame));
 ReceivedFrame[i].flags.ReceivedValidFrame=0;
 ReceivedFrame[i].flags.ReceivedInvalidFrame=0;
 ReceivedFrame[i].status=0;
 return 1;
}

void PTP::PTP_SendFrame( PTP_FRAME *frame, DInt retry )
{
 DByte *p, buf[8];
 DInt   i, ind, indbuf;
 DUint  length, crc;      //,rate
// DUlong l;
 DByte  ptrBuf[MAXFRAME+1];

//  rate = ReadBaudRate()/200;
//   rate = 1920/200;
//  set_semaphore(&sendframe_flag);
   if(frame)
   {
	if(!retry)
	{
	 frame->Preamble = 0x0FF55;
	 frame->HeaderCRC = 0x0FF;

     indbuf = 0;
     ptrBuf[indbuf++] = 0x55;
     ptrBuf[indbuf++] = 0x0FF;

// switch the low and high bytes of Length -> BAC frame
	 length = frame->Length;
	 frame->Length = ( (*((char *)&length))<<8 ) + *( (((char *)&length)+1) );
// end switch
	 frame->HeaderCRC=CalcHeaderCRC(frame->FrameType, frame->HeaderCRC);
	 p = (DByte *)&frame->Length;
	 for(i=0; i<2; i++)
	 {
	  frame->HeaderCRC=CalcHeaderCRC(*p++, frame->HeaderCRC);
	 }
	 frame->HeaderCRC = ~frame->HeaderCRC;

     ptrBuf[indbuf++] = frame->FrameType;
     memcpy(&ptrBuf[indbuf], &frame->Length, 2);
     indbuf += 2;
     ptrBuf[indbuf++] = frame->HeaderCRC;

	 ind=0;
	 if(length)
	 {
	    p = frame->Buffer;
		crc = 0x0ffff;
		for(ind=0; ind<length; ind++)
		crc=CalcDataCRC(*p++, crc);
		crc = ~crc;
		memcpy(&frame->Buffer[ind], &crc, 2);
		ind += 2;

        memcpy(&ptrBuf[indbuf], frame->Buffer, ind);
        indbuf += ind;
	 }

	 for(i=0; i<indbuf; i++)
     {
		if( ptrBuf[i]==0x10 || ptrBuf[i]==0x11 || ptrBuf[i]==0x13 )
		{
            memmove(&ptrBuf[i+1], &ptrBuf[i], indbuf-i);
            ptrBuf[i]    = 0x10;
            ptrBuf[i+1] |= 0x90;
            indbuf++;
		}
     }
//	frame->Buffer[ind++]=0x0ff;  //pad MSTP
	}
	else
	{
	 ind  = ( (*((char *)&frame->Length))<<8 ) + *( (((char *)&frame->Length)+1) ) + 2;

     indbuf = 0;
     ptrBuf[indbuf++] = 0x55;
     ptrBuf[indbuf++] = 0x0FF;
     ptrBuf[indbuf++] = frame->FrameType;
     memcpy(&ptrBuf[indbuf], &frame->Length, 2);
     indbuf += 2;
     ptrBuf[indbuf++] = frame->HeaderCRC;
     memcpy(&ptrBuf[indbuf], frame->Buffer, ind);
     indbuf += ind;
	 for(i=0; i<indbuf; i++)
     {
		if( ptrBuf[i]==0x10 || ptrBuf[i]==0x11 || ptrBuf[i]==0x13 )
		{
            memmove(&ptrBuf[i+1], &ptrBuf[i], indbuf-i);
            ptrBuf[i]    = 0x10;
            ptrBuf[i+1] |= 0x90;
            indbuf++;
		}
     }
	}
   }
   else
   {
/*
	 memset(buf, 0x55, 6);
	 buf[6]=0x015;
	 buf[7]=0x0ff;
	 ind = 0;
	 frame = (PTP_FRAME *)&buf[0];
*/
	 memset(ptrBuf, 0x55, 6);
	 buf[6]=0x015;
	 buf[7]=0x0ff;
     indbuf = 8;
   }
//	WriteString(1, (char *)frame, (DWORD)(ind+6));
	WriteString(1, ptrBuf, (DWORD)(indbuf));

	Routing_table[port_number].port_status_vars.SilenceTimer=0;
	Routing_table[port_number].port_status_vars.HeartbeatTimer=0;
//  clear_semaphore(&sendframe_flag);
}

DByte  fe = 0x0fe;

DByte CalcHeaderCRC(DByte dataValue, DByte crcValue)
{
 DUint crc;
 crc = crcValue ^ dataValue;
 crc = crc ^ (crc<<1) ^ (crc<<2) ^ (crc<<3)
			  ^ (crc<<4) ^ (crc<<5) ^ (crc<<6) ^ (crc<<7);
 return (crc&fe) ^ ((crc >> 8)&1);
}


DUint  ff = 0x0ff;

DUint CalcDataCRC(DByte dataValue, DUint crcValue)
{
 DUint crcLow;
 crcLow = (crcValue & ff) ^ dataValue;
 return (DUint)(crcValue >> 8) ^ (DUint)(crcLow<<8) ^ (DUint)(crcLow<<3) ^ (DUint)(crcLow<<12)
				^ (DUint)(crcLow>>4) ^ (DUint)(crcLow&0x0f) ^ (DUint)((crcLow&0x0f) << 7);
}

DInt pwait(DWORD timesleep, DUint msg, MSG& myMsg, DInt breakcond)
{
   DWORD            dwStopTime, timeVar;
   /*
	The following loop determines the status of the serial
	interface, which specifies whether the application will act
	as sender or receiver.

	This loop gets the system time and waits for about 4 timer
	clicks. During this time, the application releases the CPU for
	message processing, and for getting important WM_TIMER
	messages for PollRxQueue.
   */
   dwStopTime  = GetTickCount() + timesleep;
   timeVar = GetTickCount() + 800;
   while(GetTickCount() < dwStopTime)
   {
      if(GetTickCount() > timeVar)
      {
        timeVar = GetTickCount() + 800;
        runImage(tImageRun,maxImages);
      }
	  if(PeekMessage(&myMsg, NULL, NULL, NULL, PM_REMOVE))
	  {
       if( (DUint)myMsg.message == msg )
         return FALSE;
       if( breakcond )
        if( myMsg.message == WM_KEYDOWN && myMsg.wParam == VK_CANCEL )
	    {
         return TRUE;
	    }
	   TranslateMessage(&myMsg);
	   DispatchMessage (&myMsg);
	  }
   }
   return TRUE;
}

void delay(DUint timedelay)
{
   DWORD     dwStopTime;
   dwStopTime  = GetTickCount() + timedelay;
   while(GetTickCount() < dwStopTime);
}

__declspec(dllexport) void setDLLObjects(TProgressBar *progressbar, TImage *timagerun[], DInt maximage);

void setDLLObjects(TProgressBar *progressbar, TImage *timagerun[], DInt maximages)
{
  ProgressBar = progressbar;
  tImageRun = timagerun;
  maxImages = maximages;
}
