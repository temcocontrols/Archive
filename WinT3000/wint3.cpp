//-----------------------#pragma link "Grids" ----------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include <vcl\comctrls.hpp>
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
#include "define.h"
#include "wint3.h"
#include <time.h>
#include "serialint.h"
#include "about.h"
#include "hardware.h"
#include "calend.h"
#include "datetime.h"
#include "Selectpanel.h"
#include "panelstatus.h"
#include "openfile.h"
#include "Grid.h"
#include "datamodule.h"
#include "print.h"
#include "preview.h"
#include "openfile95.h"
#include "printpanel.h"
#include "confirm.h"
#include "baseclas.h"
#include "edit.h"
#include "aio.h"

TMainScreen *MainScreen;
TImage *tImage[5];
int indexcommImage;
int oldStyleOpenDialog;

void StatusBarMessage(int i, AnsiString text);
extern void ProcReadPoints(TForm *App, DInt point_type);
//extern void ProcInit(void);
extern void ProceXitClick(void);

extern DUlong timesec1970;  // sec la inceputul programului
extern Panel_info1 panel_info1;
extern DInt local_panel;
extern DInt comPort;
extern int maxgridTable;
extern GRIDTABLE gridTable[MAXGRIDTABLE];
extern Panel *ptr_panel;

extern bool connectClick(TMainScreen *App);
extern int  getClick(TMainScreen *App);
extern int  saveClick(TMainScreen *App);
extern void ProcDisconnectClick(void);
extern void ProcEvRefreshGrid(DUint wParam);
extern void ProcReadDesSerial(void);
extern void ProcBreak(void);

__declspec(dllimport) void ProcEvConnection(DInt comPort);
__declspec(dllimport) void ProcEvReception(DInt comPort);
__declspec(dllimport) void ProcEvTransmission(DInt comPort, DInt param);
__declspec(dllimport) void ProcCommTimerTimer(DInt comPort, DInt DelayTime);
__declspec(dllimport) void setDLLObjects(TProgressBar *progressbar, TImage *timagerun[], DInt maximage);


