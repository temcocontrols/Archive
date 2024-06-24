#ifdef NET_BAC_COMM

#include <windows.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <mem.h>
#include <bios.h>
#include <conio.h>
#include <string.h>
#include <process.h>
#include "gwin.h"
#include "net_bac.h"
#include "baseclas.h"
#include "mtkernel.h"
#include "router.h"
#include "t3000def.h"
#include "graph.h"
#include "serial.h"
#include "rs485.h"

#define ON   1
#define OFF  0

#define POOL_BUF_SIZE (long)MAX_SAMPLE*(long)MAX_INP*4L+MAX_INP*10*4+MAX_MEM_DIG_BUF+400+MAX_HEADERS_AMON*sizeof(Header_amon)+MAX_HEADERS_DMON*sizeof(Header_dmon)+810

extern int net_call(int command, int arg,  char *data, uint *length , int dest,
				 int network, int others=0, int timeout=TIMEOUT_NETCALL,
				 char *returnargs=NULL, int *length_returnargs=NULL,
				 char *sendargs=NULL, int length_sendargs=0, char bytearg=0, int port=-1);
extern char Station_NAME[NAME_SIZE];
extern int  Station_NUM;
extern long timestart;
extern char ready_for_descriptors;
extern char sendinfoflag;
extern NETWORK_POINTS	_far network_points_list[MAXNETWORKPOINTS];
extern REMOTE_POINTS _far  remote_points_list[MAXREMOTEPOINTS82];
extern Panel *ptr_panel;

extern int encodetag(char cl, char t, char *tag, unsigned length);
extern uint search_point( Point &point, char *buff, char * & point_adr,
																		uint & point_length, Search_type order );

extern int networklayer( int service, int priority, int network, int destination, int source,
				char *asdu_npdu, int length_asdu_npdu, char *apci, int length_apci,
				int data_expecting_reply=BACnetDataNotExpectingReply, int clientserver=CLIENT,
				int port = -1);

extern char netpointsflag;
extern int ipxport, rs485port;
extern char wantpointsentry;
extern int timepoints;
//char netpointsflag_ipx;

void NET_SENDFRAMEPOOL::Init_frame_pool( SEND_FRAME_ENTRY *buffer )
{
	no_of_used_frames = 0;
	Frame = buffer;
	memset( Frame,0, NET_NMAXSENDFRAME*sizeof(FRAME) );
}

SEND_FRAME_ENTRY *NET_SENDFRAMEPOOL::NextFreeEntry( int reply, int command )
{
	register SEND_FRAME_ENTRY *frame_ptr;
	register int i;

//	if( no_of_used_frames >= ( NET_NMAXSENDFRAME - 1 ) )
//		set_semaphore( &Using_send_frame );

	if( command )
	{
	 frame_ptr = Frame;
	 i=0;
	 do
	 {
		if( frame_ptr->being_used )
		{
			if(frame_ptr->delay == command-72+1)
			{
			 return NULL;
			}
		}
		i++;
		frame_ptr++;
	 }
	 while( i < NET_NMAXSENDFRAME );
	}



	no_of_used_frames++;
	frame_ptr = Frame;

	i=0;
	do
	{
		if( !frame_ptr->being_used )
		{
			frame_ptr->being_used = 1;
			frame_ptr->locked = 1;
			frame_ptr->reply_flag = reply;
			return frame_ptr;
		}
		i++;
		frame_ptr++;
	}
	while( i < NET_NMAXSENDFRAME );
	return NULL;
}

SEND_FRAME_ENTRY *NET_SENDFRAMEPOOL::FrameAvailable( int reply )
{
	register SEND_FRAME_ENTRY *frame_ptr;
	register int i;

	frame_ptr = Frame;

	i=0;
	do
	{
		if( frame_ptr->being_used && !frame_ptr->locked && !frame_ptr->sending )
		{
			if( reply < 0 )
			{
				frame_ptr->sending = 1;
				return frame_ptr;
			}
			else
				if( frame_ptr->reply_flag == reply )
				{
					frame_ptr->sending = 1;
					return frame_ptr;
				}
		}
		i++;
		frame_ptr++;
	}
	while( i < NET_NMAXSENDFRAME );

	return NULL;
}

void NET_SENDFRAMEPOOL::RemoveSentEntry( SEND_FRAME_ENTRY *frame_ptr )
{
	frame_ptr->sending = 0;
	frame_ptr->being_used = 0;
	frame_ptr->locked = 0;
	frame_ptr->reply_flag = 0;
	no_of_used_frames--;
//	if( Using_send_frame )
//		clear_semaphore( &Using_send_frame );
}

void NET_SENDFRAMEPOOL::Unlock_frame( SEND_FRAME_ENTRY *frame )
{
	frame->locked = 0;
}

