//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "report.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TReportForm *ReportForm;
//---------------------------------------------------------------------------
__fastcall TReportForm::TReportForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TReportForm::QRBand5AfterPrint(bool BandPrinted)
{
   QuickReport->NewPage();
}
//---------------------------------------------------------------------------
void __fastcall TReportForm::QRDBText1Print(TObject *sender, AnsiString &Value)
{
   if(Value.ToInt()==33 || Value.ToInt()==65 || Value.ToInt()==97)
	   QuickReport->NewPage();
}
//---------------------------------------------------------------------------
