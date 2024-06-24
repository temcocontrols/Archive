//---------------------------------------------------------------------------
#ifndef TableFormH
#define TableFormH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\DB.hpp>
#include <vcl\DBTables.hpp>
#include <vcl\DBGrids.hpp>
#include "Grids.hpp"
#include "define.h"
#include <vcl\Menus.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TGridForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridT3000;
	TMainMenu *MainMenu1;
	TMenuItem *Edit;
	TMenuItem *Paste;
	TMenuItem *Copy;
	TMenuItem *Cut;
	TMenuItem *N2;
	TMenuItem *Undo;
	TMenuItem *Close;
	TMenuItem *Options;
	TMenuItem *Print;
	TBevel *Bevel1;
	TMenuItem *Delete;
	TPopupMenu *PopupMenu1;
	TMenuItem *Close1;
	TMenuItem *N1;
	TMenuItem *Undo1;
	TMenuItem *Cut1;
	TMenuItem *Copy1;
	TMenuItem *Paste1;
	TMenuItem *Delete1;
	TMenuItem *Refresh1;
	TMenuItem *ShowObject1;
	TMenuItem *Font1;
	TMenuItem *Colors1;
	TMenuItem *ShowObject2;
	TMenuItem *N3;
	TMenuItem *Options1;
	TMenuItem *Refresh;
	TMenuItem *Font;
	TMenuItem *Colors;
	TMainMenu *MainMenu2;
	TMenuItem *MenuItem1;
	TMenuItem *MenuItem2;
	TMenuItem *MenuItem3;
	TMenuItem *MenuItem4;
	TMenuItem *MenuItem5;
	TMenuItem *MenuItem6;
	TMenuItem *MenuItem7;
	TMenuItem *MenuItem8;
	TMenuItem *Connect;
	TMenuItem *MenuItem9;
	TMenuItem *MenuItem10;
	TMenuItem *MenuItem11;
	TMenuItem *MenuItem12;
	TPopupMenu *PopupMenu2;
	TMenuItem *MenuItem13;
	TMenuItem *Connect2;
	TMenuItem *MenuItem14;
	TMenuItem *MenuItem15;
	TMenuItem *MenuItem16;
	TMenuItem *MenuItem17;
	TMenuItem *MenuItem18;
	TMenuItem *MenuItem19;
	TMenuItem *MenuItem20;
	TMenuItem *MenuItem21;
	TMenuItem *MenuItem22;
	TMenuItem *MenuItem23;
 	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ShowObject2Click(TObject *Sender);
private:	// User declarations
    TDataSource   *dataSource;          // points to its data source
    TTable        *Table;               // points to its table
    int           objectIndex;          // 
	DUint    	  pointType;
	int           recordCount;
    int           fieldCount;
    int           refreshTime;
    bool          refreshTimeFlag;
    int           menuType;
    bool          emptyTable;
    DLong         readOnlyFields;
public:		// User declarations
	__fastcall TGridForm(TComponent* Owner, DUint ptype, int recs, int oindex=0);
    void __fastcall Setup(TDataSource *ds, TTable *table, char *title=NULL, int width=-1,
                          DLong readonly=0, int refreshTime=-1, int menu=1, int emptytable=true);
    void __fastcall setUpData(void);
    void __fastcall refreashTable(void);
    DUint __fastcall getPointType(void);
};
//---------------------------------------------------------------------------
#define MAXGRIDTABLE 10

typedef struct {
  TGridForm *grid;
} GRIDTABLE;
//---------------------------------------------------------------------------
extern TGridForm *GridForm;
//---------------------------------------------------------------------------
#endif
