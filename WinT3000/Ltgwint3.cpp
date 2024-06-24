//-----------------------#pragma link "Grids" ----------------------------------------------------
#include <vcl\dialogs.hpp>
//---------------------------------------------------------------------------

//ltg #include "TableForm.h"
#include <string.h>
#include <io.h>
#include <dir.h>
#include <stdio.h>

#include "define.h"
#include "wint3.h"
#include "LTGLoadPanel.h"
#include "LTGUserEnsur.h"
#include "Aio.h"
#include "ptpdll.h"
#include "LTGComPortList.h"
#include "savefile95.h"
#include "serialint.h"
#include "openfile.h"
#include "Selectpanel.h"

//  Mao Hui code
#include  "MH_wint3.h"

//ltg
extern Panel * ptr_panel;    // In WinT3000Proc.cpp
extern DInt local_panel;
extern Panel_info1 Panel_Info1,panel_info1;
extern DInt  station_num;
extern DInt networkaddress;    // sec de la inceputul programului
extern Byte serial_access;     //ltg NOT connectting serial port.
extern DByte default_prg[13];

//--------------------------------------------------------------
extern void saveoldprg(char *fname);     //  In LTGT3000.cpp
extern void save_config(int what);       //  In LTGT3000.cpp
extern void disconnect_serial(int);      //  In LTGT3000.cpp


void StatusBarMessage(int i, AnsiString text);
extern void ProcReadPoints(TForm *App, DInt point_type);
                                             //ltg In WinT3000Proc.cpp
extern void ProcInit(void);                  //ltg  In WinT3000Proc.cpp
void ProceXitClick(void);

extern DInt        comPort;                  //ltg In  WinT3000Proc.cpp

extern bool ProcConnectClick(TMainScreen *App);//ltg In WinT3000Proc.cpp
extern void ProcDisconnectClick(void);         //ltg In WinT3000Proc.cpp
extern void ProcEvRefreshGrid(DUint wParam);
extern void ProcReadDesSerial(void);
extern void ProcBreak(void);

__declspec(dllimport) void ProcEvConnection(DInt comPort);
__declspec(dllimport) void ProcEvReception(DInt comPort);
__declspec(dllimport) void ProcEvTransmission(DInt comPort, DInt param);
__declspec(dllimport) void ProcCommTimerTimer(DInt comPort, DInt DelayTime);
__declspec(dllimport) void setDLLObjects(TProgressBar *progressbar, TImage *timagerun[], DInt maximage);

DByte directories[50][13];

/**********************************************************************/
/* LTG:  July 28, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Exit system.                                                     */
/*--------------------------------------------------------------------*/
/* Updating: July 28, 1997                                            */
/**********************************************************************/
void __fastcall TMainScreen::eXitClick(TObject *Sender)
{
/* ltg : old version in proj() of T3000.
	if(qtt)
	{

	if(Des)
		{
		 delete Des;
		 Des = 0;
		}

//ltg 		suspend(AMONITOR);

		exit_apl=1;
#ifdef NETWORK_COMM
			if( Netbios_state == NB_INSTALLED )
			{
				 operator_list.state = EXIT;
//ltg 				 resume(NETWORK);
//ltg 				 suspend(PROJ);
				 while(operator_list.state!=DONE && operator_list.state!=ERROR);

				 if(operator_list.state==ERROR)
							{

								error_net=1;
							}

				 NET_EXIT=1;
//ltg 				 resume(NETWORK);
				 task_switch();
				 enable();
				 while(pnet) ;
//ltg 				 kill_task(NETWORK);
			}
#endif

//ltg 		  kill_task(MSTP_MASTER);
//ltg 		  kill_task(SERVERTSM);

//ltg 		  kill_task(DIAL);

		 present_analog_monitor=0;
//ltg 		 save_monitor = 0x01 | 0x02;
//ltg 		 save_monitor_command = 1;
//ltg 		 save_m();

//ltg 	G.ReleaseMBar();
	}

//ltg 	 if(def_macro)
	 {
//ltg 		input_buffer[ind_input_buffer].key.key[0] = '$';
		if ((h=fopen("demo.mac","wb+"))!=NULL)
		{
//ltg 		  fwrite(input_buffer,sizeof(struct input_buffer),MAX_INPUT_BUFFER,h);
		  fclose(h);
		}
    }

//ltg 		pfarfree(heap_buf_handle);
//		pfarfree(heap_array_handle);
//		pfarfree(heap_dmon_handle);
//		pfarfree(heap_grp_handle);
//ltg 		pfarfree(heap_amon_handle);
		if( ptr_input_buffer )
		{
//ltg 			mfarfree(ptr_input_buffer);
		}

		if(ptr_panel) delete ptr_panel;

		points_icon[AMON].~icon();
		points_icon[PRG].~icon();
		points_icon[WR].~icon();

//ltg 		enable_ints=1;
//ltg 		reset_PIC_mask(0);
//ltg 		exit_proj();
*/

  ProceXitClick();
  PostQuitMessage(0);
}

