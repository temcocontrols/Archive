#ifndef _IPX_H
#define _IPX_H

//#include "net_bac.h"

#define IPX_SPX_OpenSocket           0x00
#define IPX_SPX_CloseSocket          0x01
#define IPX_GetLocalTarget           0x02
#define IPX_SendPacket               0x03
#define IPX_ListenForPacket          0x04
#define IPX_Schedule_IPX_Event       0x05
#define IPX_CancelEvent              0x06
#define IPX_GetIntervalMarker        0x08
#define IPX_GetInternetworkAddress   0x09
#define IPX_RelenquishControl        0x0A
#define IPX_DisconnectFromTarget     0x0B

#define NODE_ADR_LENGTH       6
#define NETWORK_ADR_LENGTH    4

#define IPX_PACKET_TYPE       4

#define DOS_INT            0x21
#define DOS_MULTIPLEX      0x2F
#define IPX_SPX_INSTALLED  0xFF

#define MAX_NAME_SIZE        47
#define MAX_FRAGMENT_COUNT    2
#define IPX_MAX_CONNECTIONS 250

#define  USER_BINDERY_OBJECT_TYPE 0x0001

#define LONG_LIVED_SOCKET  0x0FF
#define SHORT_LIVED_SOCKET 000

typedef struct
{
	DByte NetworkNumber[NETWORK_ADR_LENGTH];
	DByte NodeAddress[NODE_ADR_LENGTH];

} IpxNetworkAddress;

typedef struct
{
	DByte NetworkNumber[NETWORK_ADR_LENGTH];
	DByte NodeAddress[NODE_ADR_LENGTH];
	DUint  Socket;

} TargetAddress;

typedef struct
{
	DByte Data[4];

} IpxArea;

typedef struct
{
	DUint ConnectionId;
	DByte Unused[2];

} SpxArea;

typedef struct
{
	void far *Ptr;                   /* fill these in */
	DUint     Length;                /* fill these in */

} FragmentDescriptorStruct;

typedef struct
{
	void far *LinkAddress;
	void (far *EventServiceRoutine)();           /* fill this in  */
	DByte     InUseFlag;
	DByte     CompletionCode;
	DUint     SocketNumber;                       /* fill this in  */

	DByte     WorkSpace[4];

	DByte     DriverWorkSpace[12];
	DByte     ImmediateAddress[NODE_ADR_LENGTH];  /* IPX send ==> fill in */
	DUint     FragmentCount;                      /* fill this in  */

	FragmentDescriptorStruct FragmentDescriptor[MAX_FRAGMENT_COUNT];

}	Ecb;

typedef struct
{
	DUint 		CheckSum;
	DUint 		Length;
	DByte 		TransportControl;
	DByte 		PacketType;                /* IPX send ==> fill this in */
	DByte 		DestinationNetwork[NETWORK_ADR_LENGTH]; /* fill this in */
	DByte 		DestinationNode[NODE_ADR_LENGTH];       /* fill this in */
	DUint 		DestinationSocket;                      /* fill this in */
	DByte 		SourceNetwork[NETWORK_ADR_LENGTH];
	DByte 		SourceNode[NODE_ADR_LENGTH];
	DUint 		SourceSocket;

}	IpxHeader;

typedef struct
{
	Ecb					ecb;
	IpxHeader  	header;

	DUint in_use    : 4;      /* used to indicate if the CB is used or not
																			== 0 => not used, != 0 => in use */
	DUint index     : 3;     /* command associated with this CB */
	DUint type      : 1;     /* command type associated with this CB */

} IpxControlBlock;


class IPX : public NET_BAC
{
		IpxControlBlock	*array;
		DUint 	socket;

		DUint real_array_DS;
		DUint Ipx_segment_call;
		DUint Ipx_offset_call;

		DInt IpxCall( REAL_MODE_REGISTERS *rm_ptr );
		DInt swab( DUint data );
		DInt IxpGetInternetworkAddress( IpxNetworkAddress *AddressStructPtr );
		DInt IpxSpxOpenSocket( DUint *SocketNumber );
		DInt IpxSpxCloseSocket( DUint *SocketNumber );
		DInt IpxGetLocalTarget( DByte *request, DByte *reply );
		DInt IpxDisconnectFromTarget( TargetAddress *target );
		DInt IpxListenForPacket( DByte ecb_index, DByte frame_index, DUint socket );
		DInt IpxSendPacket( DByte ecb_index, char *packet, DUint socket, DUint length, DByte *adr_ptr );
		DInt IpxScheduleIpxEvent( void );
		DInt IpxRelinquishControl( void );

	private:
		/* virtual functions that are overloaded */
		void ResetInUseField( DByte index );
		virtual DByte TestInUseField( DByte index );
		virtual DByte TestCompletion( DByte index );
		virtual void CopySourceAddress( DByte source_index, DByte cb_index );
		virtual void ResetControlBlock( DByte index );
		virtual DByte FinalResultCode( DByte index );

		virtual DInt TestDriverPresence( void );

		virtual DInt SendFrame( DByte list_index );
		virtual DInt ReceiveFrame( DByte list_index );

	public:

		IPX( Panel_info1 *info, DByte task_no );
		DInt GetLocalAddress( void );
		DInt CloseCommunication( void );
		DInt OpenCommunication( void );
		~IPX();
};

#endif //_IPX_H

