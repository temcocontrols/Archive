// ltg : from t3000def.h ( DOS Ver).

typedef struct {
	unsigned ack0received				:1;
	unsigned ack1received				:1;
	unsigned nak0received				:1;
	unsigned nak1received				:1;
	unsigned reception_blocked			:2;
	unsigned transmission_blocked		:1;
	unsigned sending_frame_now			:1;
	unsigned send_done					:1;
	unsigned send_ok						:1;
	unsigned TxSequence_number			:1;
	unsigned RxSequence_number			:1;
	unsigned receive_error				:1;
	}	PTP_COMMUNICATION_flags;

typedef struct {
	  unsigned 		
      			  	SilenceTimer;
	  unsigned 					  	InactivityTimer;   //Timebetweentoken
	  unsigned 					  	HeartbeatTimer;    //timerunMSTP
	  int      					  	EventCount;
	  DByte     					  	ReceiveError;
	  PTP_COMMUNICATION_flags 	PTP_comm_status;
	  char                     MSTP_MASTERState;
	  DByte     						Preamble1;
	  DByte     						Preamble2;
	  DByte 	  						HeaderCRC;
	  unsigned 						DataCRC;
	  int      						Length;
	  DByte 	  						Destination;
	  DByte 	  						Source;
	  DByte     						FrameType;
	  DByte         	         HEADERState;         //HEADERStateEnum
	  DByte  	                  ReceiveFrameStatus;  //ReceiveFrameStatusEnum
	  int      						task;
	  DByte                     ring;
	  DByte     						connection;
	  DByte    			         validint;
	  DByte                     mode;
     DByte                     physical_connection_state;
     DByte                     rings_number;         // answer after number_rings
	  long                     time_modem;
	} PORT_STATUS_variables;


//ltg:  old router.h

#ifndef _ROUTER_H
#define _ROUTER_H

#define ESTABLISH_CONNECTION_NET  90
#define ESTABLISH_CONNECTION_PORT 91

#define   ASYNCRON_INSTALLED  0x80
#define   PORT_ACTIVE      0x03
#define   PTP_INSTALLED    0x05
#define   PTP_ACTIVE       0x07
#define   RS485_INSTALLED  0x11
#define   RS485_ACTIVE     0x13

#define UNREACHABIL            1
#define TEMPORARY_UNREACHABIL  2
#define REACHABIL              3

#define MAX_reachable_networks 5
#define MAX_Routing_table      9
#define MAX_NetSession         5

typedef enum {
	  Who_Is_Router_To_Network, I_Am_Router_To_Network, I_Could_Be_Router_To_Network,
	  Reject_Message_To_Network, Router_Busy_To_Network, Router_Available_To_Network,
	  Initialize_Routing_Table, Initialize_Routing_Table_Ack,
	  Establish_Connection_To_Network, Disconnect_Connection_To_Network,
	  I_Am_Router_To_Network_Prop=0x80
	 } Network_Layer_Message_Type;


typedef struct {
		char 		status;
		char     service;
		char     source;
		char     dest;
		int      network;
		signed char *semaphore;
		char        task;
		char        *data;
		int         id;
		int         timeout;
	} NetworkSession ;

struct reachable_networks {
		 int   network;
		 DByte  router_address;
		 DByte  half_router_address;
		 DByte  status;
	  };

struct Port {
	  DByte address;
	  int  network;
	  struct reachable_networks networks_list[MAX_reachable_networks];
	};

// status :   0  - free
//            0x01 Port_installed  ( 0000 0001 )
//            0x03 Port active     ( 0000 0011 )
//            0x05 PTP installed   ( 0000 0101 )
//            0x07 PTP active      ( 0000 0111 )
//            0x11 RS485 installed { 0001 0001 )
//            0x13 RS485 active    { 0001 0011 )
//            ETHERNET             { 0010 0011 )

typedef struct {
		char 		status;
		struct 	Port Port;
		void 		*ptr;
		int      task;
		PORT_STATUS_variables port_status_vars;
	} ROUTING_TABLE;

typedef struct
{
	int 		SNET;
	char		SLEN;
	char     SADR[6];
	int 		DNET;
	char		DLEN;
	char     DADR[6];
	char     termination_time_value;
	int      len;
	char     *data;
} ROUTER_PARAMETERS;

#ifndef ROUTER
extern ROUTING_TABLE Routing_table[MAX_Routing_table];  
                         // ltg :  In LTGDate.cpp
extern char trigger_sequence[];

extern int router(int service, int command, ROUTER_PARAMETERS *r=NULL, int	port_number=-1);
#endif

#endif
