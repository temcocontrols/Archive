#include <vcl\vcl.h>
#pragma hdrstop
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <dir.h>
#include "define.h"
#include "t3000def.h"
#include "aio.h"
#include "xms.h"
#include "ptpdll.h"

extern DByte *intervaltotext(DByte *textbuf, DLong seconds , DUint minutes , DUint hours, DByte *c);
extern DInt pwait(DWORD timesleep, DUint msg, MSG& myMsg, DInt breakcond=0);
extern void alignPanelInfo(int t, DByte *p_info1, Panel_info1 *panel_info1);
extern void upcustomunit(Units_element	*punits);
extern void StatusBarMessage(int i, AnsiString text);
extern void runImage(void);

extern dig_range_form dig_range_array[];
extern DUlong timestart;    // sec de la inceputul programului

extern struct  time ora_start;         // ora la inceputul programului
extern DUlong   ora_current_sec; // ora curenta exprimata in secunde
//struct  time  ora_current;      // ora curenta
extern Time_block ora_current;
extern DInt milisec;
extern DLong microsec;
extern XmsDes *Des;
extern DSignedChar point_des[AY+1];
extern AnsiString readingdestext;
extern DByte *points_text[];
extern Byte serial_access;
extern DInt networkaddress;    // sec de la inceputul programului
extern Panel *ptr_panel;
extern DInt local_panel;
extern DInt  station_num;
extern int nr_boards;
extern unsigned DByte tbl_bank[MAX_TBL_BANK];
extern DByte default_prg[13];
extern Panel_info1 Panel_Info1,panel_info1;
extern DByte GAlarm, gAlarm;
extern long galarm_read_time;
extern Password_struct passwords;
extern Station_point station_list[MAX_STATIONS];
extern DInt comPort;
extern DInt local_panel;

void adjust_readtime( Time_block *time_buf );  // mini->T3000
void adjust_writetime(Time_block *time_buf, Time_block_mini *timemini);

__declspec(dllimport) DInt desinstallPort(DInt comPort);


// DInt panels   :  -1   read from all panels
//                  >0   read only from panel number
DInt readDesSerial(void)
{
        MSG myMsg;
		DUlong activepanels;
		DUint l, l1, net_length=0;
		DInt v1,v2,i,j,result=ERROR_COMM;
		Panel_info1 panel_info,p_info1;

        v1 = 0;
        v2 = MAX_STATIONS;
/*
        if( panels == -1 )
        {
          v1 = 0;
          v2 = MAX_STATIONS;
        }
        else
        {
          v1 = panels-1;
          v2 = panels;
        }
*/
		activepanels = panel_info1.active_panels;
		activepanels = activepanels | (1L<<(station_num-1));
		for(i=v1;i<v2;i++)
		 if( (activepanels&(1L<<i)) )
		 {
				if( (result=t3000Request(COMMAND_50, PANEL_INFO1_COMMAND, (DByte*)&p_info1, 0, i+1, networkaddress))!=SUCCESS )
				 result = t3000Request(COMMAND_50, PANEL_INFO1_COMMAND, (DByte*)&p_info1, 0, i+1, networkaddress);
				if(result==SUCCESS)
				{
                    alignPanelInfo(1, (DByte *)&p_info1, &panel_info);
					station_list[i].state = 1;
					memcpy(station_list[i].name, panel_info.panel_name, sizeof(Station_NAME));
					station_list[i].des_length=panel_info.des_length;
				}
		 }
		l=net_length=0;
		for(i=0;i<MAX_STATIONS;i++)
		 if( (activepanels&(1L<<i)) )
		 {
			 net_length += station_list[i].des_length;
		 }
		if(Des)
		{
			delete Des;
			Des = 0;
		}
		if(net_length)
			Des = new XmsDes(net_length);

		if(net_length)
		{
		 for(i=0;i<MAX_STATIONS;i++)
			if( (activepanels&(1L<<i)) )
			{
				 for(j=0;j<=AY;j++)
				 if(point_des[j]>=0)
				 {
					 l1=0;
                     StatusBarMessage(SBText, readingdestext+AnsiString((char *)station_list[i].name)+AnsiString(": ")+AnsiString((char *)points_text[j]));
                     pwait(100, 0, myMsg);
					 if( (result=t3000Request(47, j+1, Des->buffer, &l1, i+1, networkaddress))!=SUCCESS )
						result=t3000Request(47, j+1, Des->buffer, &l1, i+1, networkaddress);
					 if( result==SUCCESS )
					 {
						if(l+l1>net_length) break;
						l += l1;
						Des->put(i+1,j,l1);
					 }
					 else
						 break;
				 }
			}
		}
		return result;
}

