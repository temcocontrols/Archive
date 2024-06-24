//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "MH_default.h"
//  Mao Hui code----------------------------------------------------------
#define	uint	unsigned int
#include <dos.h>
#include "Aio.h"
extern Panel *ptr_panel;
extern int net_call(int command, int arg,  char *data, uint *length , int dest,
				 int network, int others=0, int timeout=TIMEOUT_NETCALL,
				 char *returnargs=NULL, int *length_returnargs=NULL,
				 char *sendargs=NULL, int length_sendargs=0, char bytearg=0);
extern void save_config(int what);                 
extern int networkaddress;

extern int local_panel;
extern char default_prg[13];
extern int  station_num;
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMH_default_form *MH_default_form;
//---------------------------------------------------------------------------
__fastcall TMH_default_form::TMH_default_form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMH_default_form::Default_program(void)
{
	int Length_string;
    int Counter;
  	TOpenOptions 	Dialog_options;

	Dialog_options << ofFileMustExist << ofNoChangeDir;
    Dialog_options << ofShowHelp;
  	Dialog_select_file->Options = Dialog_options;
	Dialog_select_file->InitialDir = AnsiString(ptr_panel->Program_Path);
    if(local_panel)
    {
	    Dialog_select_file->FileName = AnsiString(ptr_panel->Default_Program);
    }
    else
    {
	    Dialog_select_file->FileName = AnsiString(default_prg);
    }

    if( Dialog_select_file->Execute() )
  	{
    	strcpy(Path_default_program,Dialog_select_file->FileName.c_str());
		Length_string=strlen(Path_default_program);
        Counter=Length_string;
        while (Path_default_program[Counter]!='\\')
        {
        	Counter--;
       	}
        strncpy(ptr_panel->Program_Path,Path_default_program,Counter);
		ptr_panel->Program_Path[Counter]='\0';
		strcpy(ptr_panel->Default_Program,&Path_default_program[Counter+1]);

		if(local_panel)
		{
			save_config(CNF_PATH_INFO);
			strcpy(default_prg, ptr_panel->Default_Program);
        }
		else
		{
        	uint	i;
			i = 13;
//  NO.002			net_call(50+100, 33, default_prg, &i, station_num, networkaddress);
        }
  	}
}
//---------------------------------------------------------------------------
void __fastcall TMH_default_form::File_path(void)
{
	Edit_programs->Text = AnsiString(ptr_panel->Program_Path);
    Edit_descriptors->Text = AnsiString(ptr_panel->Descriptor_Path);
    Edit_control_basic->Text = AnsiString(ptr_panel->Basic_Path);
    Edit_monitor_data->Text = AnsiString(ptr_panel->Monitor_Path);
    Edit_Graphics->Text = AnsiString(ptr_panel->Graphics_Path);
	ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMH_default_form::Button_okClick(TObject *Sender)
{
   	strcpy(Path_programs,Edit_programs->Text.c_str());
	strcpy(Path_descriptors,Edit_descriptors->Text.c_str());
   	strcpy(Path_control_basic,Edit_control_basic->Text.c_str());
   	strcpy(Path_monitor_data,Edit_monitor_data->Text.c_str());
   	strcpy(Path_Graphics,Edit_Graphics->Text.c_str());

    if ((findfirst(Path_programs,&ffblk,FA_DIREC)==0)||(strlen(Path_programs)==0))
    {
	    if ((findfirst(Path_descriptors,&ffblk,FA_DIREC)==0)||(strlen(Path_descriptors)==0))
        {
		    if ((findfirst(Path_control_basic,&ffblk,FA_DIREC)==0)||(strlen(Path_control_basic)==0))
    	    {
			    if ((findfirst(Path_monitor_data,&ffblk,FA_DIREC)==0)||(strlen(Path_monitor_data)==0))
	    	    {
				    if ((findfirst(Path_Graphics,&ffblk,FA_DIREC)==0)||(strlen(Path_Graphics)==0))
		    	    {
					   	strcpy(ptr_panel->Program_Path,Path_programs);
						strcpy(ptr_panel->Descriptor_Path,Path_descriptors);
					   	strcpy(ptr_panel->Basic_Path,Path_control_basic);
					   	strcpy(ptr_panel->Monitor_Path,Path_monitor_data);
				   		strcpy(ptr_panel->Graphics_Path,Path_Graphics);
						save_config(CNF_PATH_INFO);
				        Close();
                    }
                    else
                    {
				    	MessageBox(GetFocus(),"The path of Graphics doesn't exist\nPlease input correct file path","File path",MB_ICONEXCLAMATION|MB_OK);
                    }
                }
                else
                {
			    	MessageBox(GetFocus(),"The path of Monitor Data doesn't exist\nPlease input correct file path","File path",MB_ICONEXCLAMATION|MB_OK);
                }
            }
            else
            {
		    	MessageBox(GetFocus(),"The path of Control Basic doesn't exist\nPlease input correct file path","File path",MB_ICONEXCLAMATION|MB_OK);
            }
        }
        else
        {
	    	MessageBox(GetFocus(),"The path of Descriptor doesn't exist\nPlease input correct file path","File path",MB_ICONEXCLAMATION|MB_OK);
        }
    }
    else
    {
    	MessageBox(GetFocus(),"The path of Prgrams doesn't exist\nPlease input correct file path","File path",MB_ICONEXCLAMATION|MB_OK);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMH_default_form::Button_cancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TMH_default_form::OnKeyPress_ProgramsEdit(TObject *Sender,
	char &Key)
{
	if ((Key=='\r')||(Key=='\n'))
    {
	   	Edit_descriptors->SetFocus();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMH_default_form::OnKeyPress_DescriptorsEdit(TObject *Sender,
	char &Key)
{
	if ((Key=='\r')||(Key=='\n'))
    {
	   	Edit_control_basic->SetFocus();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMH_default_form::OnKeyPress_ControlBasicEdit(TObject *Sender,
	char &Key)
{
	if ((Key=='\r')||(Key=='\n'))
    {
	   	Edit_monitor_data->SetFocus();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMH_default_form::OnKeyPress_MonitorDataEdit(TObject *Sender,
	char &Key)
{
	if ((Key=='\r')||(Key=='\n'))
    {
	   	Edit_Graphics->SetFocus();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMH_default_form::OnKeyPress_Graphics(TObject *Sender,
	char &Key)
{
	if ((Key=='\r')||(Key=='\n'))
    {
    	Button_ok->SetFocus();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMH_default_form::OnShow_FilePathForm(TObject *Sender)
{
    Edit_programs->SetFocus();
}
//---------------------------------------------------------------------------
