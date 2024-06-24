//---------------------------------------------------------------------------
#ifndef programH
#define programH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Dialogs.hpp>

#include "baseclas.h"

//---------------------------------------------------------------------------
class TProgramForm : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *RichEdit1;
	TMainMenu *MainMenu1;
	TMenuItem *Send1;
	TMenuItem *Clear1;
	TMenuItem *Print1;
	TMenuItem *LoadFile1;
	TMenuItem *SaveFile1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall SaveFile1Click(TObject *Sender);
	void __fastcall LoadFile1Click(TObject *Sender);
private:	// User declarations
    Str_program_point pPrg;
    DInt current_point;
    AnsiString programName;
public:		// User declarations
	__fastcall TProgramForm(TComponent* Owner, Str_program_point& ptrprg, DInt currentpoint );
};
//---------------------------------------------------------------------------
extern TProgramForm *ProgramForm;
//---------------------------------------------------------------------------
#endif