/*
void getpassword(DByte *pass, DByte *text)
{
}
*/


Panel::Panel(Panel &copy):Aio(copy.number_of_boards)
{
	*this = copy;
}

void Panel::operator=(const Panel &copy)
{
	if(this == &copy) return;
	Aio( copy.number_of_boards);
	strcpy(Program_Path, copy.Program_Path);
	strcpy(Descriptor_Path, copy.Descriptor_Path);
	strcpy(Basic_Path, copy.Basic_Path);
	strcpy(Monitor_Path, copy.Monitor_Path);
	strcpy(Graphics_Path, copy.Graphics_Path);
	strcpy(DataBase_Path, copy.DataBase_Path);
	strcpy(Default_Program, copy.Default_Program);

	GlPanel=station_num;
	GlPanel_type = T3000;
   GlNetwork = networkaddress;
}

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

	GlPanel=station_num;
	GlNetwork=networkaddress;
	GlPanel_type = T3000;
}

DInt  Panel::connect_panel_proc(DInt t)
{
		 DByte tbl_bank1[MAX_TBL_BANK];
		 Panel_info1 panel_info,p_info1;
		 DByte default_prg1[13];
		 RS232Error result;
		 Time_block time_buf;
		 DUint l1;

		 memcpy( tbl_bank1, tbl_bank, sizeof(tbl_bank1));
		 memcpy( default_prg1, default_prg, sizeof(default_prg));
		 panel_info = panel_info1;
         gAlarm = 0;

			result = t3000Request(COMMAND_50, PANEL_INFO1_COMMAND, (DByte*)&p_info1, 0, station_num, networkaddress);
            alignPanelInfo(1, (DByte *)&p_info1, &panel_info1);
			if( result == RS232_SUCCESS )
			{
				l1 = sizeof(tbl_bank);
				result=t3000Request(COMMAND_50, TABLEPOINTS_COMMAND, (DByte *)tbl_bank, &l1, station_num, networkaddress);
			}
			if( result == RS232_SUCCESS )
			{
					 result = t3000Request(COMMAND_50, DEFAULT_PRG_COMMAND, default_prg, 0, station_num, networkaddress);
			}
			if( result == RS232_SUCCESS )
			{
				Units_element	*punits;
				punits = new Units_element[MAX_UNITS];
				memset(punits,0,sizeof(Units_element)*MAX_UNITS);
				if( t3000Request(UNIT+1, 0, (DByte *)punits, 0, station_num, networkaddress)==SUCCESS)
				{
				 upcustomunit(punits);
				}
				delete punits;
			}
			if( (panel_info1.panel_type == T3000 && panel_info1.version >= 216) ||
					(panel_info1.panel_type == MINI_T3000 && panel_info1.version >= 107) )
				if( result == RS232_SUCCESS )
				{
				 result = t3000Request(COMMAND_50, ALARM_NOTIFY_COMMAND, &gAlarm, 0, station_num, networkaddress);
						 galarm_read_time=timestart+60;
				}
		 if(t)
		 {
		   if( result == RS232_SUCCESS )
		   {
			if( panel_info1.panel_type == T3000 )
			{
             result = t3000Request(COMMAND_50, STATION_LIST_COMMAND, (DByte*)station_list, 0, station_num, networkaddress);
		    }
			else
			{
			 for( int i=0; i<32; i++ )
			 {
				 station_list[i].state = 0;
				 station_list[i].des_length = 0;
			 }
			 station_list[station_num-1].state = 1;
			 station_list[station_num-1].des_length = panel_info1.des_length;
			 memcpy((DByte*)station_list[station_num-1].name, panel_info1.panel_name, NAME_SIZE);
		    }
		   }
		   if( result == RS232_SUCCESS )
		   {
			 result = t3000Request(PASS+1, 0, (DByte *)&::passwords, 0, station_num, networkaddress, 0, 30);
		   }
		   if( result == RS232_SUCCESS )
		   {
				 result = t3000Request(TIME_COMMAND, 0, (DByte *)&time_buf, 0, station_num, networkaddress, 0, TIMEOUT_NETCALL);
		   }
		   if( result == RS232_SUCCESS )
		   {
				adjust_readtime(&time_buf);
				t =(DInt) mktime((struct tm *)&time_buf);
				disable();
				stime((time_t *)&t);
				memcpy(&ora_current, &time_buf, sizeof(Time_block));
				enable();
		   }
		 }
		 if( result!=SUCCESS )
		 {
			memcpy( tbl_bank, tbl_bank1, sizeof(tbl_bank));
			memcpy( default_prg, default_prg1, 13);
			panel_info1 = panel_info;
		 }
		 return result;
}


