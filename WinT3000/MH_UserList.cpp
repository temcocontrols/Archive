//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "MH_UserList.h"
//  Mao Hui code---------------------------------------------------------------------------
#include "Grid.h"
#include "DataModule.h"
#include "Define.h"

extern 	char access_level;
extern 	char right_access;
extern  long rights_access;
extern  char screen_right[8];

extern int check_access(int ptype, int num=-1, int mes=R_NA);
//---------------------------------------------------------------------------
#pragma link "Grids"
#pragma resource "*.dfm"
TMH_UserListForm *MH_UserListForm;
//---------------------------------------------------------------------------
__fastcall TMH_UserListForm::TMH_UserListForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void	__fastcall 	TMH_UserListForm::UserList(void)
{
	if( check_access(12)!=R_NA )
    {
		ShowModal();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMH_UserListForm::OnShow_UserListForm(TObject *Sender)
{
	DM -> Table_UserList -> Active = false;
	DM -> Table_UserList -> DisableControls();
	DM -> Table_UserList -> Active = true;
	DM -> Table_UserList -> First();
	DM -> Table_UserList -> Refresh();
    DM -> Table_UserList -> EnableControls();
}
//---------------------------------------------------------------------------
void __fastcall TMH_UserListForm::OnClose_UserListForm(TObject *Sender,
	TCloseAction &Action)
{
    if ((DM -> Table_UserList -> State == dsEdit)||(DM -> Table_UserList -> State == dsInsert))
    {
		DM -> Table_UserList -> Post();
    }
	DM -> Table_UserList -> Close();
}
//---------------------------------------------------------------------------
