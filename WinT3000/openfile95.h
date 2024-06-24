//---------------------------------------------------------------------------
#ifndef openfile95H
#define openfile95H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\FileCtrl.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TOpenDialog95Form : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TLabel *Label7;
	TLabel *Label4;
	TButton *Button3;
	TEdit *DirEdit;
	TLabel *Label1;
	TEdit *Edit2;

	TFilterComboBox *FilterComboBox;
	TBitBtn *BitBtn1;
	TSpeedButton *DetailsBtn;
	TSpeedButton *ListBtn;
	TListView *FileList;
	TTreeView *DirTree;
	void __fastcall BitBtn1Click(TObject *Sender);
	
	
	void __fastcall DetailsBtnClick(TObject *Sender);
	void __fastcall ListBtnClick(TObject *Sender);
	
	void __fastcall DblClick(TObject *Sender);
	void __fastcall OnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TOpenDialog95Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TOpenDialog95Form *OpenDialog95Form;
//---------------------------------------------------------------------------
#endif