void adjust_readtime( Time_block *time_buf )  // mini->T3000
{
				Time_block_mini timemini;
				if( Panel_Info1.panel_type == MINI_T3000 )
				{
				 memcpy(&timemini, time_buf, sizeof(Time_block_mini) );
				 time_buf->ti_sec=timemini.ti_sec;         // 0-59
				 time_buf->ti_min=timemini.ti_min;         // 0-59
				 time_buf->ti_hour=timemini.ti_hour;           // 0-23
				 time_buf->dayofmonth=timemini.dayofmonth;   // 1-31
				 time_buf->month=timemini.month;          // 0-11
				 time_buf->year=timemini.year;           // year - 1900
				 time_buf->dayofweek=timemini.dayofweek;        // 0-6 ; 0=sunday
				 time_buf->dayofyear=timemini.dayofyear;    // 0-365 gmtime
				 time_buf->isdst=timemini.isdst;
				}
}

void adjust_writetime(Time_block *time_buf, Time_block_mini *timemini)
{
				if( Panel_Info1.panel_type == MINI_T3000 )
				{
				  timemini->ti_sec=time_buf->ti_sec;         // 0-59
				  timemini->ti_min=time_buf->ti_min;         // 0-59
				  timemini->ti_hour=time_buf->ti_hour;           // 0-23
				  timemini->dayofmonth=time_buf->dayofmonth;   // 1-31
				  timemini->month=time_buf->month;          // 0-11
				  timemini->year=time_buf->year;           // year - 1900
				  timemini->dayofweek=time_buf->dayofweek;        // 0-6 ; 0=sunday
				  timemini->dayofyear=time_buf->dayofyear;    // 0-365 gmtime
				  timemini->isdst=time_buf->isdst;
				}
}

void upcustomunit(Units_element	*punits)
{
 for(DInt i=0;i<MAX_UNITS;i++)
 {
	DInt x,y, z;
	z = custom_digital1 - DIG1;
   if(5 < strlen(punits[i].digital_units_off))
   {
	   x = 5;
   }
   else
   {
   	x =(DInt)  strlen(punits[i].digital_units_off);
   }
	memcpy(dig_range_array[z+i].range_text, punits[i].digital_units_off,x);
	dig_range_array[z+i].range_text[x]='/';
   if( 5<strlen(punits[i].digital_units_on) )
   {
	 y =  5;
   }
   else
	 y =(DInt)  strlen(punits[i].digital_units_on);
	memcpy(&dig_range_array[z+i].range_text[x+1], punits[i].digital_units_on,x);
	dig_range_array[z+i].range_text[x+1+y]=0;
	strcpy(&dig_range_array[z+i].dunits[1][0], punits[i].digital_units_on);
	strcpy(&dig_range_array[z+i].dunits[0][0], punits[i].digital_units_off);
 }
}

