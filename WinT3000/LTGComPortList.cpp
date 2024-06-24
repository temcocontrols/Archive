//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "LTGComPortList.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TComPortListForm *ComPortListForm;
//---------------------------------------------------------------------------
__fastcall TComPortListForm::TComPortListForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TComPortListForm::ListBox1Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TComPortListForm::CancelButtonClick(TObject *Sender)
{
  ListBox1->ItemIndex = -1;
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TComPortListForm::OkButtonClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------