//---------------------------------------------------------------------------

void __fastcall TMainScreen::OnClose(TObject *Sender, TCloseAction &Action)
{
  ProceXitClick();
}

//---------------------------------------------------------------------------
/*
//void __fastcall TMainScreen::DefaultprogramClick(TObject *Sender)
//{
  // LTG  CONFIG / SETUP
// ltg
//  if( check_access(12)!=R_NA )
//    setup();

  // mao hui
//  MH_default_form->Default_program();
//}
*/
//---------------------------------------------------------------------------
/*
void __fastcall TMainScreen::FilepathClick(TObject *Sender)
{
  // mao hui
  MH_default_form->File_path();
}
 */
void __fastcall TMainScreen::COMMSettingsClick(TObject *Sender)
{
	  // LTG CONFIG / HARDWARE.
/* ltg
	  case M4+2:
			hardware();
//			portconfig();
			break;
*/

    SerialInterfaceForm->Left=MainScreen->Left+(MainScreen->Width-SerialInterfaceForm->Width)/2;
    SerialInterfaceForm->Top=MainScreen->Top+90; //(MainScreen->Height-SerialInterfaceForm->Height)/2;
    SerialInterfaceForm->ShowModal();
}
//---------------------------------------------------------------------------

/**********************************************************************/
/* LTG:  JuLY 28, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   User updates user list.                                          */
/*--------------------------------------------------------------------*/
/* Updating:  July 28, 1997                                           */
/**********************************************************************/
void __fastcall TMainScreen::UserlistClick(TObject *Sender)
{
  // mao hui.
  MH_UserListForm->UserList();

  //ltg
  // save config file.
  save_config(CNF_PASS_INFO);
}
//---------------------------------------------------------------------------

