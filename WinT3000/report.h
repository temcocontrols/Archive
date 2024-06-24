//---------------------------------------------------------------------------
#ifndef reportH
#define reportH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\QuickRpt.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\DB.hpp>
#include <vcl\DBTables.hpp>
#include <Db.hpp>
#include <Qrctrls.hpp>
#include <quickrpt.hpp>
//---------------------------------------------------------------------------
class TReportForm : public TForm
{
__published:	// IDE-managed Components
    TQuickRep *QuickReport;
	TQRBand *QRBand3;
	TQRLabel *QRLabel2;
	TQRBand *QRBand2;
	TQRSysData *QRSysData1;
	TQRSysData *QRSysData2;
	TQRLabel *QRLabel3;
	TQRBand *QRBand1;
	TQRLabel *QRLabel4;
	TQRLabel *QRLabel5;
	TQRBand *DetailBand;
	TQRDBText *QRDBText2;
	TDataSource *DataSource1;
	TTable *Table1;
	TQRDBText *QRDBText1;
	TQRBand *QRBand4;
	TQRSysData *QRSysData4;
	TQRLabel *QRLabel1;
	TQRLabel *QRLabel6;
	TQRLabel *QRLabel7;
	TQRDBText *QRDBText3;
	TQRDBText *QRDBText4;
	TQRDBText *QRDBText5;
	TQRDBText *QRDBText6;
	TQRLabel *QRLabel8;
	TQRLabel *QRLabel9;
	TQRLabel *QRLabel10;
	TQRLabel *QRLabel11;
	TQRLabel *QRLabel12;
	TQRDBText *QRDBText7;
	TQRDBText *QRDBText8;
	TQRDBText *QRDBText9;
	TQRDBText *QRDBText10;
	void __fastcall QRBand5AfterPrint(bool BandPrinted);

	
	void __fastcall QRDBText1Print(TObject *sender, AnsiString &Value);
private:	// User declarations
public:		// User declarations
	__fastcall TReportForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TReportForm *ReportForm;
//---------------------------------------------------------------------------
#endif
