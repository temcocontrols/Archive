#ifndef _RS485_H

#define _RS485_H


#include "define.h"

#define NMAXFRAMEPOOL				 1
#define PTP_NMAXRECEIVEDFRAMEPOOL    3
#define PTP_NMAXSENDFRAMEPOOL        4


#define	N_RETRIES			1
#define	T_CONN_REQ		15000		// MILI_SEC
//#define	T_CONN_REQ		   273
#define	T_CONN_RSP		15000		// MILI_SEC /
//#define	T_CONN_RSP		   273		// ticks
#define	PTP_Tframe_abort 2000		// MILI_SEC /
//#define	PTP_Tframe_abort	37
#define	T_HEARTBEAT		15000		// MILI_SEC /
//#define	T_HEARTBEAT		   273      //ticks
#define	T_INACTIVITY	60000		//* MILI_SEC /
//#define	T_INACTIVITY	   1090
#define	T_RESPONSE		5000		//* MILI_SEC /
//#define	T_RESPONSE		   91
// #define	T_SILENCE /

#define ABSENT                  -1
#define ILLEGAL                 -1
#define STSM_ILLEGAL            -1
#define CTSM_ILLEGAL            -1
#define LOCALNETWORK            -1
#define BROADCAST               -1
//#define BROADCAST           0xFFFF

#define TIMERESOLUTION    6.865686

// TS "This Station", node address
// NS "Next Station", node address
// PS "Poll Station", node address
#define NMAX_INFO_FRAMES          1
#define NMAX_MASTER              30   //127
#define NMAX_POLL                50
#define NRETRY_TOKEN              1
#define NMIN_OCTETS               4
#define TFRAME_ABORT             40
#define TFRAME_GAP               15
#define TNO_TOKEN               500
#define TPOSTDRIVE                1
#define TREPLY_DELAY            250
#define TREPLY_TIMEOUT          300    //255
#define TROFF                     1
#define TSLOT                    15    //10
#define TTURNAROUND              10
#define TUSAGE_DELAY             15
#define TUSAGE_TIMEOUT           50    //20

#define WINDOWSIZE                1

#define MAXFRAMEBUFFER          490
#define MAXFRAME                (8+MAXFRAMEBUFFER+2+1)
#define MAXAPDUSIZE             (MAXFRAMEBUFFER-40)
#define MAXAPCI                 30
#define MAXASDUSIZE             (MAXAPDUSIZE-MAXAPCI)

#define MSTP_NMAXRECEIVEDFRAMEPOOL  2
#define MSTP_NMAXSENDFRAMEPOOL      4        

#define MAXServerTSMTable           5
#define MAXClientTSMTable           4

typedef enum { HEADER_FrameType, HEADER_Destination, HEADER_Source,
			   HEADER_Length1, HEADER_Length2, HEADER_HeaderCRC
			 } HEADERStateEnum;

typedef enum {CLIENT, SERVER} ServiceSource;

typedef enum {DL_UNITDATArequest, DL_UNITDATAindication} DATALINKPrimitive;
typedef enum {N_UNITDATArequest, N_UNITDATAindication} NETWORKPrimitive;

typedef enum {NORMALmessage, URGENTmessage, CRITICALEQUIPTMENTmessage, LIFESAFETYmessage } NetworkPriority;

typedef enum { acknowledgeAlarm, confirmedCOVNotification, confirmedEventNotification,
			   getAlarmSummary, getEnrollmentSummary, subscribeCOV, atomicReadFile, atomicWriteFile,
			   addListElement, removeListElement, createObject, deleteObject, readProperty,
			   readPropertyConditional, readPropertyMultiple, writeProperty, writePropertyMultiple,
			   deviceCommunicationControl, confirmedPrivateTransfer, confirmedTestMessage,
			   reinitializeDevice, vtOpen, vtClose, vtData, authenticate, requestKey
			 } BACnetConfirmedServiceChoice;

typedef enum { IAm, IHave, UnconfirmedCOVNotification, UnconfirmedEventNotification,
			   UnconfirmedPrivateTransfer, UnconfirmedTestMessage, TimeSyncronization,
			   WhoHas, WhoIs } BACnetUnconfirmedServiceRequest;

