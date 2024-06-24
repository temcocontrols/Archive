//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
                             
//#include "router.h"
//---------------------------------------------------------------------------
#define ROUTER

//#include "windows.h"
//#include "graphics.h"
//#include <owl/pch.h>
//#include <owl/window.h>

//#include "wnt3000a.h"
#include <vcl\vcl.h>
//#include "wint3.h"
#include "..\define.h"
#include "..\baseclas.h"
#include "..\ptpdll.h"
#include "ptp.h"
#include "..\router.h"

//extern void ProcPostMessage(DUint mes, DUint w);
//extern void ProcSendMessage(DUint mes, DUint w);

//extern TApplication *Tapp;
extern DByte VendorID;
extern DInt Station_NUM;

// status :   0  - free
//            0x05 PTP installed   ( 0000 0101 )
//            0x07 PTP active      ( 0000 0111 )
//            0x11 RS485 installed { 0001 0001 )
//            0x13 RS485 active    { 0001 0011 )
//            ETHERNET             { 0010 0011 )

extern ROUTING_TABLE_T3000 Routing_table[MAX_Routing_table];
//extern DInt networkaddress;
extern DInt NetworkAddress;
//extern char NetworkName[16];
//extern DInt remote_net;
// command: 0 - update routing table

//__declspec(dllexport) DInt router(DInt service, DInt command, ROUTER_PARAMETERS *r=NULL, DInt	port_number=-1, DInt send=0);

