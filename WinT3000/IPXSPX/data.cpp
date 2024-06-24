//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "data.h"
//---------------------------------------------------------------------------
#include <vcl/comctrls.hpp>
#include "define.h"
#include "ptpdll.h"
#include "ptp.h"
#include "router.h"
#include "netb_bac.h"
#include "ipx.h"

TImage       **tImageRun;
DInt         maxImages;
TProgressBar *ProgressBar;
DByte VendorID=1;
DByte ready_for_descriptors=0x01|0x04;;

NetBios_Bac _export *pnet;
IPX         _export *pipx;
DInt        _export NET_EXIT;