int NET_SENDFRAMEPOOL::FramesInUse( void )
{
	return no_of_used_frames;
}

int NET_BAC::Get_ControlBlock( void )
{
	int i;

	if( available_CBs )
	{
		i = last_assigned_CB + 1;
		if( i == (ControlBlocksQueueSize-1) )
			i = 0;
		while( i != last_assigned_CB )
		{
			if( !TestInUseField(i) )
			{
				if( TestCompletion( i ) )
				{
					ResetControlBlock( i );
					available_CBs--;
					last_assigned_CB = i;
					return i;
				}
			}
			i++;
			if( i == (ControlBlocksQueueSize-1) )
				i = 0;
		}
	}
	return -1;
}

void NET_BAC::Release_ControlBlock( byte index )
{
	available_CBs++;
	ResetInUseField( index );
}

void NET_BAC::initData( Panel_info1 *info )
{
  int i;
	memset( ReceiveFramePool, 0, NET_NMAXRECEIVEFRAME*sizeof(FRAME) );
	activity = 0;
	rec_comm_active = 0;
	rec_br_comm_active = 0;
	memset( rec_list, 0, NET_NMAXRECEIVEFRAME*sizeof( Command_State ) );
	for( i=0; i<NET_NMAXRECEIVEFRAME; i++ )
		rec_list[i].state = IDLE_DG;
	memset( send_list, 0, NET_NMAXSENDFRAME*sizeof( Command_State ) );
	for( i=0; i<NET_NMAXSENDFRAME; i++ )
		send_list[i].state = IDLE_DG;

//	memset(&need_info, 0, sizeof(need_info));
	need_info = (1l<<(info->panel_number-1));
	memcpy( &panel_info1, info, sizeof(Panel_info1) );
	memset( station_list, 0, MAX_STATIONS*sizeof(Station_point) );
	station_list[info->panel_number-1].panel_type = info->panel_type;
	station_list[info->panel_number-1].version = info->version;
	memcpy(station_list[info->panel_number-1].tbl_bank,ptr_panel->table_bank,sizeof(ptr_panel->table_bank));
	station_list[info->panel_number-1].number = info->panel_number;
	station_list[info->panel_number-1].des_length = info->des_length;
	station_list[info->panel_number-1].state = 1;
	memcpy( station_list[info->panel_number-1].name, info->panel_name, NAME_SIZE );

	panel_info1.active_panels = ( 1L << ( info->panel_number-1 ) );

	end_task = 0;
	send_info = 3;
	FirstToken = -1;
	panelconnected = 0;
	laststation_connected = -1;
  newpanelbehind = 0;
}

void NET_BAC::InitNetData( Panel_info1 *info, byte task_no )
{
	unsigned long par_sel;
	unsigned int i, j;
	SEND_FRAME_ENTRY *sendframe_buffer;

	task_number = task_no;

	par_sel = GlobalDosAlloc( (unsigned int)(NET_NMAXSENDFRAME*sizeof(FRAME) ) );
	/* i = prot_SendFramePool_DS */
	i = par_sel&0x0ffff;
	real_Send_pool_DS = par_sel>>16;
	sendframe_buffer = ( SEND_FRAME_ENTRY far *)MK_FP( i, 0 );
	SendFramePool.Init_frame_pool( sendframe_buffer );

	par_sel = GlobalDosAlloc( (unsigned int)(NET_NMAXRECEIVEFRAME*sizeof(FRAME) ) );
	/* i = prot_ReceivedFramePool_DS */
	i = par_sel&0x0ffff;
	real_Receive_pool_DS = par_sel>>16;
	ReceiveFramePool = ( FRAME far *)MK_FP( i, 0 );

	mfarmalloc( &ser_data, POOL_BUF_SIZE );
	ser_pool.init_pool( ser_data, POOL_BUF_SIZE );

  initData( info );
}

void NET_BAC::ReleaseRealMemory( void )
{
	if( state == DRIVER_INSTALLED )
	{
		GlobalDosFree( FP_SEG(SendFramePool.Frame ) );
		GlobalDosFree( FP_SEG(ReceiveFramePool ) );
		if( ser_data )
		{
			mfarfree(ser_data);
			ser_data = NULL;
		}
	}
}

int NET_BAC::CheckForCommandCompletion( void )
{
	byte  i;

	for( i=0; i<NET_NMAXSENDFRAME; i++ )
	{
		if( send_list[i].state == SENDING_DG )
			if( TestCompletion( send_list[i].number ) )
				return 1;
	}
	for( i=0; i<NET_NMAXRECEIVEFRAME; i++ )
	{
		if( rec_list[i].state == WAITING_DG )
			if( TestCompletion( rec_list[i].number ) )
				return 1;
	}
	return 0;
}