typedef enum {ANALOG_INPUT, ANALOG_OUTPUT, ANALOG_VALUE,
			  BINARY_INPUT, BINARY_OUTPUT, BINARY_VALUE,
			  CALENDAR, CLASS, COMMAND, DEVICE, EVENT_ENROLLMENT,
			  BAC_FILE, GROUP, LOOP, MULTISTATE_INPUT, MULTISTATE_OUTPUT,
			  BAC_PROGRAM, SCHEDULE, USER_DEFINED=64 } BACnetObjectType;

typedef enum {OPERATIONAL, OPERATIONAL_READ_ONLY, DOWNLOAD_REQUIRED,
			  DOWNLOAD_IN_PROGRESS, NON_OPERATIONAL} BACnetDeviceStatus;

typedef enum {} BACnetServicesSupported;
typedef enum {} BACnetObjectTypesSupported;

typedef enum {SEGMENTED_BOTH, SEGMENTED_TRANSMIT, SEGMENTED_RECEIVE, NO_SEGMENTATION} BACnetSegmentation;

typedef enum {OTHER, BUFFEROVERFLOW, INVALIDAPDUINTHISSTATE, PREEMTEDBYHIGHERPRIORITYTASK,
			  SEGMENTATIONNOTSUPPORTED } BACnetAbortReason;


typedef struct {
		DUint instance              :14;
		DUint object_type           :9;
		DUint proprietary  :1;  // 0=standard; 1=proprietary
	} BACnetObjectIdentifier;

typedef struct {
	Byte hour;
	Byte minute;
	Byte second;
	Byte hundredths;
} Time;

typedef struct {
	Byte year;
	Byte month;
	Byte dayofmonth;
	Byte dayofweek;
} Date;

typedef struct {
	} BACnetAddress;

typedef union {
	  BACnetObjectIdentifier device;
	  BACnetAddress          address;
	} BACnetRecipient;

typedef struct {
	} BACnetRecipients;

typedef struct {
	} BACnetAddressBinding;

typedef struct {
	BACnetObjectIdentifier     Object_Identifier;
	Byte                       Object_Name[22];
	BACnetObjectType           Object_Type;
	BACnetDeviceStatus         System_Status;
	Byte                       Vendor_Name[16];
	DUint               Vendor_Identifier;
	Byte                       Model_Name[8];
	Byte                       Firmware_Revision[8];
	Byte                       Application_Software_Version[21];
	Byte						      Location[21];
	Byte						      Description[21];
	Byte                       Protocol_Version;
	Byte                       Protocol_Conformance_Class;
	BACnetServicesSupported     Protocol_Services_Supported;
	BACnetObjectTypesSupported  ProtocolObjectTypesSupported;
//	BACnetObjectIdentifier    Object_List[N];
	DUint					Max_APDU_Length_Supported;
	BACnetSegmentation         Segmentation_Supported;
//	Time                       Local_Time;
//	Date                       Local_Date;
	DInt                        UTC_Offset;
	Byte                       Daylight_Savings_Status;
	DUint               APDU_Segment_Timeout;
	DUint               APDU_Timeout;
	Byte                       Number_of_APDU_Retries;
	BACnetRecipients           Time_Syncronizaton_Recipients;
	DUint               Max_Master;
	DUint               Max_Info_Frames;
	BACnetAddressBinding       Device_Address_Binding;
 } BACnetDevice;


typedef enum { Token=0, PollForMaster, ReplyToPollForMaster, TestRequest, TestResponse,
				BACnetDataExpectingReply, BACnetDataNotExpectingReply, ReplyPostponed
			 } FrameTypeEnum;

typedef enum { MSTP_MASTER_IDLE, MSTP_MASTER_USE_TOKEN, MSTP_MASTER_WAIT_FOR_REPLY,
			   MSTP_MASTER_DONE_WITH_TOKEN, MSTP_MASTER_PASS_TOKEN, MSTP_MASTER_NO_TOKEN,
			   MSTP_MASTER_POLL_FOR_MASTER, MSTP_MASTER_ANSWER_DATA_REQUEST } MSTP_MASTERStateEnum;

