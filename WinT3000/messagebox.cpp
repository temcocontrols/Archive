//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "messagebox.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMessageBoxForm *MessageBoxForm;
//---------------------------------------------------------------------------
__fastcall TMessageBoxForm::TMessageBoxForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMessageBoxForm::SetMessageBox(int top, int left, int width, int height)
{
  Top = top+(height-Height)/2;
  Left = left+width/2-Message->Width/2-10;
  Width = Message->Width+20;
  Message->Left = 10;
  Button1->Left = Width/2-Button1->Width/2;
}
//---------------------------------------------------------------------------