/*
int NET_BAC::Sendinfo( int status, int panel, int dest )
{
	SEND_FRAME_ENTRY *frame;
	int i=0;
	if( ( frame = SendFramePool.NextFreeEntry( 0 ) ) )
	{
		frame->FrameType = BACnetDataNotExpectingReply;
		frame->Destination = dest;
		frame->Source = panel_info1.panel_number;
// npci
		frame->Buffer[i++]=0x01;      // npci  version BACnet
		frame->Buffer[i++]=0x00;      // npci  local network
// apci
		frame->Buffer[i++] = (BACnetUnconfirmedRequestPDU<<4);
		frame->Buffer[i++] = UnconfirmedPrivateTransfer;
// asdu - service request
		frame->Buffer[i++]=0x09;           //tag
		frame->Buffer[i++]=VendorID;
		frame->Buffer[i++]=0x1A;           //tag
		frame->Buffer[i++]=50+100;
		frame->Buffer[i++]=70;
		frame->Buffer[i++]=0x2D;               			//Octet String, L>4
//	  if(status&0x10)
//		  frame->Buffer[i++]=3+2+sizeof(Station_NAME);  // L
//	  else
		{
		frame->Buffer[i] = 3+2;     // panel_num
		if(status&0x01==ON)
			frame->Buffer[i] += sizeof(Station_NAME)+2;
		if(status&0x02==0x02)
			frame->Buffer[i] += sizeof(Time_block);  // L
		i++;
		}
// parameters
		frame->Buffer[i++]=status;                 				//extra low
		frame->Buffer[i++]=0;             				//extra high
		frame->Buffer[i++]=0;                  				//reserved
		memcpy( &frame->Buffer[i], &panel, 2);
		i += 2;
		if(status&0x01==ON)
		{
//		  Point point;
//		  unsigned int l = search_point( point, NULL, NULL, 0, LENGTH );
//		  station_list[Station_NUM-1].des_length = l;
			unsigned int l;
			l = panel_info1.des_length;
			memcpy( &frame->Buffer[i], panel_info1.panel_name, sizeof(Station_NAME));
			i += sizeof(Station_NAME);
			memcpy( &frame->Buffer[i], &l, 2);
			i += 2;
		}
		if(status&0x02)
		{
//			memcpy( &frame->Buffer[i], (char*)&ora_current, sizeof(Time_block));
			i += sizeof(struct tm);
		}

// end parameters
//	  frame->Buffer[i++]=0x2f;           				//closing tag
		frame->Length = i;
		frame->locked = 0;
		return 0;
	}
	return -1;
}
*/

void NET_BAC::SignOff( void )
{
/*	router(N_UNITDATArequest, Initialize_Routing_Table, NULL, port_number);
	router(N_UNITDATArequest, Initialize_Routing_Table, NULL, port_number);
	while( SendFramePool.FramesInUse() )
	{
		NetControl();
		msleep( 2 );
	}
*/
	unsigned long tstart;
	sendinfoflag = 1;
	nextpanelisoff = Station_NUM;
	sendinfo_flag|=SENDINFO_PANELOFF;
	resume(NETTASK);
	send_info = 0;
	end_task = 1;
	suspend(NETWORK1);
	tstart = timestart;
	while( SendFramePool.FramesInUse() && timestart<tstart+10)
	{
		NetControl();
		msleep( 2 );
	}
}


