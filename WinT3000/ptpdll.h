// ************************* START OF _PTPDLL_H *************************
#ifndef _PTPDLL_H
#define _PTPDLL_H
#include "define.h"

#define UNCHANGED -1
#define DISABLE    0
#define ENABLE     1
#define CLEAR      0
#define SET        1
#define RESET      1

// size of CommInfo table
#define MAX_COMM_INFO  4

#define MAXCOMPORTS    4
#define MAXBAUDRATE    8
#define MAXMODEMTYPE   5
#define MAXCONNECTION  3

#define NETCALL_BACnetPduType     0x07
#define NETCALL_SIGN              0x08
#define NETCALL_COMPRESS          0x10
#define NETCALL_NOTCTRLBREAK      0x20
#define NETCALL_NOTTIMEOUT        0x30
#define NETCALL_NOTCHANGENETADR   0x40

#define MAXFRAMEBUFFER          490
#define MAXFRAME                (8+MAXFRAMEBUFFER+2+1)
#define MAXAPDUSIZE             (MAXFRAMEBUFFER-40)
#define MAXAPCI                 30
#define MAXASDUSIZE             (MAXAPDUSIZE-MAXAPCI)

typedef enum {
      SUCCESS_REQUEST,            // success request
      ERROR_REQUEST,              // error request
      ERROR_INSTALL_PORT,         // not enough memory to install the port
      NOT_AVAILABLE_PORT,            // to many ports installed
      ERROR_OPEN_COM_PORT,           // error initialize and open COM port
      ERROR_COM_PORT_NOT_CONFIGURED, // COM port not configured 
      PORT_NOT_INSTALLED,         // port not installed
      ERROR_ESTABLISH_CONNECTION, // connection not established
      SAME_NETWORK_NUMBER         // the remote network and the local network
                                  // have the same number number
	 } PortError;

//
// used by t3000Request.cpp
//
typedef enum {
               CONF_SERVrequest, CONF_SERVindication, CONF_SERVresponse, CONF_SERVconfirm,
			   UNCONF_SERVrequest, UNCONF_SERVindication, ERRORrequest, ERRORindication,
			   REJECTrequest, REJECTindication, SEGMENT_ACKrequest,  SEGMENT_ACKindication,
			   ABORTrequest, ABORTindication
             } ServicePrimitive;

typedef enum { APFLOWCONTROL, AEFLOWCONTROL } FlowControl;

typedef enum { BACnetConfirmedRequestPDU, BACnetUnconfirmedRequestPDU, BACnetSimpleACKPDU,
			   BACnetComplexACKPDU, BACnetSegmentACKPDU, BACnetErrorPDU, BACnetRejectPDU,
			   BACnetAbortPDU } BACnetPDU;

typedef enum { AcknowledgeAlarm, ConfirmedCOVNotification, ConfirmedEventNotification,
			   GetAlarmSummary, GetEnrollmentSummary, SubscribeCOV, AtomicReadFile, AtomicWriteFile,
			   AddListElement, RemoveListElement, CreateObject, BACDeleteObject, ReadProperty,
			   ReadPropertyConditional, ReadPropertyMultiple, WriteProperty, WritePropertyMultiple,
			   DeviceCommunicationControl, ConfirmedPrivateTransfer, ConfirmedTestMessage,
			   ReinitializeDevice, VtOpen, VtClose, VtData, Authenticate, RequestKey
			  } BACnetConfirmedServiceRequest;

typedef enum { iAm, iHave, unconfirmedCOVNotification, unconfirmedEventNotification,
			   unconfirmedPrivateTransfer, unconfirmedTestMessage, timeSyncronization,
			   whoHas, whoIs } BACnetUnconfirmedServiceChoice;

//
//
// The Settings class provides a convenient mechanism for saving or
// assigning the state of a port.
//

typedef class
{
	public:
			DLong BaudRate;       // baudrate
			DInt Parity;         // parity
			DInt WordLength;     // wordlenth
			DInt StopBits;       // stop bits
			DInt Dtr;            // dtr
			DInt Rts;            // rts
			DInt XonXoff;        // xonxoff
			DInt RtsCts;         // rtscts
			DInt DtrDsr;         // dtrdsr
    	    void Adjust( DLong baudRate = UNCHANGED,
				 DInt parity = UNCHANGED,
				 DInt wordLength = UNCHANGED,
				 DInt stopBits = UNCHANGED,
				 DInt dtr = DISABLE,
				 DInt rts = DISABLE,
				 DInt xon_xoff = DISABLE,
				 DInt rts_cts = DISABLE,
				 DInt dtr_dsr = DISABLE );
} SETTINGS;

// This structure defines the layout of the modem capability
// database elements.  At present, all of the definitions that
// can be used to define a particular brand or type of modem
// are stored in a static array in SERIALINT.CPP.  A commercial
// application might store these off line in a database.

struct ModemCapabilities {
		DByte name[41];
		DByte first_init_string[41];
		DByte second_init_string[41];
		DByte fail_strings[61];
		DByte compression_strings[41];
		DByte protocol_strings[41];
		DLong initial_baud_rate;
		DLong locked_baud_rate;
		DByte handshaking;
};

enum ModemError
			{
					MODEM_SUCCESS                 = 0,
					MODEM_NO_RESPONSE             = -100,
					MODEM_NO_RING                 = -101,
					MODEM_NO_CONNECTION           = -102,
					MODEM_DISCONNECT_FAILED       = -103,
					MODEM_RING					  = -104,
					MODEM_USER_ABORT              = -105
			};