void setlocalpanel(void)
{
			local_panel = 1;
			station_num=Station_NUM;
			networkaddress=NetworkAddress;
			panel_info1 = Panel_Info1;
			memcpy( tbl_bank, ptr_panel->table_bank, sizeof(tbl_bank));
			memcpy( default_prg, ptr_panel->Default_Program, 13);
			upcustomunit(ptr_panel->units);
			ptr_panel->GlPanel = station_num;
			ptr_panel->GlNetwork = networkaddress;
}

void disconnect_serial(void)
{
	 setlocalpanel();
	 serial_access = 0;
     desinstallPort(comPort);
	 local_panel   = 1;
}

void reset_serial(void)
{
 serial_access = 0;
 local_panel = 1;
}

/*******************intervaltotext ********************/
DByte *intervaltotext(DByte *textbuf, DLong seconds , DUint minutes , DUint hours, DByte *c)
{
	DByte buf[10], *textbuffer ;
	DByte *separator = c ;
	textbuffer = buf;
	if(*c!='-')
	{
	 hours +=(DInt) (seconds/3600);
	 minutes += (DUint)((seconds%3600)/60);
	 seconds = (DUint)(seconds%3600)%60;
	}
	if( hours < 10 ) {
			strcpy((char*)textbuffer++, "0" ) ;        /* add the leading zero 0#:##:## */
	}
	itoa(hours,(char*)textbuffer,10) ;
	textbuffer += strlen(textbuffer);
	strcpy((char*)textbuffer++, separator ) ;        /* add the ":" separator*/

	if( minutes < 10 ) {
			strcpy((char*)textbuffer++, "0" ) ;        /* add the leading zero ##:0#:## */
	}
	itoa(minutes,(char*)textbuffer,10) ;
	textbuffer += strlen(textbuffer);
	strcpy((char*)textbuffer++,(char*)separator ) ;        /* add the ":" separator*/
	if( seconds < 10 ) {
			strcpy((char*)textbuffer++, "0" ) ;        /* add the leading zero ##:##:0# */
	}
	itoa(seconds,(char*)textbuffer,10)  ;

	if(textbuf) strcpy((char*)textbuf,(char*) buf);
	return(buf) ;

}

/*******************intervaltotext ********************/

char *intervaltotext( DLong seconds , DInt minutes , DInt hours, char *c )
{ /*  char * text;          /* return this string */
	char *separator = c ;
	char itoabuffer[10];       /* temporary sotrage for itoa */
	char textbuffer[10] ;
	textbuffer[0]=0;

	if( hours < 10 ) {
		strcat(textbuffer, "0" ) ;        /* add the leading zero 0#:##:## */
	}
	itoa(hours,itoabuffer,10) ;             /* put panel in ASCII onto buffer */
	strcat(textbuffer, itoabuffer ) ;        /* add the OUT text */
	strcat(textbuffer, separator ) ;        /* add the ":" separator*/
	if( minutes < 10 ) {
			strcat(textbuffer, "0" ) ;        /* add the leading zero ##:0#:## */
	}
	itoa(minutes,itoabuffer,10) ;        /* add the OUT text */
	strcat(textbuffer, itoabuffer ) ;        /* add the OUT text */
	strcat(textbuffer, separator ) ;        /* add the ":" separator*/
	if( seconds < 10 ) {
			strcat(textbuffer, "0" ) ;        /* add the leading zero ##:##:0# */
	}
	itoa(seconds,itoabuffer,10)  ;        /* add the OUT text */
	strcat(textbuffer, itoabuffer ) ;        /* add the OUT text */
	return( &textbuffer[0] ) ;

}
/*******************intervaltotext ********************/



