//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "RefreshIn.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TRefreshInForm *RefreshInForm;
//---------------------------------------------------------------------------
__fastcall TRefreshInForm::TRefreshInForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRefreshInForm::procKeyPress(TObject *Sender, char &Key)
{
  if (Key == '\r')
  {
    ModalResult = mrOk;
    Close();
  }
}
//---------------------------------------------------------------------------