//---------------------------------------------------------------------------
#ifndef openfileH
#define openfileH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\FileCtrl.hpp>
//---------------------------------------------------------------------------
class TOpenDialogForm : public TForm
{
__published:	// IDE-managed Components
	TFileListBox *FileListBox1;
	TDirectoryListBox *DirectoryListBox1;
	TDriveComboBox *DriveComboBox1;
	TFilterComboBox *FilterComboBox1;
	TEdit *Edit1;
	TLabel *Label1;
	TButton *Button1;
	TButton *Button2;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label7;
	TLabel *Label4;
	TButton *Button3;
private:	// User declarations
public:		// User declarations
	__fastcall TOpenDialogForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TOpenDialogForm *OpenDialogForm;
//---------------------------------------------------------------------------
#endif
