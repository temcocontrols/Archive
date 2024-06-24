//---------------------------------------------------------------------------
#ifndef MH_defaultH
#define MH_defaultH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Dialogs.hpp>
//  Mao Hui code ------------------------------------------------------------
#include <dir.h>
//---------------------------------------------------------------------------
class TMH_default_form : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *Dialog_select_file;
	TLabel *Label_programs;
	TLabel *Label_descriptors;
	TLabel *Label_control_basic;
	TLabel *Label_monitor_data;
	TLabel *LabelGraphics;
	TButton *Button_ok;
	TButton *Button_cancel;
	TEdit *Edit_programs;
	TEdit *Edit_descriptors;
	TEdit *Edit_control_basic;
	TEdit *Edit_monitor_data;
	TEdit *Edit_Graphics;
	void __fastcall Button_okClick(TObject *Sender);
	void __fastcall Button_cancelClick(TObject *Sender);
	void __fastcall OnKeyPress_ProgramsEdit(TObject *Sender, char &Key);
	void __fastcall OnKeyPress_DescriptorsEdit(TObject *Sender, char &Key);
	void __fastcall OnKeyPress_ControlBasicEdit(TObject *Sender, char &Key);
	void __fastcall OnKeyPress_MonitorDataEdit(TObject *Sender, char &Key);
	void __fastcall OnKeyPress_Graphics(TObject *Sender, char &Key);
	void __fastcall OnShow_FilePathForm(TObject *Sender);
private:	// User declarations
	char	Path_default_program[64];
    char	Path_temporary[64];
    char	Path_programs[64];
	char 	Path_descriptors[64];
	char 	Path_control_basic[64];
	char 	Path_monitor_data[64];
	char 	Path_Graphics[64];
    struct 	ffblk 	ffblk;
public:		// User declarations
	__fastcall TMH_default_form(TComponent* Owner);
    void __fastcall Default_program(void);
    void __fastcall File_path(void);
};
//---------------------------------------------------------------------------
extern TMH_default_form *MH_default_form;
//---------------------------------------------------------------------------
#endif
