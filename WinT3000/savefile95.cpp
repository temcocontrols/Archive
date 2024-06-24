//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include <shellapi.hpp>
#include <dir.h>
#include "define.h"
#include "savefile95.h"
#include "ptpdll.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TSaveDialog95Form *SaveDialog95Form;

extern DInt networkaddress;    // sec de la inceputul programului
extern DInt  station_num;
extern DInt getfiles(DByte *term, DByte (*files)[13], DInt nmax, DInt local);
extern DInt getdirectories(DByte *ndir, DByte (*directories)[13], DInt nmax, DInt local);
//---------------------------------------------------------------------------
__fastcall TSaveDialog95Form::TSaveDialog95Form(TComponent* Owner,char *filetype)
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
 DirEdit->Text = dir;
 strcpy(term,filetype);
 memset(directories, 0, sizeof(directories));
 memset(files, 0, sizeof(files));
 nrdirectories=getdirectories(dir, directories, 50,0);
 nrfiles=getfiles(term, files, 500, 0);
 if(!nrdirectories)
 {
  delete files;
  return;
 }

// Root = DirTree->Items->AddChild(NULL, &directories[0][0]);
// DirTree->Selected = Root;
 for(i=0;i<nrdirectories;i++)
 {
//   DirTree->Items->Add(NULL,AnsiString((char *)&directories[i][0]));
   FileList->Items->Add();
   FileList->Items->Item[i]->Caption = AnsiString((char *)&directories[i][0]);
 }
 for(int j=0;j<nrfiles;j++)
 {
   FileList->Items->Add();
   FileList->Items->Item[i+j]->Caption = AnsiString((char *)&files[j][0]);
 }
 ActiveControl = FileName;
}
//---------------------------------------------------------------------------
void TSaveDialog95Form::Set(char *filename, char *ext)
{
    FileName->Text = filename;
}
//---------------------------------------------------------------------------
void __fastcall TSaveDialog95Form::BitBtn1Click(TObject *Sender)
{
// DirTree->Visible = true;
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
//  ActiveControl = DirEdit;
//  DirTree->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TSaveDialog95Form::OnClick(TObject *Sender)
{
// DirTree->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TSaveDialog95Form::FormCreate(TObject *Sender)
{
   TSHFileInfo SHFileInfo;
//   DirTree->Images = SmallImages;
//   SmallImages->ResourceLoad(rtBitmap, "IMAGES", clOlive);
   FileList->SmallImages = new TImageList(16, 16);
   FileList->SmallImages->ShareImages = true;
   FileList->SmallImages->Handle = SHGetFileInfo("*.*", 0, &SHFileInfo,
        sizeof(SHFileInfo), SHGFI_SMALLICON|SHGFI_ICON|SHGFI_SYSICONINDEX);
   FileList->LargeImages = new TImageList(32, 32);
   FileList->LargeImages->ShareImages = true;
   FileList->LargeImages->Handle = SHGetFileInfo("*.*", 0, &SHFileInfo,
        sizeof(SHFileInfo), SHGFI_LARGEICON|SHGFI_ICON|SHGFI_SYSICONINDEX);

}
//---------------------------------------------------------------------------