/**********************************************************************/
/* LTG:  Aug.  1, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Connect min-panel through serial port.                           */
/*--------------------------------------------------------------------*/
/* Updating: Aug.  4, 1997                                            */
/**********************************************************************/
bool connectClick(TMainScreen *App)
{
  bool ret = false;
  DInt j,i = 0;
  DInt iConfigPort = -1;      // 0-3: only one port configured.
                              //  -1: no port configured.
                              // 16 : multi-ports configured.

  // search ports configured.
  while ( i < MAX_COMM_INFO )            // ( <= 4 )
  {
    if ( CommInfo[i].Connection() == 0 )   // COM port
    {
      i++;
      continue;
    }  

    if ( iConfigPort == -1 )
      iConfigPort = i;
    else
      iConfigPort = 16;

    i++;  
  }

  // no port configured.
  if ( iConfigPort == -1 )
  {
    MessageBeep( MB_ICONEXCLAMATION );
    MessageBox(GetFocus()," No Port Configured ! ", "", MB_OK );
    return false;
  }

  // only one port configured.
  if ( iConfigPort != 16 )
  {
    comPort = iConfigPort;
  }
  else
  {  // 16 : multi-ports configured.

    ComPortListForm->ListBox1->Items->Clear();
    j=0;
    for ( i=0; i < MAX_COMM_INFO; i++ )    // ( <= 4 )
    {
      DByte lpszComPortItem[30];
      AnsiString ASComPortItem = "                    ";

//      ComPortListForm->ListBox1->Items->Delete(i);

/*
      if ( CommInfo[i].Connection() == 0 )   // COM port not configured.
      {
        sprintf(lpszComPortItem, " COM %d      ", i+1 );
        strcpy( ASComPortItem.c_str(), lpszComPortItem );
        ComPortListForm->ListBox1->Items->Insert(i, ASComPortItem);

        //LTG:  I don't know how to disable it. ???
        // ComPortListForm->ListBox1->Items->Visible(i) = false;
        continue;
      }
*/
      // COM port configured as serial.
      if ( CommInfo[i].Connection() == SERIAL_LINK )
      {
        sprintf(lpszComPortItem, " COM %d   Serial", i+1 );
        strcpy( ASComPortItem.c_str(), lpszComPortItem );
        ComPortListForm->ListBox1->Items->Insert(j, ASComPortItem);
        j++;
        continue;
      }

      // COM port configured as modem.
      if ( CommInfo[i].Connection() == MODEM_LINK )
      {
        sprintf(lpszComPortItem, " COM %d   Modem", i+1 );
        strcpy( ASComPortItem.c_str(), lpszComPortItem );
        ComPortListForm->ListBox1->Items->Insert(j, ASComPortItem);
        j++;
        continue;
      }

    }  // end of FOR.

    // user select a COM port configured.
    ComPortListForm->Left=
        MainScreen->Left+(MainScreen->Width-ComPortListForm->Width)/2;
    ComPortListForm->Top = MainScreen->Top + 90;
    ComPortListForm->ShowModal();

    // no item is selected.
    if ( ComPortListForm->ListBox1->ItemIndex == -1 )
      return false;

    comPort = ComPortListForm->ListBox1->ItemIndex;
  }  // end of if.

  if ( (ret=ProcConnectClick(App))==true )
  {
    App->DisconnectSpeedButton->Visible = true;
    App->ConnectSpeedButton->Visible = false;
    local_panel = 0;
  }

  return ret;
}
//---------------------------------------------------------------------------

