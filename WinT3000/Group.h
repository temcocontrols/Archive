//---------------------------------------------------------------------------
#ifndef GroupH
#define GroupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "cgauges.h"
#include "cspin.h"
#include "perfgrap.h"
#include <ComCtrls.hpp>
#include "baseclas.h"

//---------------------------------------------------------------------------
class TGroupForm : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *ScreenMainMenu;
    TMenuItem *Close;
    TMenuItem *Objects;
    TMenuItem *Refresh;
    TPopupMenu *ScreenPopupMenu;
    TMenuItem *ShowObject;
    TMenuItem *Edit;
    TMenuItem *Delete1;
    TMenuItem *Move1;
    TMenuItem *Insert1;
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormDblClick(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormClick(TObject *Sender);
private:	// User declarations
     DInt mouseX, mouseY;
     AnsiString picture;
     Control_group_point ptrcgrp;
     DInt  current_point;
     DByte panel;
     DInt network;
     DInt print_des(Str_grp_element *group_element, Str_grp_work_data *grp_workdata, DByte panel, DInt network, char len = 0);
//     DInt fnc_group(Control_group_point *block, GGrid *pgrid, byte current_point, int call_type, byte panel, int network, int display, Point_Net *retpoint)
     DInt fnc_group(Control_group_point *block, DByte current_point, DInt call_type, DByte panel, DInt network, DInt display); //, Point_Net *retpoint);

public:		// User declarations
     Point_Net retpoint;
     DInt esc;
    __fastcall TGroupForm(TComponent* Owner, Control_group_point& ptrgrp, DInt currentpoint, DByte panel);
};
//---------------------------------------------------------------------------
extern PACKAGE TGroupForm *GroupForm;
//---------------------------------------------------------------------------
#endif
