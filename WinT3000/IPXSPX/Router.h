//---------------------------------------------------------------------------
#ifndef routerH
#define routerH
#define _ROUTER_H
//---------------------------------------------------------------------------

#define ESTABLISH_CONNECTION_NET  90
#define ESTABLISH_CONNECTION_PORT 91

#define   PORT_FREE        0x00
#define   PORT_ACTIVE      0x03
#define   PTP_INSTALLED    0x05
#define   PTP_ACTIVE       0x07
#define   RS485_INSTALLED  0x11
#define   RS485_ACTIVE     0x13
#define   IPX_INSTALLED    0x21
#define   IPX_ACTIVE       0x23
#define   TCPIP_INSTALLED  0x41
#define   TCPIP_ACTIVE     0x43

#define UNREACHABIL            1
#define TEMPORARY_UNREACHABIL  2
#define REACHABIL              3

#define MAX_reachable_networks 5
#define MAX_Routing_table      2
#define MAX_NetSession         5


typedef enum {
	  Who_Is_Router_To_Network, I_Am_Router_To_Network, I_Could_Be_Router_To_Network,
	  Reject_Message_To_Network, Router_Busy_To_Network, Router_Available_To_Network,
	  Initialize_Routing_Table, Initialize_Routing_Table_Ack,
	  Establish_Connection_To_Network, Disconnect_Connection_To_Network,
	  I_Am_Router_To_Network_Prop=0x80
	 } Network_Layer_Message_Type;


struct reachable_networks {
		 DInt   network;
		 DByte  router_address;
		 DByte  half_router_address;
		 DByte  status;
	  };

struct Port {
	  DByte address;
	  DInt  network;
	  struct reachable_networks networks_list[MAX_reachable_networks];
	};


typedef struct {
	DUint ack0received				:1;
	DUint ack1received				:1;
	DUint nak0received				:1;
	DUint nak1received				:1;
	DUint reception_blocked			:2;
	DUint transmission_blocked		:1;
	DUint sending_frame_now			:1;
	DUint send_done					:1;
	DUint send_ok					:1;
	DUint TxSequence_number			:1;
	DUint RxSequence_number			:1;
	DUint receive_error				:1;
	}	PTP_COMMUNICATION_flags;

typedef struct {
	  DUint   					SilenceTimer;
	  DUint 					InactivityTimer;   //Timebetweentoken
	  DUint 					HeartbeatTimer;    //timerunMSTP
	  DInt      				EventCount;
	  DSignedChar     			ReceiveError;
	  PTP_COMMUNICATION_flags 	PTP_comm_status;
	  DByte                     MSTP_MASTERState;
	  DByte     				Preamble1;
	  DByte     				Preamble2;
	  DByte 	  				HeaderCRC;
	  DUint   					DataCRC;
	  DInt      				Length;
	  DByte 	  				Destination;
	  DByte 	  				Source;
	  DByte     				FrameType;
	  DByte         	        HEADERState;         //HEADERStateEnum
	  DByte  	                ReceiveFrameStatus;  //ReceiveFrameStatusEnum
	  DInt      				task;
	  DByte                     ring;
	  DByte     				connection;
	  DByte    			        validint;
	  DByte                     mode;
      DByte                     physical_connection_state;

      DInt                      head_Data;
      DInt                      tail_Data;
      DInt                      overflow;
      DInt                      RxDelayTime;
      DByte                     ComData[MAXDATA];
	} PORT_STATUS_variables;

// status :   0  - free
//            0x01 Port_installed  ( 0000 0101 )
//            0x03 Port active     ( 0000 0101 )
//            0x05 PTP installed   ( 0000 0101 )
//            0x07 PTP active      ( 0000 0111 )
//            0x11 RS485 installed { 0001 0001 )
//            0x13 RS485 active    { 0001 0011 )
//            ETHERNET             { 0010 0011 )

typedef struct {
		DByte 		status;
		struct Port Port;
		void 		*ptr;
		DInt        task;
		PORT_STATUS_variables port_status_vars;
	} ROUTING_TABLE_T3000;

/*
typedef struct {
		DByte 		status;
		DByte       service;
		DByte       source;
		DByte       dest;
		DInt        network;
		DSignedChar *semaphore;
		DByte       task;
		DByte       *data;
		DInt        id;
		DInt        timeout;
	} NetworkSession ;
*/

typedef struct
{
	DInt 		 SNET;
	DByte		 SLEN;
	DByte     SADR[6];
	DInt 		 DNET;
	DByte		 DLEN;
	DByte     DADR[6];
	DByte    termination_time_value;
	DInt          len;
	DByte       *data;
} ROUTER_PARAMETERS;

#endif
