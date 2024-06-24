//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include <vcl/comctrls.hpp>
#include <time.h>
#include "define.h"
#include "baseclas.h"
#include "ptpdll.h"
#include "ptp.h"
#include "router.h"
#include "net_bac.h"
#include "netb_bac.h"
#include "ipx.h"

extern Panel_info1 Panel_Info1;

extern int NET_EXIT;
extern IPX *pipx;


__declspec(dllexport) void ProcIPXTimerTimer(DInt conPort, DInt DelayTime);

void ProcIPXTimerTimer(DInt conPort, DInt DelayTime)
{
   IPX *pipx;
   pipx = (IPX *)Routing_table[conPort].ptr;
   pipx->time = GetTickCount();
   while(1)
   {
    pipx->activity = 0;
    pipx->NetControl();
	if( !pipx->activity )
       break;
   }
}

__declspec(dllexport) DInt InitIPX(void);

DInt InitIPX(void)
{
  DInt ret=-1;
  pipx = NULL;
  pipx = new IPX( &Panel_Info1, 0 );
  if( pipx->state != DRIVER_INSTALLED )
  {
 	delete pipx;
 	pipx = NULL;
    return -1;
  }

  if( pipx  != NULL )
  {
		if( pipx->state == DRIVER_INSTALLED )
		{
			for(int j=0; j<MAX_Routing_table ; j++)
			{
				if( !Routing_table[j].status )
				{
					pipx->routing_entry = j;
					Routing_table[j].ptr = pipx;
					Routing_table[j].Port.address = Station_NUM;
					Routing_table[j].Port.network = NetworkAddress;
					Routing_table[j].status = IPX_ACTIVE;
					Routing_table[j].task = 0;
					Routing_table[j].port_status_vars.task = 0;
					Routing_table[j].port_status_vars.connection = IPX_CONNECTED;
                    ret = j;
					break;
				}
			}
			NET_EXIT = 0;
//			resume(NETWORK1);
//			task_switch();
		    if( pipx->GetLocalAddress()  )
		    {
//			DisplayMessageT( 20, 6, 60, 10, NULL, "          Adapter error !!!            Network services unavailable !!!", NULL, Blue, NULL, 3000);
			 NET_EXIT = 1;
		    }
		    else
		    {
			 if( pipx->OpenCommunication() )
			 {
//				DisplayMessageT( 20, 6, 60, 10, NULL, "          Adapter error !!!            Network services unavailable !!!", NULL, Blue, NULL, 3000);
				NET_EXIT = 1;
			 }
			 else
			 {
//				DisplayMessageT( 20, 6, 60, 10, NULL, " Adapter succesfully initialized !!!      Network services available !!!", NULL, Blue, NULL, 3000);
                ;
			 }
		    }
        }
  }
  if( NET_EXIT )
  {
  	delete pipx;
 	pipx = NULL;
    return -1;
 }
 return ret;
}