DInt pwait(DWORD timesleep, DUint msg, MSG& myMsg, DInt breakcond)
{
   DWORD            dwStopTime, timeVar;
   /*
	The following loop determines the status of the serial
	interface, which specifies whether the application will act
	as sender or receiver.

	This loop gets the system time and waits for about 4 timer
	clicks. During this time, the application releases the CPU for
	message processing, and for getting important WM_TIMER
	messages for PollRxQueue.
   */
   dwStopTime  = GetTickCount() + timesleep;
   timeVar = GetTickCount() + 800;
   while(GetTickCount() < dwStopTime)
   {
      if(GetTickCount() > timeVar)
      {
        timeVar = GetTickCount() + 800;
        runImage();
      }
	  if(PeekMessage(&myMsg, NULL, NULL, NULL, PM_REMOVE))
	  {
       if( myMsg.message == msg )
         return FALSE;
       if( breakcond )
        if( myMsg.message == WM_KEYDOWN && myMsg.wParam == VK_CANCEL )
	    {
         return TRUE;
	    }
	   TranslateMessage(&myMsg);
	   DispatchMessage (&myMsg);
	  }
   }
   return TRUE;
}

DInt timewait(DWORD timesleep, DUint msg, MSG& myMsg)
{
   DWORD            dwStopTime/*, timeVar*/;
   dwStopTime  = GetTickCount() + timesleep;
   while(GetTickCount() < dwStopTime)
   {
	  if(PeekMessage(&myMsg, NULL, NULL, NULL, PM_REMOVE))
	  {
       if( myMsg.message == msg )
         return FALSE;
	   TranslateMessage(&myMsg);
	   DispatchMessage (&myMsg);
	  }
   }
   return TRUE;
}

/*
DInt pointtotext(DByte *buf,Point_T3000 *point)
{
 DByte x[4];
 DByte num,panel,point_type;
 num=point->number;
 panel=point->panel;
 point_type=point->point_type;
 if (point_type==0)
 {
		buf[0]=0;
		return 1;
 }
 strcpy(buf,itoa(panel+1,x,10));
 if(panel+1<10 || num+1 < 100)
		 strcat(buf,"-");
 strcat(buf,ptr_panel->info[point_type-1].name);
 strcat(buf,itoa(num+1,x,10));
 return 0;
}
*/

/*
DInt pointtotext(DByte *buf,Point_Net *point)
{
 DByte x[4];
 DByte num,panel,point_type;
		 num=point->number;
		 panel=point->panel;
		 point_type=point->point_type;
		 if (point_type==0)
			 {
				buf[0]=0;
				return 1;
			 }
		 strcpy(buf,itoa(panel+1,x,10));
		 if(panel+1<10 || num+1 < 100)
			 strcat(buf,"-");
		 strcat(buf,ptr_panel->info[point_type-1].name);
		 strcat(buf,itoa(num+1,x,10));
 return 0;
}
*/

/*
DByte *rtrim(DByte *text)
{
	DInt n,i;
	n=strlen(text);
	for (i=n-1;i>=0;i--)
			if (text[i]!=' ')
					break;
	text[i+1]='\0';
	return text;
}
*/
/*
DByte *islabel(DByte *token, DByte *num_point, DByte *var_type, DByte *point_type, DByte *num_panel)
{
 DByte *p;
 DInt i,j;

 if(!local_panel)
 {
  p=0;
  if(Des)
  {
		p=Des->islabel(token,num_point,var_type,point_type,num_panel);
  }
  if( p )
	  return(p);
 }

 for(i=T3000_OUT;i<=AY;i++)
		for(j=0;j<ptr_panel->table_bank[i];j++)
				 {
					p = 0;
					if(i!=CON)
					{
					switch (i) {
						case T3000_OUT:
									p = ptr_panel->outputs[j].label;
									break;
						case T3000_IN:
									p = ptr_panel->inputs[j].label;
									break;
						case VAR:
									p = ptr_panel->vars[j].label;
									break;
						case CON:
//									p = ptr_panel->controllers[j].label;
									break;
						case WR:
									p = ptr_panel->weekly_routines[j].label;
									break;
						case AR:
									p = ptr_panel->annual_routines[j].label;
									break;
						case PRG:
									p = ptr_panel->programs[j].label;
									break;
						case GRP:
									p = ptr_panel->control_groups[j].label;
									break;
						case AMON:
									p = ptr_panel->analog_mon[j].label;
									break;
						case AY:
									p = ptr_panel->arrays[j].label;
									break;
						default:
									break;
						}
					 if(p)
					 if(!strcmp(rtrim(p),rtrim(token)))  // de verificat duplicate
						{
						 *var_type = LABEL_VAR;
						 itoa(Station_NUM,token,10);
						 strcat(token,"-");
						 strcat(token,ptr_panel->info[i].name);
						 itoa(j+1,&token[strlen(token)],10);
						 *num_point = j+1;
						 *point_type = i;
//						 *num_panel = ptr_panel->GlPanel;
						 *num_panel = Station_NUM;
						 return p;
						}
					}
				}
 p=0;
 if(local_panel)
 {
  if(Des)
  {
		p=Des->islabel(token,num_point,var_type,point_type,num_panel);
  }
 }
 return(p);
}
*/


