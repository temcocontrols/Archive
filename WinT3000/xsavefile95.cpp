//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include <dir.h>
#include "define.h"
#include "xsavefile95.h"
#include "ptpdll.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TSaveDialog95Form *SaveDialog95Form;

extern DInt networkaddress;    // sec de la inceputul programului
extern DInt  station_num;
extern DInt getfiles(DByte *term, DByte (*files)[13], DInt nmax, DInt local);
extern DInt getdirectories(DByte *ndir, DByte (*directories)[13], DInt nmax, DInt local);
//---------------------------------------------------------------------------
__fastcall TSaveDialog95Form::TSaveDialog95Form(TComponent* Owner,char *ext)
	: TForm(Owner)
{
 int i;
 ocwd[0]=0;
 i_files=0;
 i_directories=0;
 i_drives=0;
 if(t3000Request(COMMAND_50, GETCURRENTPATH_COMMAND, ocwd, 0, station_num, networkaddress)!=SUCCESS)
    return;

 files = new DByte [500][13];
 if(ocwd[strlen(ocwd)-1] != '\\')
		strcat(ocwd,"\\");

 fnsplit(ocwd,drive,dir,name,ext);


 strcpy(dir,ocwd);
/*
 if( strlen(defaultname) )
 {
	 strcpy(name,defaultname);
 }
 else
 {
		memcpy(name,lin_text,12);
		name[8] = 0;
 }
 strcpy(nname,"*.*");
*/
 strcpy(term,ext);
 memset(directories, 0, sizeof(directories));
 memset(files, 0, sizeof(files));
 nrfiles=getfiles(term, files, 500, 0);
 nrdirectories=getdirectories(dir, directories, 50,0);
 if(!nrdirectories)
 {
  delete files;
  return;
 }

// Root = DirTree->Items->AddChild(NULL, &directories[0][0]);
// DirTree->Selected = Root;
 for(i=1;i<=5 && i<=nrdirectories;i++)
 {
   SaveDialog95Form->DirTree->Items->Add(NULL,AnsiString(directories[i_directories+i-1][0]));
 }

}
//---------------------------------------------------------------------------
void TSaveDialog95Form::Set(char *filename, char *ext)
{
//  DefaultExt = String(ext);
//  SaveDialog->Filter = "PRG files|*.PRG";
  SaveDialog->FileName = filename;
}
//---------------------------------------------------------------------------
void __fastcall TSaveDialog95Form::BitBtn1Click(TObject *Sender)
{
 DirTree->Visible = true;
}
//---------------------------------------------------------------------------//---------------------------------------------------------------------------B
void __fastcall TSaveDialog95Form::DetailsBtnClick(TObject *Sender)
{
  FileList->ViewStyle = vsReport;
}
//---------------------------------------------------------------------------
void __fastcall TSaveDialog95Form::ListBtnClick(TObject *Sender)
{
	FileList->ViewStyle = vsList;
}
//---------------------------------------------------------------------------
void __fastcall TSaveDialog95Form::DblClick(TObject *Sender)
{
  ActiveControl = DirEdit;
  DirTree->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TSaveDialog95Form::OnClick(TObject *Sender)
{
 DirTree->Visible = true;
}
//---------------------------------------------------------------------------
