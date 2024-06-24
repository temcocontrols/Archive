#include <vcl\vcl.h>
#pragma hdrstop
#include <dir.h>
#include <dos.h>
#include <bios.h>
#include <nb30.h>
#include <net_bac.h>
#include <netb_bac.h>
#include <ipx.h>
#include <time.h>
#include "define.h"
#include "wint3.h"
#include "baseclas.h"
#include "LTGLoginInit.h"
#include "LTGUserEnsur.h"
#include "LTGdef.h"
#include "parser.h"
#include "aio.h"
#include "xms.h"
#include "ptpdll.h"

//ltg  config file format.
#define CNF_VER             0
#define CNF_RESERVED        CNF_VER+2
#define CNF_STATION_NUM     CNF_RESERVED+10
#define CNF_STATION_NAME    CNF_STATION_NUM+2
#define CNF_NETWORK_NUMBER  CNF_STATION_NAME+16
#define CNF_NR_BOARDS       CNF_NETWORK_NUMBER+2+16
#define CNF_SIZE_BOARD_INFO CNF_NR_BOARDS+2
#define CNF_BOARD           CNF_SIZE_BOARD_INFO+2
#define CNF_SIZE_COMM_INFO  CNF_BOARD+MAX_BOARDS*sizeof(Board_info)
#define CNF_COMM_INFO       CNF_SIZE_COMM_INFO+2
#define CNF_MODEM         CNF_COMM_INFO+MAX_COMM_INFO*sizeof(CommINFO)
#define CNF_PROGRAM_PATH  CNF_MODEM+sizeof(ModemCapabilities)
#define CNF_PROGRAM_DES   CNF_PROGRAM_PATH+64
#define CNF_PROGRAM_BASIC CNF_PROGRAM_DES+64
#define CNF_PROGRAM_MONITOR  CNF_PROGRAM_BASIC+64
#define CNF_PROGRAM_GRAPHICS CNF_PROGRAM_MONITOR+64
#define CNF_DEFAULT_PROGRAM  CNF_PROGRAM_GRAPHICS+64
#define CNF_CONSOLE          CNF_DEFAULT_PROGRAM+13
#define CNF_RINGS            CNF_CONSOLE+1
#define CNF_STATION_LIST     CNF_RINGS+1
#define CNF_IND_PASS         CNF_STATION_LIST+MAX_STATIONS*sizeof(Station_point)

#define MAX_INPUT_BUFFER 2000