typedef enum { CTSM_IDLE, CTSM_SEGMENTING_REQUEST, CTSM_AWAIT_CONFIRMATION,
			   CTSM_AE_SEGMENTED_CONFIRMATION }	ClientTSMStateEnum;

typedef enum { STSM_IDLE, STSM_SEGMENTING_REQUEST, STSM_AWAIT_RESPONSE, STSM_SEGMENTING_RESPONSE
			 }	ServerTSMStateEnum;

typedef struct {
	  DUint Preamble;
	  Byte FrameType;
	  Byte Destination;
	  Byte Source;
	  DUint Length;
	  Byte HeaderCRC;
	  Byte Buffer[MAXFRAMEBUFFER+2+1];  //2-CRC , 1-'FF'
	 } FRAME;

typedef struct
{
	DInt  				service;
	DInt 				network;
	DInt 				destination;
	DInt 				source;
	Byte 				*apci;
	DUint               length_apci;
	Byte 				*asdu;
	Byte 				*npdu;
	DUint               length;
	DInt 				port;
	Byte 				*semaphore;
	DUint priority						:2;
	DUint data_expecting_reply		    :1;
	DUint clientserver       		    :1;
	DUint AtoN							:1;
	DUint DtoN							:1;
	DUint NtoA							:1;
	DUint NtoD							:1;
} NL_PARAMETERS;

struct CTSMTable {
	 signed char     state;
	 DInt            task;
	 DInt  		     network;
	 DInt  		     destination;
	 DInt  		     source;
	 Byte invokeID;
//	 Byte 		     noseq;
//	 Byte 		     retrycount;
//	 DInt  		     timeout;
	 Byte            data[MAXFRAME];    // read data parameters
	 DUint			 LengthReceivedClientAPDU;
//	 unsigned      length;              //length of data to be send
//	 unsigned 	   last_length;         //length of last segment(s)
//	 Byte     	   noseg;               //no of segs received
//	 Byte     	   windowsize;
//	 DInt      	   entitysize;
//	 Byte     	   vendorID;
//	 Byte     	   command;
//	 DInt      	   arg;
//	 Byte     	   service;
//	 Byte          compressed;
} ;

class ClientTSMTable {
 public:
	struct CTSMTable table[MAXClientTSMTable];
 public:
	ClientTSMTable(void);
	DInt newentry(DInt task, DInt n, DInt d, DInt s, DInt id);
	void received(DInt s, DInt d, Byte *data, DUint length);
	void free( DInt entry=-1 );
};

#ifdef RS485
ClientTSMTable::ClientTSMTable(void)
{
 for(DInt i=0; i<MAXClientTSMTable; i++)
 {
	table[i].state = CTSM_ILLEGAL;
	table[i].task  = CTSM_ILLEGAL;
	table[i].LengthReceivedClientAPDU=0;
 }
}

DInt ClientTSMTable::newentry(DInt task, DInt n, DInt d, DInt s, DInt id)
{
 DInt i;
 for(i=0; i<MAXClientTSMTable; i++)
 {
	if(table[i].state == CTSM_ILLEGAL) break;
 }
 if (i==MAXClientTSMTable) return -1;
 table[i].task = task;
 table[i].network = n;
 table[i].destination = d;
 table[i].source = s;
 table[i].invokeID = id;
 table[i].state = 0;
 table[i].LengthReceivedClientAPDU=0;
// table[i].noseq = 1;
// table[i].noseg = 0;
// table[i].length = 0;
// table[i].last_length = 0;
// table[i].windowsize  = WINDOWSIZE;
// table[i].compressed = 0;
// table[i].retrycount = 0;
 return i;
}

void ClientTSMTable::received(DInt s, DInt d, Byte *data, DUint length)
{
	 DInt j;
	 for(j=0; j<MAXClientTSMTable; j++)
	 {
				if(	!table[j].state )
				{
					if( s==table[j].source && d==table[j].destination )
					{
						memcpy(table[j].data, data, length);
						table[j].LengthReceivedClientAPDU = length;
						if( table[j].data[1]==table[j].invokeID )
						{
							table[j].state = 1;
//							if( table[j].task >= 0 )
//								resume(table[j].task);
            }
						break;
					}
				}
	 }
}

