//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include <stdio.h>

#include "define.h"
#include "aio.h"
#include "ptpdll.h"
//---------------------------------------------------------------------------
#define READ  0
#define WRITE 1

__declspec(dllimport) int ClientTransactionStateMachine(
			 ServicePrimitive Event,  DInt Service, FlowControl flowcontrol, DInt network,
			 DInt destination, DInt source, DByte invokeID, DByte *data, DUint *data_length,
			 DByte command, DInt arg, DByte vendorID, DLong timeout = TIMEOUT_NETCALL,
			 DByte *returnargs=NULL, DInt *length_returnargs=NULL,
			 DByte *sendargs=NULL, DInt length_sendargs=0, DByte bytearg=0, DInt task = -1,
             DInt str_size=MAXASDUSIZE
			);

extern Panel *ptr_panel;
extern DByte lastid;
extern DByte VendorID;
extern unsigned DByte tbl_bank[MAX_TBL_BANK];
int   lockedaccess;


DByte getID(void)
{
 return random(254);
}

DUint countlength(DInt command, DInt arg)
{
 DUint length;
 DByte no_points;
 Bank_Type tbank;
 DInt str_new_size=0;
 DByte *ptr;
	no_points = 1;
	tbank = *( Bank_Type *)&arg;        //arg
	switch( command )            //command
	{
		case T3000_OUT+1:
		case T3000_IN+1:
		case VAR+1:
		case CON+1:
		case WR+1:
		case AR+1:
		case PRG+1:
		case GRP+1:
		case AMON+1:
		case AY+1:
		case UNIT+1:
			ptr = (DByte *)&ptr_panel->info[command-1];
			str_new_size = ((Info_Table *)ptr)->str_size;
			if( tbank.bank_type )
			{
				no_points = tbank.no_points;
			}
			else
			{
				no_points = tbl_bank[command-1];
			}
			break;
		case USER_NAME+1:
			 str_new_size = 16+9+1+4+1+1+(2*((tbl_bank[GRP]%8)?tbl_bank[GRP]/8+1:tbl_bank[GRP]/8))+
								 (2*((tbl_bank[PRG]%8)?tbl_bank[PRG]/8+1:tbl_bank[PRG]/8));
			 length = 2+(DUint)(tbl_bank[USER_NAME]) * str_new_size;
			 return length;
		case WR_TIME+1:
			str_new_size = 9*sizeof(Wr_one_day);
			break;
		case AR_Y+1:
			str_new_size = 46;
			break;
		case 20:
			PointT *point;
			point = (PointT *)&arg;
			no_points = 1;
			switch( point->point_type )
			{
				case T3000_OUT+1:
				case T3000_IN+1:
				case VAR+1:
				case CON+1:
				case WR+1:
				case AR+1:
				case PRG+1:
				case TBL+1:
				case AMON+1:
				case GRP+1:
				case AY+1:
				{
					ptr = (DByte *)&ptr_panel->info[point->point_type-1];
					str_new_size = ((Info_Table *)ptr)->str_size;
					break;
				}
				case DIGITAL_MONITOR:
					 break;
				default :
				{
						printf( "Wrong point type" );
						break;
				}
			}
			break;
	}
	length = (DUint)(no_points) * str_new_size;
	return length;
}

DInt t3000Request(DInt command, DInt arg,  DByte *data, DUint *length , DInt dest,
				 DInt network, DInt others, DInt timeout,
				 DByte *returnargs, DInt *length_returnargs,
                 DByte *sendargs, DInt length_sendargs, DByte bytearg)
{

 DByte id;
 DInt task, i, com, ctype, command_number, status;
 DUint length1, length2=0, bank = arg;
 DInt service, event, str_size;
 BACnetPDU pdutype;
 DUlong l;
 DWORD            dwStopTime;
 MSG              myMsg;


    if (lockedaccess ) return ERROR_COMM;
    lockedaccess = 1;

	task = -1;
	pdutype = others&NETCALL_BACnetPduType;
	service = ConfirmedPrivateTransfer;
	status = RS232_ERROR;
	switch (pdutype) {
	 case BACnetConfirmedRequestPDU:
		event = CONF_SERVrequest;
		break;
	 case BACnetUnconfirmedRequestPDU:
		event = UNCONF_SERVrequest;
		break;
	 default:
		return 0;  //error
	}
	id = lastid++;
    length1 = 0;

	if( service == ConfirmedPrivateTransfer )
	{
	 if(command > 100 && command < 200)
	 {
	  ctype = WRITE;
	  command_number = command-100;
	 }
	 else
	 {
	  ctype = READ;
	  command_number = command;
	  if(length)
		*length = 0;
	 }
	 if (ctype == WRITE)
	 {
		if(!length)
			length2 = countlength(command_number, arg);
	 }
	 if (ctype == READ)
	 if (length) *length = 0;
	 if(command_number == 22 ) timeout = 30;
	 if(command_number == 50 && (arg==40 || arg==41 || arg==42) ) timeout = 30;
//	  showsign = others&NETCALL_SIGN;
	str_size = MAXASDUSIZE;

	switch( command_number )            //command
	{
		case T3000_OUT+1:
		case T3000_IN+1:
		case VAR+1:
		case CON+1:
		case WR+1:
		case AR+1:
		case PRG+1:
		case GRP+1:
		case AY+1:
		case ALARMM+1:
		{
		  str_size = ptr_panel->info[command_number-1].str_size;
		  break;
		}
	}

	status=ClientTransactionStateMachine(event, ConfirmedPrivateTransfer,	AEFLOWCONTROL,
														network, dest, Station_NUM, id,
														data, length?length:&length2,
														command, arg, VendorID, timeout,
														returnargs, length_returnargs,
														sendargs, length_sendargs, bytearg, task,
                                                        str_size);

     lockedaccess = 0;
//	  showsign = others&NETCALL_SIGN;
	 if(status)
	 {
//		    result_ser(status, 1);
			if(status == USER_ABORT)
			 return USER_ABORT;
			else
			 return ERROR_COMM;
	 }
	 return SUCCESS;
	}

}


