//---------------------------------------------------------------------------
#ifndef systemlistH
#define systemlistH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\DBGrids.hpp>
#include "Grids.hpp"
#include <vcl\Menus.hpp>
#include <vcl\DB.hpp>
#include <vcl\DBTables.hpp>
//---------------------------------------------------------------------------
class TSysListForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridT3000;
	TMainMenu *MainMenu1;
	TMenuItem *Close;
	TMenuItem *Edit;
	TMenuItem *Undo;
	TMenuItem *N2;
	TMenuItem *Cut;
	TMenuItem *Copy;
	TMenuItem *Paste;
	TMenuItem *Delete;
	TMenuItem *Print;
	TMenuItem *Options;
	TMenuItem *Font1;
	TMenuItem *Colors1;
	TMenuItem *Connect;
	TPopupMenu *PopupMenu1;
	TMenuItem *Close1;
	TMenuItem *Connect2;
	TMenuItem *N1;
	TMenuItem *Undo1;
	TMenuItem *Cut1;
	TMenuItem *Copy1;
	TMenuItem *Paste1;
	TMenuItem *Delete1;
	TMenuItem *N3;
	TMenuItem *Options1;
	TMenuItem *Font;
	TMenuItem *Colors;
	TDataSource *DataSource2;
	TTable *SysListTable;
	TStringField *StringField11;
	TStringField *StringField12;
	TStringField *StringField13;
	TStringField *StringField14;
	TStringField *StringField15;
	TStringField *StringField16;
	TStringField *StringField17;
	TStringField *StringField18;
	TStringField *StringField19;
	TStringField *StringField20;
private:	// User declarations
public:		// User declarations
	__fastcall TSysListForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSysListForm *SysListForm;
//---------------------------------------------------------------------------
#endif
