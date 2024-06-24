//---------------------------------------------------------------------------
#ifndef objectH
#define objectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include "baseclas.h"

//---------------------------------------------------------------------------
class TObjectForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TPanel *Panel2;
    TLabel *Label4;
    TEdit *Value;
    TEdit *Auto;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TEdit *high;
    TEdit *low;
    TComboBox *display;
    TColorDialog *highColorDialog;
    TButton *highcolor;
    TButton *lowcolor;
    TButton *displaycolor;
private:	// User declarations
    Str_grp_element   pgrp;
    Str_grp_work_data pworkdata;
public:		// User declarations
    __fastcall TObjectForm(TComponent* Owner, Str_grp_element *ppgrp, Str_grp_work_data *pworkdata);
};
//---------------------------------------------------------------------------
extern PACKAGE TObjectForm *ObjectForm;
//---------------------------------------------------------------------------
#endif
