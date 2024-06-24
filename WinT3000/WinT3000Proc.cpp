//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include <time.h>
#include "define.h"
#include "wint3.h"
#include "Grid.h"
#include "DataModule.h"
#include "systemlist.h"
//---------------------------------------------------------------------------
#include "t3000def.h"
#include "aio.h"
#include "ptpdll.h"
#include "xms.h"

extern DInt           Version;   
extern Panel_info1    Panel_Info1, panel_info1;
extern DInt           local_panel;
extern DInt           refreshTime;
extern bool           refreshTimeFlag;
extern Byte           serial_access;
extern Panel_info1    panel_info1;
extern DInt           station_num;
extern DInt           networkaddress;
extern unsigned DByte tbl_bank[MAX_TBL_BANK];
extern XmsDes         *Des;
extern int maxgridTable;
extern GRIDTABLE gridTable[MAXGRIDTABLE];
extern DByte *points_text[];
extern char *heap_buf;
extern char *heap_amon;
extern icon *points_icon;

extern void disconnect_serial(void);
extern void StatusBarMessage(int i, AnsiString text);
extern void ProcEnableTimer(void);
extern void ProcDisableTimer(void);
extern bool ProcTimerEnable(void);
extern void ProcSendMessage(int mes);
extern void PanelNameMessage( AnsiString text );
extern void ShowGridForm(void);
extern void setUpData(TGridForm *grid, DInt point_type);
extern void resetImage(void);
extern DInt readDesSerial(void);

/*============================< Local variables >========================*/
Panel    *ptr_panel, *PTR_panel;
DInt     comPort;
DInt     Point_type;
DByte    *ptr_point_table;
int timevar;

AnsiString estab_text     = "Trying to establish connection on COM1 ...";
AnsiString connecttopanel = "Connected to ";
AnsiString readingdestext = "Reading descriptors... ";


DInt checkConnection(void);

extern int alignInputs(int t, DByte *buf, Str_in_point *inputs, int n);
extern int alignOutputs(int t, DByte *buf, Str_out_point *inputs, int n);
extern int alignVars(int t, DByte *buf, Str_variable_point *vars, int n);
extern int alignCons(int t, DByte *buf, Str_controller_point *cons, int n);
extern int alignWeeklyroutines(int t, DByte *buf, Str_weekly_routine_point *wr, int n);
extern int alignAnnualroutines(int t, DByte *buf, Str_annual_routine_point *annual, int n);
extern int alignArrays(int t, DByte *buf, Str_array_point *ay, int n);
extern int alignScreens(int t, DByte *buf, Control_group_point *grps, int n);
extern int alignPrograms(int t, DByte *buf, Str_program_point *prgs, int n);

void ProcReadDesSerial()
{
  if(serial_access)
  {
 	readDesSerial();
    StatusBarMessage(SBText, connecttopanel + AnsiString((char *)panel_info1.panel_name));
  }
}

bool ProcConnectClick(TMainScreen *App)
{
 DInt TimeOut;
 MSG myMsg;
 DByte j;
 DInt result;
 bool ret = false;
 if( !serial_access )
 {
     if( (result = installPort(App->Handle,comPort))==SUCCESS_REQUEST )
     {
         StatusBarMessage(SBText, estab_text);
		 if( (result=establishConnection(comPort, &networkaddress, &station_num))==SUCCESS_REQUEST)
		 {
                StatusBarMessage(SBText, "Set up connection...");
				if( (result=ptr_panel->connect_panel_proc(1))==RS232_SUCCESS )
				{
				 serial_access = 1;
				 local_panel   = 0;
                 StatusBarMessage(SBText, readingdestext);
				 readDesSerial();

                 StatusBarMessage(SBText, connecttopanel + AnsiString((char *)panel_info1.panel_name));
                 PanelNameMessage(AnsiString((char *)panel_info1.panel_name));
                 resetImage();
				 MessageBeep( MB_ICONEXCLAMATION );
                 ret = true;
				}
				else
				{
				 disconnect_serial();
				 MessageBeep( MB_ICONEXCLAMATION );
				 MessageBox(GetFocus(), " Error setup connection! ", "", MB_OK );
				}
		 }
		 else
		 {
	         MessageBeep( MB_ICONEXCLAMATION );
			 disconnect_serial();
             if( result == SAME_NETWORK_NUMBER )
             {
				 MessageBox(GetFocus(), " Connection failed.Change the local network number! ", "", MB_OK );
             }
             StatusBarMessage(SBText, "Disconnected");
		 }
     }
 }
 else
 {
		 MessageBeep( MB_ICONEXCLAMATION );
		 MessageBox(GetFocus(), " Connection alredy established! ", "", MB_OK );
 }
 return ret;
}

