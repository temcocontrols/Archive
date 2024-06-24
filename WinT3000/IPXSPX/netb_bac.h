/******************************************************************/
/*                                                                */
/* 							NETB_BAC.H                                         */
/*                                                                */
/******************************************************************/
#ifndef _NETB_BAC_H
#define _NETB_BAC_H

#ifdef __cplusplus
	 #define __CPPARGS ...
#else
	 #define __CPPARGS
#endif

#include "net_bac.h"

#define NcbQueueSize          12

/* Symbolic names for NetBIOS commands  */

#define RESET_CMD                       0x032
#define CANCEL                          0x035
#define UNLINK                          0x070
#define STATUS                          0x0b3
#define STATUS_WAIT                     0x033
#define TRACE                           0x0f9
#define TRACE_WAIT                      0x079
#define ADD_NAME                        0x0b0
#define ADD_NAME_WAIT                   0x030
#define ADD_GROUP_NAME                  0x0b6
#define ADD_GROUP_NAME_WAIT             0x036
#define DELETE_NAME                     0x0b1
#define DELETE_NAME_WAIT                0x031
#define CALL                            0x090
#define CALL_WAIT                       0x010
#define LISTEN                          0x091
#define LISTEN_WAIT                     0x011
#define HANG_UP                         0x092
#define HANG_UP_WAIT                    0x012
#define SEND                            0x094
#define SEND_WAIT                       0x014
#define SEND_NO_ACK                     0x0f1
#define SEND_NO_ACK_WAIT                0x071
#define CHAIN_SEND                      0x097
#define CHAIN_SEND_WAIT                 0x017
#define CHAIN_SEND_NO_ACK               0x0f2
#define CHAIN_SEND_NO_ACK_WAIT          0x072
#define RECEIVE                         0x095
#define RECEIVE_WAIT                    0x015
#define RECEIVE_ANY                     0x096
#define RECEIVE_ANY_WAIT                0x016
#define SESSION_STATUS                  0x0b4
#define SESSION_STATUS_WAIT             0x034
#define SEND_DATAGRAM                   0x0a0
#define SEND_DATAGRAM_WAIT              0x020
#define SEND_BROADCAST_DATAGRAM         0x0a2
#define SEND_BROADCAST_DATAGRAM_WAIT    0x022
#define RECEIVE_DATAGRAM                0x0a1
#define RECEIVE_DATAGRAM_WAIT           0x021
#define RECEIVE_BROADCAST_DATAGRAM      0x0a3
#define RECEIVE_BROADCAST_DATAGRAM_WAIT 0x023

#define NETBIOS_INVALID_COMMAND         0x07F
#define ERROR_INVALID_COMMAND           0x003

class NetBios_Bac;

/* Network Control Block (NCB)  */
/*
typedef struct
	 {
	 DByte NCB_COMMAND;               // command id                         /
	 DByte NCB_RETCODE;               // immediate return code              /
	 DByte NCB_LSN;                   // local session number               /
	 DByte NCB_NUM;                   // network name number                /
	 void far *NCB_BUFFER_PTR;       // address of message packet          /
	 DUint NCB_LENGTH;                // length of message packet           /
	 DByte NCB_CALLNAME[16];          // name of the other computer         /
	 DByte NCB_NAME[16];              // our network name                   /
	 DByte NCB_RTO;                   // receive time-out in 500 ms. incrs. /
	 DByte NCB_STO;                   // send time-out - 500 ms. increments /
//	 void interrupt (NetBios_Bac::*POST_FUNC)(__CPPARGS); /* address of POST routine         /
     DByte adrPOST[4];
	 DByte NCB_LANA_NUM;              // adapter number (0 or 1)            /
	 DByte NCB_CMD_CPLT;              // final return code                  /
	 DByte NCB_RESERVE[16];           // Reserved area                      /
	 DUint in_use     : 4;      // extra parameters used to indicate if the NCB
//																		is used or not; == 0 => not used, != 0 => in use /
	 DUint index      : 3;     //command associated with this NCB /
	 DUint tyepe      : 1;     // command type associated with this NCB /
	 }
	 NCB;
*/

typedef struct {
		  DByte   card_id[6];
		  DByte   release_level;
		  DByte   reserved1;
		  DByte   type_of_adapter;
		  DByte   old_or_new_parameters;
		  DUint   reporting_period_minutes;
		  DUint   frame_reject_recvd_count;
		  DUint   frame_reject_sent_count;
		  DUint   recvd_frame_errors;
		  DUint   unsuccessful_transmissions;
		  DUlong  good_transmissions;
		  DUlong  good_receptions;
		  DUint   retransmissions;
		  DUint   exhausted_resource_count;
		  DUint   t1_timer_expired_count;
		  DUint   ti_timer_expired_count;
		  char    reserved2[4];
		  DUint   available_ncbs;
		  DUint   max_ncbs_configured;
		  DUint   max_ncbs_possible;
		  DUint   buffer_or_station_busy_count;
		  DUint   max_datagram_size;
		  DUint   pending_sessions;
		  DUint   max_sessions_configured;
		  DUint   max_sessions_possible;
		  DUint   max_frame_size;
		  DInt             name_count;
		  struct {
				char            tbl_name[16];
				DByte  tbl_name_number;
				DByte  tbl_name_status;
				} name_table[20];
			}
			ADAPTER_DATA;

typedef struct {
			DByte  name_num;
			DByte  session_count;
			DByte  junk1;
			DByte  junk2;
//		  A_SESSION      session_data[40];
			}
			STATUS_INFO;

class NetBios_Bac : public NET_BAC
{

	NCB far 	*ncb_array;
	DUint       real_ncb_DS;

	private:

//		void interrupt command_post(__CPPARGS);
		DUint adapter_status( DByte ncb , DByte *remotename, DUint real_data_segment );

//		void set_panel_name( char *name, DByte dest );
		DInt  NetBios_call( DUint ncb_index );

		DUint adapter_status( DByte ncb , DByte *remotename );
		DUint send_datagram( DByte ncb, DByte *mess, DUint length, DByte index, DByte *name );
		DUint receive_datagram( DByte ncb, DByte *buffer, DByte index, DByte type );
		DUint add_name( DByte ncb, DByte *name, DByte type );
		DUint delete_name( DByte ncb, DByte *name );
		DUint cancel( DByte ncb, NCB *ptr );

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

	NetBios_Bac( Panel_info1 *info, DByte task_no );

	DInt GetLocalAddress( void );
	DInt CloseCommunication( void );
	DInt OpenCommunication( void );

//	void delete_names( void );
	DInt reset_adapter( void );

	~NetBios_Bac();
};

#endif //_NETB_BAC_H


