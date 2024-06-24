//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "openfile95.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TOpenDialog95Form *OpenDialog95Form;
//---------------------------------------------------------------------------
__fastcall TOpenDialog95Form::TOpenDialog95Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOpenDialog95Form::BitBtn1Click(TObject *Sender)
{
 DirTree->Visible = true;
}
//---------------------------------------------------------------------------//---------------------------------------------------------------------------B
void __fastcall TOpenDialog95Form::DetailsBtnClick(TObject *Sender)
{
  FileList->ViewStyle = vsReport;
}
//---------------------------------------------------------------------------
void __fastcall TOpenDialog95Form::ListBtnClick(TObject *Sender)
{
	FileList->ViewStyle = vsList;
}
//---------------------------------------------------------------------------
void __fastcall TOpenDialog95Form::DblClick(TObject *Sender)
{
  ActiveControl = DirEdit;
  DirTree->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TOpenDialog95Form::OnClick(TObject *Sender)
{
 DirTree->Visible = true;
}
//---------------------------------------------------------------------------
