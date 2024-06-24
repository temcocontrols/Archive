//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "data.h"
//---------------------------------------------------------------------------
#include <vcl/comctrls.hpp>
#include "..\define.h"
#include "..\router.h"

DByte tbl_bank[MAX_TBL_BANK]={
											MAX_OUTS,     //OUT
											MAX_INS,     //IN
											MAX_VARS,      //VAR
											MAX_CONS,     //CON
											MAX_WR,        //WR
											MAX_AR,          //AR
											MAX_PRGS,     //PRG
											MAX_TABS,
											32,           //DMON
											MAX_ANALM,       //AMON
											MAX_GRPS,      //GRP
											MAX_ARRAYS,
											MAX_ALARMS,
											MAX_UNITS,
											MAX_PASSW
										 };
DInt  _export Station_NUM=29;
DByte _export Station_NAME[17]="PANEL 29";
DInt  _export NetworkAddress=77;
DByte _export NetworkName[16]="UserNetwork";
DByte _export ready_for_descriptors=0x01|0x04;   //0x01 read_des
										 //0x02 ; 0x04 start_read_des
DByte _export VendorID=1;

ROUTING_TABLE_T3000 _export Routing_table[MAX_Routing_table];

TImage       **tImageRun;
DInt         maxImages;
TProgressBar *ProgressBar;

DUint  msgWM_PTP_CONNECTION           =  WM_PTP_CONNECTION;
DUint  msgWM_PTP_RECEPTION            =	 WM_PTP_RECEPTION;
DUint  msgWM_PTP_TRANSMISSION    	  =  WM_PTP_TRANSMISSION;
DUint  msgWM_PTP_ESTABLISH_CONNECTION =  WM_PTP_ESTABLISH_CONNECTION; 

