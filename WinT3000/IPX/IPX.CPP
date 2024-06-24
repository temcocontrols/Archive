#ifdef NET_BAC_COMM

#include <vcl\vcl.h>
#pragma hdrstop
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <mem.h>
#include <bios.h>
#include <conio.h>
#include <string.h>
#include <process.h>
#include "baseclas.h"
#include "t3000def.h"
#include "net_bac.h"
#include "ipx.h"

#define ON   1
#define OFF  0

Panel_info1 _import Panel_Info1;

extern int NET_EXIT;
extern long timestart;
extern IPX *pipx;

IPX::IPX( Panel_info1 *info, byte task_no )
{
	DUlong par_sel;
	DUint  i;
	SEND_FRAME_ENTRY *sendframe_buffer;

	ControlBlocksQueueSize = 8;
//	par_sel = GlobalDosAlloc( (unsigned int)(ControlBlocksQueueSize*sizeof( IpxControlBlock ) ) );
//	i = par_sel&0x0ffff;  /* i = protDS */
//	real_array_DS = par_sel>>16;      /* j = realDS */
//	array = ( IpxControlBlock far *)MK_FP( i, 0 );
	array = new IpxControlBlock[ControlBlocksQueueSize];
	memset( array, 0, ControlBlocksQueueSize*sizeof(IpxControlBlock));
	available_CBs = ControlBlocksQueueSize - 1;
	last_assigned_CB = ControlBlocksQueueSize - 2;

	if( TestDriverPresence() == 0 )
	{
		InitNetData( info, task_no  );
		state = DRIVER_INSTALLED;
		media = IPX_LINK;
		socket = 0x4444;
	}
	else
		state = DRIVER_NOT_INSTALLED;
}

IPX::~IPX()
{
	ReleaseRealMemory();
//	GlobalDosFree( FP_SEG( array ) );
	delete array;
}

DInt IPX::IpxSpxOpenSocket( DUint *SocketNumber )
{
//	REAL_MODE_REGISTERS real_data_regs;
//  	memset( &real_data_regs, 0, sizeof( REAL_MODE_REGISTERS ) );
    DByte CompletionCode;
    DUint AssignedSocketNumber,bp;
	_AL = SHORT_LIVED_SOCKET;
	_BX = IPX_SPX_OpenSocket;
	_DX = swab( *SocketNumber );
//	real_data_regs.CS = Ipx_segment_call;
//	real_data_regs.IP = Ipx_offset_call;
//	if( !IpxCall( &real_data_regs ) )
    bp=_BP;
	IpxSpxCallPtr();
    _BP=bp;
//	if( !IpxSpxCallPtr() )
	{
        CompletionCode = _AL;
        AssignedSocketNumber = _DX;
		switch( CompletionCode & 0x0FF ) /* CompletionCode */
		{
			case 0x00 :
					*SocketNumber = AssignedSocketNumber;
					return 0;
			case 0xFE :
//				printf("[Failure, full socket table]");
					break;
			case 0xFF :
//					printf("[Failure, socket already open]");
					break;
			default   :
//					printf("[Unknown completion code (%02X)]", CompletionCode);
					break;
		}
		return ( CompletionCode & 0x0FF );
	}
//	else
//		return -1;
}

DInt IPX::IpxSpxCloseSocket( DUint *SocketNumber )
{
/*
	REAL_MODE_REGISTERS real_data_regs;

	memset( &real_data_regs, 0, sizeof( REAL_MODE_REGISTERS ) );

	real_data_regs.AX.w = SHORT_LIVED_SOCKET;
	real_data_regs.BX.w = IPX_SPX_CloseSocket;
	real_data_regs.DX.w = *SocketNumber;
	real_data_regs.CS = Ipx_segment_call;
	real_data_regs.IP = Ipx_offset_call;

	if( !IpxCall( &real_data_regs ) )
		return 0;
	else
		return 1;

*/
}

