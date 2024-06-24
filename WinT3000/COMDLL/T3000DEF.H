#ifndef _BURKEDEF_H
#define _BURKEDEF_H				/* avoid recompilation */

//#include <vcl/sysdefs.h>
#include "define.h"


typedef struct {
	DInt range;
	char range_text[SIZE_TEXT_RANGE];
	char aunits[SIZE_TEXT_UNITS];
	char dunits[2][12];
	char value;
} range_form;

typedef struct {
	DInt range;
	char range_text[SIZE_TEXT_RANGE];
	char aunits[SIZE_TEXT_UNITS];
	char value;
} an_range_form;

typedef struct {
	DInt range;
	char range_text[SIZE_TEXT_RANGE];
	char dunits[2][12];
	char value;
} dig_range_form;

typedef enum { RECEIVE_FRAME_IDLE, RECEIVE_FRAME_HEADER, RECEIVE_FRAME_DATA } ReceiveFrameStatusEnum;

typedef enum { HEADER_FrameType, HEADER_Destination, HEADER_Source,
					HEADER_Length1, HEADER_Length2, HEADER_HeaderCRC
				 } HEADERStateEnum;

typedef struct {
	DUint ack0received				:1;
	DUint ack1received				:1;
	DUint nak0received				:1;
	DUint nak1received				:1;
	DUint reception_blocked			:2;
	DUint transmission_blocked		:1;
	DUint sending_frame_now			:1;
	DUint send_done					:1;
	DUint send_ok						:1;
	DUint TxSequence_number			:1;
	DUint RxSequence_number			:1;
	DUint receive_error				:1;
	}	PTP_COMMUNICATION_flags;

typedef struct {
	  DUint   					  	SilenceTimer;
	  DUint 					  	   InactivityTimer;   //Timebetweentoken
	  DUint 					  	   HeartbeatTimer;    //timerunMSTP
	  DInt      					  	EventCount;
	  DSignedChar     					  	ReceiveError;
	  PTP_COMMUNICATION_flags 	PTP_comm_status;
	  DByte                          MSTP_MASTERState;
	  DByte     						Preamble1;
	  DByte     						Preamble2;
	  DByte 	  						HeaderCRC;
	  DUint   						DataCRC;
	  DInt      						Length;
	  DByte 	  						Destination;
	  DByte 	  						Source;
	  DByte     						FrameType;
	  DByte         	         HEADERState;         //HEADERStateEnum
	  DByte  	                  ReceiveFrameStatus;  //ReceiveFrameStatusEnum
	  DInt      						task;
	  DByte                     ring;
	  DByte     						connection;
	  DByte    			         validint;
	  DByte                     mode;
     DByte                     physical_connection_state;
	} PORT_STATUS_variables;

typedef struct {
	DUint status    :4;
   DUint pri       :4;
	DInt    length;
	DInt    next;
	DInt    last;
} Header_pool;

class POOL {
 private:
	DInt length;
	DInt first;
	DInt last;
 public:
   char *buf;
   char access;
	POOL(void);
	POOL(char *p, DInt l);
	void init_pool(char *p, DInt l);
	char *put(char *p, DInt l);
	char *next(void);
	void release(void);
	DUint alloc(DInt l);
	void free(DInt offset);
};

#endif