void ClientTSMTable::free( DInt entry )
{
 if(entry>=0 && entry<MAXClientTSMTable )
 {
	table[entry].state = CTSM_ILLEGAL;
	table[entry].task  = CTSM_ILLEGAL;
 }
}
#endif

#ifdef RS485
//Byte sawtoken;
Byte  replyflag;
//DInt  LengthReceivedClientAPDU;
//Byte *PTRReceivedClientAPDU;
//unsigned DInt timerunMSTP=0xffff;

//DInt  		NetworkAddress=1;
Byte 		VendorID=1;
Byte 		RS485_RECEIVEFRAMEswitchflag;
//unsigned Timebetweentoken;
DInt        ServerTSM_flag=-1;

//Byte     MSTP_Preamble1;
//Byte     MSTP_Preamble2;
//Byte 		MSTP_HeaderCRC;
//Byte 		MSTP_FrameType;
//Byte 		MSTP_Destination;
//Byte 		MSTP_Source;
//unsigned MSTP_DataCRC;
//unsigned MSTP_DataLength;

//unsigned IndexBuffer;
//Byte 		Buffer[MSTP_MAXFRAMEBUFFER];
//Byte 		FrameCount;
//Byte     ReceiveError;
//DInt      EventCount;
//unsigned SilenceTimer;
//Byte     UsedToken;
//unsigned MSTP_EventCount;
//Byte 		NS; 	// Next Station
//Byte 		PS;    // Poll Station
  Byte 		TS;    // This Station
//Byte 		ReceivedInvalidFrame;
//Byte 		ReceivedValidFrame;
//Byte 		RetryCount;
//Byte 		SoleMaster;
//Byte 		TokenCount;

//FRAME 	frame={0x55FF};

Byte 	Nmax_info_frames      = NMAX_INFO_FRAMES;
Byte	Nmax_master           = NMAX_MASTER;
Byte	NPoll                 = NMAX_POLL;
Byte 	Nretry_token          = NRETRY_TOKEN;
Byte 	Nmin_octets           = NMIN_OCTETS;
DInt  	Tframe_abort          = TFRAME_ABORT/TIMERESOLUTION;
Byte 	Tframe_gap            = TFRAME_GAP;
DInt	Tno_token             = TNO_TOKEN/TIMERESOLUTION;
DInt 	Tpostdrive            = TPOSTDRIVE;
DInt 	Treply_delay          = TREPLY_DELAY;
DInt 	Treply_timeout        = TREPLY_TIMEOUT/TIMERESOLUTION;
DInt 	Troff                 = TROFF;
float	Tslot                 = TSLOT/TIMERESOLUTION;;
DInt 	Tturnaround           = TTURNAROUND/TIMERESOLUTION;
DInt 	Tusage_delay          = TUSAGE_DELAY;
DInt 	Tusage_timeout        = TUSAGE_TIMEOUT/TIMERESOLUTION;
DInt 	T_Inactivity          = T_INACTIVITY;

//HEADERStateEnum               MSTP_HEADERState;
//ReceiveFrameStatusEnum        MSTP_ReceiveFrameStatus=RECEIVE_FRAME_IDLE;
//MSTP_MASTERStateEnum          MSTP_MASTERState=MSTP_MASTER_IDLE;
ClientTSMStateEnum            CTSM_State;

DUint sendframepool_flag;
//Byte disable_485transmitter;
//Byte FirstToken;
DInt  laststation_connected=-1;
Byte ready_for_descriptors=0x01|0x04;   //0x01 read_des
													 //0x02 ; 0x04 start_read_des
//unsigned free_pool_index;

//Byte 		MSTP_SendFrameType;
//Byte 		MSTP_SendDestination;
//Byte 		MSTP_SendSource;
//Byte ClientSA, ClientDA;
//Byte validInt=0;
 Byte netpoIntsflag=0;
 DInt timepoInts=400;
//DInt timeonnet, mstpnotrun=10000;
//DInt time_run_MSTP = 6000;

 ClientTSMTable ClientTSMTable;
//uInt number_of_remote_poInts;

//Long lost_connection;
 Byte reject;
//Byte disabletransmitter;
 Byte STSMremoveflag;