DInt IPX::IpxGetLocalTarget( DByte *request, DByte *reply )
{
}

DInt IPX::IpxDisconnectFromTarget( TargetAddress *target )
{
/*
	REAL_MODE_REGISTERS real_data_regs;
	DUint real_ES, selector;
	DUlong par_sel;
	TargetAddress *rm_target;

	par_sel = GlobalDosAlloc( (DUint)sizeof( TargetAddress ) );
	selector = par_sel&0x0ffff;
	real_ES = par_sel>>16;
	rm_target = ( TargetAddress far *)MK_FP( real_ES, selector );
	memcpy( rm_target, target, sizeof( TargetAddress ) );

	memset( &real_data_regs, 0, sizeof( REAL_MODE_REGISTERS ) );

	real_data_regs.BX.w = IPX_DisconnectFromTarget;
	real_data_regs.ES = real_ES;
	real_data_regs.SI.w = 0;
	real_data_regs.CS = Ipx_segment_call;
	real_data_regs.IP = Ipx_offset_call;

	real_ES = IpxCall( &real_data_regs );
	memcpy( target, rm_target, sizeof( TargetAddress ) );

	if( !real_ES )
		return 0;
	else
		return 1;
*/
}

DInt IPX::IpxListenForPacket( DByte ecb_index, DByte frame_index, DUint socket )
{
/*
	REAL_MODE_REGISTERS real_data_regs;
	Ecb *ecb_ptr;

	ecb_ptr = (Ecb*)(array+ecb_index);

	ecb_ptr->EventServiceRoutine = NULL;
	ecb_ptr->SocketNumber = socket;
	ecb_ptr->FragmentCount = 2;
//	  real mode pointer to IpxHeader
	ecb_ptr->FragmentDescriptor[0].Ptr = MK_FP( real_array_DS, ( FP_OFF(ecb_ptr)+ sizeof(Ecb) ) );
	ecb_ptr->FragmentDescriptor[0].Length = sizeof(IpxHeader);
//  real mode pointer to FRAME 
	ecb_ptr->FragmentDescriptor[1].Ptr = MK_FP( real_Receive_pool_DS, frame_index*sizeof(FRAME));
	ecb_ptr->FragmentDescriptor[1].Length = sizeof(FRAME);

	real_data_regs.BX.w = IPX_ListenForPacket;
	real_data_regs.ES = real_array_DS;
	real_data_regs.SI.w = ecb_index*sizeof(IpxControlBlock);
	real_data_regs.CS = Ipx_segment_call;
	real_data_regs.IP = Ipx_offset_call;
	if( !IpxCall( &real_data_regs ) )
		return real_data_regs.AX.r.l;
	else
		return -1;
*/
}

DInt IPX::IpxSendPacket( DByte ecb_index, DByte *packet, DUint socket, DUint length, DByte *adr_ptr )
{
/*
	REAL_MODE_REGISTERS real_data_regs;
	Ecb *ecb_ptr;
	IpxHeader *header_ptr;

	ecb_ptr = (Ecb *)(array+ecb_index);

	header_ptr = (IpxHeader *)( (char*)(ecb_ptr) + sizeof(Ecb) );

	header_ptr->PacketType = IPX_PACKET_TYPE;
	memcpy( header_ptr->DestinationNetwork, adr_ptr, NETWORK_ADR_LENGTH+NODE_ADR_LENGTH+2 );
	memcpy( ecb_ptr->ImmediateAddress, adr_ptr+NETWORK_ADR_LENGTH, NODE_ADR_LENGTH );

	ecb_ptr->EventServiceRoutine = NULL;
	ecb_ptr->SocketNumber = socket;
	ecb_ptr->FragmentCount = 2;
//  real mode pointer to IpxHeader
	ecb_ptr->FragmentDescriptor[0].Ptr = MK_FP( real_array_DS, FP_OFF(header_ptr) );
	ecb_ptr->FragmentDescriptor[0].Length = sizeof(IpxHeader);
//	  real mode pointer to FRAME
	ecb_ptr->FragmentDescriptor[1].Ptr = MK_FP( real_Send_pool_DS, FP_OFF(packet) );
	ecb_ptr->FragmentDescriptor[1].Length = length;

	real_data_regs.BX.w = IPX_SendPacket;
	real_data_regs.ES = real_array_DS;
	real_data_regs.SI.w = FP_OFF( ecb_ptr );
	real_data_regs.CS = Ipx_segment_call;
	real_data_regs.IP = Ipx_offset_call;
	if( !IpxCall( &real_data_regs ) )
		return 0;
	else
		return -1;
*/
}

