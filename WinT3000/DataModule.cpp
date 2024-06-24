//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "Grid.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TDM *DM;
char DATABASEDIRECTOR[] = "d:\public\WinT3000\t3000Database";
//---------------------------------------------------------------------------
__fastcall TDM::TDM(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------

int i,j;
void __fastcall TDM::OnChange(TObject *Sender, TField *Field)
{
  grid->dataChange(Field);
}
//---------------------------------------------------------------------------
void __fastcall TDM::OnUpdate(TObject *Sender)
{
  grid->onUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TDM::DMCreate(TObject *Sender)
{
    InputsTable->DatabaseName = DATABASEDIRECTOR;
}
//---------------------------------------------------------------------------