void ProcReadPoints(TForm *App, DInt point_type)
{
 int        menu, refresh, width, height, recordcount, i;
 bool       empty;
 DLong      readonlyfields;
 RS232Error result;
 DUint      l1;
 DByte      *buf;
 char       title[80];
 TTable     *table;
 TDataSource *ds;

 result  = RS232_SUCCESS;
 menu    = 1;
 empty   = true;
 refresh = 10000;
 readonlyfields = 0;
 height         = 364;

 buf = new DByte [10000];

 if( point_type!=SYSTEM_LIST && point_type!=NETSTAT)
 {
  strcpy(title,"  ");
  strcat(title,(char *)points_text[point_type]);
  recordcount = tbl_bank[point_type];
    StatusBarMessage(SBText, "Reading ...");

// read data from the remote panel
  if( serial_access )
    result=t3000Request(point_type+1, 0, buf, &l1, station_num, networkaddress);
  if(result==SUCCESS)
  {
   switch( point_type ) {
       case T3000_OUT:
         width = 630;
         readonlyfields = 0x35;          // field 0 and 4
         table = DM->OutputsTable;
         ds    = DM->OutputsDataSource;
         if( serial_access )
         	alignOutputs(1,buf,ptr_panel->outputs,tbl_bank[point_type]);
         break;
       case T3000_IN:
         width = 610;
         readonlyfields = 0x01B5;      //0x01B5    // field 0 and 4
         table = DM->InputsTable;
         ds    = DM->InputsDataSource;
         if( serial_access )
	         alignInputs(1,buf,ptr_panel->inputs,tbl_bank[point_type]);
         break;
       case VAR:
         width = 500;
         readonlyfields = 0x01;          // field 0
         table = DM->VarsTable;
         ds    = DM->VarsDataSource;
         if( serial_access )
	         alignVars(1,buf,ptr_panel->vars,tbl_bank[point_type]);
         break;
       case CON:
         width = 656;
         readonlyfields = 0x012D;
         table = DM->ConsTable;
         ds    = DM->ConsDataSource;
         if( serial_access )
	         alignCons(1,buf,ptr_panel->controllers,tbl_bank[point_type]);
         break;
       case WR:
         width = 528;
         readonlyfields = 0x0A1;
         table = DM->WrsTable;
         ds    = DM->WrsDataSource;
         if( serial_access )
	         alignWeeklyroutines(1,buf,ptr_panel->weekly_routines,tbl_bank[point_type]);
         break;
       case AR:
         width = 424;
         height = 220;
         readonlyfields = 0x01;
         table = DM->ArsTable;
         ds    = DM->ArsDataSource;
         if( serial_access )
	         alignAnnualroutines(1,buf,ptr_panel->annual_routines,tbl_bank[point_type]);
         break;
       case PRG:
         width = 464;
         readonlyfields = 0x01;
         table = DM->PrgsTable;
         ds    = DM->PrgsDataSource;
         if( serial_access )
	         alignPrograms(1,buf,ptr_panel->programs,tbl_bank[point_type]);
         break;
       case GRP:
         width = 394;
         readonlyfields = 0x01;
         table = DM->ScreensTable;
         ds    = DM->ScreensDataSource;
         if( serial_access )
	         alignScreens(1,buf,ptr_panel->control_groups,tbl_bank[point_type]);
         break;
       case AY:
         width = 408;
         readonlyfields = 0x01;
         table = DM->AysTable;
         ds    = DM->AysDataSource;
         if( serial_access )
	         alignArrays(1,buf,ptr_panel->arrays,tbl_bank[point_type]);
         break;
   }
  }
 }
 else
 {
  if( point_type==SYSTEM_LIST )
  {
   menu    = 2;
   empty   = false;
   refresh = -1;
   width   = 518;
   strcpy(title,"  System list");
   recordcount = 64;
   table = DM->SysListTable;
   ds    = DM->SysListDataSource;
  }
  if( point_type==NETSTAT )
  {
   menu    = 3;
   empty   = true;
   refresh = -1;
   width   = 600;
   strcpy(title,"  Network status");
   recordcount = 32;
   table = DM->NetStatTable;
   ds    = DM->NetStatDataSource;
  }
 }

 resetImage();
 if( result == RS232_SUCCESS )
 {
     MainScreen->Update();
     // find an next empty entry
     for(i=0; i<MAXGRIDTABLE; i++)
      if ( !gridTable[i].grid ) break;
     if( i<MAXGRIDTABLE )
     {
//  create an object of type TGridForm
      gridTable[i].grid = new TGridForm(App, point_type, recordcount, i);
      gridTable[i].grid->Setup(station_num, networkaddress, ds, table, title, width, height, readonlyfields, refresh, menu, empty);
      gridTable[i].grid->setUpData();
      StatusBarMessage(SBText, connecttopanel + AnsiString((char *)panel_info1.panel_name));
     }
     else
     {
      StatusBarMessage(SBText, connecttopanel + AnsiString((char *)panel_info1.panel_name));
	  MessageBeep( MB_ICONEXCLAMATION );
	  MessageBox(GetFocus(), " To many windows open! ", "", MB_OK );
     }
 }
 else
     StatusBarMessage(SBText, connecttopanel + AnsiString((char *)panel_info1.panel_name));
if (buf)
  delete [] buf;
}

