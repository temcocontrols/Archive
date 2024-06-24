//----------------------------------------------------------------------------
//Borland C++Builder
//Copyright (c) 1987, 1997 Borland International Inc. All Rights Reserved.
//----------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include "preview.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TPrevForm *PrevForm;
//---------------------------------------------------------------------------
__fastcall TPrevForm::TPrevForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPrevForm::FormShow(TObject *Sender)
{
//   UpDown1->Max=(short)QRPrinter->PageCount;
   UpDown1->Max=(short)10;
   UpDown1->Min=1;
   UpDown1->Position=1;
   QRPreview1->Zoom=100;
  QRPreview1->ZoomToFit();
  UpDown2->Position=(short)QRPreview1->Zoom;
}
//---------------------------------------------------------------------
void __fastcall TPrevForm::Button1Click(TObject *Sender)
{
  QRPreview1->ZoomToFit();
  UpDown2->Position=(short)QRPreview1->Zoom;
}
//---------------------------------------------------------------------
void __fastcall TPrevForm::Button2Click(TObject *Sender)
{
  QRPreview1->ZoomToWidth();
  UpDown2->Position=(short)QRPreview1->Zoom;
}
//---------------------------------------------------------------------
void __fastcall TPrevForm::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
    char buf[3];

    Edit1->Text=itoa(UpDown1->Position, buf, 10);
    QRPreview1->PageNumber=UpDown1->Position;
}
//---------------------------------------------------------------------------
void __fastcall TPrevForm::UpDown2Click(TObject *Sender, TUDBtnType Button)
{
    char buf[3];

    Edit2->Text=itoa(UpDown2->Position, buf, 10);
    QRPreview1->Zoom=UpDown2->Position;
}
//---------------------------------------------------------------------------
void __fastcall TPrevForm::Edit1KeyUp(TObject *Sender, WORD &Key,
    TShiftState Shift)
{
    if (atoi(Edit1->Text.c_str()))
    {
        UpDown1->Position=(short)atoi(Edit1->Text.c_str());
        QRPreview1->PageNumber=UpDown1->Position;
    }
}
//---------------------------------------------------------------------------
void __fastcall TPrevForm::Edit2KeyUp(TObject *Sender, WORD &Key,
    TShiftState Shift)
{
    if (atoi(Edit2->Text.c_str()))
    {
        UpDown2->Position=(short)atoi(Edit2->Text.c_str());
        QRPreview1->Zoom=UpDown2->Position;
    }
}
//---------------------------------------------------------------------------
