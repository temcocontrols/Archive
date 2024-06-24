//---------------------------------------------------------------------------
#ifndef MH_UserListH
#define MH_UserListH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\DBGrids.hpp>
#include "Grids.hpp"
#include <vcl\Menus.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMH_UserListForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGrid_UserList;
	TMainMenu *MainMenu_UserList;
	TMenuItem *MenuItem_Change;
	TMenuItem *MenuItem_Add;
	TMenuItem *MenuItem_Delete;
	TBevel *Bevel1;
	TMenuItem *Close1;
	void __fastcall OnShow_UserListForm(TObject *Sender);
	void __fastcall OnClose_UserListForm(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TMH_UserListForm(TComponent* Owner);
    void 	__fastcall 	UserList(void);
};
//---------------------------------------------------------------------------
extern TMH_UserListForm *MH_UserListForm;
//---------------------------------------------------------------------------
#endif
