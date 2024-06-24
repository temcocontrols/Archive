#ifdef NET_BAC_COMM
#include <vcl\vcl.h>
#pragma hdrstop

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <mem.h>
#include <bios.h>
#include <conio.h>
#include <string.h>
#include <process.h>
#include "baseclas.h"
#include "t3000def.h"
#include "net_bac.h"
#include "ipx.h"

DInt IPX::IpxCall( REAL_MODE_REGISTERS *rm_ptr )
{
/*
	asm
	{
		push bp
		mov ax, 0301h
		mov bh, 0
		mov cx, 0
		les di, rm_ptr
		nop
		int 31h
		pop bp
		jnc ok_ipx_call
		jmp err_ipx_call
	ok_ipx_call:
		}
	return 0;
	asm
	{
	err_ipx_call:
		nop
	}
	return -1;
*/
}

DInt IPX::TestDriverPresence( void )
{
//    struct SREGS SegReg;
//    union  REGS  InReg, OutReg;
    DUint ax,es,di;
    DByte al;
   _AH = 0x7A;
   _AL = 0x00;
   geninterrupt(0x2F);
   al = _AL;
   es = _ES;
   di = _DI;
/*
    InReg.h.ah = 0x7A;
    InReg.h.al = 0x00;
    int86x(0x2F,&InReg,&OutReg,&SegReg);
	if( ( OutReg.h.al == 0xFF )
	{
		Ipx_segment_call = SegReg.es;
		Ipx_offset_call =  OutReg.x.di;
        IpxSpxCallPtr = (void *)((((long)SegReg.es<<16)+((long)OutReg.x.di));
    	return 0;
	}
*/
	if(  _AL == 0xFF )
	{
		Ipx_segment_call = es;
		Ipx_offset_call =  di;
        memcpy(&IpxSpxCallPtr, &di, 2);
        memcpy( ((char *)&IpxSpxCallPtr)+2, &es, 2);
    	return 0;
	}
	return 1;
}

DInt IPX::IxpGetInternetworkAddress( IpxNetworkAddress *AddressStructPtr )
{
/*
	unsigned int realES, selector;
	unsigned long par_sel;
	REAL_MODE_REGISTERS *rm_ptr, real_data_regs;
	IpxNetworkAddress *reply_buffer;

	par_sel = GlobalDosAlloc( (unsigned int)sizeof( IpxNetworkAddress ) );
	selector = par_sel&0x0ffff;
	realES = par_sel>>16;
	reply_buffer = ( IpxNetworkAddress far *)MK_FP( selector, 0 );

	rm_ptr = &real_data_regs;
	memset( rm_ptr, 0, sizeof( REAL_MODE_REGISTERS ) );
	rm_ptr->BX.w = IPX_GetInternetworkAddress;
	rm_ptr->ES   = realES;
	rm_ptr->SI.w = 0;
	rm_ptr->CS = Ipx_segment_call;
	rm_ptr->IP = Ipx_offset_call;

	asm{
		push bp
		mov ax, 0301h
		mov bh, 0
		mov cx, 0
		les di, rm_ptr
		nop
		int 31h
		pop bp
		jnc ok_net_add
		jmp err_net_add
	ok_net_add:
		}
	memcpy( AddressStructPtr, reply_buffer, sizeof( IpxNetworkAddress ) );
	GlobalDosFree( selector );
	return 0;
	asm {
	err_net_add:
		nop
	}
	GlobalDosFree( selector );
	return -1;
*/
}

DInt IPX::swab( DUint data )
{
    DByte al;
	_AX = data;
    al = _AL;
    _AL = _AH;
    _AH = al;
	return _AX;
}

#endif
