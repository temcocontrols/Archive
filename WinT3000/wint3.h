//---------------------------------------------------------------------------
#ifndef wint3H
#define wint3H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Menus.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\DBGrids.hpp>
#include "Grids.hpp"
#include <vcl\Dialogs.hpp>
//---------------------------------------------------------------------------
class TMainScreen : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TMenuItem *Help2;
	TMenuItem *Contents2;
	TMenuItem *SearchforHelpOn2;
	TMenuItem *HowtoUseHelp2;
	TMenuItem *About;
	TMenuItem *Miscellaneous;
	TMenuItem *Panel;
	TMenuItem *Control;
	TMenuItem *Data;
	TMenuItem *ConFiguration;
	TMenuItem *Connection;
	TMenuItem *Infoscreen;
	TMenuItem *Loaddescriptors;
	TMenuItem *Updatemini;
	TMenuItem *N1;
	TMenuItem *eXit;
	TMenuItem *File;
	TMenuItem *Selectpanel;
	TMenuItem *Screens;
	TMenuItem *Programs;
	TMenuItem *Outputs;
	TMenuItem *Variables;
	TMenuItem *Controllers;
	TMenuItem *Weeklyroutines;
	TMenuItem *Annualroutines;
	TMenuItem *ArraYs;
	TMenuItem *Monitors;
	TMenuItem *Tables;
	TMenuItem *CustomUnits;
	TMenuItem *Alarmlog;
	TMenuItem *Setup;
	TMenuItem *Userlist;
	TMenuItem *Hardware;
	TMenuItem *COMMSettings;
	TMenuItem *T3000config;
	TMenuItem *Connect;
	TMenuItem *Disconnect;
	TPanel *SpeedPanel;
	TSpeedButton *LoadSpeedButton;
	TSpeedButton *HelpSpeedButton;
	TSpeedButton *ConnectSpeedButton;
	TSpeedButton *DisconnectSpeedButton;
	TSpeedButton *SaveSpeedButton;
	TSpeedButton *SelectSpeedButton;
	TSpeedButton *HardwareSpeedButton;
	TTimer *CommTimer;
	TStatusBar *StatusBar;
	TProgressBar *ProgressBar;
	TImage *ImageT;
	TImage *ImageE;
	TImage *ImageM;
	TImage *ImageC;
	TImage *ImageO;
	TImage *AlarmMsgImage;
	TBevel *Bevel1;
	TBevel *Bevel3;
	TBevel *Bevel4;
	TPanel *StatusPanel;
	TEdit *PanelNameEdit;
	TMenuItem *Inputs;
	TMenuItem *Defaultprogram;
	TMenuItem *Filepath;
	TMenuItem *Timeanddate;
	TMenuItem *Get;
	TMenuItem *Send;
	TMenuItem *N2;
	TMenuItem *Load;
	TMenuItem *Save;
	TImage *AlarmNoMsgImage;
	TMenuItem *Systemlist;
	TMenuItem *Logout;
	TMenuItem *LoadDesNet;
	TMenuItem *LoaddescritorsPanel;
	TMenuItem *Viewdescriptors;
	TMenuItem *Printpanel;
	TMenuItem *Clearpanel;
	TMenuItem *N3;
	TMenuItem *Networkstatus;
	TMenuItem *Filetransfer1;
	TMenuItem *Renamefile1;
	TMenuItem *Deletefile1;
	TMenuItem *Sendfile1;
	TMenuItem *Readfile1;
	TMenuItem *Copyfile1;
	TMenuItem *N4;
	TMenuItem *Print;
	TMenuItem *PrintSetup;
	TMenuItem *Searchdescriptors;
	TMenuItem *SaveNetwork;
	TMenuItem *N5;
	TOpenDialog *OpenDialog;
	TPrintDialog *PrintDialog;
	TPrinterSetupDialog *PrinterSetupDialog;
	TBevel *Bevel2;
	TStatusBar *AlarmBar;
	TPanel *ObjectsPanel;
	TSaveDialog *SaveDialog;
	TOpenDialog *LoadPanelFileDlg;
	TSpeedButton *CommandButton;
	TPanel *CommandPanel;
	TEdit *CommandEdit;
	TPanel *LogoPanel;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TImage *Image2;
	TBevel *AroundBevel;
	TSpeedButton *ScreenB;
	TBevel *ProgramBevel;
	TSpeedButton *ProgramB;
	TBevel *MonitorBevel;
	TSpeedButton *MonitorB;
	TBevel *InputBevel;
	TSpeedButton *InputB;
	TBevel *OutputBevel;
	TSpeedButton *OutputB;
	TBevel *VarBevel;
	TSpeedButton *VarB;
	TBevel *ConBevel;
	TSpeedButton *ConB;
	TBevel *WrBevel;
	TSpeedButton *WrB;
	TBevel *ArBevel;
	TSpeedButton *ArB;
	TBevel *ArrayBevel;
	TSpeedButton *AyB;
	TBevel *TableBevel;
	TSpeedButton *TableB;
	TBevel *UnitBevel;
	TSpeedButton *UnitB;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedB1;
	void __fastcall OnClose(TObject *Sender, TCloseAction &Action);
	void __fastcall OnResize(TObject *Sender);
	void __fastcall ConnectClick(TObject *Sender);
	void __fastcall DisconnectClick(TObject *Sender);
	void __fastcall eXitClick(TObject *Sender);
	void __fastcall CommTimerTimer(TObject *Sender);
	void __fastcall OutputsClick(TObject *Sender);
	void __fastcall VariablesClick(TObject *Sender);
	void __fastcall ControllersClick(TObject *Sender);
	void __fastcall ScreensClick(TObject *Sender);
	void __fastcall InputsClick(TObject *Sender);
	void __fastcall DisconnectSpeedButtonClick(TObject *Sender);
	void __fastcall ConnectSpeedButtonClick(TObject *Sender);
	void __fastcall ImageTClick(TObject *Sender);
	void __fastcall COMMSettingsClick(TObject *Sender);
	void __fastcall AboutClick(TObject *Sender);
	void __fastcall HardwareSpeedButtonClick(TObject *Sender);
	void __fastcall TimeanddateClick(TObject *Sender);
	void __fastcall AnnualroutinesClick(TObject *Sender);
	void __fastcall PrintClick(TObject *Sender);
	void __fastcall SelectpanelClick(TObject *Sender);
	void __fastcall SystemlistClick(TObject *Sender);
	void __fastcall InfoscreenClick(TObject *Sender);
	void __fastcall UpdateminiClick(TObject *Sender);
	void __fastcall Readfile1Click(TObject *Sender);
	void __fastcall WeeklyroutinesClick(TObject *Sender);
	void __fastcall ArraYsClick(TObject *Sender);
	void __fastcall GetClick(TObject *Sender);
	void __fastcall SendClick(TObject *Sender);
	void __fastcall LoadClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall ProgramsClick(TObject *Sender);
	void __fastcall LoadDesNetClick(TObject *Sender);
	void __fastcall PrintSetupClick(TObject *Sender);
	void __fastcall PrintpanelClick(TObject *Sender);
	void __fastcall NetworkstatusClick(TObject *Sender);
	void __fastcall ClearpanelClick(TObject *Sender);
    void __fastcall SpeedB1Click(TObject *Sender);
	void __fastcall ScreenBClick(TObject *Sender);
	void __fastcall UserlistClick(TObject *Sender);
	void __fastcall CommandButtonClick(TObject *Sender);
	void __fastcall CommandEditExit(TObject *Sender);
	void __fastcall CommandEditKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall ScreenBMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y);
	void __fastcall ObjectsPanelMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y);
	void __fastcall ProgramBMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y);
	void __fastcall MonitorBMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y);
	void __fastcall InputBMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y);
	void __fastcall OutputBMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y);
	void __fastcall VarBMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ConBMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall WrBMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ArBMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall AyBMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall TableBMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y);
	void __fastcall UnitBMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y);
	void __fastcall ProgramBClick(TObject *Sender);
	void __fastcall MonitorBClick(TObject *Sender);
	void __fastcall InputBClick(TObject *Sender);
	void __fastcall OutputBClick(TObject *Sender);
	void __fastcall VarBClick(TObject *Sender);
	void __fastcall ConBClick(TObject *Sender);
	void __fastcall WrBClick(TObject *Sender);
	void __fastcall ArBClick(TObject *Sender);
	void __fastcall AyBClick(TObject *Sender);
	void __fastcall TableBClick(TObject *Sender);
	void __fastcall UnitBClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall SpeedButton2MouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y);
    
    
    
    
    
private:	// User declarations
    void aroundBevelFalse(void);
public:		// User declarations
	__fastcall TMainScreen(TComponent* Owner);
	void __fastcall EvReception(TMessage& Message);
	void __fastcall EvTransmission(TMessage& Message);
	void __fastcall EvConnection(TMessage& Message);
	void __fastcall EvRefreshGrid(TMessage& Message);
    BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER(WM_PTP_RECEPTION, TMessage, EvReception)
      MESSAGE_HANDLER(WM_PTP_TRANSMISSION, TMessage, EvTransmission)
      MESSAGE_HANDLER(WM_PTP_CONNECTION, TMessage, EvConnection)
      MESSAGE_HANDLER(WM_REFRESHGRID, TMessage, EvRefreshGrid)
    END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern TMainScreen *MainScreen;
//---------------------------------------------------------------------------
#endif