//---------------------------------------------------------------------------
__fastcall TMainScreen::TMainScreen(TComponent* Owner)
	: TForm(Owner)
{
  tImage[0]=ImageT;
  tImage[1]=ImageE;
  tImage[2]=ImageM;
  tImage[3]=ImageC;                       
  tImage[4]=ImageO;
  Top = 0;
  Left = 0;
  Width = 666;    //540;
  Height = 460;   //400;
  indexcommImage=0;
  CommTimer->Enabled=true;
  setDLLObjects(ProgressBar, tImage, 5);
  time((long *)&timesec1970);
//ProcInit();
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ConnectClick(TObject *Sender)
{
 connectClick(this);
// DisconnectSpeedButton->Visible = true;
// ConnectSpeedButton->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::OnResize(TObject *Sender)
{
  time_t t;
  char ptext[30];
  ImageT->Left = Width - ImageT->Width - 12;
  ImageE->Left = Width - ImageT->Width - 12;
  ImageM->Left = Width - ImageT->Width - 12;
  ImageC->Left = Width - ImageT->Width - 12;
  ImageO->Left = Width - ImageT->Width - 12;
  Bevel1->Left = Width - ImageT->Width - 16;       //18 ;
//  Bevel2->Left = Width - 8  ;
  StatusPanel->Left =   Bevel1->Left - StatusPanel->Width - 10;
  StatusBar->Panels->Items[1]->Width = Width / 2 - 12;
  time(&t);
  strcpy(ptext,ctime(&t));
  ptext[10] = 0;
  ptext[16] = 0;
  StatusBarMessage(SBTime, AnsiString(ptext) + "    " + AnsiString(&ptext[11]) );
//  AlarmNoMsgImage->Left = Width - ImageT->Width - 14 - AlarmNoMsgImage->Width ;
//  AlarmMsgImage->Left = Width - ImageT->Width - 14 - AlarmMsgImage->Width ;
  MainScreen->ProgressBar->Top = MainScreen->StatusBar->Top+2;
  MainScreen->ProgressBar->Left = 0;
  ObjectsPanel->Width = Width - 8;
  CommandEdit->Width  = CommandPanel->Width - 55;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::CommTimerTimer(TObject *Sender)
{
  ProcCommTimerTimer(comPort, CommTimer->Interval);
  for(int i=0; i<MAXGRIDTABLE; i++)
    if ( gridTable[i].grid )
    {
      if(  gridTable[i].grid->readRefreshTimeFlag() == true )
      {
        if( gridTable[i].grid->readRefreshTime()>0 )
        {
          gridTable[i].grid->setRefreshTime( gridTable[i].grid->readRefreshTime() - CommTimer->Interval );
        }
        else
        {
          gridTable[i].grid->RefreshGrid();
        }
      }  
    }

}                        
//---------------------------------------------------------------------------
void __fastcall TMainScreen::eXitClick(TObject *Sender)
{
 ProceXitClick();
 PostQuitMessage(0);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::OnClose(TObject *Sender, TCloseAction &Action)
{
 ProceXitClick();
}
//---------------------------------------------------------------------------
void StatusBarMessage(int i, AnsiString text)
{
 MainScreen->StatusBar->Panels->Items[i]->Text = text;
}
//---------------------------------------------------------------------------
void PanelNameMessage( AnsiString text )
{
 MainScreen->PanelNameEdit->Text = text;
}
//---------------------------------------------------------------------------
void ProcEnableTimer(void)
{
 MainScreen->CommTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void ProcDisableTimer(void)
{
 MainScreen->CommTimer->Enabled=false;
}
//---------------------------------------------------------------------------
bool ProcTimerEnable(void)
{
 return MainScreen->CommTimer->Enabled;
}
//---------------------------------------------------------------------------
void ProcSendMessage(DUint mes, DUint w, DInt l=0)
{
	 SendMessage(
				MainScreen->Handle,	// handle of destination window
				mes,	// message to post
				w,	// first message parameter
				l 	// second message parameter
	 );
}
//---------------------------------------------------------------------------
void ProcPostMessage(DUint mes, DUint w, DInt l=0)
{
     PostMessage(
        MainScreen->Handle,	// handle of destination window
        mes,	// message to post
        w,	// first message parameter
        l  	// second message parameter
     );
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ScreensClick(TObject *Sender)
{
  ProcReadPoints(this, GRP);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ProgramsClick(TObject *Sender)
{
  ProcReadPoints(this, PRG);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::InputsClick(TObject *Sender)
{
  ProcReadPoints(this, T3000_IN);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::OutputsClick(TObject *Sender)
{
  ProcReadPoints(this, T3000_OUT);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::VariablesClick(TObject *Sender)
{
  ProcReadPoints(this, VAR);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ControllersClick(TObject *Sender)
{
  ProcReadPoints(this, CON);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::WeeklyroutinesClick(TObject *Sender)
{
  ProcReadPoints(this, WR);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::AnnualroutinesClick(TObject *Sender)
{
  ProcReadPoints(this, AR);
//  CalendarForm = new TCalendarForm( Application );
 // CalendarForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ArraYsClick(TObject *Sender)
{
  ProcReadPoints(this, AY);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::DisconnectClick(TObject *Sender)
{
  ProcDisconnectClick();
  ConnectSpeedButton->Visible = true;
  DisconnectSpeedButton->Visible = false;
}
//---------------------------------------------------------------------------
void Init()
{
  MainScreen->PanelNameEdit->Visible=true;
}
//---------------------------------------------------------------------------
void runImage(void)
{
  if(indexcommImage==4)
  {
     tImage[indexcommImage]->Visible = false;
     tImage[0]->Visible = true;
     indexcommImage=0;
  }
  else
  {
     tImage[indexcommImage]->Visible = false;
     tImage[indexcommImage+1]->Visible = true;
     ++indexcommImage;
  }
// MainScreen->Image1->Picture->LoadFromFile(commImage[indexcommImage++]);
}
//---------------------------------------------------------------------------
void resetImage(void)
{
    MainScreen->ImageT->Visible = true;
    MainScreen->ImageE->Visible = false;
    MainScreen->ImageM->Visible = false;
    MainScreen->ImageC->Visible = false;
    MainScreen->ImageO->Visible = false;
    indexcommImage=0;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::EvTransmission(TMessage& Msg)
{
 ProcEvTransmission((DInt)Msg.WParam, (DInt)Msg.LParam);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::EvConnection(TMessage& Msg)
{
 ProcEvConnection((DInt)Msg.WParam);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::EvReception(TMessage& Msg)
{
 ProcEvReception((DInt)Msg.WParam);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::EvRefreshGrid(TMessage& Msg)
{
 ProcEvRefreshGrid(Msg.WParam);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::DisconnectSpeedButtonClick(TObject *Sender)
{
 ProcDisconnectClick();
 ConnectSpeedButton->Visible = true;
 DisconnectSpeedButton->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ConnectSpeedButtonClick(TObject *Sender)
{
 connectClick(this);
/*
 if (ProcConnectClick(this))
 {
  DisconnectSpeedButton->Visible = true;
  ConnectSpeedButton->Visible = false;
 }
*/
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ImageTClick(TObject *Sender)
{
     PostMessage(
        MainScreen->Handle,	// handle of destination window
        WM_KEYDOWN,	// message to post
        VK_CANCEL,	// first message parameter
        0  	// second message parameter
     );
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::COMMSettingsClick(TObject *Sender)
{
    SerialInterfaceForm->Left=MainScreen->Left+(MainScreen->Width-SerialInterfaceForm->Width)/2;
    SerialInterfaceForm->Top=MainScreen->Top+90; //(MainScreen->Height-SerialInterfaceForm->Height)/2;
    SerialInterfaceForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::AboutClick(TObject *Sender)
{
  TAboutBox *AboutBox;
  AboutBox = new TAboutBox(Application);
  AboutBox->Left=Left+(Width-AboutBox->Width)/2;
  AboutBox->Top=Top+85; //(MainScreen->Height-SerialInterfaceForm->Height)/2;
  AboutBox->ShowModal();
  delete AboutBox;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::HardwareSpeedButtonClick(TObject *Sender)
{
/*
  int r;
  HardwareForm->Left=MainScreen->Left+140;  //(MainScreen->Width-HardwareForm->Width)/2;
  HardwareForm->Top=MainScreen->Top+90;    //(MainScreen->Height-HardwareForm->Height)/2;
  r=HardwareForm->ShowModal();
  if(r==mrNo+1)
  {
    SerialInterfaceForm->Left=MainScreen->Left+(MainScreen->Width-SerialInterfaceForm->Width)/2;
    SerialInterfaceForm->Top=MainScreen->Top+90; //(MainScreen->Height-SerialInterfaceForm->Height)/2;
    SerialInterfaceForm->ShowModal();
  }
  if(r==mrNo+2)
  {
  }
*/
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::TimeanddateClick(TObject *Sender)
{
  DateTimeForm = new TDateTimeForm( Application );
  DateTimeForm->ShowModal();
  delete DateTimeForm;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::PrintClick(TObject *Sender)
{
  TPrintForm *PrintForm;
  PrintForm = new TPrintForm( Application );
  if( PrintForm->ShowModal()==mrOk)
  {
    if (!PrintDialog->Execute())
    {
       delete PrintForm;
       return; // user did not press OK
    }   
  }
  delete PrintForm;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::SelectpanelClick(TObject *Sender)
{
  PanelSelect->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::SystemlistClick(TObject *Sender)
{
  ProcReadPoints(this, SYSTEM_LIST);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::InfoscreenClick(TObject *Sender)
{
  PanelStatusForm = new TPanelStatusForm( Application );
  PanelStatusForm->ShowModal();
  delete PanelStatusForm;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::UpdateminiClick(TObject *Sender)
{
  TOpenOptions o;
  o << ofFileMustExist << ofNoChangeDir	<< ofShowHelp;   // Initialize
//  if ( oldStyleOpenDialog )
     o << ofOldStyleDialog;
  TFileName NameOfFile;
  OpenDialog->DefaultExt = String("hex");
  OpenDialog->Filter = "HEX files|*.HEX";
  OpenDialog->Title  = "Update OS";
  OpenDialog->Options = o;
  if( OpenDialog->Execute() )
  {
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::Readfile1Click(TObject *Sender)
{
  OpenDialogForm = new TOpenDialogForm( Application );
  OpenDialogForm->Caption = " Select the remote source file";
  OpenDialogForm->ShowModal();
  delete OpenDialogForm;
}
//---------------------------------------------------------------------------

void __fastcall TMainScreen::GetClick(TObject *Sender)
{
  getClick(this);
/*
  TOpenOptions o;
  o << ofFileMustExist << ofNoChangeDir	<< ofShowHelp;   // Initialize
  if ( oldStyleOpenDialog )
     o << ofOldStyleDialog;
  TFileName NameOfFile;
  OpenDialog->DefaultExt = String("prg");
  OpenDialog->Filter = "PRG files|*.PRG";
  OpenDialog->Title  = " Program file name to be saved";
  OpenDialog->Options = o;
  if( OpenDialog->Execute() )
  {
  }
*/
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::SendClick(TObject *Sender)
{
  TOpenOptions o;
  o << ofFileMustExist << ofNoChangeDir	<< ofShowHelp;   // Initialize
  if ( oldStyleOpenDialog )
     o << ofOldStyleDialog;
  TFileName NameOfFile;
  OpenDialog->DefaultExt = String("prg");
  OpenDialog->Filter = "PRG files|*.PRG";
  OpenDialog->Title  = " Program file name to be sent";
  OpenDialog->Options = o;
  if( OpenDialog->Execute() )
  {
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::LoadClick(TObject *Sender)
{
// read the drive and directory information from the remote panel
// and fill in the OpenDialogForm and then open the dialog

/*
 if( local_panel )
 {
  TOpenOptions o;
  o << ofFileMustExist << ofNoChangeDir	<< ofShowHelp;   // Initialize
  if ( oldStyleOpenDialog )
     o << ofOldStyleDialog;
  TFileName NameOfFile;
  OpenDialog->DefaultExt = String("prg");
  OpenDialog->Filter = "PRG files|*.PRG";
  OpenDialog->Title  = " Program file name to be load";
  OpenDialog->Options = o;
  if( OpenDialog->Execute() )
  {
  }
 }
 else
 {
  if ( oldStyleOpenDialog )
  {
   OpenDialogForm = new TOpenDialogForm( Application );
   OpenDialogForm->Caption = " Select the remote program file";
   OpenDialogForm->ShowModal();
   delete OpenDialogForm;
  }
  else
  {
   OpenDialog95Form = new TOpenDialog95Form( Application );
   OpenDialog95Form->Caption = " Select the remote program file";
   OpenDialog95Form->ShowModal();
   delete OpenDialog95Form;
  }
 }
*/
 if( local_panel )
 {
  TOpenOptions o;
  o << ofFileMustExist << ofNoChangeDir	<< ofShowHelp;   // Initialize
  if ( oldStyleOpenDialog )
     o << ofOldStyleDialog;
  TFileName NameOfFile;
  OpenDialog->DefaultExt = String("prg");
  OpenDialog->Filter = "PRG files|*.PRG";
  OpenDialog->Title  = " Program file name to be load";
  OpenDialog->Options = o;
  if( OpenDialog->Execute() )
  {
      ptr_panel->loadprg(OpenDialog->FileName.c_str());
  }
 }
 else
 {
  if ( oldStyleOpenDialog )
  {
   OpenDialogForm = new TOpenDialogForm( Application );
   OpenDialogForm->Caption = " Select the remote program file";
   OpenDialogForm->ShowModal();
   delete OpenDialogForm;
  }
  else
  {
   OpenDialog95Form = new TOpenDialog95Form( Application );
   OpenDialog95Form->Caption = " Select the remote program file";
   OpenDialog95Form->ShowModal();
   delete OpenDialog95Form;
  }
 }

}
//---------------------------------------------------------------------------

void __fastcall TMainScreen::SaveClick(TObject *Sender)
{
  saveClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::LoadDesNetClick(TObject *Sender)
{
  ProcReadDesSerial();
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::PrintSetupClick(TObject *Sender)
{
  PrinterSetupDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::PrintpanelClick(TObject *Sender)
{
  TPrintPanelForm *PrintPanelForm;
  PrintPanelForm = new TPrintPanelForm( Application );
  if( PrintPanelForm->ShowModal()==mrOk)
  {
    if (!PrintDialog->Execute())
    {
		  delete PrintPanelForm;
          return; // user did not press OK
    }
  }
  delete PrintPanelForm;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::NetworkstatusClick(TObject *Sender)
{
  ProcReadPoints(this, NETSTAT);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ClearpanelClick(TObject *Sender)
{

  TEditForm *EditForm;
  EditForm = new TEditForm( Application );
  EditForm->ShowModal();
  delete EditForm;

  ConfirmForm = new TConfirmForm( Application );
  ConfirmForm->Caption = "";
  ConfirmForm->Label1->Caption = AnsiString(" Clear the panel  ")+AnsiString((char *)panel_info1.panel_name)+" ?";
  if( ConfirmForm->ShowModal()==mrOk )
  {
  }
  delete ConfirmForm;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::SpeedB1Click(TObject *Sender)
{
  if( ObjectsPanel->Visible )
  {
    ObjectsPanel->Visible = false;
  }
  else
  {
    ObjectsPanel->Visible = true;
    ObjectsPanel->Width = Width - 8;
  }
}
//---------------------------------------------------------------------------
void TMainScreen::aroundBevelFalse(void)
{
  AroundBevel->Visible  = false;
  ProgramBevel->Visible = false;
  MonitorBevel->Visible = false;
  InputBevel->Visible   = false;
  OutputBevel->Visible  = false;
  VarBevel->Visible     = false;
  ConBevel->Visible     = false;
  WrBevel->Visible      = false;
  ArBevel->Visible      = false;
  ArrayBevel->Visible   = false;
  TableBevel->Visible   = false;
  UnitBevel->Visible    = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ScreenBClick(TObject *Sender)
{
  aroundBevelFalse();
  ProcReadPoints(this, GRP);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ProgramBClick(TObject *Sender)
{
  aroundBevelFalse();
  ProcReadPoints(this, PRG);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::MonitorBClick(TObject *Sender)
{
  aroundBevelFalse();
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::InputBClick(TObject *Sender)
{
  aroundBevelFalse();
  ProcReadPoints(this, T3000_IN);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::OutputBClick(TObject *Sender)
{
  aroundBevelFalse();
  ProcReadPoints(this, T3000_OUT);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::VarBClick(TObject *Sender)
{
  aroundBevelFalse();
  ProcReadPoints(this, VAR);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ConBClick(TObject *Sender)
{
  aroundBevelFalse();
  ProcReadPoints(this, CON);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::WrBClick(TObject *Sender)
{
  aroundBevelFalse();
  ProcReadPoints(this, WR);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ArBClick(TObject *Sender)
{
  aroundBevelFalse();
  ProcReadPoints(this, AR);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::AyBClick(TObject *Sender)
{
  aroundBevelFalse();
  ProcReadPoints(this, AY);
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::TableBClick(TObject *Sender)
{
  aroundBevelFalse();
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::UnitBClick(TObject *Sender)
{
  aroundBevelFalse();
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::CommandButtonClick(TObject *Sender)
{
// CommandButton->Down   = true;
 CommandPanel->Visible = true;
 ActiveControl = CommandEdit;
 CommandEdit->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::CommandEditExit(TObject *Sender)
{
 CommandButton->Down = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::CommandEditKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
  if(Key==27)
  {
   CommandButton->Down = false;
   CommandPanel->Visible = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ObjectsPanelMouseMove(TObject *Sender,
	TShiftState Shift, int X, int Y)
{
  aroundBevelFalse();
  SpeedButton2->Visible = true;
  SpeedB1->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ScreenBMouseMove(TObject *Sender,
	TShiftState Shift, int X, int Y)
{
  aroundBevelFalse();
  AroundBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ProgramBMouseMove(TObject *Sender,
	TShiftState Shift, int X, int Y)
{
  aroundBevelFalse();
  ProgramBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::MonitorBMouseMove(TObject *Sender,
	TShiftState Shift, int X, int Y)
{
  aroundBevelFalse();
  MonitorBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::InputBMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  aroundBevelFalse();
  InputBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::OutputBMouseMove(TObject *Sender,
	TShiftState Shift, int X, int Y)
{
  aroundBevelFalse();
  OutputBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::VarBMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  aroundBevelFalse();
  VarBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ConBMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  aroundBevelFalse();
  ConBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::WrBMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  aroundBevelFalse();
  WrBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::ArBMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  aroundBevelFalse();
  ArBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::AyBMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  aroundBevelFalse();
  ArrayBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::TableBMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  aroundBevelFalse();
  TableBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::UnitBMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  aroundBevelFalse();
  UnitBevel->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::FormKeyPress(TObject *Sender, char &Key)
{
 if( Key == ' ' )
 {
  CommandPanel->Visible = true;
  ActiveControl = CommandEdit;
  CommandEdit->Text = "";
 }
}
//---------------------------------------------------------------------------
void __fastcall TMainScreen::SpeedButton2MouseMove(TObject *Sender,
	TShiftState Shift, int X, int Y)
{
  SpeedButton2->Visible = false;
  SpeedB1->Visible = true;
}
//---------------------------------------------------------------------------




















/*
void __fastcall TMainScreen::T3000configClick(TObject *Sender)
{

}
*/
//---------------------------------------------------------------------------







