//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "LTGUserEnsur.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TUserEnsureForm *UserEnsureForm;
//---------------------------------------------------------------------------
__fastcall TUserEnsureForm::TUserEnsureForm(TComponent* Owner)
	: TForm(Owner)
{
  
}
//---------------------------------------------------------------------------

void __fastcall TUserEnsureForm::OkButtonClick(TObject *Sender)
{
	Ok = 1;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TUserEnsureForm::CancelButtonClick(TObject *Sender)
{
	Ok = 0;
    Close();
}
//---------------------------------------------------------------------------