/**********************************************************************/
/* LTG:  JuLY 18, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Load a program file from external media attached to a control    */
/*   panel ( disk) to the control memory.                             */
/*--------------------------------------------------------------------*/
/* Updating:  Aug. 17, 1997                                           */
/**********************************************************************/
void __fastcall TMainScreen::LoadClick(TObject *Sender)
{
  char buf1[65];      // File name.
  char DirFiles[50][13];
  DUint l = 13;
  int i = 3;

  // from control panel disk to control panel memory,
  // not local user station.
  if ( ! local_panel )
  {
    // read the drive and directory information from the remote panel
    // and fill in the OpenDialogForm and then open the dialog

    OpenDialogForm = new TOpenDialogForm ( Application );
    OpenDialogForm->Caption = " Select the remote source file";


    // Get the directories structure:

    // 1. set the current path for the remote panel.
    l = 65;
/* NEW VERSION.
    if ( t3000Request( COMMAND_50+100, SETCURRENTPATH,
                       ptr_panel->Program_Path, &l, station_num,
                       networkaddress) != SUCCESS )
      return;
*/

/* NEW VERSION.
    // read the directorise.
    if ( t3000Request( COMMAND_50, READDIRECTIRIES,
                       DirFiles, &l, station_num,
                       networkaddress ) != SUCCESS )
      return;
*/    

//ltg    OpenDialogForm->DirectoryListBox1->Directory = AnsiString(DirFiles);

/* NEW VERSION.
    // Set the file type.
    l = 13;
    if ( t3000Request( COMMAND_50+100, SETFILETYPE,
                       "*.prg", &l, station_num,
                       networkaddress) != SUCCESS )
      return;
*/

/* NEW VERSION.
    // read the file names.
    if ( t3000Request( COMMAND_50, READFILES,
                       DirFiles, 0, station_num,
                       networkaddress, NETCALL_SIGN ) != SUCCESS )
      return;
*/    

    OpenDialogForm->FileListBox1->Update();

    // Items->Directory = AnsiString(DirFiles);
  // OpenDialogForm->FileListBox1->Items->Add( AnsiString(DirFiles) );


  /*
  strcpy( OpenDialogForm->InitialDir.c_str(), ptr_panel->Program_Path );
  strcpy( OpenDialogForm->FileName.c_str(), ptr_panel->Default_Program );
  */
    if ( ! OpenDialogForm->ShowModal() )
      return;    
    
  }   // if ( ! local_panel )
  else
  {
    // LoadPanelFileDlg->Filter = "Panel files (*.prg)|All files (*.*)";

    strcpy( LoadPanelFileDlg->InitialDir.c_str(),
            ptr_panel->Program_Path );
    strcpy( LoadPanelFileDlg->FileName.c_str(),
            ptr_panel->Default_Program );

    if ( ! LoadPanelFileDlg->Execute() )
      return;
      
  }   // NOT if ( ! local_panel )

  // panel file name.
  strcpy(buf1, LoadPanelFileDlg->FileName.c_str() );

  // inquire if user ensure to open this panel file.
  UserEnsureForm->Left=
        MainScreen->Left+(MainScreen->Width-UserEnsureForm->Width)/2;
  UserEnsureForm->Top = MainScreen->Top + 90;
  {
    char lpszTemp[100];

    sprintf(lpszTemp, "Load program '%s' to panel ?", buf1);
    UserEnsureForm->EnsureWinLabel->Caption = "Load the program to this panel ?";
    UserEnsureForm->EnsureWinLabel->Caption = AnsiString( lpszTemp );
  // UserEnsureForm->EnsureWinLabel->Alignment = taCenter;
  }
  UserEnsureForm->ShowModal();

  // if user not want to load this file
  if ( UserEnsureForm->Ok == 0 )
      return;


  if ( local_panel )
	ptr_panel->loadprg(buf1);          // In LTGT3000.cpp.
  else
  {
    //	if( panel_info1.panel_type == MINI_T3000 )
    l = 13;

    i = 3;
    while ( i-- )
    {
      if ( t3000Request(50+100,31,buf1,&l,station_num, networkaddress)
             ==SUCCESS )
        break;
    }    
  }    

  // set the new program file that is loaded as the
  // 'default program' of that panel.
  {
    char *p;

    p = strrchr(
        LoadPanelFileDlg->FileName.c_str(), '\\');

    *p = '\0';    
    strcpy( ptr_panel->Program_Path,
            LoadPanelFileDlg->FileName.c_str() );
    strcpy( ptr_panel->Default_Program, (p+1) );
  }

}
//---------------------------------------------------------------------------