void ProcInit(void)
{
  DByte i;

  Panel_Info1.panel_type   = T3000;
  Panel_Info1.panel_number = Station_NUM;
  Panel_Info1.network      = NetworkAddress;
  Panel_Info1.version      = Version;
  strcpy( Station_NAME, "PANEL 29");
  strcpy( NetworkName, "Network77");
  memcpy(Panel_Info1.network_name, NetworkName, sizeof(Panel_Info1.network_name));
  memcpy(Panel_Info1.panel_name, Station_NAME, sizeof(Panel_Info1.panel_name));
  Panel_Info1.active_panels = (1L<<(Station_NUM-1));
  panel_info1 = Panel_Info1;

  PTR_panel = new Panel();
  ptr_panel = PTR_panel;
}


void ProcDisconnectClick(void)
{
    if( serial_access )
    {
     StatusBarMessage(SBText, "Wait for disconnection...");
	 if(Des)
	 {
			delete Des;
			Des = 0;
	 }
     disconnectRequest(comPort);
	 disconnect_serial();
	 MessageBeep( MB_ICONEXCLAMATION		 );
     StatusBarMessage(SBText, "Disconnected");
     PanelNameMessage("Disconnected");
    }
}

void ProceXitClick(void)
{
  ProcDisconnectClick();
  if (heap_buf)
  {
    delete heap_buf;
    heap_buf=NULL;
  }
  if (heap_amon)
  {
    delete heap_amon;
    heap_amon=NULL;
  }
  if ( points_icon )
  {
    delete points_icon;
    points_icon=NULL;
  }
  if(ptr_panel)
  {
  		delete ptr_panel;
        ptr_panel=NULL;
  }
}



