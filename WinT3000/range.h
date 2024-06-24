//---------------------------------------------------------------------------
#ifndef rangeH
#define rangeH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TRangeForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TCheckBox *CheckBox;
	TButton *OkButton;
	TButton *CancelButton;
	TButton *EditButton;
	TLabel *Label3;
	TListBox *StandardList;
	TBevel *Bevel1;
	TLabel *Label1;
	TListBox *CustomList;
	TLabel *Label4;
	TBevel *Bevel2;
	TLabel *Label2;
	TListBox *AnalogList;
	void __fastcall StandardClick(TObject *Sender);
	void __fastcall CustomClick(TObject *Sender);
	void __fastcall AnalogClick(TObject *Sender);
	void __fastcall InvertClick(TObject *Sender);
	void __fastcall CustomListDblClick(TObject *Sender);
	void __fastcall StandardListDblClick(TObject *Sender);
	void __fastcall AnalogListDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TRangeForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TRangeForm *RangeForm;
//---------------------------------------------------------------------------
#endif
