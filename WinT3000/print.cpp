//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
#include <vcl\Quickrpt.hpp>

#include "print.h"
#include "preview.h"
#include "report.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TPrintForm *PrintForm;
//---------------------------------------------------------------------------
__fastcall TPrintForm::TPrintForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPrintForm::ShowPreview()
{
  PrevForm->ShowModal();
}//---------------------------------------------------------------------------
void __fastcall TPrintForm::PreviewClick(TObject *Sender)
{
//  QRPrinter->OnPreview = ShowPreview;
  PickReport();
  aReport->Preview();
}
//---------------------------------------------------------------------------
void TPrintForm::PickReport()
{
   aReport=ReportForm->QuickReport;
//   aReport->DisplayPrintDialog = true;   //PrintDialogChk->Checked;
//   aReport->Orientation=poPortrait;
}

