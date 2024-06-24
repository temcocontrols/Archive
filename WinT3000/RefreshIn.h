//---------------------------------------------------------------------------
#ifndef RefreshInH
#define RefreshInH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
//---------------------------------------------------------------------------
class TRefreshInForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *RefreshIn;
	TButton *OkButton;
	TButton *CancelButton;
	void __fastcall procKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
	__fastcall TRefreshInForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TRefreshInForm *RefreshInForm;
//---------------------------------------------------------------------------
#endif
