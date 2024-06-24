//---------------------------------------------------------------------------
#ifndef ptpH
#define ptpH
#define _PTP_H
//---------------------------------------------------------------------------

#include <vcl\system.hpp>
#include "rs485.h"

typedef enum  {	CONNECTION_INACTIVE, MODEM_ACTIVE, SERIAL_ACTIVE } Physical_Connection_State;

typedef enum {
	        	NO_ROUTE_TO_DNET = 0x01, ROUTER_BUSY, UNKNOWN_MESSAGE_TYPE, OTHER_ERROR
      	     } Router_Rejection_reasons;

typedef enum {
		      PTP_IDLE, PTP_PREAMBLE, PTP_HEADER, PTP_HEADER_CRC, PTP_DATA,
		      PTP_DATA_CRC, INBOUND, OUTBOUND, PTP_CONNECTED, PTP_DISCONNECTED, PTP_DISCONNECTING,
		      PTP_REC_TRIGGER_SEQ, PTP_SEND_TRIGGER_SEQ, MSTP_CONNECTED, MSTP_DISCONNECTED,
		      IPX_CONNECTED, IPX_DISCONNECTED, TCPIP_CONNECTED, TCPIP_DISCONNECTED,
              NETBIOS_CONNECTED, NETBIOS_DISCONNECTED
	         } PTP_States;

typedef enum { REC_IDLE, REC_READY, DATA_ACK, DATA_NAK, DATA } PTP_REC_States;

typedef enum { TR_IDLE, TR_PENDING, TR_READY, TR_BLOCKED } PTP_TR_States;

typedef enum
	{
		HEARTBEAT_XOFF = 0, HEARTBEAT_XON, DATA_0, DATA_1, DATA_ACK_0_XOFF,
		DATA_ACK_1_XOFF, DATA_ACK_0_XON, DATA_ACK_1_XON, DATA_NAK_0_XOFF,
		DATA_NAK_1_XOFF, DATA_NAK_0_XON, DATA_NAK_1_XON, CONNECT_REQUEST,
		CONNECT_RESPONSE, DISCONNECT_REQUEST, DISCONNECT_RESPONSE,
		TEST_REQUEST, TEST_RESPONSE
	} Standard_Frame_Types;

typedef enum
	{
		DL_IDLE, DL_UNITDATA_REQUEST, DL_UNITDATA_INDICATION, DL_UNITDATA_RESPONSE,
		DL_CONNECT_REQUEST, DL_CONNECT_INDICATION, DL_CONNECT_RESPONSE,
		DL_DISCONNECT_REQUEST, DL_TEST_REQUEST, DL_TEST_RESPONSE,
		DL_CONNECT_ATTEMPT_FAILED
	} DL_Primitives;

typedef enum
	{
		CONFIRMED, UNCONFIRMED, ERROR_SERV, REJECT, SEGMENT_ACK,
		ABORT_SERV, CONNECT, DISCONNECT
	} Services;

typedef enum { Q_BLOCKED, Q_ALMOST_BLOCKED, Q_NOT_BLOCKED } Blocked_States;

typedef enum
	{
		APP_REQUEST, APP_INDICATION, APP_RESPONSE, APP_CONNECT_REQUEST,
		APP_CONNECT_INDICATION, APP_CONNECT_RESPONSE, APP_DISCONNECT_REQUEST,
		APP_TEST_REQUEST, APP_TEST_RESPONSE
	} APP_Primitives;

typedef enum
	{
		APP_IDLE, APP_SEGMENTING_REQUEST, APP_SEGMENTED_REQUEST,
		APP_AWAIT_CONFIRMATION, APP_AWAIT_RESPONSE,
		APP_SEGMENTED_CONFIRMATION, APP_SEGMENTING_RESPONSE
	} APP_States;

typedef struct {
	DUint ReceivedValidFrame   	:1;
	DUint ReceivedInvalidFrame  :1;
	DUint sending_frame_now	  	:1;
	DUint send_done			  	:1;
	DUint send_ok			  	:1;
	DUint retry_count		  	:2;
	DUint in_use			  	:1;
	}	FRAME_flags;


typedef struct {
	FRAME_flags flags;
	DUint DataIndex;
	Byte FrameType;
	Byte Destination;
	Byte Source;
	DUint Length;
	Byte HeaderCRC;
	Byte Buffer[MAXFRAMEBUFFER+2+1];
	DUint DataCRC;
	} FRAME_ENTRY;

typedef struct {
	  DUint Preamble;
	  DByte FrameType;
	  DUint Length;
	  DByte HeaderCRC;
	  DByte Buffer[MAXFRAMEBUFFER+2+2];  //2-CRC , 1-'FF'
	 } PTP_FRAME;