DInt router(DInt service, DInt command, ROUTER_PARAMETERS *r, DInt	port_number, DInt send)
{
 char t,npci[40];
 DInt length_npci,i,j,k,l;

 if( service==N_UNITDATArequest )
 {
  if( command==Who_Is_Router_To_Network )
  {
 }
 if( command==Establish_Connection_To_Network )
 {
	if(port_number>=0)
	{
	 if( (Routing_table[port_number].status&PTP_INSTALLED) == PTP_INSTALLED )
	 {
		 if(!(Routing_table[port_number].status^PTP_ACTIVE) ) return 0;
		 ((class PTP *)Routing_table[port_number].ptr)->NL_parameters.primitive = DL_CONNECT_REQUEST;
	       PostMessage(
		     ((class PTP *)Routing_table[port_number].ptr)->Handle,	// handle of destination window
		     WM_PTP_CONNECTION,	// message to post
		     0,	// first message parameter
		     0  	// second message parameter
		 );
	 }
   }
 }
}
 if( command==I_Am_Router_To_Network || command==I_Am_Router_To_Network_Prop )
 {
	if(service==N_UNITDATArequest)
  {
	npci[0]=0x01;        //VERSION
	npci[1]=0xF0;        //network message, DNET, DLEN, Hopcount, SNET,SLEN=0
	npci[2]=0xFF;           //global networks
	npci[3]=0xFF;           //global networks
	npci[4]=0;                //broadcast MAC DADR
	memcpy( &npci[5], &NetworkAddress, 2);   //SNET
	npci[7]=Station_NUM;      //SADR
	npci[8]=5;                //HopCount
	npci[9]=command;           //I_Am_Router_To_Network;
	l = 10;
	if( command==I_Am_Router_To_Network_Prop )
	  npci[l++]=VendorID;
//	if( (Routing_table[port_number].status&PORT_ACTIVE)==PORT_ACTIVE )
//	{
	for(k=0; k<MAX_Routing_table; k++)
	{
	  length_npci = l;
	  if( (Routing_table[k].status&PORT_ACTIVE)==PORT_ACTIVE )
	  {
		t=0;
		for(i=0; i<MAX_Routing_table; i++)
		{
//		 if( i!=k && !Routing_table[i].status )
//			t++;
		 if( i!=k && ( (Routing_table[i].status&PORT_ACTIVE)==PORT_ACTIVE) )
		 {
            t++;
//		  if( Routing_table[i].Port.network!=0xFFFF )
			if( Routing_table[i].Port.network )
			{
//			if( Routing_table[i].Port.network!=NetworkAddress )
			if( Routing_table[i].Port.network!=Routing_table[k].Port.network )
			{
			 memcpy(&npci[length_npci], &Routing_table[i].Port.network, 2);
			 length_npci += 2;
			 if( command==I_Am_Router_To_Network_Prop )
			 {
				npci[length_npci]=0; npci[length_npci+1]=0;
				if( (Routing_table[i].status&PTP_ACTIVE)==PTP_ACTIVE )
				 npci[length_npci] = Routing_table[i].Port.address;
				length_npci += 2;
			 }
			}
			for(j=0; j<MAX_reachable_networks; j++)
			if( Routing_table[i].Port.networks_list[j].status >= TEMPORARY_UNREACHABIL )
			{
			 if( Routing_table[i].Port.networks_list[j].network!=Routing_table[k].Port.network )
			 {
			  memcpy(&npci[length_npci], &Routing_table[i].Port.networks_list[j].network, 2);
			  length_npci += 2;
			  if( command==I_Am_Router_To_Network_Prop )
			  {
				npci[length_npci] = Routing_table[i].Port.networks_list[j].half_router_address;
			   npci[length_npci+1]=0;
				length_npci += 2;
				}
			 }
			}
		  }
		 }
		}
//		if(length_npci)
//		{
		 if(!(Routing_table[k].status^PTP_ACTIVE))
		 {
			((class PTP *)Routing_table[k].ptr)->sendframe(length_npci, npci, 0, NULL, 0, NULL, 1);
		 }
   		 if( (Routing_table[k].status&RS485_ACTIVE)==RS485_ACTIVE )
		 {
/*
			if( t||send )
			{
			// build frame
			 if( (nextentry = ((class MSTP *)Routing_table[k].ptr)->SendFramePool.NextFreeEntry()) >= 0 )
			 {
 				pframe = &((class MSTP *)Routing_table[k].ptr)->SendFramePool.Frame[nextentry];
				pframe->FrameType = BACnetDataNotExpectingReply;
				pframe->Destination = 255;
				pframe->Source = TS;
				pframe->Length = length_npci;
				memcpy(pframe->Buffer, npci, length_npci);
//				((class MSTP *)Routing_table[k].ptr)->SendFramePool.Unlockhead();
				((class MSTP *)Routing_table[k].ptr)->SendFramePool.status[nextentry]=1;
			 }
			}
*/
		 }
//		}
		}
	}
//	}
	}
  else
  {
	 if(service==DL_UNITDATAindication)
	 {
	   if(r->SNET!=NetworkAddress)
	   {
		 Routing_table[port_number].Port.network = r->SNET;
		 Routing_table[port_number].Port.address = r->SADR[0];
	   }
//	  memset( Routing_table[port_number].Port.networks_list, 0, sizeof(Routing_table[port_number].Port.networks_list));
	   for(l=0;l<MAX_reachable_networks;l++)
	    if(Routing_table[port_number].Port.networks_list[l].router_address == r->SADR[0])
	    {
			memset( &Routing_table[port_number].Port.networks_list[l], 0, sizeof(struct reachable_networks));
	    }
//		remote_net = r->SNET;
	   if( command==I_Am_Router_To_Network_Prop )
		 k = 4;
	   else
		 k = 2;
	   for(i=0; i<(r->len/k); i++)
	   {
		j = *((DInt *)r->data);
		if( j!=r->SNET || Routing_table[port_number].Port.address!=r->SADR[0] )
		{
		 for(l=0;l<MAX_reachable_networks;l++)
			if(Routing_table[port_number].Port.networks_list[l].network == j) break;
		 if(l>=MAX_reachable_networks)
		 {
		  for(l=0;l<MAX_reachable_networks;l++)
			if(!Routing_table[port_number].Port.networks_list[l].network) break;
		  if(l<MAX_reachable_networks)
		  {
			Routing_table[port_number].Port.networks_list[l].status = REACHABIL;
			Routing_table[port_number].Port.networks_list[l].network = j;
			Routing_table[port_number].Port.networks_list[l].router_address = r->SADR[0];
			if( command==I_Am_Router_To_Network_Prop )
			{
			 Routing_table[port_number].Port.networks_list[l].half_router_address = *((DInt *)(r->data+2));
			}
			else
			 Routing_table[port_number].Port.networks_list[l].half_router_address = 0;
		  }
		 }
	    }
		if( command==I_Am_Router_To_Network_Prop )
		  r->data += 4;
		else
		  r->data += 2;
	   }
	   PostMessage(
		     ((class PTP *)Routing_table[port_number].ptr)->Handle,	// handle of destination window
		     WM_PTP_ESTABLISH_CONNECTION,	// message to post
		     0,	// first message parameter
		     0  	// second message parameter
	   );
	 }
	}
 }
}

DByte Routing_TABLE_T3000::Status(void)
{
 return status;
};
void Routing_TABLE_T3000::Status(DByte s)
{
 status = s;
};
void *Routing_TABLE_T3000::Ptr(void)
{
 return ptr;
};
PORT_STATUS_variables *Routing_TABLE_T3000::Ps(void)
{
 return &port_status_vars;
}
DInt Routing_TABLE_T3000::PortNet(void)
{
 return Port.network;
}
void Routing_TABLE_T3000::PortNet(DInt net)
{
 Port.network = net;
}
void Routing_TABLE_T3000::SilenceTimer(DUint st)
{
 port_status_vars.SilenceTimer = st;
};
void Routing_TABLE_T3000::HeartbeatTimer(DUint ht)
{
 port_status_vars.HeartbeatTimer = ht;
};