//Header_pool *ptr_h;
 DInt dtr;
 Byte wantpoIntsentry;

#else
extern Byte RS485_RECEIVEFRAMEswitchflag;

//extern Byte     	MSTP_Preamble1;
//extern Byte     	MSTP_Preamble2;
//extern Byte 		MSTP_HeaderCRC;
//extern Byte 		MSTP_FrameType;
//extern Byte 		MSTP_Destination;
//extern Byte 		MSTP_Source;
//extern unsigned   MSTP_DataCRC;
//extern unsigned   MSTP_DataLength;
//extern unsigned DInt IndexBuffer;
//extern Byte Buffer[MSTP_MAXFRAMEBUFFER];
//extern Byte       ReceiveError;
//extern DInt        EventCount;
//extern unsigned   SilenceTimer;
//extern unsigned   MSTP_EventCount;
//extern Byte 		MSTP_FrameCount;
//extern Byte NS; 	// Next Station
//extern Byte PS;    // Poll Station
	extern Byte TS;    // This Station
//extern Byte ReceivedInvalidFrame;
//extern Byte ReceivedValidFrame;
//extern Byte RetryCount;
//extern Byte SoleMaster;
//extern Byte TokenCount;

//extern FRAME frame;

extern Byte Nmax_info_frames;
extern Byte NPoll;
extern Byte Nmax_master;
extern Byte Nretry_token;
extern Byte Nmin_octets;
extern DInt  Tframe_abort;
extern Byte Tframe_gap;
extern DInt Tno_token;
extern DInt Tpostdrive;
extern DInt Treply_delay;
extern DInt Treply_timeout;
extern DInt Troff;
extern DInt Tslot;
extern DInt Tturnaround;
extern DInt Tusage_delay;
extern DInt Tusage_timeout;
extern DInt T_Inactivity;

//extern HEADERStateEnum               MSTP_HEADERState;
//extern ReceiveFrameStatusEnum        MSTP_ReceiveFrameStatus;
//extern MSTP_MASTERStateEnum          MSTP_MASTERState;
extern ClientTSMStateEnum            CTSM_State;
extern Byte disable_485transmitter;
extern Byte ready_for_descriptors;
//extern unsigned free_pool_index;
#endif

/*
class RECEIVEDFRAMEPOOL
{
 protected:
	signed Byte TailFrame;
	signed Byte HeadFrame;
	Byte overflow;
	Byte lockedhead;
 public:
	RECEIVEDFRAMEPOOL(void ){ TailFrame=HeadFrame=lockedhead=overflow=0;}
	void Clear(void){TailFrame=HeadFrame=lockedhead=overflow=0;}
	inline void Unlockhead(void){lockedhead=0;}
//	virtual DInt NextFreeEntry(void)=0;
};
*/
/*
struct MSTP_ReceivedFrame {
	  FRAME Frame;
	  Byte  ReceivedValidFrame;
	  Byte  ReceivedInvalidFrame;
	  Byte  status;
};


class MSTP_RECEIVEDFRAMEPOOL
{
 public:
//	Byte  entry;
	struct MSTP_ReceivedFrame ReceivedFrame[MSTP_NMAXRECEIVEDFRAMEPOOL];
 public:
	MSTP_RECEIVEDFRAMEPOOL(void);
//	DInt NextFreeEntry(void);
	void *NextFreeEntry(void);
	DInt  RemoveEntry(MSTP_ReceivedFrame *frame);
};
*/

class SENDFRAMEPOOL
{
 protected:
	signed char TailFrame;
	signed char HeadFrame;
	Byte overflow;
	Byte lockedhead;
	Byte access;

 public:
	SENDFRAMEPOOL(void) { TailFrame=HeadFrame=lockedhead=overflow=access=0;}
	void Clear(void){TailFrame=HeadFrame=lockedhead=overflow=access=0;}
	inline void Unlockhead(void){lockedhead=0;}
	DInt NextFreeEntry(void);
};

