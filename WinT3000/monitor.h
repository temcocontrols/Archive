//---------------------------------------------------------------------------
#ifndef monitorH
#define monitorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "perfgrap.h"
#include <Buttons.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TGraphForm : public TForm
{
__published:	// IDE-managed Components
    TPerformanceGraph *PerformanceGraph1;
    TMainMenu *MainMenu1;
    TMenuItem *Exit1;
    TMenuItem *Load1;
    TMenuItem *Save1;
    TMenuItem *Delete1;
    TMenuItem *Reset1;
    TMenuItem *TimeBase1;
    TPanel *Panel1;
    TButton *Button1;
    TButton *Button2;
    TButton *Button3;
    TButton *Button4;
    TButton *Button5;
    TButton *Button6;
    TButton *Button7;
    TButton *Button8;
    TButton *Button9;
    TButton *Button10;
    TButton *Button11;
    TButton *Button12;
    TButton *Button13;
    TButton *Button14;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label5;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
private:	// User declarations
public:		// User declarations
    __fastcall TGraphForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGraphForm *GraphForm;
//---------------------------------------------------------------------------
#endif