void NET_BAC::NetControl( void )
{
	byte j, i;
	uint length;
	FRAME *rec_frame;
	SEND_FRAME_ENTRY *frame_ptr;

	if( newpanelbehind )
	{
			sendinfoflag = 1;
			sendinfo_flag|=SENDINFO_IAMONNET;
			resume(NETTASK);
	}
	if( laststation_connected>=0 )
	{
			sendinfoflag = 1;
			sendinfo_flag|=LASTSTCONNECTED;
			resume(NETTASK);
	}
	if ( FirstToken < 0 )
	{
			FirstToken = 0;
			sendinfoflag = 1;
			sendinfo_flag|=FIRSTTOKEN;
			resume(NETTASK);
	}
/*
	if(ready_for_descriptors&0x02)
	{
			sendinfo_flag|=READYFORDES;
			sendinfoflag = 1;
			resume(NETTASK);
	}
*/
//           if time want points and network points
				 if(netpointsflag==3)
				 {
					 if( ipxport!=-1 )
					 net_call(COMMAND_50, SEND_NETWORKPOINTS_COMMAND, 0, 0, 255, Routing_table[rs485port].Port.network, BACnetUnconfirmedRequestPDU,    //|NETCALL_NOTTIMEOUT,
											 TIMEOUT_NETCALL,NULL,NULL,NULL,0,0,ipxport);
					 memset(network_points_list, 0, sizeof(network_points_list));
					 netpointsflag=0;
					 wantpointsentry=0;
					 timepoints = 40;
				 }
				 if(netpointsflag==1)
				 {
					 if(wantpointsentry)
					 {
						netpointsflag=2;
						resume(NETTASK);
					 }
					 else
					 {
						netpointsflag=0;
						timepoints = 50;
					 }
				 }
				 if (!timepoints && !netpointsflag)
				 {
					if( ipxport!=-1 )
					 net_call(COMMAND_50, SEND_WANTPOINTS_COMMAND+(1<<8), 0, 0, 255, Routing_table[rs485port].Port.network, BACnetUnconfirmedRequestPDU,   //|NETCALL_NOTTIMEOUT,
											 TIMEOUT_NETCALL,NULL,NULL,NULL,0,0,ipxport);
					netpointsflag=1;
				 }
//  syncronize time
/*
				 if( dayofyear!=ora_current.dayofyear )
				 {
						dayofyear=ora_current.dayofyear;
						if(TS<mstp->OS)
						{
						 sendinfoflag = 1;
						 mstp->sendinfo_flag|=SENDINFO_TIME;
						 resume(NETTASK);
						}
				 }
*/

	while( ( frame_ptr = SendFramePool.FrameAvailable( 0 ) ) )
	{
		i=0;
		while( i<NET_NMAXSENDFRAME && send_list[i].state != IDLE_DG )
			i++;
		if( i < NET_NMAXSENDFRAME )
		{
				send_list[i].number = Get_ControlBlock();
				if( send_list[i].number >= 0 )
				{
					send_list[i].frame.send = frame_ptr;
					send_list[i].state = SENDING_DG;
					if( frame_ptr->Destination == 0x0FF )
						msleep( 2 );
					SendFrame( i );
				}
				else
					send_list[i].number = -1;
		}
	}
	for( i=0; i<NET_NMAXSENDFRAME; i++ )
	{
		if( send_list[i].state == SENDING_DG )
		{
			if( TestCompletion( send_list[i].number ) )
			{
				SendFramePool.RemoveSentEntry( send_list[i].frame.send );
				Release_ControlBlock( send_list[i].number );
				send_list[i].number = -1;
				send_list[i].state = IDLE_SD;
				activity = 1;
			}
		}
	}

	for( i=0; i<NET_NMAXRECEIVEFRAME; i++ )
	{
		if( rec_list[i].state == WAITING_DG )
		{
			if( TestCompletion( rec_list[i].number ) )
			{
				if( FinalResultCode( rec_list[i].number ) == 0 )
				{
					rec_list[i].state = RECEIVED_DG;
					rec_frame = rec_list[i].frame.rec;
					if( rec_frame->Source != Station_NUM && !end_task )
					{
//			int networklayer( int service, int priority, int network, int destination, int source,
//						char *asdu_npdu, int length_asdu_npdu, char *apci, int length_apci,
//						int data_expecting_reply=BACnetDataNotExpectingReply, int clientserver=CLIENT, int port = -1 );
						CopySourceAddress( rec_frame->Source-1, rec_list[i].number );
						networklayer( DL_UNITDATAindication, NORMALmessage, panel_info1.network,
								rec_frame->Destination, rec_frame->Source,
								rec_frame->Buffer, rec_frame->Length, NULL, 0,
								rec_list[i].type?BACnetDataNotExpectingReply:BACnetDataExpectingReply,
								SERVER, port_number );
					}
				}
//				else
//					rec_list[i].state = REC_ERROR;

				rec_comm_active--;
				if( rec_list[i].type )
					rec_br_comm_active--;
				Release_ControlBlock( rec_list[i].number );
				rec_list[i].number = -1;
				rec_list[i].state = IDLE_DG;
				activity = 1;
			}
		}
	}

	while( rec_comm_active < NET_NMAXRECEIVEFRAME && !end_task )
	{
		for( i=0; i<NET_NMAXRECEIVEFRAME; i++ )
		{
			if( rec_list[i].state == IDLE_DG )
			{
					rec_list[i].number = Get_ControlBlock();
					if( rec_list[i].number >= 0 )
					{
						rec_comm_active++;
						rec_list[i].state = WAITING_DG;
						if( !rec_br_comm_active )
						{
							rec_br_comm_active++;
							rec_list[i].type = 1;
						}
						else
							rec_list[i].type = 0;
						rec_list[i].frame.rec = ReceiveFramePool+i;
						ReceiveFrame( i );
					}
					else
						rec_list[i].number = -1;
			}
		}
	}
}

#endif // NET_BAC_COMM