// The Modem class definition

class Modem {
/*
	public:
	     DByte com_port;
         DByte port_number;
		 long local_baud_rate;
 	protected :
		 ModemCapabilities *modem_data;
		 int tone_dial;
		 int protocol;
		 int compressing;
		 long carrier_timeout;
		 ModemError wait_for_response( long time );
		 ModemError wait_for_connection( long time_out = 0 );
//		 virtual void echo( char c ) { mputchar( c, BLACK, WHITE ); }
		 void read_line( char *buffer, int buf_size );
		 char *scan_strings( char *buffer, char *strings );

	public :
		 Modem(DInt com, DInt port, PTP *ptp);
		 ModemError Initialize( void );
		 ModemError Answer( void );
		 ModemError Dial( char *number, int  = 0 );
		 ModemError Wait_for_ring( void );
		 ModemError Disconnect( void );
		 ModemError SendCommand( char * );
//	   int ReadRegister( int reg );
		 virtual ModemError UserAbort( void );
		 void PulseDial( void ){ tone_dial = 0; }
		 void ToneDial( void ){ tone_dial = 1; }
		 void SetCarrierTimeout( long length ){ carrier_timeout = length; }
		 char *ErrorName( ModemError status );
       virtual void DumpState( void );
		 int Protocol( void ){ return protocol; }
		 int Compressing( void ){ return compressing; }
*/
};


class
#ifdef __DLL__
_export
#endif

 CommINFO
 {
  private:
    static DInt       objectcount;
	DByte             PortName;          // 0-COM1; 1-COM2; 2-COM3; 3-COM4
	DByte	          IntName;           // not used in windows version
	DByte             connection;        // 0-none
                                         // 1-Serial connection
                                         // 2-Modem connection
	DByte             rings;
	DByte             Baudrateindex;     // 0-1200
                                         // 1-2400
                                         // 2-4800
                                         // 3-9600
                                         // 4-19200
                                         // 5-38400
                                         // 6-57600
                                         // 7-115200
	DByte             Modemtypeindex;
	HANDLE            handle;            // handle of communcation port. Set and
                                         // used internally by ptpdll.dll when
                                         // a COM port is installed.
                                         // When the port is not installed the
                                         // value is INVALID_HANDLE_VALUE=-1
    DInt              conport;           // number of connection port (index in
                                         // Routing_table). It is set and used
                                         // internally by ptpdll.dll when a
                                         // communication port is installed.
                                         // When the port is not installed the
                                         // value -1
	ModemCapabilities modemcapabilities;
 public:
	SETTINGS          Settings;
 public:
       CommINFO(void);
	   void Set( DLong baudRate  = UNCHANGED,
				 DInt parity     = UNCHANGED,
				 DInt wordLength = UNCHANGED,
				 DInt stopBits   = UNCHANGED,
				 DInt dtr        = UNCHANGED,
				 DInt rts        = UNCHANGED,
				 DInt xon_xoff   = UNCHANGED,
				 DInt rts_cts    = UNCHANGED,
				 DInt dtr_dsr    = UNCHANGED );
	   void  setmodemCapabilities(ModemCapabilities *mc);  // set modemcapabilities
	   void  getmodemCapabilities(ModemCapabilities &mc);  // set modemcapabilities
       void  modemTypeIndex(DByte mti);                 // set Modemtypeindex
       void  firstInitString(DByte *first);             // set firstinitstring
	   void  setRings( DByte r );

       DByte modemTypeIndex(void);                      // get Modemtypeindex
       DByte *firstInitString(void);                    // get firstinitstring

	   HANDLE Handle(void){ return handle;};        // get handle of communcation port
	   void Handle(HANDLE h){ handle=h;};           // set handle of communcation port

       DInt conPort(void);                             // get connection port
       void conPort(DInt cport){ conport=cport;};      // set connection port

	   DByte Connection( void );                        // get connection type
	   void Connection( DByte c );                      // set connection type

       DByte baudRateIndex(void);                       // get Baudrateindex
       DLong baudRate(void);                            // get Baudrate

       DByte portName(void){return PortName;};
 };

typedef class CommINFO COMMINFO;

#ifndef PTPDLL_DLL
 class CommINFO _import CommInfo[MAX_COMM_INFO];
 DInt  _import Station_NUM;
 DByte _import Station_NAME[17];
 DInt  _import NetworkAddress;
 DByte _import NetworkName[16];
__declspec(dllimport) DInt establishConnection(DInt comPort, DInt *networkaddress, DInt *station_num);
__declspec(dllimport) void disconnectRequest(DInt comPort);
__declspec(dllimport) DInt desinstallPort(DInt comPort);
__declspec(dllimport) DInt installPort(HWND Handle, DInt comPort);

 extern DInt t3000Request(DInt command, DInt arg,  DByte *data, DUint *length , DInt dest,
			 DInt network, DInt others=0, DInt timeout=TIMEOUT_NETCALL,
			 DByte *returnargs=NULL, DInt *length_returnargs=NULL,
             DByte *sendargs=NULL, DInt length_sendargs=0, DByte bytearg=0);
#endif

#endif  // #ifndef _PTPDLL_H

// *************************** END OF _PTPDLL_H ***************************