extern char control;          //?????
extern char interface_mode;
extern char oldpath[65];
extern int oldrive;
extern int refresh_time_init;
extern int refresh;
extern long t;
extern int refresh_time;
extern unsigned long grp_time;
extern unsigned long display_grp_time;
extern char present_analog_monitor, save_prg_flag;
extern char access_level;
extern char *version;
extern int Version;    //1.3
extern unsigned int MAX_HEAP_BUF, MAX_HEAP_GRP, MAX_HEAP_AMON, MAX_HEAP_ARRAY;
extern unsigned int MAX_HEAP_DIGM;
extern long *heap_array;
extern char *heap_buf;
extern char *heap_grp;
extern char *heap_amon;
extern Heap_dmon *heap_dmon;
extern DUlong timestart;    // sec de la inceputul programului
extern DInt networkaddress;    // sec de la inceputul programului
extern Panel *ptr_panel;
extern DInt local_panel;
extern DInt  station_num;
extern Byte serial_access;
extern long timest;
extern DInt timewait(DWORD timesleep, DUint msg, MSG& myMsg);
extern short int cnf_version;
extern int nr_boards;
extern Board_info	boards[MAX_BOARDS]; // In Date.cpp
extern DInt ind_station;                      // In date.cpp
extern DByte station_name[NAME_SIZE];         // In Date.cpp
extern void init_boards_struct( void );
extern Panel_info1 Panel_Info1,panel_info1;
extern char Panel_Type;          // In Date.cpp  = T3000;
extern Password_struct passwords;
extern Station_point station_list[MAX_STATIONS];
extern void save_config(int what);
/**********************************************************************/
/* LTG:  July 23, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Initialization of system.                                        */
/*   1. read config;                                                  */
/*   2. login;                                                        */
/*--------------------------------------------------------------------*/
/*   Calling:  WinMain() In WinT3000.cpp                              */
/*--------------------------------------------------------------------*/
/* Updating: Aug. 12, 1997                                            */
/**********************************************************************/
int InitializeSystem()
{
  char *ontext;
  int i,j,n;


//B******************    Adi    ******************************************
 heap_buf = new char [(long)MAX_HEAP_BUF+(long)MAX_HEAP_GRP];
 heap_grp = ((char *)heap_buf)+MAX_HEAP_BUF;

 heap_amon = new char [(long)MAX_HEAP_AMON+(long)MAX_HEAP_DIGM*sizeof(Heap_dmon)+(long)MAX_HEAP_ARRAY*4L];
 heap_dmon = (Heap_dmon *)(((char *)heap_amon)+(long)MAX_HEAP_AMON);
 heap_array = (long *)(((char *)heap_dmon)+(long)MAX_HEAP_DIGM*sizeof(Heap_dmon));

 //E******************    Adi    ******************************************


/**********************************************************************/
/* LTG:  July 23, 1997.                                               */
/*       Read config file.                                            */
/**********************************************************************/
#ifndef WORKSTATION
  FILE *fhandle;
  
  if( (fhandle=fopen("config.cnf","rb+"))!=NULL)
  {
	fseek(fhandle,CNF_VER,SEEK_SET);       // from 0 (CNF_VER).
	fread(&cnf_version, 2, 1, fhandle);    //config.cnf version

	if(cnf_version >= 212)
	{
	  fseek(fhandle,CNF_STATION_NUM,SEEK_SET);
	  fread(&Station_NUM, 1, 2, fhandle);
 	  fread(Station_NAME, 16, 1, fhandle);

	  fread(&NetworkAddress, 1, 2, fhandle);
//ltg** next statement only for dddeleting previous
//ltg** statement:   fread(&NetworkAddress, 1, sizeof(DInt), fhandle);
//      fread(NetworkName, 1, 2, fhandle);

//ltg 	  fread(&NetworkName, 16, 1, fhandle);
      fread(NetworkName, 16, 1, fhandle);

	  fread(&nr_boards, 2, 1, fhandle);
	  fread(&n, 2, 1, fhandle);    //size
	  for(i=0;i<MAX_BOARDS;i++)
	  {
	    if(sizeof(Board_info)>=n)
		  fread(&boards[i], n, 1, fhandle);
		else
		{
		  fread(&boards[i], sizeof(Board_info), 1, fhandle);
		  fseek(fhandle,n-sizeof(Board_info),SEEK_CUR);
		}
	  }

	  fread(&n, 2, 1, fhandle);   //size CommInfo
	  j=MAX_COMM_INFO;
	  for(i=0;i<j;i++)
	  {
// ltg 			if(sizeof(CommINFO)>=n)
// ltg 			 fread(&CommInfo[i], n, 1, fhandle);
// ltg 			else
			{
// ltg 			 fread(&CommInfo[i], sizeof(CommINFO), 1, fhandle);
// ltg 			 fseek(fhandle,n-sizeof(CommINFO),SEEK_CUR);
			}
	  }
	  fclose(fhandle);
	}  // end of " if(cnf_version >= 212) "
	else
	{
	  fclose(fhandle);
      MessageBox(GetFocus(),
        " Old configuration file! Created a new one.", "", MB_OK );

	  init_boards_struct();
	}
  }   // end of open file successfully.
  else
  {
	//  fail to open config file.
    init_boards_struct();
  }

  strcpy((char *)station_name, (char *)Station_NAME);
  station_num=Station_NUM;
  local_panel=1;
  networkaddress = NetworkAddress;
  Panel_Info1.panel_type   = Panel_Type;
  Panel_Info1.panel_number = Station_NUM;
  Panel_Info1.network      = NetworkAddress;
  Panel_Info1.version      = Version;
  memcpy(Panel_Info1.network_name, NetworkName, sizeof(Panel_Info1.network_name));
  memcpy(Panel_Info1.panel_name, Station_NAME, sizeof(Panel_Info1.panel_name));
  Panel_Info1.active_panels = (1L<<(Station_NUM-1));
  panel_info1 = Panel_Info1;

#endif

#ifdef WORKSTATION
  strcpy(station_name, "Station 1       ");
  station_num=1;
  local_panel=0;
#endif

  memset(&passwords,'\0',sizeof(passwords));
  access_level=0;

  if( (fhandle=fopen("config.cnf","rb+"))!=NULL)
  {
    long temp;

    temp = CNF_BOARD;
    temp = CNF_SIZE_COMM_INFO;
    temp = CNF_COMM_INFO;
    temp = CNF_MODEM;

 	fseek(fhandle,temp,SEEK_SET);
    fseek(fhandle,CNF_STATION_LIST,SEEK_SET);
    fread(station_list,sizeof(Station_point),MAX_STATIONS,fhandle);

    fseek(fhandle,CNF_STATION_LIST+MAX_STATIONS*sizeof(Station_point),
      			  SEEK_SET);
	fread(&passwords.ind_passwords,2,1,fhandle);
	fread(passwords.passwords,sizeof(Password_point),
    						  passwords.ind_passwords,fhandle);

	fclose(fhandle);
  }

strcpy((char *)station_list[Station_NUM-1].name,(char *)Station_NAME);

//ltg 	hide_pointer();

	for(i=0; i<4; i++)
	{
// ltg 	 if( CommInfo[i].connection == MOUSE_LINK )
	 {
// ltg 		 CommInfo[i].connection = 0;
// ltg 		 CommInfo[i].baudrate = 38400L;
	 }
	}
//ltg 	mouseint();

/**********************************************************************/
/* LTG:  July 25, 1997.                                               */
/*       Read ????? file.                                            */
/**********************************************************************/
  {

	oldrive=getdisk();
	getcwd(oldpath,60);
	if(oldpath[strlen(oldpath)-1]=='\\') oldpath[strlen(oldpath)-1]=0;
	if(!ptr_panel)
	{
      ptr_panel = new Panel();
	  if(!ptr_panel)
	  {
        MessageBox(GetFocus(),"Not enough memory!", "", MB_OK );
		exit(0);
	  }

	  if(cnf_version < 140)
	  {
	    save_config(CNF_ALL);
		cnf_version = Version;
	  }

#ifndef WORKSTATION
	  if(!Station_NUM)
	  {
 	    strcpy((char *)Station_NAME, (char *)"PANEL1");
	    Station_NUM=1;
	    if( control )
        {
// ltg 		boardconfig();
	    }
	    logout=1;
 	    strcpy((char *)station_list[Station_NUM-1].name,(char *)Station_NAME);
 	    strcpy((char *)station_name, (char *)Station_NAME);
	    station_num=Station_NUM;
	    local_panel=1;
	  }
#endif

	  ptr_panel->GlPanel=station_num;
//ltg 	ptr_panel->GlNetwork = networkaddress;

#ifdef NETWORK_COMM
//ltg 	  operator_list.state = IDLE;
	  direct_point.state = DIRECT_IDLE;
#endif

	  Station_point *stp;

	  for(i=0,stp=station_list; i < MAX_STATIONS; stp++,i++)
	  {
		 if(i+1!=Station_NUM)
			 stp->state = 0;
		 else
		  {
			 stp->state = 1;
 			 strcpy( (char *)stp->name, (char *)Station_NAME );
		  }
		 stp->des_length = 0;
	  }


/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   ??? adr of  TASK InitializeSystem.                               */
/**********************************************************************/

	  if(logout)
	  {
		 save_config(CNF_ALL);
		 logout = 0;
	  }

#ifdef NETWORK_COMM
	  user.state = REQUEST_HOSTS_NAMES;
	  if(tasks[NETWORK].status!=DEAD)
	  {
        MessageBox(GetFocus()," Connecting to network... ", "", MB_OK );
//ltg 			resume(NETWORK);
//ltg 			suspend(PROJ);
	  }

#endif
	}  // end of "if(!ptr_panel)".

#ifdef NETWORK_COMM
	if( Netbios_state == NB_INSTALLED && panels_number )
	{
	 ind_station = (int)panels_number;
//	 if(extended_memory)
	 {
//		DisplayMessage(20, 7, 60, 11, " Reading descriptors... ",&D);
		DisplayMessageT(20, 7, 60, 11, ctrlbreak_text, " Reading descriptors... ", NULL, Black, &D);
		int net_length=0;
		for(i=0,Station_point *p=station_list; p < &station_list[MAX_STATIONS]; p++,i++)
//		for(i=0;i<32;i++)
		 if(p.state && Station_NUM!=i+1)
			 net_length += p.des_length;
		Des = new XmsDes(net_length);
		for(i=0;i<MAX_STATIONS;i++)
		if(Station_NUM!=i+1)
		 if(station_list[i].state)
			{
				for(int j=0;j<=AY;j++)
				 if(point_des[j]>=0)
					{
 					 t3000Request(47, j+1, Des->buffer, 0, i+1);
					 int l;

//ltg 					 Des->put(i+1,j,operator_list.length);
		}

	  }
    }
#endif
  }

/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   If load default prg ?                                            */
/**********************************************************************/
     NCB ncb;
     memset(&ncb,0,sizeof(NCB));
     ncb.ncb_command=0x7F;
     Netbios(&ncb);

	 if(!console_mode)
	 if(!LOADPRG)
	 {
		tempbuf[0] = 0;
		strcpy(tempbuf, ptr_panel->Program_Path);
		if(strlen(tempbuf)) strcat(tempbuf,"\\");
		strcat(tempbuf, ptr_panel->Default_Program);
		ptr_panel->loadprg(tempbuf);
		LOADPRG=1;
	 }


//ltg 	G.GShowMenuBar(message,listonscreen);
//ltg 	G.ShowunderMBar();
//ltg 	install_port();

	main_screen = 0x02;

/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   Main LOOP of T3000.                                              */
/*   1. Start T3000.                                                  */
/*   2. Get password.                                                 */
/*   3. Start default panel (PRG).                                    */
/*   4. Display main menu.                                            */
/*   5. get user access.                                              */
/**********************************************************************/
  int x;

/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   Display first graph of T3000.                                    */
/*   TITLE: THIS IS OUR T3000.                                        */
/**********************************************************************/
  if( (h=fopen("T3000.TIF","r")) != NULL )
  {
    fclose(h);

//ltg 		  hide_pointer();

// ltg 	rgb pal1[16];
    
//ltg 		  grp_current_pal=(rgb *)pal1;
//ltg 		  tif("T3000.TIF",0,0,maxx,maxy);
//ltg 		  Darkgray=closest_rgb( init_pal[Darkgray], grp_current_pal, 16);
//		set16( init_pal, 0 );
//ltg 		  display_pointer();
  }
  else              // open t3000.tif successfullly.
  {
//ltg 		  Bordergr(1, 1, 80, 25, Blue1, White, Black, Blue1, 3, 0);
  }

  ex_apl = 0;       // not exit.

/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   Check user name and password.                                    */
/**********************************************************************/

  MSG myMsg;
  if( time(NULL) < timesec1970+2 )
  {
    timewait( (timesec1970+2-time(NULL))*1000L+500, 0, myMsg);
  }
  MainScreen->LogoPanel->Visible = false;
  if( get_access() )
  {
   memcpy(Password, password, sizeof(Password));
   memcpy(User_name, user_name, sizeof(User_name));

   if ( default_group )
   {
		  itoa(default_group, tempbuf, 10);
		  for(i=0;i<strlen(tempbuf); i++)
			{
//ltg 			  input_buffer[i+4].key.i=tempbuf[i];
			}
//ltg 		  input_buffer[i+4].key.i=13;
//ltg 		  simulate=1;
//ltg 		  ind_input_buffer=0;
   }
   return 1;
  }
  else
  {
    ProceXitClick();
    return 0;
  }
}
//---------------------------------------------------------------------------

/**********************************************************************/
/* LTG:  JuLY 28, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Saving config file.                                              */
/*--------------------------------------------------------------------*/
/*	2 bytes  : version                                                */
/*	10 bytes : reserved                                               */
/*	2 bytes  : Station_NUM                                            */
/*	16 bytes : Station_NAME                                           */
/*	2 bytes  : nr_boards                                              */
/*	2 bytes  : sizeof(Board_info);                                    */
/*	MAX_BOARDS*sizeof(Board_info) bytes  : boards                     */
/*	2 bytes  : sizeof(CommINFO)                                      */
/*	MAX_COMM_INFO * sizeof(CommINFO) bytes : CommInfo               */
/*--------------------------------------------------------------------*/
/* Updating:  July 28, 1997                                           */
/**********************************************************************/
void save_config(int what)
{
  char buf[13]={0,0,0,0,0};
  char fname[65];
  int n;
  FILE *h;

  strcpy(fname, oldpath);
  strcat(fname,"\\config.cnf");

  if( (h=fopen(fname,"rb+"))==NULL)
    if( (h=fopen(fname,"wb+"))==NULL)
	{
	  return;
	}

  fseek(h,CNF_VER ,SEEK_SET);
  fwrite(&Version, 2, 1, h);
  fwrite(buf, 10, 1, h);
  if( what & CNF_BOARD_INFO )
  {
    fseek(h,CNF_STATION_NUM,SEEK_SET);
	fwrite(&Station_NUM, 2, 1, h);
	fwrite(Station_NAME, 16, 1, h);
	fwrite(&NetworkAddress, 1, 2, h);
	fwrite(NetworkName, 16, 1, h);

	fwrite(&nr_boards, 2, 1, h);
	n=sizeof(Board_info);
	fwrite(&n, 2, 1, h);
	fwrite(boards, MAX_BOARDS*sizeof(Board_info), 1, h);
  }

  if( what & CNF_COMM )
  {
    fseek(h,CNF_SIZE_COMM_INFO, SEEK_SET);
 	n=sizeof(CommINFO);
	fwrite(&n, 2, 1, h);
 	fwrite(CommInfo, MAX_COMM_INFO * sizeof(CommINFO), 1, h);
  }

  if( what & CNF_PATH_INFO )
  {
 	fseek(h,CNF_PROGRAM_PATH,SEEK_SET);
	fwrite(ptr_panel->Program_Path, 64, 1, h);

	fwrite(ptr_panel->Descriptor_Path, 64, 1, h);
	fwrite(ptr_panel->Basic_Path, 64, 1, h);
	fwrite(ptr_panel->Monitor_Path, 64, 1, h);
	fwrite(ptr_panel->Graphics_Path, 64, 1, h);
	fwrite(ptr_panel->Default_Program, 13, 1, h);
  }

  if( what & CNF_CONSOLE_INFO )
  {
    fseek(h,CNF_CONSOLE, SEEK_SET);
    fwrite(&console_mode, 1, 1, h);
  }

  if( what & CNF_STATION_INFO )
  {
    fseek(h,CNF_STATION_LIST,SEEK_SET);
	fwrite(station_list,sizeof(Station_point),MAX_STATIONS,h);
  }

  if( what & CNF_PASS_INFO )
  {
    fseek(h,CNF_IND_PASS,SEEK_SET);
	fwrite(&passwords.ind_passwords,2,1,h);
	fwrite(passwords.passwords,sizeof(Password_point),passwords.ind_passwords,h);
  }

  fclose(h);
}
//----------------------------------------------------------------------------
//  ***********************************************************
//  Mao Hui Aug 14,1997
//  Panel::Panel(void) : Aio(nr_boards)
//	begin
//  ***********************************************************
Panel::Panel(void) : Aio(nr_boards)
{
//	start_apl=1;
//	ptr_panel = this;
//	strcpy(Program_Path, oldpath);
//	strcpy(Descriptor_Path, oldpath);
//	strcpy(Basic_Path, oldpath);
//	strcpy(Monitor_Path, oldpath);
//	strcpy(Graphics_Path, oldpath);
//	strcpy(DataBase_Path, oldpath);
    Default_Program[0] = 0;
	if( (h=fopen("config.cnf","rb+"))!=NULL)
	{
		fseek(h,20+MAX_BOARDS*sizeof(Board_info)+3*sizeof(CommInfo)+sizeof(ModemCapabilities),SEEK_SET);
        fseek(h,CNF_PROGRAM_PATH,SEEK_SET);
        fread(Program_Path, 64, 1, h);
        fread(Descriptor_Path, 64, 1, h);
        fread(Basic_Path, 64, 1, h);
        fread(Monitor_Path, 64, 1, h);
        fread(Graphics_Path, 64, 1, h);
        fread(Default_Program, 13, 1, h);
        fread(&console_mode, 1, 1, h);
        fread(&number_rings, 1, 1, h);
        memcpy(default_prg, Default_Program, 13);
        fclose(h);
	}
	GlPanel=station_num;
	GlNetwork=networkaddress;
	GlPanel_type = T3000;
}
//  ***********************************************************
//  Mao Hui Aug 14,1997
//  Panel::Panel(void) : Aio(nr_boards)
//	end
//  ***********************************************************

