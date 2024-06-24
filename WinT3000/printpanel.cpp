//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
#include <vcl\QuickRpt.hpp>

#include "printpanel.h"
#include "preview.h"
#include "report.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TPrintPanelForm *PrintPanelForm;
//---------------------------------------------------------------------------
__fastcall TPrintPanelForm::TPrintPanelForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPrintPanelForm::ShowPreview()
{
  PrevForm->ShowModal();
}//---------------------------------------------------------------------------
void __fastcall TPrintPanelForm::PreviewClick(TObject *Sender)
{
//  QRPrinter->OnPreview = ShowPreview;
  PickReport();
//  aReport->QRPrinter = &ShowPreview;
  Report->Preview();
}
//---------------------------------------------------------------------------
void TPrintPanelForm::PickReport()
{
//   Report=ReportForm->QuickReport;
//   Report->DisplayPrintDialog = true;   //PrintDialogChk->Checked;
   Report->PrinterSetup();
//   Report->Orientation=poPortrait;
}

void __fastcall TPrintPanelForm::Button1Click(TObject *Sender)
{
  PickReport();
  Report->Print();
}
//---------------------------------------------------------------------------