/**********************************************************************/
/* LTG:  Aug.  8, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   save the control panel memory to a program file on               */
/*   the attached external media (disk, flash memory).                */
/*    1. For a T3000 control panel.                                   */
/*       save to disk.                                                */
/*    2. For a mini control panel.                                    */
/*       save to its flash memory. The program name is not relevant.  */
/*--------------------------------------------------------------------*/
/* Updating:  Aug.  8, 1997                                           */
/**********************************************************************/
int saveClick(TMainScreen *App)
{
/*
// read the drive and directory information from the remote panel
// and fill in the OpenDialogForm and then open the dialog

 if( local_panel )
 {
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
 }
 else
 {
  if ( oldStyleOpenDialog )
  {
   OpenDialogForm = new TOpenDialogForm( Application );
   OpenDialogForm->Caption = " Select the remote program file name";
   OpenDialogForm->ShowModal();
   delete OpenDialogForm;
  }
  else
  {
   OpenDialog95Form = new TOpenDialog95Form( Application );
   OpenDialog95Form->Caption = " Select the remote program file name";
   OpenDialog95Form->ShowModal();
   delete OpenDialog95Form;
  }
 }
*/
// read the drive and directory information from the remote panel
// and fill in the OpenDialogForm and then open the dialog

  int i = 3;
  DUint l = 13;

  char lpszTemp[100];
  char lpszFileName[65];      // File name.

    // User save it as a panel file on the external media attached to
    // the control panel....

 if( local_panel )
 {
  App->SaveDialog->DefaultExt = String("prg");
  App->SaveDialog->Filter     = "PRG files|*.PRG";
  App->SaveDialog->InitialDir = ptr_panel->Program_Path;
  App->SaveDialog->FileName   = ptr_panel->Default_Program;

  if ( ! App->SaveDialog->Execute() )
      return 0;
  strcpy(lpszFileName, App->SaveDialog->FileName.c_str() );

    // inquire if user ensure to open this panel file.
  UserEnsureForm->Left= App->Left+(App->Width-UserEnsureForm->Width)/2;
  UserEnsureForm->Top = App->Top + 150;
  strcpy(lpszTemp, "Save panel ");
  itoa(panel_info1.panel_number,&lpszTemp[strlen(lpszTemp)],10);
  strcat(lpszTemp, " as program ");
  strcat(lpszTemp, lpszFileName);

  UserEnsureForm->EnsureWinLabel->Caption = AnsiString( lpszTemp );
  UserEnsureForm->EnsureWinLabel->Alignment = taCenter;
  UserEnsureForm->ShowModal();

    // if user not want to load this file
  if ( UserEnsureForm->Ok == 0 )
      return 0;

  saveoldprg(lpszFileName);                    // In LTGT3000.cpp.
  ptr_panel->savefile(lpszFileName, 1);        // In LTGT3000.cpp.
  return 1;
 }
 else
 {
  if ( panel_info1.panel_type == T3000 )
  {
    TSaveDialog95Form *SaveDialog95Form;
    SaveDialog95Form = new TSaveDialog95Form(Application,"*.prg");
    SaveDialog95Form->Set( default_prg, "prg");
//    SaveDialog->Filter = "PRG files|*.PRG";
    if(SaveDialog95Form->ShowModal()==mrOk)
    {
 // T3000
     i = 3;
     while(i--)
	 {
      // save control panel memory to it's  disk.
      strcpy(lpszFileName, SaveDialog95Form->FileName->Text.c_str() );
      if( strlen(lpszFileName) )
	    if( t3000Request(WRITE_COMMAND_50,SAVEPROGRAM_COMMAND,lpszFileName,&l,station_num, networkaddress )==SUCCESS )
        {
           break;
        }
        else
        {
         if ( i == 0 )
         {
          MessageBeep( MB_ICONEXCLAMATION );
	      MessageBox(GetFocus(), " Fail to save program ! ", "", MB_OK );
         }
        }
     }
    }
  } // end of "if( panel_info1.panel_type == T3000 )"
  else
  {
// mini -> save to flash
	if(panel_info1.panel_type == MINI_T3000)
	{
	 if( t3000Request(COMMAND_50, WRITEPRGFLASH_COMMAND, NULL, 0, station_num, networkaddress)!=SUCCESS )
	   if (t3000Request(COMMAND_50, WRITEPRGFLASH_COMMAND, NULL, 0, station_num, networkaddress)==SUCCESS)
       {
         MessageBeep( MB_ICONEXCLAMATION );
	     MessageBox(GetFocus(), " The panel is saving it's PRG into the Flash memory.         It will take about 25 seconds.", "", MB_OK );
       }
       else
       {
          MessageBeep( MB_ICONEXCLAMATION );
	      MessageBox(GetFocus(), " Fail to save program ! ", "", MB_OK );
       }
	}

  } // end of NOT "if( panel_info1.panel_type == T3000 )"
 }
}
//---------------------------------------------------------------------------

