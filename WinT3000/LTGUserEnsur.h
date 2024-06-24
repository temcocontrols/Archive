//---------------------------------------------------------------------------
#ifndef LTGUserEnsurH
#define LTGUserEnsurH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
//---------------------------------------------------------------------------
class TUserEnsureForm : public TForm
{
__published:	// IDE-managed Components
	TButton *OkButton;
	TButton *CancelButton;
	TLabel *EnsureWinLabel;
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);

private:	// User declarations

public:		// User declarations
   	bool     Ok;
	__fastcall TUserEnsureForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TUserEnsureForm *UserEnsureForm;
//---------------------------------------------------------------------------
#endif
