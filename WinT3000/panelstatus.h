//---------------------------------------------------------------------------
#ifndef panelstatusH
#define panelstatusH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPanelStatusForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TLabel *Label11;
	TEdit *Edit1;
	TLabel *Label12;
	TEdit *Edit2;
	TLabel *Label14;
	TEdit *Edit3;
	TGroupBox *GroupBox2;
	TLabel *Label6;
	TEdit *Edit4;
	TLabel *Label7;
	TEdit *Edit5;
	TGroupBox *GroupBox3;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *Edit6;
	TEdit *Edit7;
	TLabel *Label9;
	TEdit *Edit8;
	TEdit *Edit9;
	TLabel *Label10;
	TBevel *Bevel1;
	TGroupBox *GroupBox4;
	TLabel *Label8;
	TEdit *Edit10;
	TButton *Button1;
	TBevel *Bevel2;
	TButton *Button3;
	TButton *Button2;
private:	// User declarations
public:		// User declarations
	__fastcall TPanelStatusForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TPanelStatusForm *PanelStatusForm;
//---------------------------------------------------------------------------
#endif
