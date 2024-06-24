//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "openfile.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TOpenDialogForm *OpenDialogForm;
//---------------------------------------------------------------------------
__fastcall TOpenDialogForm::TOpenDialogForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------