struct PTP_ReceivedFrame {
	  PTP_FRAME   Frame;
	  FRAME_flags flags;
      Byte  status;
};

class PTP_RECEIVEDFRAMEPOOL
{
 public:
	struct PTP_ReceivedFrame ReceivedFrame[PTP_NMAXRECEIVEDFRAMEPOOL];
	PTP_RECEIVEDFRAMEPOOL(void);
	void *NextFreeEntry(void);
	DInt  RemoveEntry(void *);
   void Clear(void);
};

class PTP_SENDFRAMEPOOL:public SENDFRAMEPOOL
{
 public:
	PTP_FRAME Frame[PTP_NMAXSENDFRAMEPOOL];

	DInt RemoveEntry(PTP_FRAME *pframe);
	void AdvanceTail(void);
};

typedef struct
{
	DL_Primitives primitive;
	DInt destination_panel;
	DUint DNET;
	Byte dest_MAC_address[6];
	Byte dest_LSAP;
	DInt source_panel;
	DUint SNET;
	Byte source_MAC_address[6];
	Byte source_LSAP;
	Byte apci[10];
	DUint length_apci;
	Byte *asdu;
	DUint length_asdu;
	Byte *apdu;
	DUint length_apdu;
	Byte *npdu;
	DUint length_npdu;
	DUint priority	 				:2;
	DUint data_expecting_reply		:1;
	DUint AtoN		 				:1;
	DUint DtoN		 				:1;
	DUint NtoA		 				:1;
	DUint NtoD		 				:1;
	DUint unused	 				:1;
} UNITDATA_PARAMETERS;


struct isr_data_block {
	 DInt      uart;
//	 UARTType uart_type;
	 Byte     port_number;
	 Byte     media;
	 Byte     task;
	 volatile DInt          overflow;
	 volatile Byte          tx_running;
	 volatile DUint         rx_Int_count;
	 volatile DUint         tx_Int_count;
	 volatile DUint         ms_Int_count;
	 volatile DUint         ls_Int_count;
	 volatile DUint         line_status;
	 DUint                  handshaking;
	 volatile DUint         blocking;
	 volatile DUint         blocked;
	 volatile DInt          send_handshake_Byte;
	 volatile DUint         modem_status;
//	 Queue TXQueue;
//	 Queue RXQueue;
	 Byte                   rx_control_char;
	 Byte                   tr_index;
	 void					*recframe;
	 DInt                   length;
	 Byte                   *ptr;
};

//class
//#ifdef __DLL__
//  _export
//#endif
class PTP
{
	public:
        HWND Handle;
		UNITDATA_PARAMETERS NL_parameters;
		DUlong frame_send_timer;
		DInt  ResponseTimer;
		DByte  RetryCount;
		DUint sendframe_flag;
		DByte password_needed, password_OK;
		DByte task;
		PTP_SENDFRAMEPOOL     SendFramePool;
		PTP_RECEIVEDFRAMEPOOL ReceivedFramePool;
		DByte connection_state;
        struct isr_data_block data;
        DByte port_number;
	    DByte com_port;
        DInt  port_status;
		DInt  activity;
		DByte reception_state;
		DByte transmission_state;

	    DByte      modem_active;
		DByte      modem_present;
		Modem      *modem_obj;
		ModemError modem_status;
	private:

//		Byte physical_connection_state;

//		uInt Byte_period;
//		Byte trigger_sequence[7] = "BACnet\x0d";
//		Byte trigger_sequence[7];
//		Byte rec_trigger_sequence;
//		Long lastsend_timemicro;
//		unsigned Long  lastsend_timesec;

//		Byte reception_state;
//		Byte transmission_state;
//		Byte rec_frame_state;
//		Byte send_frame_state;
		DByte ReceptionBlocked;

	public:
		void PTP_SendFrame( PTP_FRAME *frame, DInt retry=0 );
		DInt sendframe(DInt=0 , Byte*p1 = NULL, DInt=0, Byte*p2 = NULL, DInt=0, Byte*p3 = NULL, DInt send=0);
		DInt init_idle_state(DInt m=1);
		PTP(HWND Handle, DInt com, DInt port);
};


/*
class ASYNCRON : public Serial
{
	public:
		Byte task;
      DInt  program_number;
	public:
		ASYNCRON( DInt c_port, DInt nr_port );
		static DInt ASYNCRON_task( ASYNCRON* async );
};

*/
	void crc_header( Byte octet, Byte *header_CRC );
	void crc_data( Byte octet, DUint *data_CRC );

#endif


