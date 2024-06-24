//---------------------------------------------------------------------------
#ifndef LTGComPortListH
#define LTGComPortListH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
//---------------------------------------------------------------------------
class TComPortListForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox1;
	TLabel *Label1;
	TButton *OkButton;
	TButton *CancelButton;
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall OkButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TComPortListForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TComPortListForm *ComPortListForm;
//---------------------------------------------------------------------------
#endif