/*
DByte *ispoint(DByte *token, DByte *num_point, DByte *var_type, DByte *point_type, DByte *num_panel, DInt *num_net, DInt network, DByte panel)
{
DInt i,j,k,l;
DByte pc[11],*p,*q,buf[21];
    		 *num_net = network;
			 if(strlen(token)==0) return 0;
			 if ((q=strchr((char *)token,'_'))!=NULL)
			 {
						memcpy(pc,token,min( (int)(q-token),10));
						pc[min((int)(q-token),10)]=0;
						q++;
						*num_net = atoi(pc);
						token = q;
			 }
			 if ((q=strchr((char *)token,'-'))!=NULL)
			 {
						memcpy(pc,token,min( (int)(q-token),10));
						pc[min((int)(q-token),10)]=0;
						q++;
						*num_panel = atoi(pc);
						if ( *num_panel>32 || *num_panel==0 )
										 q=token;
			 }
			 else
			 {
					for(l=0;l<3 && token[l]!=0;l++)
						if (token[l]<='0' || token[l]>'9')
								 break;
					q=token+l;
					memcpy(pc,token,l);
					pc[l]=0;
					*num_panel = atoi(pc);
					if (*num_panel==0)
					  *num_panel = panel;
					if ( *num_panel>32 && strlen(pc) )
										 return(islabel(token,num_point,var_type,point_type,num_panel));
			 }
			 if ((p=strpbrk((char *)q, "123456789"))!=NULL)
			 {
				memcpy(pc,q,p-q);
				pc[p-q]=0;
				for(k=T3000_OUT;k<=AY;k++)
				  if(k!=DMON)
					 if (!strcmp(pc,ptr_panel->info[k].name))
						 break;
				if (k<=AY)
					{
					if (p==NULL) {
												memcpy(pmes,"error line : ",13);
												pmes += 13;
												itoa(lline,pmes,10);
												pmes += strlen(pmes);
												*pmes++ = 0x0d;
//												fprintf(pmes,"error line %d\n",line);
												error=TRUE;return 0;
												}
					else  if ((strlen(p)==1) && (*p=='0')) {
																									memcpy(pmes,"error line : ",13);
																									pmes += 13;
																									itoa(lline,pmes,10);
																									pmes += strlen(pmes);
																									*pmes++ = 0x0d;
//																									fprintf(pmes,"error line %d\n",line);
																									error=TRUE;return 0;
																									}
								else {
											for(l=0;l<strlen(p);l++)
												 if (p[l]<'0' || p[l]>'9')
														 break;
											if (l<strlen(p))
												 return(islabel(token,num_point,var_type,point_type,num_panel));
											else
													 {
//														itoa(panel,buf,10);
														itoa(*num_panel,buf,10);
														*num_point=atoi(p);
														*point_type = k;
														if(*num_panel<10 || *num_point<100)
															strcat(buf,"-");
														strcat(buf,pc);
														strcat(buf,p);
														strcpy(token,buf);
														if((p=look_label(*num_panel,k,*num_point)) != NULL)
															 {
																*var_type = LABEL_VAR;
																return p;
															 }
														else
															 {
																*var_type = POINT_VAR;
																return token;
															 }
													 }
										 }
					}
			 }
			 return(islabel(token,num_point,var_type,point_type,num_panel));
}
*/

