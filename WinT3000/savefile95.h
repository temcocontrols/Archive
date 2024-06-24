//---------------------------------------------------------------------------
#ifndef savefile95H
#define savefile95H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\FileCtrl.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TSaveDialog95Form : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TLabel *Label7;
	TLabel *Label4;
	TButton *Button3;
	TEdit *DirEdit;
	TLabel *Label1;
	TEdit *FileName;
	TFilterComboBox *FilterComboBox;
	TBitBtn *BitBtn1;
	TSpeedButton *DetailsBtn;
	TSpeedButton *ListBtn;
	TListView *FileList;
	void __fastcall BitBtn1Click(TObject *Sender);
	
	
	void __fastcall DetailsBtnClick(TObject *Sender);
	void __fastcall ListBtnClick(TObject *Sender);
	
	void __fastcall DblClick(TObject *Sender);
	void __fastcall OnClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
	TTreeNode *Root;
 char ocwd[65];
 char tmp[65];
 char drive[3];
 char dir[65];
 char nname[13];
 char ext[4];
 char name[13];
 int  nrfiles, nrdirectories;
 char term[13];

 int disk;
 long l;
 int done,colour;
 int last_cur,firstrun,secondrun;
 struct ffblk block;

 struct nume{
				char  names[13];
			};
 struct nume *tab;


 char *file;
 char odir[65];
 int col;

 int i_files;
 int i_directories;
 int i_drives;
 DByte (*files)[13];
 DByte directories[50][13];
public:		// User declarations
	__fastcall TSaveDialog95Form(TComponent* Owner, char *ext);
    void Set(char *filename, char *ext);
};
//---------------------------------------------------------------------------
extern TSaveDialog95Form *SaveDialog95Form;
//---------------------------------------------------------------------------
#endif
