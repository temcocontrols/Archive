//---------------------------------------------------------------------------
#ifndef serialintH
#define serialintH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
//---------------------------------------------------------------------------
class TSerialInterfaceForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *InterfaceLabel;
	TLabel *ConnectionLabel;
	TLabel *BaudRateLabel;
	TComboBox *ConnectionBox;
	TComboBox *BaudRateBox;
	TButton *OKButton;
	TButton *CancelButton;
	TGroupBox *ModemGroupBox;
	TLabel *ModemTypeLabel;
	TComboBox *ModemTypeBox;
	TLabel *InitStringLabel;
	TButton *ModemCheckButton;
	TEdit *InitStringEdit;
	TComboBox *InterfaceBox;
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall InterfaceBoxChange(TObject *Sender);
	void __fastcall ConnectionBoxChange(TObject *Sender);
	void __fastcall BaudRateBoxChange(TObject *Sender);
	void __fastcall ModemTypeBoxChange(TObject *Sender);
	
	void __fastcall InitStringEditChange(TObject *Sender);
private:	// User declarations
    int currentCom;
    int changeState;
public:		// User declarations
	__fastcall TSerialInterfaceForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSerialInterfaceForm *SerialInterfaceForm;
//---------------------------------------------------------------------------
#endif