/*
class MSTP_SENDFRAMEPOOL
{
 public:
	FRAME Frame[MSTP_NMAXSENDFRAMEPOOL];
	Byte  status[MSTP_NMAXSENDFRAMEPOOL];
	Byte  entry, access;
 public:
	MSTP_SENDFRAMEPOOL(void);
	DInt NextFreeEntry(void);
	DInt RemoveEntry(FRAME *buffer);
};

struct TSMTable {
	 signed char   state;
	 DInt  		   network;
	 DInt  		   destination;
	 DInt  		   source;
	 Byte        invokeID;
	 Byte 		   noseq;
	 Byte 		   retrycount;
	 DInt  		   timeout;
	 DUint      pool_index;
	 POOL          *ser_pool;
	 Byte          read_data[MAXASDUSIZE];    // read data parameters
	 Byte          *data;               	   // data
	 DUint      length;              //length of data to be send
	 DUint 	   last_length;         //length of last segment(s)
	 Byte     	   noseg;               //no of segs received
	 Byte     	   windowsize;
	 DInt      	   entitysize;
	 Byte     	   vendorID;
	 Byte     	   command;
	 DInt      	   arg;
	 Byte     	   service;
    Byte          compressed;
  } ;

class ServerTSMTable {
 public:
  struct TSMTable table[MAXServerTSMTable];
 public:
  ServerTSMTable(void);
  DInt lookid(DInt n, DInt d, DInt s, DInt id, POOL *spool);
  void free( DInt entry=-1, DInt n=0, DInt d=0, DInt s=0, DInt id=0);
  void resetsegments(DInt entry);
};
*/

#ifdef RS485

/*
ServerTSMTable::ServerTSMTable(void)
{
 for(DInt i=0; i<MAXServerTSMTable; i++)
  table[i].state = STSM_ILLEGAL;
}

void ServerTSMTable::resetsegments(DInt entry)
{
  table[entry].length += table[entry].last_length;
  table[entry].data -= table[entry].last_length;
  table[entry].noseq -= table[entry].windowsize;
  table[entry].last_length = 0;
  table[entry].noseg = 0;
}

DInt ServerTSMTable::lookid(DInt n, DInt d, DInt s, DInt id, POOL *spool)
{
}

void ServerTSMTable::free( DInt entry, DInt n, DInt d, DInt s, DInt id)
{
}
*/

#endif

//class Serial;

/*
class MSTP             //: public Serial, public ConnectionData
{
	public:
		Byte FirstToken;
//		unsigned Timebetweentoken;
		DInt      ServerTSM_flag;
		Byte 		NS; 	// Next Station
		Byte 		PS;    // Poll Station
//		Byte 		TS;    // This Station
		Byte 		RetryCount;
		Byte 		SoleMaster;
		Byte 		TokenCount;
		Byte 		FrameCount;
		Byte     UsedToken;
		Byte task;
		MSTP_SENDFRAMEPOOL            SendFramePool;
		MSTP_RECEIVEDFRAMEPOOL        ReceivedFramePool;
		HEADERStateEnum               MSTP_HEADERState;
		ReceiveFrameStatusEnum        MSTP_ReceiveFrameStatus;
//		MSTP_MASTERStateEnum          MSTP_MASTERState;
	private:
		Byte connection_state;

	public:
		MSTP( DInt c_port, DInt nr_port );
		void MSTP_Master_initialize(void);
		void sendpoInts(Byte *asdu, Byte type);     //0 want_poInts, 1 network_poInts
		void sendinfo(FRAME *frame, DInt status, DInt panel, DInt dest=255 );
		void SendFrame(FRAME *frame);
//		static DInt MSTP_receive_frame( MSTP *mstp );
		static DInt MSTP_Master_node( MSTP *mstp );
};
*/

#ifdef RS485
//MSTP_SENDFRAMEPOOL     SendFramePool;
//MSTP_RECEIVEDFRAMEPOOL ReceivedFramePool;
Byte ClientBuffer[MAXFRAME];
//Byte ServerBuffer[MAXFRAME];
//class ServerTSMTable ServerTSMTable;
#else
//extern MSTP_RECEIVEDFRAMEPOOL RECEIVEDFramePool;
//extern MSTP_SENDFRAMEPOOL     SendFramePool;
extern Byte ClientBuffer[MAXFRAME];
//extern ServerTSMTable ServerTSMTable;
#endif

#endif



