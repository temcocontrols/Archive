//----------------------------------------------------------------------------
//Borland C++Builder
//Copyright (c) 1987, 1997 Borland International Inc. All Rights Reserved.
//----------------------------------------------------------------------------
//---------------------------------------------------------------------
// RICH EDIT DEMO v.02
//
//
//---------------------------------------------------------------------
#include <vcl.h>
#include <windows.hpp>
#pragma hdrstop
#include <stdlib.h>

#include "edit.h"
//#include "RichAbt.h"

const float RulerAdj = 4.0/3.0;
const int GutterWid = 6;
//----------------------------------------------------------------------------
#pragma resource "*.dfm"
TEditForm *EditForm;
//----------------------------------------------------------------------------
__fastcall TEditForm::TEditForm(TComponent *Owner)
  : TForm(Owner)
{
//     SetFileName((AnsiString)"Untitled");
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::SelectionChange(TObject */*Sender*/)
{
  char sizebuf[6];

  try {
	   FUpdating = True;
/*
	   FirstInd->Left = int(RichEdit->Paragraph->FirstIndent*RulerAdj)-
							4+GutterWid;
	   LeftInd->Left  = int((RichEdit->Paragraph->LeftIndent+
							RichEdit->Paragraph->FirstIndent)*RulerAdj)-
							4+GutterWid;
	   RightInd->Left = Ruler->ClientWidth-6-int(
						(RichEdit->Paragraph->RightIndent+GutterWid)*RulerAdj);

*/
//	   BoldButton->Down = RichEdit->SelAttributes->Style.Contains(fsBold);
//	   ItalicButton->Down = RichEdit->SelAttributes->Style.Contains(fsItalic);
//	   UnderlineButton->Down = RichEdit->SelAttributes->Style.Contains(fsUnderline);

//	   BulletsButton->Down = bool(RichEdit->Paragraph->Numbering);

	   FontSize->Text = itoa(RichEdit->SelAttributes->Size, sizebuf, 10);
	   FontName->Text = RichEdit->SelAttributes->Name;

/*
	   switch((int)RichEdit->Paragraph->Alignment)
	   { case 0: LeftAlign->Down   = True; break;
		 case 1: RightAlign->Down  = True; break;
		 case 2: CenterAlign->Down = True; break;
	   }
*/
	 }
     catch (...) {
       FUpdating = False;
     }
     FUpdating = False;
}
//----------------------------------------------------------------------------
TTextAttributes *__fastcall TEditForm::CurrText(void)
{
    return RichEdit->SelAttributes;
}
//----------------------------------------------------------------------------
int __stdcall EnumFontsProc(TLogFontA &LogFont, TTextMetricA &/*TextMetric*/,
                                int /*FontType*/, Pointer Data)
{   ((TStrings *)Data)->Add((AnsiString)LogFont.lfFaceName);
     return 1;
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::GetFontNames(void)
{    HDC hDC = GetDC(0);
     void * cTmp = (void *)FontName->Items;
     EnumFonts(hDC, NULL, (FONTENUMPROC) EnumFontsProc, (long) cTmp );
     ReleaseDC(0,hDC);
     FontName->Sorted = True;
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::SetFileName(const AnsiString FileName)
{
/*
     LPSTR lpBuf = new char[MAX_PATH];
     sprintf(lpBuf, "%s - %s", ExtractFileName(FileName).c_str(),
             Application->Title.c_str());
     Caption = (AnsiString)lpBuf;
     FFileName = FileName;
     delete lpBuf;
*/
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::CheckFileSave(void)
{
/*
    if ( RichEdit->Modified ) {
        switch(MessageBox(Handle,"Save Changes?","Confimation",MB_YESNOCANCEL | MB_ICONQUESTION))
        {  case ID_YES    : FileSaveClick(this);
           case ID_CANCEL : Abort();
        };
     }
*/
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::SetEditRect(void)
{
    TRect Rct = Rect(GutterWid, 0, RichEdit->ClientWidth-GutterWid,
                               ClientHeight);
     SendMessage(RichEdit->Handle, EM_SETRECT, 0, long(&Rct));
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::FormCreate(TObject* /*Sender*/)
{
    Application->OnHint = &ShowHint;
     OpenDialog->InitialDir = ExtractFilePath(ParamStr(0));
     SaveDialog->InitialDir = OpenDialog->InitialDir;
     GetFontNames();
     SelectionChange(this);
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::ShowHint(TObject* /*Sender*/)
{
    StatusBar->SimpleText = Application->Hint;
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::SendClick(TObject* /*Sender*/)
{
/*
     CheckFileSave();
     SetFileName((AnsiString)"Untitled");
     RichEdit->Lines->Clear();
     RichEdit->Modified = False;
*/
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::OpenClick(TObject* /*Sender*/)
{
/*
    CheckFileSave();
     if (OpenDialog->Execute()) {
        RichEdit->Lines->LoadFromFile(OpenDialog->FileName);
        SetFileName(OpenDialog->FileName);
        RichEdit->SetFocus();
        RichEdit->Modified = False;
        RichEdit->ReadOnly = OpenDialog->Options.Contains(ofReadOnly);
     }
*/
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::SaveClick(TObject* Sender)
{
/*
    if ( !strcmp(FFileName.c_str(), "Untitled") )
        FileSaveAsClick(Sender);
     else
     {
        RichEdit->Lines->SaveToFile(FFileName);
        RichEdit->Modified = False;
     }
*/
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::FileSaveAsClick(TObject* /*Sender*/)
{
/*
    if ( SaveDialog->Execute() ) {
        // Options + OverwritePrompt = True thus no need to check.
        RichEdit->Lines->SaveToFile(SaveDialog->FileName);
        SetFileName(SaveDialog->FileName);
        RichEdit->Modified = False;
     }
*/
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::PrintClick(TObject* /*Sender*/)
{
    if ( PrintDialog->Execute() ) RichEdit->Print( FFileName );
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::FileExitClick(TObject* /*Sender*/)
{
    Close();
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::EditUndoClick(TObject* /*Sender*/)
{
    if ( RichEdit->HandleAllocated() )
        SendMessage(RichEdit->Handle, EM_UNDO, 0, 0);
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::EditCutClick(TObject* /*Sender*/)
{
    RichEdit->CutToClipboard();
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::EditCopyClick(TObject* /*Sender*/)
{
    RichEdit->CopyToClipboard();
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::EditPasteClick(TObject* /*Sender*/)
{
    RichEdit->PasteFromClipboard();
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::HelpContentsClick(TObject* /*Sender*/)
{
//     Application->HelpCommand(HELP_CONTENTS, 0);
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::HelpSearchClick(TObject* /*Sender*/)
{
//     Application->HelpCommand(HELP_PARTIALKEY, (long) "");
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::HelpHowToClick(TObject* /*Sender*/)
{
//     Application->HelpCommand(HELP_HELPONHELP, 0);
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::HelpAboutClick(TObject* /*Sender*/)
{
/*
     Form2 = new TForm2(Application);
     Form2->ShowModal();
     delete Form2;
*/
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::SelectFont(TObject* /*Sender*/)
{
    FontDialog->Font->Assign( RichEdit->SelAttributes );
     if ( FontDialog->Execute() )
        CurrText()->Assign( FontDialog->Font );

     RichEdit->SetFocus();
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::RulerResize(TObject* /*Sender*/)
{
//     RulerLine->Width = (int)Ruler->ClientWidth - (RulerLine->Left*2);
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::FormResize(TObject* Sender)
{
    SetEditRect();
     SelectionChange(Sender);
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::FormPaint(TObject* /*Sender*/)
{
    SetEditRect();
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::FontSizeChange(TObject* /*Sender*/)
{
     int fontsize = atoi(FontSize->Text.c_str());

     if ((!FUpdating) &&  (fontsize))
     {
         if (fontsize < 1)
         {
             ShowMessage("The number must be between 1 and 1638.");
             FontSize->Text = 1;
         }
         else if (fontsize > 1638)
         {
             ShowMessage("The number must be between 1 and 1638.");
             FontSize->Text = 1638;
         }
         CurrText()->Size = atoi(FontSize->Text.c_str());
     }
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::AlignClick(TObject* Sender)
{
     if ( !FUpdating ) {
        TControl *oAliBtn = (TControl*)(Sender);
        RichEdit->Paragraph->Alignment = (TAlignment)oAliBtn->Tag;
     }
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::FontNameChange(TObject* /*Sender*/)
{
     if ( !FUpdating )
     {
        CurrText()->Name = FontName->Items->Strings[FontName->ItemIndex];
     }
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::FormCloseQuery(TObject* /*Sender*/,
     bool & CanClose)
{
     try {
       CheckFileSave();
     }
     catch (...) {
       CanClose = False;
     }
}
//----------------------------------------------------------------------------
void __fastcall TEditForm::FormActivate(TObject *Sender)
{
//     Application->HelpFile = "RICHEDIT.HLP";
     RichEdit->Paragraph->FirstIndent = int(16);
     RichEdit->Paragraph->LeftIndent = int(16);
     RichEdit->SetFocus();
}
//---------------------------------------------------------------------------
