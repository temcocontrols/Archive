//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "monitor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "perfgrap"
#pragma resource "*.dfm"
TGraphForm *GraphForm;
//---------------------------------------------------------------------------
__fastcall TGraphForm::TGraphForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
