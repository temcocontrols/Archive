//---------------------------------------------------------------------------
#ifndef printpanelH
#define printpanelH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\QuickRpt.hpp>
//---------------------------------------------------------------------------
class TPrintPanelForm : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TPanel *Panel2;
	TButton *Preview;
	TPanel *Panel10;
	TCheckBox *CheckBox13;
	TCheckBox *CheckBox15;
	TPanel *Panel3;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBox4;
	TCheckBox *CheckBox9;
	TCheckBox *CheckBox10;
	TCheckBox *CheckBox11;
	TPanel *Panel4;
	TCheckBox *CheckBox21;
	TCheckBox *WrsAllCheckBox;
	TComboBox *WrsComboBox;
	TPanel *Panel5;
	TCheckBox *ArsCheckBox;
	TComboBox *ArsComboBox;
	TCheckBox *CheckBox5;
	TPanel *Panel6;
	TCheckBox *CheckBox6;
	TCheckBox *AysCheckBox;
	TComboBox *AysComboBox;
	TPanel *Panel7;
	TCheckBox *CheckBox7;
	TCheckBox *PrgsCheckBox;
	TComboBox *PrgsComboBox;
	TPanel *Panel8;
	TCheckBox *CheckBox14;
	TCheckBox *GroupsCheckBox;
	TComboBox *GroupsComboBox;
	TPanel *Panel9;
	TCheckBox *CheckBox12;
	TCheckBox *AmonsCheckBox;
	TComboBox *AmonsComboBox;
	void __fastcall PreviewClick(TObject *Sender);
    void __fastcall ShowPreview();
	void __fastcall Button1Click(TObject *Sender);
    
private:	// User declarations
//    TQuickRep *aReport;
    TQuickRep * FReport;
	void SetReport(TQuickRep *);
    void PickReport();
public:		// User declarations
	__fastcall TPrintPanelForm(TComponent* Owner);
    __property TQuickRep * Report = {read = FReport, write = SetReport};
};
//---------------------------------------------------------------------------
extern TPrintPanelForm *PrintPanelForm;
//---------------------------------------------------------------------------
#endif
