//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "datetime.h"
//---------------------------------------------------------------------------
#pragma link "Grids"
//#pragma link "t3000reg"
#pragma resource "*.dfm"
TDateTimeForm *DateTimeForm;
//---------------------------------------------------------------------------
__fastcall TDateTimeForm::TDateTimeForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------