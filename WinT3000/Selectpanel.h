//---------------------------------------------------------------------------
#ifndef SelectpanelH
#define SelectpanelH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPanelSelect : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TComboBox *NetworkNumber;
	TComboBox *NetworkName;
	TComboBox *PanelNumber;
	
	
	void __fastcall NetworkNameChange(TObject *Sender);
	void __fastcall PanelNumberChange(TObject *Sender);
	void __fastcall NetworkNumberChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TPanelSelect(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TPanelSelect *PanelSelect;
//---------------------------------------------------------------------------
#endif
