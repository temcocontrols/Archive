//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "hardware.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
THardwareForm *HardwareForm;
//---------------------------------------------------------------------------
__fastcall THardwareForm::THardwareForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall THardwareForm::Button1Click(TObject *Sender)
{
 ModalResult = mrNo+1;
}
//---------------------------------------------------------------------------
void __fastcall THardwareForm::Button2Click(TObject *Sender)
{
 ModalResult = mrNo+2;
}
//---------------------------------------------------------------------------