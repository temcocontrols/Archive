//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
                                 
#include "Selectpanel.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TPanelSelect *PanelSelect;
//---------------------------------------------------------------------------
__fastcall TPanelSelect::TPanelSelect(TComponent* Owner)
	: TForm(Owner)
{
  // code similar as in alarm.cpp (DOS versio ), function:
  //   Net_block::Net_block(GGrid *pthis, byte pt, byte p, int net, signed char type, byte pnum)
  //			 :Block(pthis, pt, p, net, type, pnum, N_NET, (char *)&line_buf)
}
//---------------------------------------------------------------------------
void __fastcall TPanelSelect::NetworkNameChange(TObject *Sender)
{
  // code similar as in alarm.cpp (DOS versio ), function:
  // int Net_block::Read(int field, char key)
  // When a network name is selected, its network number and panel list should be
  // updated.
}
//---------------------------------------------------------------------------
void __fastcall TPanelSelect::PanelNumberChange(TObject *Sender)
{
  // code similar as in t3000.cpp (DOS versio ), function:
  // void select_panel(int type, int net_no, int where)
}
//---------------------------------------------------------------------------
void __fastcall TPanelSelect::NetworkNumberChange(TObject *Sender)
{
  // code similar as in alarm.cpp (DOS versio ), function:
  // int Net_block::Read(int field, char key)
  // When a network number is selected, its network name and panel list should be
  // updated.
}
//---------------------------------------------------------------------------
