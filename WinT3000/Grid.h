//---------------------------------------------------------------------------
#ifndef GridH
#define GridH
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
#include "baseclas.h"
#include <vcl\Menus.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Dialogs.hpp>
//#include "t3000reg.h"
#include "tdbgrid.h"
#include <Db.hpp>
//---------------------------------------------------------------------------
class TGridForm : public TForm
{
__published:	// IDE-managed Components
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
	TMenuItem *Color1;
	TMenuItem *ShowObject2;
	TMenuItem *N3;
	TMenuItem *Options1;
	TMenuItem *Refresh;
	TMenuItem *Font;
	TMenuItem *Color;
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
	TMainMenu *MainMenu3;
	TMenuItem *MenuItem24;
	TMenuItem *PrintItem;
	TMenuItem *MenuItem34;
	TMenuItem *MenuItem35;
	TMenuItem *MenuItem36;
	TMenuItem *Note;
	TPopupMenu *PopupMenu3;
	TMenuItem *MenuItem25;
	TMenuItem *MenuItem26;
	TMenuItem *MenuItem33;
	TMenuItem *MenuItem37;
	TMenuItem *MenuItem38;
	TMenuItem *MenuItem39;
	TPanel *Panel1;
	TFontDialog *FontDialog;
	TColorDialog *ColorDialog;
 	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ShowObject2Click(TObject *Sender);
	void __fastcall CutClick(TObject *Sender);
	void __fastcall CopyClick(TObject *Sender);
	void __fastcall PasteClick(TObject *Sender);
	void __fastcall FontClick(TObject *Sender);
	void __fastcall ColorClick(TObject *Sender);
	void __fastcall CloseClick(TObject *Sender);
	void __fastcall OnColExit(TObject *Sender);
	void __fastcall OnKeyPress(TObject *Sender, char &Key);
	void __fastcall OnColEnter(TObject *Sender);
	void __fastcall OnActivate(TObject *Sender);
	void __fastcall OnDblClick(TObject *Sender);

	

	void __fastcall Refresh1Click(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);

	void __fastcall DBGridT3000DrawColumnCell(TObject *Sender, const TRect &Rect,
	int DataCol, TColumn *Column, TGridDrawState State);
	
    
    

    
private:	// User declarations
    TDataSource   *dataSource;          // points to its data source
    TTable        *Table;               // points to its table
    TTDBGrid    *DBGridT3000;
    int           objectIndex;          // index in gridTable[]
	DUint    	  pointType;            // point type
	DUint    	  panel;                // panel
    DInt          network;              // network
    DInt          accesstype;           // -1 = individual, 0 = block
	int           recordCount;          // number of records to be displayed
    int           fieldCount;           // number of fields
    int           refreshTime;          // used as a counter to trigger the
                                        // refresh time event
    int           refreshTimeConst;     // refresh time constant in seconds
    bool          refreshTimeFlag;      // is 'true' if the grid should refresh
                                        // at a specified refreshTimeConst time
    int           menuType;             // the menu number to use with the grid
    bool          emptyTable;           // 'true' if the table should empty
                                        // its contents before to add the new
                                        // records. All the table points should
                                        // empty their contents
    DLong         readOnlyFields;       // specified the fields that are read only
    int           editField;            // monitors the field editing:
    int           changeStatus;         // monitors the field editing:
                                        //   1 if the at least one field was edited
                                        //   0 no field where edited
                                        // This flag is used when refreshing or
                                        // close the window. If the flag is 1
                                        // the new data should be sent to the
                                        // panel before refreshing
	char          refresh_table[128];   // keep tracks of record that where editet
    long          currentRec;           // the current record ( 1 -> MAX )
    long          currentCol;           // the current column ( 1 -> MAX )
    long          lockedRow;            // the row that are locked for updating
                                        // ( 1 -> MAX )

//Added By Paul Mao on 6/8/97 for GRID edit:
//	int 		     PickFieldsNum;
//    IndexedPickList  IndexedPickLists[10];
	char ClipBuffer[100];
    int  ClipBufferEmpty;

public:		// User declarations
	__fastcall TGridForm(TComponent* Owner, DUint ptype, int recs, int oindex=0);
    void __fastcall Setup(DUint panel, DInt network, TDataSource *ds, TTable *table,
                          char *title=NULL, int width=-1, int height = -1,
                          DLong readonly=0, int refreshTime=-1, int menu=1,
                          int emptytable=true);
    void __fastcall setUpData(void);
    DUint __fastcall getPointType(void);
//Updated by Paul Mao on 13/8/97 for the GRID EDIT:
//The reason is: after each refresh, the current edit data are lost.
    void __fastcall updateRecTable(int recno);

    void __fastcall SendIndivPoint(DInt pointnumber);
    void __fastcall RefreshGrid(void);
    bool __fastcall readRefreshTimeFlag(void){return refreshTimeFlag;};
    void __fastcall setRefreshTimeFlag(bool flag){ refreshTimeFlag=flag;};
    int  __fastcall readRefreshTimeConst(void){return refreshTimeConst;};
    int  __fastcall readRefreshTime(void){return refreshTime;};
    void __fastcall setRefreshTime(int rt){refreshTime=rt;};
    void __fastcall dataChange(TField *Field);
    void __fastcall onUpdate();
    int  __fastcall checkFinishedEdit(char key, int exitfield);
    void __fastcall keyPressIn(long row, long field, char key, int exitfield=0);
    void __fastcall keyPressOut(long row, long field, char key, int exitfield=0);
    void __fastcall keyPressVar(long row, long field, char key, int exitfield=0);
    void __fastcall keyPressCon(long row, long field, char key, int exitfield=0);
    void __fastcall keyPressWr(long row, long field, char key, int exitfield=0);
    void __fastcall keyPressAr(long row, long field, char key, int exitfield=0);
    void __fastcall keyPressPrg(long row, long field, char key, int exitfield=0);
    void __fastcall keyPressGrp(long row, long field, char key, int exitfield=0);
    void __fastcall keyPressAy(long row, long field, char key, int exitfield=0);

    void __fastcall checkreadonlyIn( long row );
    void __fastcall checkreadonlyOut( long row );
    void __fastcall checkreadonlyVar( long row );
    void __fastcall checkreadonlyCon( long row );
    void __fastcall checkreadonlyWr( long row );
    void __fastcall checkreadonlyAr( long row );
    void __fastcall checkreadonlyPrg( long row );
    void __fastcall checkreadonlyGrp( long row );
    void __fastcall checkreadonlyAY( long row );

//Added by Paul Mao for "edit by list selection"(GRID) on 4/8/97:
//    void __fastcall editSetup(DInt point_type);
//    void __fastcall editSetup(int num, IndexedPickList *IndexedPickLists);
    void __fastcall writeback();
    int __fastcall getpointlabel(Point_T3000 point, char *label);

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