/**********************************************************************/
/* LTG:  Aug. 13, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   user to select a panel after a connection was established.       */
/*--------------------------------------------------------------------*/
/* Updating:  Aug. 13, 1997                                           */
/**********************************************************************/
void __fastcall TMainScreen::SelectpanelClick(TObject *Sender)
{
/*
  RS232Error  error;
  DInt        i, j, one_net = -1, l1, st, net;
  int  network_select, panel_select, 
       network_sel_tmp, panel_sel_tmp;
        
  for(j=0,i=0; i<MAX_Routing_table ; i++)
  {
    if( (Routing_table[i].status&PORT_ACTIVE)==PORT_ACTIVE )
	{
	  j++;
	  one_net=i;
	}
  }

  if ( j != 1 )
    one_net = -1;

  // ltg  ::select_panel(CSTATION, Routing_table[one_net].Port.network, 1);
  
  PanelSelect->ShowModal();
  
  network_select = network_sel_tmp;
  panel_select   = panel_sel_tmp;
                  
  if ( station_num != panel_select || networkaddress != network_select )
  {
    st = station_num;
	net= networkaddress;
	l1 = local_panel;
	station_num=panel_select;
	networkaddress=network_select;

	if ( station_num == Station_NUM && networkaddress == NetworkAddress ) 
      local_panel=1;
	else 
      local_panel=0;
      
	if(!local_panel)
	{
	  if( (error=ptr_panel->connect_panel_proc())!=SUCCESS )
	  {
	    station_num = st;
		networkaddress = net;
		local_panel=l1;
	  }
	}
	else
	{
// ltg	  setlocalpanel();
	}
    
	ptr_panel->GlPanel = station_num;
	ptr_panel->GlNetwork = networkaddress;

// ltg	display_pointer();
  }

  return;
*/
}
//---------------------------------------------------------------------------

/**********************************************************************/
/* LTG:  Aug.  6, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   read panel file from remote control panel memory                 */
/*   and save it in a program file on the disk attached               */
/*   to the user station.                                             */
/*--------------------------------------------------------------------*/
/* Updating:  Aug. 14, 1997                                           */
/**********************************************************************/
int  getClick(TMainScreen *App)
{
  char  lpszFileName[65];      // File name.
  char  lpszTemp[100];         // temp buffer.
  DInt  i = 3;
  DUint l = 13;

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


  /* ltg
  // read the remote control panel memory.
  while(i--)
  {
    // read all program to buffer.
    if ( t3000Request(READPROGRAM_T3000, 0, lpszBuf, &l, station_num,
                      networkaddress, BACnetConfirmedRequestPDU,
                      TIMEOUT_NETCALL, NULL, NULL, NULL, 0, 0) 
         ==SUCCESS )
    {
      i = 5;
	  break;
    }

    // delay.
    l = 10000;
    while (l--);
  }

  if ( i != 5 )
    return;
  */

  // User save it as a panel file on the external media attached to
  // the user station.....
  App->SaveDialog->DefaultExt = String("prg");
  App->SaveDialog->Filter     = "PRG files|*.PRG";
  App->SaveDialog->InitialDir = AnsiString( ptr_panel->Program_Path );
  App->SaveDialog->FileName   = AnsiString( ptr_panel->Default_Program );

  if ( ! App->SaveDialog->Execute() )
    return 0;

  strcpy(lpszFileName, App->SaveDialog->FileName.c_str() );

  // inquire if user ensure to open this panel file.
  UserEnsureForm->Left= App->Left+(App->Width-UserEnsureForm->Width)/2;
  UserEnsureForm->Top = App->Top + 150;
  strcpy(lpszTemp, "Save panel ");
  itoa(panel_info1.panel_number,&lpszTemp[strlen(lpszTemp)],10);
  strcat(lpszTemp, " as program ");
  strcat(lpszTemp, lpszFileName);

  UserEnsureForm->EnsureWinLabel->Caption = AnsiString( lpszTemp );
  UserEnsureForm->EnsureWinLabel->Alignment = taCenter;
  UserEnsureForm->ShowModal();

    // if user not want to load this file
  if ( UserEnsureForm->Ok == 0 )
      return 0;

  saveoldprg(lpszFileName);                    // In LTGT3000.cpp.
  ptr_panel->savefile(lpszFileName, 0);        // In LTGT3000.cpp.
  return 1;
}
//---------------------------------------------------------------------------

