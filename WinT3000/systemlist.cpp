//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "systemlist.h"
//---------------------------------------------------------------------------
#pragma link "Grids"
#pragma resource "*.dfm"
TSysListForm *SysListForm;
//---------------------------------------------------------------------------
__fastcall TSysListForm::TSysListForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------