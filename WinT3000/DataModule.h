//---------------------------------------------------------------------------
#ifndef DataModuleH
#define DataModuleH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\DB.hpp>
#include <vcl\DBTables.hpp>
#include <Db.hpp>
//---------------------------------------------------------------------------
class TDM : public TDataModule
{
__published:	// IDE-managed Components
	TDataSource *DataSource;
	TDatabase   *T3000Database;
	TTable      *InputsTable;
	TTable      *OutputsTable;
	TTable *PointsTable;
	TTable *VarsTable;
	TStringField *InputsTableNr;
	TStringField *InputsTableDescription;
	TStringField *InputsTableAutoMan;
	TStringField *InputsTableValue;
	TStringField *InputsTableUnits;
	TStringField *InputsTableRange;
	TStringField *InputsTableCal;
	TStringField *InputsTableFilter;
	TStringField *InputsTableD;
	TStringField *InputsTableLabel;
	TStringField *OutputsTableNr;
	TStringField *OutputsTableDescription;
	TStringField *OutputsTableAutoMan;
	TStringField *OutputsTableValue;
	TStringField *OutputsTableUnits;
	TStringField *OutputsTableRange;
	TStringField *OutputsTableStringField0;
	TStringField *OutputsTableStringField100;
	TStringField *OutputsTableD;
	TStringField *OutputsTableLabel;
	TStringField *VarsTableNr;
	TStringField *VarsTableDescription;
	TStringField *VarsTableAutoMan;
	TStringField *VarsTableValue;
	TStringField *VarsTableUnits;
	TStringField *VarsTableLabel;
	TTable *ArsTable;
	TStringField *ArsTableNr;
	TStringField *ArsTableDescription;
	TStringField *ArsTableAuto;
	TStringField *ArsTableValue;
	TStringField *ArsTableLabel;
	TTable *SysListTable;
	TStringField *SysListTableNr;
	TStringField *SysListTableSystemname;
	TStringField *SysListTablePhonenumber;
	TStringField *SysListTableDirectory;
	TDataSource *InputsDataSource;
	TDataSource *OutputsDataSource;
	TDataSource *VarsDataSource;
	TDataSource *WrsDataSource;
	TDataSource *ArsDataSource;
	TTable *WrsTable;
	TTable *ConsTable;
	TDataSource *ConsDataSource;
	TTable *PrgsTable;
	TTable *ScreensTable;
	TDataSource *PrgsDataSource;
	TDataSource *ScreensDataSource;
	TDataSource *SysListDataSource;
	TTable *AysTable;
	TDataSource *AysDataSource;
	TTable *NetStatTable;
	TDataSource *NetStatDataSource;
	TStringField *NetStatTableNr;
	TStringField *NetStatTablePanelname;
	TStringField *NetStatTablePaneltype;
	TStringField *NetStatTableVersion;
	TStringField *NetStatTableProgram;
	TFloatField *NetStatTableImportNet;
	TFloatField *NetStatTableExportNet;
	TStringField *WrsTableNr;
	TStringField *WrsTableDescription;
	TStringField *WrsTableAutoMan;
	TStringField *WrsTableOutput;
	TStringField *WrsTableHoliday1;
	TStringField *WrsTableState1;
	TStringField *WrsTableHoliday2;
	TStringField *WrsTableState2;
	TStringField *PrgsTableNr;
	TStringField *PrgsTableDescription;
	TStringField *PrgsTableStatus;
	TStringField *PrgsTableAutoMan;
	TStringField *PrgsTableSize;
	TStringField *PrgsTableRunstatus;
	TStringField *PrgsTableLabel;
	TStringField *ScreensTableNr;
	TStringField *ScreensTableDescription;
	TStringField *ScreensTableLabel;
	TStringField *ScreensTablePicture;
	TStringField *ScreensTableMode;
	TStringField *ScreensTableRefresh;
	TStringField *ConsTableNr;
	TStringField *ConsTableInput;
	TStringField *ConsTableUnits;
	TStringField *ConsTableAutoMan;
	TStringField *ConsTableOutput;
	TStringField *ConsTableSetpoint;
	TStringField *ConsTableSetvalue;
	TStringField *ConsTableUnit;
	TStringField *ConsTableAction;
	TStringField *ConsTableProp;
	TStringField *ConsTableReset;
	TStringField *ConsTableRate;
	TStringField *ConsTableBias;
	TStringField *AysTableNr;
	TStringField *AysTableLabel;
	TStringField *AysTableLength;
	TDataSource *MH_DataSourceUserList;
	TTable *Table_UserList;
	void __fastcall OnChange(TObject *Sender, TField *Field);
	void __fastcall OnUpdate(TObject *Sender);
    void __fastcall DMCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    TGridForm *grid;
	__fastcall TDM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TDM *DM;
//---------------------------------------------------------------------------
#endif