/**********************************************************************/
/* LTG:  Aug.  7, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   load a program file from the user station disk to                */
/*   a remote control panel ( T3000 or mini ) memory.                 */
/*--------------------------------------------------------------------*/
/* Updating:  Aug. 15, 1997                                           */
/**********************************************************************/
void __fastcall TMainScreen::SendClick(TObject *Sender)
{
/* ltg
  TOpenOptions o;

  o << ofFileMustExist << ofNoChangeDir	<< ofShowHelp;   // Initialize
//  if ( oldStyleOpenDialog )
     o << ofOldStyleDialog;

  TFileName NameOfFile;

  OpenDialog->DefaultExt = String("hex");
  OpenDialog->Filter = "HEX files|*.HEX";
  OpenDialog->Title  = " Program file name to be send";
  OpenDialog->Options = o;

  if( ! OpenDialog->Execute() )
  {
    return;
  }
*/

  char lpszFileName[65];      // File name.

  // User choose a panel file.....
  LoadPanelFileDlg->InitialDir =
                          AnsiString( ptr_panel->Program_Path );
  LoadPanelFileDlg->FileName = 
                          AnsiString( ptr_panel->Default_Program );

  if ( ! LoadPanelFileDlg->Execute() )
    return;

  // panel file name.
  strcpy(lpszFileName, LoadPanelFileDlg->FileName.c_str() );

  // inquire if user ensure to open this panel file.
  UserEnsureForm->Left=
        MainScreen->Left+(MainScreen->Width-UserEnsureForm->Width)/2;
  UserEnsureForm->Top = MainScreen->Top + 90;
  {
    char lpszTemp[100];

    sprintf(lpszTemp, "Send program '%s' to panel ?", lpszFileName);
    UserEnsureForm->EnsureWinLabel->Caption = "Send the program to this panel ?";
    UserEnsureForm->EnsureWinLabel->Caption = AnsiString(lpszTemp);
    UserEnsureForm->EnsureWinLabel->Alignment = taCenter;
  }
  UserEnsureForm->ShowModal();

  // if user not want to load this file
  if ( UserEnsureForm->Ok == 0 )
      return;

  // write to mini panel memory.  ??????      
  if(panel_info1.panel_type == MINI_T3000)
  {
    // t3000.cpp (DOS version).
    if( t3000Request(COMMAND_50, WRITEPRGFLASH_COMMAND, NULL, 0,
					 station_num, networkaddress)!=SUCCESS )
 	  t3000Request(COMMAND_50, WRITEPRGFLASH_COMMAND, NULL, 0,
 				   station_num, networkaddress);


    MessageBox(GetFocus(),
    " The panel is saving it's PRG into the Flash memory.         It will take about 25 seconds. ",
    "", MB_OK );
  }
  else
  {
    ptr_panel->loadprg(lpszFileName);          // In LTGT3000.cpp.

/*    
    // write to t3000 memory.
    DUint l = 13;
          
    DInt i = 3;

	while(i--)
	{
	  if ( t3000Request(50+100,30,lpszFileName,&l,station_num, networkaddress)
             ==SUCCESS )
		  break;

      int iTemp = 10000;
      while ( iTemp-- );
	}
*/    

  /*
    char *FileBuf;
    FILE *h;

    if ( ( h = fopen(lpszFileName, "r" )) == NULL )
    {
      MessageBox(GetFocus()," No this File:", "", MB_OK );
      return;
    }

    l = (DUint)filelength(fileno(h));
    FileBuf = (char *)malloc(l+2);

    fseek(h, 0, SEEK_SET);
    fread(FileBuf, l, 1, h);
    fclose(h);

    t3000Request(WRITEPROGRAM_T3000, 0, FileBuf, &l, station_num,
                networkaddress, BACnetConfirmedRequestPDU,
                TIMEOUT_NETCALL, NULL, NULL, NULL, 0, 0);
*/                
  }

  // set the new program file that is loaded as the
  // 'default program' of that panel.
  {
    char *p;

    p = strrchr(
        LoadPanelFileDlg->FileName.c_str(), '\\');

    *p = '\0';
    strcpy( ptr_panel->Program_Path,
            LoadPanelFileDlg->FileName.c_str() );
    strcpy( ptr_panel->Default_Program, (p+1) );
  }

  return;
}