DInt getdirectories(DByte *ndir, DByte (*directories)[13], DInt nmax, DInt local)
{
 DUint done;
 DInt i=0;
 struct ffblk block;
 if(local)
 {
	done=findfirst("*.*",&block,FA_DIREC);
	while(!done && i<nmax)
	{
	 if (block.ff_attrib == 0x10)
		if ( (strlen(block.ff_name)==1 && block.ff_name[0]!='.') || (strlen(block.ff_name)>1	) )
		 {
			strcpy((char *)directories,block.ff_name);
			directories++;
			i++;
		 }
	 done=(DUint)findnext(&block);
	}
 }
 else
 {
	done = 65;
	if(t3000Request(WRITE_COMMAND_50, SETCURRENTPATH_COMMAND, ndir, &done, station_num, networkaddress)!=SUCCESS) return 0;
//	GWindow *D;
//	DisplayMessage(19, 8, 59, 12, ctrlbreak_text," Reading directories...   ",&D);
	if(t3000Request(READ_COMMAND_50, READDIRECTORIES_COMMAND, (DByte *)directories, 0, station_num, networkaddress, NETCALL_SIGN)!=SUCCESS)
	{
//	 DeleteMessage(D);
	 return 0;
	}
//	DeleteMessage(D);
//	Delay(100);
	memcpy(&i, directories, 2);
	if(i) memmove(directories, ((DByte *)directories)+2, i*13);
 }
 return i;
}

DInt getfiles(DByte *term, DByte (*files)[13], DInt nmax, DInt local)
{
 DUint done;
 DInt i=0;
 struct ffblk block;
 if(local)
 {
	done=findfirst((char*)term,&block,FA_ARCH);
	while(!done && i < nmax)
	{
	 strcpy((char *)files,block.ff_name);
	 files++;
	 i++;
	 done=(DUint)findnext(&block);
	}
 }
 else
 {
	done = 13;
	if(t3000Request(WRITE_COMMAND_50, SETFILETYPE_COMMAND, term, &done, station_num, networkaddress)!=SUCCESS) return 0;
//	GWindow *D;
//	DisplayMessage(19, 8, 59, 12, ctrlbreak_text," Reading directories...   ",&D);
	if(t3000Request(READ_COMMAND_50, READFILES_COMMAND, (DByte *)files, 0, station_num, networkaddress, NETCALL_SIGN)!=SUCCESS)
	{
//	 DeleteMessage(D);
	 return 0;
	}
//	DeleteMessage(D);
//	Delay(100);
	memcpy(&i, files, 2);
	if(i) memmove(files, ((DByte *)files)+2, i*13);
 }
 return i;
}

