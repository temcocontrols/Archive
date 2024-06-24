//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
#include <stdio.h>

#include "program.h"
#include "aio.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TProgramForm *ProgramForm;
extern Panel *ptr_panel;
extern AnsiString Progname;
//---------------------------------------------------------------------------
__fastcall TProgramForm::TProgramForm(TComponent* Owner, Str_program_point& ptrprg, DInt currentpoint )
	: TForm(Owner)
{
  char buffer[20];

  Top = 0;
  Left = 0;
  pPrg = ptrprg;
  current_point = currentpoint;
  memset(buffer,'\0',20);
  sprintf(buffer,"prg%d.bas",current_point);
  programName = AnsiString(buffer);
}
//---------------------------------------------------------------------------
void __fastcall TProgramForm::FormActivate(TObject *Sender)
{   FILE *fp;
    if((fp=fopen(programName.c_str(),"rb+"))!=NULL)
      {
        fclose(fp);
        RichEdit1->Lines->LoadFromFile(programName);
      }
    else
        RichEdit1->Lines->Clear();
    OpenDialog1->Filter = "Basic Program files (*.bas)|*.BAS";
    SaveDialog1->Filter = "Basic Program files (*.bas)|*.BAS";
}
//---------------------------------------------------------------------------
void __fastcall TProgramForm::SaveFile1Click(TObject *Sender)
{
    SaveDialog1->FileName=programName;
    if(SaveDialog1->Execute())
    RichEdit1->Lines->SaveToFile(SaveDialog1->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TProgramForm::LoadFile1Click(TObject *Sender)
{
	 if(OpenDialog1->Execute())
     RichEdit1->Lines->LoadFromFile(OpenDialog1->FileName);
}
//---------------------------------------------------------------------------