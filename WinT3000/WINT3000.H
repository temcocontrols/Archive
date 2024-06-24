//---------------------------------------------------------------------------
#ifndef wint3000H
#define wint3000H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Menus.hpp>
//---------------------------------------------------------------------------
class TMainScreen : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TMenuItem *Help2;
	TMenuItem *Contents2;
	TMenuItem *SearchforHelpOn2;
	TMenuItem *HowtoUseHelp2;
	TMenuItem *About2;
	TMenuItem *Miscellaneous;
	TMenuItem *Panel;
	TMenuItem *Control;
	TMenuItem *Data;
	TMenuItem *ConFiguration;
	TMenuItem *Connect;
	TMenuItem *About;
	TMenuItem *Infoscreen;
	TMenuItem *Loaddescriptors;
	TMenuItem *Updatemini;
	TMenuItem *N1;
	TMenuItem *eXit;
	TMenuItem *File;
	TMenuItem *Selectpanel;
	TMenuItem *Clearpanel1;
	TMenuItem *Screens;
	TMenuItem *Programs;
	TMenuItem *Inputs;
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
private:	// User declarations
public:		// User declarations
	__fastcall TMainScreen(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TMainScreen *MainScreen;
//---------------------------------------------------------------------------
#endif
