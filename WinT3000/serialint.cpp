//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "serialint.h"
#include "confirm.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
#include "define.h"
#include "ptpdll.h"

TSerialInterfaceForm *SerialInterfaceForm;
DLong     tBaudRate[MAXBAUDRATE] = { 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
DByte     tByteSize[2]          = { 7, 8 };
DByte     tStopBits[3]          = { ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS };
DByte     tParity[3]            = { NOPARITY,   ODDPARITY,    EVENPARITY };
DByte     szaCommPort[MAX_COMM_INFO][5]  = {
											 {"COM1"},
											 {"COM2"},
											 {"COM3"},
											 {"COM4"} };

ModemCapabilities ModemDataBase[MAXMODEMTYPE+1] = {
	{
		"User Defined",
		"AT &F&C1&D2V1",
		"AT S0=0",
		"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0\0",
		"",
		"",
		38400L,
		0,
		1
	},
	{   "Generic Modem",
		"AT &F&C1&D2V1",
		"AT S0=0",
		"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0\0",
		"",
		"",
		38400L,
		0,
		1
	},
	{ "Hayes Compatible",
		"AT &F B1E1Q0V1X4Y0 &C1&D2 S7=90",
		"\0",
		"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0",
		"",
		"",
		2400L,
		0,
		0
	},
	{ "Practical Peripherals 14400FX",
		"AT &F0 &C1 &D2 S95=44",
		"\0",
		"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0",
		"CLASS 5\0V.42BIS\0\0",
		"LAP-M\0ALT\0\0",
		57600L,
		1,
		1
	},
	{ "Intel SatisFAXtion 400e",
		"AT &F",
		"\0",
		"NO CARRIER\0ERROR\0NO DIALTONE\0BUSY\0NO ANSWER\0\0",
		"COMP\0\0",
		"LAPM\0MNP\0REL\0\0",
		57600L,
		1,
		1
	},
    { ""
    }
};

extern             DInt   comPort;

//---------------------------------------------------------------------------
__fastcall TSerialInterfaceForm::TSerialInterfaceForm(TComponent* Owner)
	: TForm(Owner)
{
    changeState = 0;
	InterfaceBox->ItemIndex  = comPort;
	currentCom               = InterfaceBox->ItemIndex;
	BaudRateBox->ItemIndex   = CommInfo[comPort].baudRateIndex();
	ConnectionBox->ItemIndex = CommInfo[comPort].Connection();
	ModemTypeBox->ItemIndex  = CommInfo[comPort].modemTypeIndex();
	if(!CommInfo[comPort].modemTypeIndex())
         CommInfo[comPort].getmodemCapabilities(ModemDataBase[0]);
    InitStringEdit->Text = AnsiString((char *)CommInfo[comPort].firstInitString());
	if(CommInfo[comPort].Connection()==MODEM_LINK)
	{
		 ModemGroupBox->Enabled = true;
         if(!CommInfo[comPort].modemTypeIndex())
		    InitStringEdit->Enabled = true;
         else
		    InitStringEdit->Enabled = false;
	}
    else
		 ModemGroupBox->Enabled = false;

}
//---------------------------------------------------------------------------
void __fastcall TSerialInterfaceForm::OKButtonClick(TObject *Sender)
{
	int  j;
	j = InterfaceBox->ItemIndex;
	CommInfo[j].Set((DLong)tBaudRate[BaudRateBox->ItemIndex]);
	CommInfo[j].Connection((DByte)ConnectionBox->ItemIndex);
	CommInfo[j].modemTypeIndex((DByte)ModemTypeBox->ItemIndex);
//	if(!CommInfo[j].modemTypeIndex())
    CommInfo[j].setmodemCapabilities(&ModemDataBase[ModemTypeBox->ItemIndex]);
    comPort = j;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TSerialInterfaceForm::CancelButtonClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TSerialInterfaceForm::InterfaceBoxChange(TObject *Sender)
{
  TConfirmForm *ConfirmForm;
  if ( currentCom != InterfaceBox->ItemIndex && changeState )
  {
    ConfirmForm = new TConfirmForm( Application );
    ConfirmForm->Label1->Caption = AnsiString("Save the new settings for ")+AnsiString((char *)szaCommPort[currentCom]);
    if( ConfirmForm->ShowModal()==mrOk )
    {
	 CommInfo[currentCom].Set((DLong)tBaudRate[BaudRateBox->ItemIndex]);
	 CommInfo[currentCom].Connection((DByte)ConnectionBox->ItemIndex);
	 CommInfo[currentCom].modemTypeIndex((DByte)ModemTypeBox->ItemIndex);
//	 if(!CommInfo[currentCom].modemTypeIndex())
     CommInfo[currentCom].setmodemCapabilities(&ModemDataBase[ModemTypeBox->ItemIndex]);
    }
    delete ConfirmForm;
  }

  BaudRateBox->ItemIndex   = CommInfo[InterfaceBox->ItemIndex].baudRateIndex();
  ConnectionBox->ItemIndex = CommInfo[InterfaceBox->ItemIndex].Connection();
  ModemTypeBox->ItemIndex  = CommInfo[InterfaceBox->ItemIndex].modemTypeIndex();
  if(!CommInfo[InterfaceBox->ItemIndex].modemTypeIndex())
        CommInfo[InterfaceBox->ItemIndex].getmodemCapabilities(ModemDataBase[0]);
  InitStringEdit->Text = AnsiString((char *)CommInfo[InterfaceBox->ItemIndex].firstInitString());
  if(CommInfo[InterfaceBox->ItemIndex].Connection()==MODEM_LINK)
  {
		 ModemGroupBox->Enabled = true;
         if(!CommInfo[InterfaceBox->ItemIndex].modemTypeIndex())
		    InitStringEdit->Enabled = true;
         else
		    InitStringEdit->Enabled = false;
  }
   else
		 ModemGroupBox->Enabled = false;

  comPort = InterfaceBox->ItemIndex;
  changeState = 0;
  currentCom = InterfaceBox->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TSerialInterfaceForm::ConnectionBoxChange(TObject *Sender)
{
 changeState = 1;
 if( ConnectionBox->ItemIndex == MODEM_LINK )
 {
	ModemGroupBox->Enabled = true;
    if(!CommInfo[InterfaceBox->ItemIndex].modemTypeIndex())
	    InitStringEdit->Enabled = true;
    else
	    InitStringEdit->Enabled = false;
 }
 else
 	 ModemGroupBox->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TSerialInterfaceForm::BaudRateBoxChange(TObject *Sender)
{
 changeState = 1;
}
//---------------------------------------------------------------------------
void __fastcall TSerialInterfaceForm::ModemTypeBoxChange(TObject *Sender)
{
 changeState = 1;
 InitStringEdit->Text = AnsiString((char *)ModemDataBase[ModemTypeBox->ItemIndex].first_init_string);
 if(!ModemTypeBox->ItemIndex)
    InitStringEdit->Enabled = true;
 else
    InitStringEdit->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TSerialInterfaceForm::InitStringEditChange(TObject *Sender)
{
 strcpy(ModemDataBase[0].first_init_string,  InitStringEdit->Text.c_str());
}
//---------------------------------------------------------------------------
