//---------------------------------------------------------------------------
#ifndef messageboxH
#define messageboxH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
//---------------------------------------------------------------------------
class TMessageBoxForm : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TLabel *Message;
private:	// User declarations
public:		// User declarations
	__fastcall TMessageBoxForm(TComponent* Owner);
	void __fastcall SetMessageBox(int top, int left, int width, int height);
};
//---------------------------------------------------------------------------
extern TMessageBoxForm *MessageBoxForm;
//---------------------------------------------------------------------------
#endif