/*
DUint GetFile(char *title, char *defaultname, char *term, char *ptrpath, int type)
{
 char ocwd[65];
 char tmp[65];
 char drive[3];
 char dir[65];
 char nname[13];
 char ext[4];
// char fullname[75];

 int disk;
//  int done,i=0,colour;
 long l;
 int done,i=0,colour;
 int last_cur,firstrun,secondrun;
 int last_m=0;
 int upfiles=0, updir=0;
 struct ffblk block;

 struct nume{
				char  names[13];
			};
 struct nume *tab;


 char *file;
 char odir[65];
 int col;

 int i_files;
 int i_directories;
 int i_drives;
 char (*files)[13];
 char directories[50][13];

 ocwd[0]=0;
 i_files=0;
 i_directories=0;
 i_drives=0;

 if(t3000request(COMMAND_50, GETCURRENTPATH_COMMAND, ocwd, 0, station_num, networkaddress)!=SUCCESS) return 0;

 files = new char [500][13];
 if(ocwd[strlen(ocwd)-1] != '\\')
		strcat(ocwd,"\\");

 fnsplit(ocwd,drive,dir,name,ext);


 strcpy(dir,ocwd);
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

 memset(directories, 0, sizeof(directories));
 memset(files, 0, sizeof(files));
 nrfiles=getfiles(term, files, 500, local);
 nrdirectories=getdirectories(dir, directories, 50,local);
 if(!nrdirectories)
 {
  delete files;
  return 0;
 }

 if(type)
 {
  TSaveDialog95Form *SaveDialog95Form;
  SaveDialog95Form = new TSaveDialog95Form(Application);



//Adds a root node
	TreeView1->Items->Add(TreeView1->Selected, "RootTreeNode1");

	//Sets MyTreeNode to first node in
	//tree view and adds a child node to it
	TTreeNode *MyTreeNode1 = TreeView1->Items->Item[0];
	TreeView1->Items->AddChild(MyTreeNode1,"ChildNode1");

	//Adds a root node
	TreeView1->Items->Add(TreeView1->Selected, "RootTreeNode2");

	//Sets MyTreeNode to third node in
	//tree view and adds a child node to it
	TTreeNode *MyTreeNode2 = TreeView1->Items->Item[2];

TreeView1->Items->AddChild(MyTreeNode2,"ChildNode2");

	//Sets MyTreeNode to fourth node in
	//tree view and adds a child node to it
	MyTreeNode2=TreeView1->Items->Item[3];
	TreeView1->Items->AddChild(MyTreeNode2,"ChildNode2a");

	//Sets MyTreeNode to fifth node in
	//tree view and adds a child node to it
	MyTreeNode2 = TreeView1->Items->Item[4];
	TreeView1->Items->Add(MyTreeNode2,"ChildNode2b");

	//add another root node
	TreeView1->Items->Add(TreeView1->Selected, "RootTreeNode3");






  Root = DirTree->Items->AddChild(NULL, &directories[0][0]);
  DirTree->Selected = Root;
  for(i=1;i<=5 && i<=nrdirectories;i++)
  {
   SaveDialog95Form->DirTree->Items->Add(SaveDialog95Form->DirTree->Selected,&directories[i_directories+i-1][0]);
  }

  for(i=1;i<=9 && i<=nrfiles;i++)
  {
   SaveDialog95Form->ListFile->Items->Add(SaveDialog95Form->DirTree,&directories[i_directories+i-1][0]);
		D->GDDef_object(pfiles,3,i+3,12,Lightgra,Black);
//		D->GDDef_object(NULL,3,i+3,12,Lightgra,Black,&files[i_files][0],Blu,White);
		D->GDPuts(i+3,3,&files[i_files+i-1][0],12,Lightgra,Black);
  }


 }




 // ********  the old code  ********
// GWDialog *D=new GWDialog(hx,hy,hx+55,hy+16,NO_STACK,0);
// ********************************

// ***********************************
// ********      mao hui      ********
// ********  1997.8.4 NO.025  ********
// ********       begin       ********
// ***********************************
 if (!strcmp(title,"Load panel "))
 {
	D=new GWDialog(100,"need help",hx,hy,hx+55,hy+16,NO_STACK,0);
 }
 else
 {
	if (!strcmp(title,"Save panel "))
	{
		D=new GWDialog(101,"need help",hx,hy,hx+55,hy+16,NO_STACK,0);
	}
	else
	{
		if (!strcmp(title,"Save file"))
		{
			D=new GWDialog(21,"need_help",hx,hy,hx+55,hy+16,NO_STACK,0);
		}
		else
		{
			if (!strcmp(title,"Load file"))
			{
				D=new GWDialog(21,"need_help",hx,hy,hx+55,hy+16,NO_STACK,0);
			}
			else
			{
				D=new GWDialog(hx,hy,hx+55,hy+16,NO_STACK,0);
			}
		}
	}
 }

}
*/