/*
int IPX::IpxScheduleIpxEvent( void )
{
}
*/

DInt IPX::IpxRelinquishControl( void )
{
/*
	REAL_MODE_REGISTERS real_data_regs;
	real_data_regs.BX.w = IPX_RelenquishControl;
	real_data_regs.CS = Ipx_segment_call;
	real_data_regs.IP = Ipx_offset_call;
	if( !IpxCall( &real_data_regs ) )
		return 0;
	else
		return -1;
*/
}

/* virtual functions that are overloaded */
void IPX::ResetInUseField( DByte index )
{
	array[index].in_use = 0;
}

DByte IPX::TestInUseField( DByte index )
{
	return array[index].in_use;
}

DByte IPX::TestCompletion( DByte index )
{
//  IpxRelinquishControl();
	if( array[index].ecb.InUseFlag == 0 )
		return 1;
	else
		return 0;
}

void IPX::CopySourceAddress( DByte source_index, DByte cb_index )
{
	memset( station_list[source_index].hard_name, 0, 16 );
	/*  copy the following fields from the received header
	byte 		SourceNetwork[NETWORK_ADR_LENGTH];
	byte 		SourceNode[NODE_ADR_LENGTH];
	uint 		SourceSocket;
	*/
	memcpy( station_list[source_index].hard_name+4, array[cb_index].header.SourceNetwork , 12 );
}

void IPX::ResetControlBlock( DByte index )
{
	memset( &array[index], 0, sizeof(IpxControlBlock));
	array[index].in_use = 7;
}

DByte IPX::FinalResultCode( DByte index )
{
	return array[index].ecb.CompletionCode;
}

DInt IPX::SendFrame( DByte list_index )
{
/*
	SEND_FRAME_ENTRY *frame;
	DByte dest_adr[12];
	DByte status;

	frame = send_list[list_index].frame.send;

	memset( dest_adr, 0, 12 );
	if( frame->Destination == 0x0FF )
	{
		memcpy( dest_adr, station_list[panel_info1.panel_number-1].hard_name+4, 12 );
		memset( dest_adr+4, '\xFF', 6 );
	}
	else
		memcpy( dest_adr, station_list[frame->Destination-1].hard_name+4, 12 );

	status = IpxSendPacket( send_list[list_index].number, (char*)frame, socket, frame->Length+8, dest_adr );
	return status;
*/
}

DInt IPX::ReceiveFrame( DByte list_index )
{
/*
	DByte status;
	status = IpxListenForPacket( rec_list[list_index].number, list_index, socket );
	return status;
*/
}

DInt IPX::GetLocalAddress( void )
{
/*
	DInt status;
	status = IxpGetInternetworkAddress( ( IpxNetworkAddress * )
											 ( station_list[panel_info1.panel_number-1].hard_name+4 ) );
	memcpy( station_list[panel_info1.panel_number-1].hard_name+14, &socket, 2 );
	return status;
*/
}

DInt IPX::CloseCommunication( void )
{
	return IpxSpxCloseSocket( &socket );
}

DInt IPX::OpenCommunication( void )
{
	return IpxSpxOpenSocket( &socket );
}

////////////////////////////////////////////////////
#endif


