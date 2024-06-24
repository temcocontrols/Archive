//---------------------------------------------------------------------------
#ifndef LTGLoginInitH
#define LTGLoginInitH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TLoginForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *UserNameEdit;
	TEdit *PasswordEdit;
	TPanel *Panel2;
	TButton *Button2;
	TButton *Button3;
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall UserNameEditKeyPress(TObject *Sender, char &Key);
	void __fastcall PasswordEditKeyPress(TObject *Sender, char &Key);
	void __fastcall PasswordEditEnter(TObject *Sender);
	
	
    
private:	// User declarations
public:		// User declarations
	__fastcall TLoginForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TLoginForm *LoginForm;
//---------------------------------------------------------------------------
#endif
