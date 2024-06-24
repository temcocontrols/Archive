//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("WinT3000ComponentPackage.res");
USEPACKAGE("vcl35.bpi");
USEUNIT("tcalendar.cpp");
USEUNIT("tdbgrid.cpp");
USEPACKAGE("VCLDB35.bpi");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Package source.
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
    return 1;
}
//---------------------------------------------------------------------------
