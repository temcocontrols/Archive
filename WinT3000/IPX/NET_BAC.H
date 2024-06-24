#ifndef _NET_BAC_H
#define _NET_BAC_H

//#include "netbios.h"
#include "baseclas.h"
#include "rs485.h"
//#include "ptp.h"
//#include "serial.h"
#include "aio.h"

#define NET_NMAXSENDFRAME       4
#define NET_NMAXRECEIVEFRAME   4

typedef enum { SENDING_DG, SENT_DG, SEND_ERROR, IDLE_SD } SENDING_DG_STATUS;

typedef enum { WAITING_DG, RECEIVED_DG, REC_ERROR, IDLE_DG } DATAGRAM_STATUS;


typedef enum { DRIVER_INSTALLED, DRIVER_NOT_INSTALLED } Driver_Status;

typedef struct  {
		FRAME Frame;
		DByte ReceivedValidFrame;
		DByte ReceivedInvalidFrame;
		DByte status;
} NET_RECEIEVE_FRAME;

class NET_RECEIVEDFRAMEPOOL
{
	public:
		NET_RECEIEVE_FRAME *ReceivedFrame;
	public:
		void Init_frame_pool( NET_RECEIEVE_FRAME *buffer );
		void *NextFreeEntry(void);
		DInt  RemoveEntry( NET_RECEIEVE_FRAME *frame);
};

/*
typedef struct {
		unsigned DInt Preamble;
		DByte FrameType;
		DByte Destination;
		DByte Source;
		unsigned DInt Length;
		DByte HeaderCRC;
		char Buffer[MAXFRAMEBUFFER+2+1];  //2-CRC , 1-'FF'
	 } FRAME;
*/
typedef struct {
	DUint Preamble;
	DByte FrameType;
	DByte Destination;
	DByte Source;
	DUint Length;
	DByte HeaderCRC;
	DByte Buffer[MAXFRAMEBUFFER];
	DUint DataCRC;
//  DByte Terminator;
	DByte being_used    : 1;
	DByte locked        : 1;
	DByte reply_flag    : 1;
	DByte number        : 2;
	DByte unused_bits   : 2;
	DByte sending       : 1;
} SEND_FRAME_ENTRY;


class NET_SENDFRAMEPOOL
{
	DByte no_of_used_frames;
	public:
		SEND_FRAME_ENTRY *Frame;
	public:
		void Init_frame_pool( SEND_FRAME_ENTRY *buffer );
		SEND_FRAME_ENTRY *NextFreeEntry( DInt reply );
		SEND_FRAME_ENTRY *FrameAvailable( DInt reply );
		void RemoveSentEntry( SEND_FRAME_ENTRY *frame_ptr );
		void Unlock_frame( SEND_FRAME_ENTRY *frame );
		DInt  FramesInUse( void );
};

typedef struct
{
		signed char  number;
		DByte  state;
		DByte  type;
		union {
		FRAME *rec;
		SEND_FRAME_ENTRY *send;
		} frame;

} Command_State;

typedef struct
{
	DByte l;
	DByte h;
} REGISTER_2x8;

typedef union
{
	REGISTER_2x8  r;
	DUint  w;
	DUlong e;
} REGISTER_DATA;

typedef struct
{
	REGISTER_DATA   DI;
	REGISTER_DATA   SI;
	REGISTER_DATA   BP;
	REGISTER_DATA   RES;
	REGISTER_DATA   BX;
	REGISTER_DATA   DX;
	REGISTER_DATA   CX;
	REGISTER_DATA   AX;
	DUint    flags;
	DUint    ES;
	DUint    DS;
	DUint    FS;
	DUint    GS;
	DUint    IP;
	DUint    CS;
	DUint    SP;
	DUint    SS;

	} REAL_MODE_REGISTERS;

class NET_BAC : public ConnectionData
{
	protected:

    DByte                end_task;

		Command_State     	send_list[NET_NMAXSENDFRAME];
		Command_State     	rec_list[NET_NMAXRECEIVEFRAME];
		DByte            		rec_comm_active;
		DByte            		rec_br_comm_active;

		DByte                ControlBlocksQueueSize;
		DByte    		        available_CBs;
		DByte                last_assigned_CB;

		DUint 				real_Send_pool_DS;
		DUint 				real_Receive_pool_DS;

	public:

		Driver_Status       state;
		T3000_ERROR			 		comm_status;
		char 					 			*data;

		NET_SENDFRAMEPOOL    SendFramePool;
		FRAME 				 *ReceiveFramePool;

		DByte                 activity;
		DByte                 routing_entry;
		DUlong        time;

	protected:

		void InitNetData( Panel_info1 *info, DByte task_no );
		void ReleaseRealMemory( void );
		DInt Get_ControlBlock( void );
		void Release_ControlBlock( DByte index );

		virtual void ResetInUseField( DByte index ) = 0;
		virtual DByte TestInUseField( DByte index ) = 0;
		virtual DByte TestCompletion( DByte index ) = 0;
		virtual void CopySourceAddress( DByte source_index, DByte cb_index ) = 0;
		virtual void ResetControlBlock( DByte index ) = 0;
		virtual DByte FinalResultCode( DByte index ) = 0;

		virtual DInt TestDriverPresence( void ) = 0;

		virtual DInt SendFrame( DByte list_index ) = 0;
		virtual DInt ReceiveFrame( DByte list_index ) = 0;

public:

		virtual DInt OpenCommunication( void ) = 0;
		virtual DInt CloseCommunication( void ) = 0;
		virtual DInt GetLocalAddress( void ) = 0;
		DInt Sendinfo( DInt status, DInt panel, DInt dest=255 );
		void SignOff( void );
		DInt CheckForCommandCompletion( void );
		void NetControl( void );
};

#endif //_NET_BAC_H


