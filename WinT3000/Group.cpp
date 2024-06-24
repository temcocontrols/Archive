//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Group.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgauges"
#pragma link "cspin"
#pragma link "perfgrap"
#pragma resource "*.dfm"

#include <conio.h>
#include "aio.h"
#include "program.h"
#include "monitor.h"
#include "object.h"

TGroupForm *GroupForm;

extern DInt networkaddress;
extern Panel *ptr_panel;
extern DInt local_panel;
extern DInt  station_num;
extern char point_cod[MAX_TBL_BANK][5];
extern char gdescriptor[22];
extern char glabel[10];
extern char gpointname[14];
extern char *blank;
extern dig_range_form dig_range_array[];
extern an_range_form  in_range_array[];
extern an_range_form  out_range_array[];
extern an_range_form  var_range_array[];

extern char *ltrim(char *text);
extern char *rtrim(char *text);
extern DInt get_point_info(Point_info *point_info, char **des, char **label, char **ontext, char **offtext, char pri, DInt network);
extern char * intervaltotext( DLong seconds , DInt minutes , DInt hours, char *c );
extern char * intervaltotext(char *textbuf, DLong seconds , DUint minutes , DUint hours, char *c=":");

extern void longtoa(DLong f, DInt length, DInt ndec, char *buf);


enum  {  DISPLAY_LABEL = 0, DISPLAY_DES, DISPLAY_VALUE,  DISPLAY_ICON,
		 DISPLAY_ICON_LABEL, DISPLAY_ICON_DES, DISPLAY_ICON_VALUE };

char far Icon_name_table[MAX_ICON_NAME_TABLE][14];
char (*icon_name_table)[14];
Str_grp_element	*group_element;
Str_grp_element	*group_element_temp;
Str_grp_work_data *grp_workdata;
char error_communication;
extern DInt refresh_time;
DInt index_cur=0;
DInt index_max_group_element=0;
DInt index_max=0;
DInt xcur_grp,ycur_grp;
DInt exit_time_display;
extern icon *points_icon;

//---------------------------------------------------------------------------
__fastcall TGroupForm::TGroupForm(TComponent* Owner, Control_group_point& ptrgrp, DInt currentpoint, DByte ppanel)
    : TForm(Owner)
{
  Top = 0;
  Left = 0;
  Height  = 526;
  Width   = 648;
  ptrcgrp = ptrgrp;
  current_point = currentpoint;
  panel = ppanel;
  picture = AnsiString(ltrim(rtrim(ptrgrp.picture_file)))+".bmp";
}
//---------------------------------------------------------------------------
void __fastcall TGroupForm::FormPaint(TObject *Sender)
{
  Point_Net point;
  Graphics::TBitmap* bitmap1 = new Graphics::TBitmap();
  bitmap1->LoadFromFile(picture);
  Canvas->Draw(0,0, bitmap1);
//  Canvas->TextOut(20, 50, "Set Point 16 C ");
  fnc_group(&ptrcgrp, current_point, 0, station_num, networkaddress, 0);  //, &point);
}

//---------------------------------------------------------------------------

DInt TGroupForm::print_des(Str_grp_element *group_element, Str_grp_work_data *grp_workdata, DByte panel, DInt network, char len)
{
					 char buf1[11],buf[41],*des=NULL,*label=NULL,*ontext=NULL,*offtext=NULL;
					 DByte var_type,point_type;
					 DInt num_net,color,num_point,num_panel;
                     DInt mode_text=0;
					 Point_Net *p;
					 Point_info point_info;
					 point_info = group_element->point_info;
					 if(!refresh_time && (point_info.point.point_type-1 == GRP ))
							 return 0;

					 if(group_element->graphic_x_coordinate+group_element->graphic_y_coordinate+
							 group_element->text_x_coordinate+group_element->text_y_coordinate==0) return 0;
					 memset(buf,0,41);
//					 if(mode_text)
//							buf[0]=' ';
					 if (point_info.description_label==DISPLAY_ICON_VALUE +1)
					 {
						 strcat(buf,point_cod[point_info.point.point_type-1]);
						 strcat(buf,"S");
					 }
					 else
					 {
						 if(group_element->point_absent)
							point_info.description_label=DISPLAY_LABEL;
//						 if(local_request(panel,network))
						 {
							 if(!group_element->point_absent)
								 get_point_info(&group_element->point_info, &des, &label,&ontext,&offtext,0,network);
							 point_info = group_element->point_info;
						 }
/*
						 else
						 {
							if( !point_info.digital_analog )
							{
							 if(!group_element->point_absent)
							 {
							  num_point=dig_range_array[point_info.units].value % 2;
							  ontext= &dig_range_array[point_info.units].dunits[!num_point][0];
							  offtext= &(dig_range_array[point_info.units].dunits[num_point][0]);
							 }
							}
							p = &point_info.point;
							pointtotext(buf1,p);
							label=ispoint(rtrim(buf1),&num_point,&var_type, &point_type, &num_panel, &num_net, network, panel);
							if(Des)
							{
								if(point_type!=CON && point_type!=TBL)
									des = Des->look_descriptor(num_panel,point_type,num_point);
								if(!des) des = label;
							}
						 }
*/

//						 if(!mode_text)
//							 moveto(group_element->graphic_x_coordinate,group_element->graphic_y_coordinate);

						 if(!group_element->display_point_name)
						 {
						  if (point_info.description_label==DISPLAY_DES || point_info.description_label==DISPLAY_ICON_DES)
						  {
							 if(!des) des = label;
							 {
								if(des)
								 strcat(buf,des);
							 }
							}
							else
							 if (point_info.description_label==DISPLAY_LABEL || point_info.description_label==DISPLAY_ICON_LABEL)
							 {
								if(label)
								 strcat(buf,label);
							 }

						  if(point_info.description_label!=DISPLAY_VALUE && point_info.description_label!=DISPLAY_ICON_VALUE	&& !strlen(rtrim(buf)))
							{
/*
							 if(mode_text)                                                                               
							 {
								buf[0]=' '; buf[1]=0;
							 }
*/
							 strcat(buf, gpointname);
						  }
						}
/*
						if(mode_text && buf[1])
							 lalign(buf,22);
*/
						if(!mode_text && buf[0])
							 rtrim(buf);

						color = group_element->normal_color;

						if(!group_element->point_absent)
						{
						 if(point_info.point.point_type-1 != GRP &&
								point_info.point.point_type-1 != PRG &&
								point_info.point.point_type-1 != AMON &&
								point_info.point.point_type-1 != AR &&
								point_info.point.point_type-1 != WR )
						 {
								if(!mode_text && buf[0])
									strcat(buf," ");

								if(point_info.digital_analog)
								{
										if(point_info.units == time_unit)
										{
											intervaltotext( &buf[strlen(buf)], (DLong)((DLong)point_info.point_value/(DLong)1000L) , (DUint)0 , (DUint)0);
										}
										else
										{
											longtoa(point_info.point_value, 7, 2, &buf[strlen(buf)]);
											ltrim(&buf[strlen(buf)-6]);
											strcat(buf,blank);
											var_type = 0;
											if( point_info.point.point_type-1==T3000_IN )
											{
												var_type = 1;
												if( point_info.units >=0 && point_info.units <=24 )
													strcat(buf,in_range_array[point_info.units].aunits);
											}
											else
											 if( point_info.point.point_type-1==T3000_OUT )
											 {
												var_type = 1;
												if( point_info.units >=0 && point_info.units <=6 )
													strcat(buf,out_range_array[point_info.units].aunits);
											 }
											 else
											  if( point_info.point.point_type-1==VAR || point_info.point.point_type-1==CON )
											  {
												 var_type = 1;
												 if( point_info.units >=0 && point_info.units <=42 )
													 strcat(buf,var_range_array[point_info.units].aunits);
												}
											if(var_type)
											{
											  if(point_info.point_value>(DLong)(group_element->high_limit*1000l) ) color = group_element->on_high_color;
											  if(point_info.point_value<(DLong)(group_element->low_limit*1000l) ) color = group_element->off_low_color;
											}
										}
								}
								else
								{
										if(point_info.point_value)
										{
											 color = group_element->on_high_color;
											 if(ontext)
												strcat(buf,ontext);
										}
										else
										{
											 color = group_element->off_low_color;
											 if(offtext)
												strcat(buf,offtext);
										}
								}
						 }
						 else
						 {
								 if( point_info.point.point_type-1!=GRP )
								 {
										if(!mode_text && buf[0])
										 strcat(buf,blank);
										if(point_info.description_label < DISPLAY_ICON || point_info.description_label == DISPLAY_ICON_VALUE )
										{
										 if(point_info.point_value)
										 {
											 if(ontext)
												strcat(buf,ontext);
										 }
										 else
										 {
											 if(offtext)
												strcat(buf,offtext);
										 }
                    }
								 }
						 }
            }
						if(!mode_text)
							rtrim(buf);
						strcat(buf,blank);
						if(!group_element->point_absent)
						{
							if (point_info.auto_manual!=AUTO)
										strcpy(&buf[strlen(buf)],"M");
						}
						else
								strcpy(&buf[strlen(buf)],"Absent");

					 }
					 var_type = strlen(buf);
					 num_point = 0;
					 if(!len)
					 {
//						hide_pointer();
/*
						if(mode_text)
						{
						 lalign(buf, 40);

						 mxyputs(group_element->text_x_coordinate,group_element->text_y_coordinate,buf,-1,color);
						}
						else
*/
						{
						 if(grp_workdata->ptr_save_bkgnd)
						 {
/*
								mputimage(group_element->graphic_x_coordinate,group_element->graphic_y_coordinate,grp_workdata->ptr_save_bkgnd, COPY_PUT);
								mfarfree(grp_workdata->ptr_save_bkgnd);
								grp_workdata->ptr_save_bkgnd = NULL;
*/
						 }
						 if(refresh_time && point_info.description_label >= DISPLAY_ICON)
						 {
//							if( !grp_workdata->icon )
							{
							 if( group_element->default_icon )
							 {
								 if(!points_icon[group_element->point_info.point.point_type-1].status)
								 {
                                     Graphics::TBitmap* pBitmap = new Graphics::TBitmap();
                                     AnsiString s = AnsiString( (char *)points_icon[group_element->point_info.point.point_type-1].name);
                                     pBitmap->LoadFromFile( s );
//                                     if((group_element->point_info.point.point_type-1)!=WR)
//                                         pBitmap->Transparent = true;
                                     Canvas->Draw(group_element->xicon,group_element->yicon, pBitmap);
                                     grp_workdata->icon.width = pBitmap->Width;
                                     grp_workdata->icon.height = pBitmap->Height;
                                     strcpy(grp_workdata->icon.name,s.c_str());
//									 set_semaphore_dos();
//									 grp_workdata->icon = new Icon((icon *)&points_icon[group_element->point_info.point.point_type-1],group_element->bkgnd_icon);
//									 clear_semaphore_dos();
								 }
//								 else num_point = 1;
							 }
							 else
							 {
                                     Graphics::TBitmap* pBitmap = new Graphics::TBitmap();
                                     AnsiString s = AnsiString( (char *)icon_name_table[group_element->icon_name_index]);
                                     AnsiString s1;
                                     DInt i=s.LastDelimiter("." );
                                     if(i)
                                         s1 = s.SubString(0, i-1)+".bmp";
                                     else
                                         s1 = s+".bmp";
                                     pBitmap->LoadFromFile( s1 );
                                     pBitmap->Transparent = true;
                                     Canvas->Draw(group_element->xicon,group_element->yicon, pBitmap);
                                     grp_workdata->icon.width = pBitmap->Width;
                                     grp_workdata->icon.height = pBitmap->Height;
                                     strcpy(grp_workdata->icon.name,s1.c_str());
/*
								set_semaphore_dos();
								icon *ic=new icon(icon_name_table[group_element->icon_name_index]);
								clear_semaphore_dos();
								if(!ic->status)
								{
								 set_semaphore_dos();
//								 group_element[index_cur].icon = new Icon((icon *)ic, group_element[index_cur].bkgnd_icon);
								 grp_workdata->icon = new Icon((icon *)ic, group_element->bkgnd_icon);
								 clear_semaphore_dos();
								}
								else
								{
								 set_semaphore_dos();
								 delete ic;
								 clear_semaphore_dos();
								 num_point = 1;
								}
*/
							 }
							}
							if(!num_point)
							{
//							 grp_workdata->icon->draw_icon(group_element->xicon, group_element->yicon, copy_icon, grp_current_pal);
							 int x1=0,x2=0,y1=0,y2=0;
//							 x1 = group_element->xicon; x2 = x1 + grp_workdata->icon->ptr_icon->width;
//							 y1 = group_element->yicon; y2 = y1 + grp_workdata->icon->ptr_icon->height;
							 switch(group_element->text_place){
								case 0: y1 = y2 + 4;
													 x1 = (x1+x2)/2 - ((strlen(buf)-1)/2)*8;
													 break;
								case 1: y1 -= 12;
													 x1 = (x1+x2)/2 - ((strlen(buf)-1)/2)*8;
													 break;
								case 2: y1 = y2 - 12;
													 x1 = x1 - (strlen(buf)+1)*8;
													 break;
								case 3: y1 = y2 - 12;
														x1 = x2 + 8;
													 break;
							 }
							 group_element->graphic_x_coordinate = x1;
							 group_element->graphic_y_coordinate = y1;
							 if( x1 < 0 ) group_element->graphic_x_coordinate = 0;
							}
							else
							{
							 group_element->icon_present = 0;
							 group_element->text_present = 1;
							 if(point_info.description_label == DISPLAY_ICON )
							 {
								 group_element->point_info.description_label = DISPLAY_LABEL;
							 }
							 if(point_info.description_label > DISPLAY_ICON )
							 {
								 group_element->point_info.description_label -= DISPLAY_ICON_LABEL;
							 }
							 point_info.description_label = group_element->point_info.description_label;
							}
						 }
						 if(point_info.description_label != DISPLAY_ICON)
						 {
/*
							grp_workdata->nchar=strlen(buf);
							unsigned long big;
							big = mimagesize(group_element->graphic_x_coordinate,group_element->graphic_y_coordinate,min(group_element->graphic_x_coordinate+grp_workdata->nchar*lat_char_n,maxx),group_element->graphic_y_coordinate+lung_char_n);
							mfarmalloc((char far **)&grp_workdata->ptr_save_bkgnd, (unsigned long)big);
							set_semaphore(&screen);
							getimage(group_element->graphic_x_coordinate,group_element->graphic_y_coordinate,min(group_element->graphic_x_coordinate+grp_workdata->nchar*lat_char_n, maxx),group_element->graphic_y_coordinate+lung_char_n,(void *)grp_workdata->ptr_save_bkgnd);
							settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
							if(color == fond_color)
								setcolor(Red);
							else
								setcolor(color);
							outtextxy(group_element->graphic_x_coordinate,group_element->graphic_y_coordinate,buf);
							settextstyle(SMALL_FONT,0,5);
							clear_semaphore(&screen);
*/
//                            Canvas->MoveTo(group_element->graphic_x_coordinate,group_element->graphic_y_coordinate);
                            Canvas->TextOut(group_element->graphic_x_coordinate,group_element->graphic_y_coordinate,buf);

//                            cprintf("%s",buf);
						 }
						 else
						 {
							var_type = 0;
						 }
						}
//						display_pointer();
					 }
					 else
						 if(point_info.description_label == DISPLAY_ICON)
							var_type = 0;

					 return var_type;
}

//---------------------------------------------------------------------------
DInt TGroupForm::fnc_group(Control_group_point *block, DByte current_point, DInt call_type, DByte panel, DInt network, DInt display)  //, Point_Net *retpoint)
{
 Str_grp_element        *pgrp;
// Str_grp_work_data      *pworkdata;
 Control_group_elements *cgrp;
// DInt nrow = 24, ncol = 2;
// DInt curline=2,curcol=1,menu=0;
 char *des,*label;
// DInt bkgnd=FONDCOLOR, frgnd=White;
// DInt bkgnd, frgnd;
 DInt index_obj = MAX_ELEM;
 DInt local_panel_old;
 DInt x,y,k;
 DInt r=0;
 DUint i;
 char bitmap[100];
// Point_Net openscreen;
 DInt rtime;
 index_cur=0;
 error_communication=0;
// DUlong lastreadopenscreen;
// lastreadopenscreen	= timestart+timesec1970;

// on_screen=0;
// picture_write=1;
 group_element_temp = NULL;
// openscreen=current_opened_grp;
// localopenscreen.setzero();

/*
 if(!mode_text && call_type!=INDIVIDUAL && call_type!=1 && dontch==0)
 {
	 pgrid->RestoreArea();
 }
*/
     group_element = new Str_grp_element[MAX_ELEM];
     group_element_temp = new Str_grp_element[MAX_ELEM];
     grp_workdata = new Str_grp_work_data[MAX_ELEM];
	 memset(group_element, 0, MAX_ELEM*sizeof(Str_grp_element));
	 memset(group_element_temp, 0, MAX_ELEM*sizeof(Str_grp_element));
	 memset(grp_workdata, 0, MAX_ELEM*sizeof(Str_grp_work_data));


//	 if(local_request(panel,network))
		icon_name_table = Icon_name_table;
/*
	 else
	 {
		set_semaphore_dos();
		icon_name_table = (char (*)[14])new char [MAX_ICON_NAME_TABLE*14];
	    clear_semaphore_dos();
		memset(icon_name_table,'\0',sizeof(Icon_name_table));
	 }
*/
//	 exit_time_display=0;
//	 if(!local_request(panel,network))
//		if( net_call(COMMAND_50, ICON_NAME_TABLE_COMMAND, (char*)icon_name_table, 0, panel, network,NETCALL_RETRY) != RS232_SUCCESS )
//		r=1;

//	 if(!r)
// call the  r=graphic_mode(block,current_point,call_type,panel,network,display,retpoint);

//int Control_group_block::graphic_mode(void)
//int graphic_mode(Control_group_point *block, int current_point, int call_type, byte panel, int network, int display, Point_Net *retpoint)
{
// struct palettetype Ppal;
// MouseStatus stat;
// Str_grp_work_data *pworkdata;
 DUint i;
 DInt local_panel_old;
 DInt curline=2,curcol=1,menu=0;
 DInt x,y;
 char *des,*label;
 char filename[65];
// rgb pal[16],pal1[16];
// rgb pal1[16];
 Point_Net openscreen;
 DUlong lastreadopenscreen;
 DInt rtime;
// lastreadopenscreen	= timestart+timesec1970;
// openscreen=current_opened_grp;
// localopenscreen.setzero();

// grp_current_pal=(rgb *)pal1;
// BOXCOLOR=Cyan;
// DInt bkgnd=FONDCOLOR, frgnd=White;
 DInt bkgnd, frgnd;
 DInt index_obj = MAX_ELEM;
 index_max_group_element=0;
 index_cur=0;
// hot_box hotgroup={3,1,96,15};
// hot_box hotload={112,1,145,15};
// hot_box hotsend={161,1,195,15};
// hot_box hotedit={211,1,245,15};
// hot_box hotexit={260,1,292,15};

	 filename[0] = 0;
//	 if( ptpnetwork(network) )
//	  strcpy(filename, remote_path);
//	 else
//		strcpy(filename, ptr_panel->Graphics_Path);

//	 if(strlen(filename)) strcat(filename, "\\");
//	 strcat(filename,block->picture_file);
//	 strcat(rtrim(filename),".tif");

// hide_pointer();

//
// int old_mode;
// if(dontch==-1)
// {
//	 old_mode=MODE_TEXT;
//	 dontch = 0;
// }
// else
//	if(dontch)
//		old_mode=MODE_TEXT;
//	else
//		old_mode=MODE_GRAPHIC;
//
/*
 if(mode_text )
 {
	mode_text = MODE_GRAPHIC;
	hide_pointer();
	restorechar();
	setgraphmode(getgraphmode());
	Green=GREEN;                 //2
	White=WHITE; 						 //5
	Darkgray=DARKGRAY;                 //6
	Brown=BROWN;           //8
	Lightblue=LIGHTBLUE;         //9
	Magenta=MAGENTA;                 //15
//				 setpalette(BROWN,BROWN);
//				 setpalette(GREEN,GREEN);
//				 setpalette(MAGENTA,MAGENTA);
//				 setpalette(DARKGRAY,DARKGRAY);
//				 setpalette(WHITE,WHITE);
//				 setpalette(LIGHTBLUE,LIGHTBLUE);

// setpalette(Green,88);
// setpalette(White,255);
// setpalette(Magenta,99);
// setpalette(Brown,8);
asm {
	 mov ax,21h
	 int 33h
	 }
 display_pointer();
// mouse_installed();
 }
*/
 DInt esc = 0;
 char cont;
// GWindow *D=NULL;
// msettextstyle(DEFAULT_FONT, 0, 1);
// modify_grp_new=0;
//while(!esc)
{
// hide_pointer();
// setbkcolor(Black);
// fond_color=Black;
// cleardevice();
 DInt nclrs;
// struct palettetype ppal;
// getpalette(&ppal);
// get16( pal, 0 );
/*
 if(strlen(rtrim(block->picture_file)))
 {
	struct ffblk block;
	set_semaphore_dos();
	int_disk=1;
	int done=findfirst(filename, &block,FA_ARCH);
	int_disk=0;
	clear_semaphore_dos();
	if(!done)
		tif(filename,0,0,maxx,maxy);
	else
	{
		if(!test_tiff)
		{
			test_tiff = 1;
			DisplayMessage(190, 170, 490, 230, NULL, " Picture file not found! ",NULL,Black,NULL,1300);
//			DisplayError(190, 170, 490, 230, "     Picture file not found! ", Lightblue, White, Blue);
		}
		grp_current_pal=init_pal;
	}
 }
 else
	grp_current_pal=init_pal;
	BOXCOLOR=closest_rgb( init_pal[BOXCOLOR], grp_current_pal, 16);
*/
 /*
	draw_icon(100, 100, &points_icon[AMON], pal1, 1);
	for(int i=0; i< 40; i++)
	{
	 Delay(500);
	 draw_icon(100+i*2, 100+i*2, &points_icon[AMON], pal1, 1);
	}
	if( points_icon[AMON].image_under )
	{
	 delete points_icon[AMON].image_under ;
	 points_icon[AMON].image_under = NULL;
	}
*/
//	setallpalette(&ppal);
//	set16( pal, 0 );
// setcolor(Red);
// outtextxy(200, 4, block->description);
// outtextxy(370, 4, block->label);

/*
 union key key;
 GMenuBar *grp_menu;
 if(!display)
 {
	grp_menu = new GMenuBar(0,0,maxx,19,Lightgray,Black,White,3,BARMENU,NULL);
 }
 key.ch[0]='\t';
 screen_access = check_access(GRP, current_point);
 display_pointer();
*/
 Str_grp_element *pgrp;
 Control_group_elements *cgrp;
// if((local_panel && call_type!=INDIVIDUAL) || (call_type==INDIVIDUAL && panel==Station_NUM ))
// if((local_panel &&  panel==Station_NUM) || (!local_panel &&  panel==Station_NUM && panel!=station_num))
// if( local_request(panel,network) )
 {
	cgrp = &ptr_panel->control_group_elements[current_point];

	pgrp = group_element;
	for(i=0; i<cgrp->nr_elements;i++, pgrp++)
	 {
		memcpy(pgrp,cgrp->ptrgrp+i,sizeof(Str_grp_element));
	 }
	index_max=index_max_group_element=cgrp->nr_elements;
 }
/*
 else
 {
	 if( DIAL_updategrp==1)
			 resume(DIAL);
	 while(DIAL_updategrp==1);
	 DisplayMessage(160, 150, 500, 210, ctrlbreak_text,"   Reading data ... ",&D);
	 Delay(100);
		i=0;
	 NotResponse1=0;
	 int s=net_call(19, current_point, (char *)group_element, &i, panel, network, NETCALL_SIGN);
	 NotResponse1=1;
	 if(D){ DeleteMessage(D);  D=NULL;}
	 if(s==SUCCESS)
	 {
		index_max=i/sizeof(Str_grp_element);
		index_max_group_element=index_max;
	 }
	 else
	 {
		key.ch[0]=27;
		error_communication=1;
	 }
	}
*/
 if (ycur_grp < 20 ) ycur_grp = 23;
// status_mouse = SHOWMOUSE;

// move_mouse(xcur_grp,ycur_grp);

// msettextstyle(SMALL_FONT, 0, 5);

 DInt page = -1;
 DInt first=1, last_mouse=0;
// refresh_time = refresh_time_init;
 local_panel_old = local_panel;
// setscreenstate(current_opened_grp);
// while (1)
 {
//	if (bioskey_new(1) || first)
	{
/*
	 if(!first)
	 {
		key.i = bioskey_new(0);
		if (key.ch[0]==0)
		{
			key.ch[0]=key.ch[1];
			if (key.ch[0]==F1KEY)

			{
						T3000help(400);
			}
		}
	 }
*/
/*
	 if (key.ch[0]==27)  //esc
	 {
		 {
			esc = 1;
		 }
		 break ;
	 }
	 if (key.ch[0]==68)  //f10
	 {
		memsize();
	 }
	 if(key.ch[0]==1)  //CTRL-A  alarm  screen
	 {
				 menu=0;
				 first=1;
				 key.ch[0]=27;  //esc
				 esc = ALARMS+1;
				 break;
	 }
*/
/*
	 if (key.ch[0]==INSERT || key.ch[0]==ENTER || key.ch[0]==50 || key.ch[0]==DELETE)
	 {
		if( DIAL_updategrp==1 )
		 resume(DIAL);

		mouse_status(stat);
		curcol = stat.where.x;
		curline = stat.where.y;
		pgrp = group_element;
		pworkdata = grp_workdata;
		for(i=0;i<index_max; pgrp++,pworkdata++,i++)
		{
		 if( pgrp->text_present && (stat.where.x >= pgrp->graphic_x_coordinate && stat.where.x <= pgrp->graphic_x_coordinate+print_des(pgrp,pworkdata,panel,network,1)*8 &&
					stat.where.y >= pgrp->graphic_y_coordinate && stat.where.y <= pgrp->graphic_y_coordinate+lat_char_n) )      // lung_char_n  am fixat la 8
					 break;
//			 (pgrp->icon_present && (stat.where.x >= pgrp->xicon && stat.where.x <= pgrp->xicon+pworkdata->icon->ptr_icon->width &&
//				stat.where.y >= pgrp->yicon && stat.where.y <= pgrp->yicon+pworkdata->icon->ptr_icon->height)) )
		 if( pworkdata->icon )
			 if ( pworkdata->icon->ptr_icon )
			 {
				if ( stat.where.x >= pgrp->xicon && stat.where.x <= pgrp->xicon+pworkdata->icon->ptr_icon->width &&
						 stat.where.y >= pgrp->yicon && stat.where.y <= pgrp->yicon+pworkdata->icon->ptr_icon->height )
					break;
			 }
		}
		index_cur=i;
		if(index_cur!=index_max && key.ch[0]==DELETE && !display) //Delete
		{
		 delete_elem(i, panel, network, current_point);
		}
		pgrp = &group_element[index_cur];
		pworkdata = &grp_workdata[index_cur];
		if(index_cur!=index_max && key.ch[0]==50 && !display) //Alt-M move
		{
//		char bitmap[800];
//		int l = print_des(&group_element[i]);
//		hide_pointer();
//		int n = imagesize(group_element[i].graphic_x_coordinate,group_element[i].graphic_y_coordinate, group_element[i].graphic_x_coordinate+l*textwidth(" "),group_element[i].graphic_y_coordinate+8);
//		mgetimage(group_element[i].graphic_x_coordinate,group_element[i].graphic_y_coordinate, group_element[i].graphic_x_coordinate+l*textwidth(" "),group_element[i].graphic_y_coordinate+8,bitmap);
//		mputimage(group_element[i].graphic_x_coordinate,group_element[i].graphic_y_coordinate, bitmap,XOR_PUT);
//		print_des(&group_element[i]);
//		hide_pointer();
//		mgetimage(group_element[i].graphic_x_coordinate,group_element[i].graphic_y_coordinate, group_element[i].graphic_x_coordinate+l*textwidth(" "),group_element[i].graphic_y_coordinate+8,bitmap);
//		display_pointer();
		if(screen_access==R_READ_WRITE)
		{
		 int l = print_des(pgrp,pworkdata,panel,network);
		 copy_icon = 1;
		 while(1)
		 {
//		 while(!bioskey_new(1));
			if(bioskey_new(1))
			{
			 key.i = bioskey_new(0);
			 if(key.ch[0]==ENTER)
			 {
				copy_icon = 0;
				modify_grp=1;
				if( !local_request(panel,network) )
				 if( (panel_info1.panel_type == T3000 && panel_info1.version >= 221) ||
						(panel_info1.panel_type == MINI_T3000 && panel_info1.version >= 111) )
					{
//						 int k;
						 i = sizeof(Str_grp_element);
//						 k = elemindex(group_element, index_cur);
						 while( DIAL_updategrp==1);
						 net_call(READGROUPELEMENT_T3000+100, UPDATE_ELEMENT+(current_point<<8), (char *)pgrp, &i, panel, network,
											NETCALL_RETRY,TIMEOUT_NETCALL,NULL,NULL,NULL,0,pgrp->nr_element);
						 modify_grp=0;
						 modify_grp_new = 1;
					}
				break;
			 }
			 if(key.ch[0]==0)
			 {
				hide_pointer();
				if( pworkdata->ptr_save_bkgnd )
				{
				 mputimage(pgrp->graphic_x_coordinate,pgrp->graphic_y_coordinate,pworkdata->ptr_save_bkgnd, COPY_PUT);
//			mfarfree( (HANDLE)FP_SEG(group_element[i].ptr_save_bkgnd) );
				 mfarfree(pworkdata->ptr_save_bkgnd);
				 pworkdata->ptr_save_bkgnd=NULL;
				}

				i=2;
				if( bioskey_new(2)&0x02 ) i=8;
				switch (key.ch[1]) {
				case DOWN:
				 if(pgrp->icon_present)
				 {
					 if (pgrp->yicon+pworkdata->icon->ptr_icon->height < maxy - i) pgrp->yicon += i;
					 break;
				 }
				 if(pgrp->text_present && pgrp->graphic_y_coordinate+i < maxy - 10) pgrp->graphic_y_coordinate += i;
				 break;
				case UP:
				 if(pgrp->icon_present)
				 {
					 if ( ((int)pgrp->yicon)-i > 20) pgrp->yicon -= i;
					 break;
				 }
				 if( ((int)pgrp->graphic_y_coordinate)-i > 20)  pgrp->graphic_y_coordinate -= i;
				 break;
				case LEFT:
				 if(pgrp->icon_present)
				 {
					if(((int)pgrp->xicon)-i > 0)  pgrp->xicon -= i;
					break;
				 }
				 if( ((int)pgrp->graphic_x_coordinate)-i > 0)  pgrp->graphic_x_coordinate -= i;
				 break;
				case RIGHT:
				 if(pgrp->icon_present)
				 {
					if(pgrp->xicon+pworkdata->icon->ptr_icon->width+i < maxx+1) pgrp->xicon += i;
					break;
				 }
				 if(pgrp->graphic_x_coordinate+i+l*8 < maxx+1) pgrp->graphic_x_coordinate += i;
				 break;
				}
				if(pgrp->icon_present)
				 move_mouse(pgrp->xicon,pgrp->yicon);
				else
				 move_mouse(pgrp->graphic_x_coordinate,pgrp->graphic_y_coordinate);
				if (refresh_time < 15) refresh_time = 15;
				print_des(pgrp,pworkdata,panel,network);
//		 display_pointer();
			 }
			}
			else
			{
			 mouse_status(stat);
			 if(stat.leftButton)
			 {
				while(!get_button_released(BLEFT,stat));
				copy_icon = 0;
				modify_grp=1;
				break;
			 }
//		 int x,y;
			 if (mouse_motion(&x,&y))
			 {
				hide_pointer();
//			mmputimage(group_element[i].graphic_x_coordinate,group_element[i].graphic_y_coordinate,group_element[i].graphic_x_coordinate+group_element[i].nchar*lat_char_n,group_element[i].graphic_y_coordinate+8,group_element[i].ptr_save_bkgnd);
//			set_semaphore_dos();
//			mfarfree( group_element[i].ptr_save_bkgnd);
//			clear_semaphore_dos();
				if( pworkdata->ptr_save_bkgnd )
				{
				 mputimage(pgrp->graphic_x_coordinate,pgrp->graphic_y_coordinate,pworkdata->ptr_save_bkgnd, COPY_PUT);
//			mfarfree( (HANDLE)FP_SEG(group_element[i].ptr_save_bkgnd));
				 mfarfree( pworkdata->ptr_save_bkgnd );
				 pworkdata->ptr_save_bkgnd=NULL;
				}
//	  	 mputimage(group_element[i].graphic_x_coordinate,group_element[i].graphic_y_coordinate, bitmap,XOR_PUT);
				if(	stat.where.x < 0) stat.where.x=0;
				if(pgrp->icon_present)
				{
				 if(stat.where.x+pworkdata->icon->ptr_icon->width >= maxx) stat.where.x = maxx-pworkdata->icon->ptr_icon->width-1;
				}
				else
				{
				 if( stat.where.x+l*8 >= maxx) stat.where.x = maxx-l*8;
				}
				pgrp->graphic_x_coordinate=stat.where.x;
				pgrp->xicon=stat.where.x;
				if(	stat.where.y < 22) stat.where.y=22;
				if(pgrp->icon_present)
				{
				 if(stat.where.y+pworkdata->icon->ptr_icon->height + 16 >= maxy) stat.where.y = maxy-pworkdata->icon->ptr_icon->height-16;
				}
				else
				{
				 if( stat.where.y+8 >= maxy) stat.where.y = maxy-8;
				}
				pgrp->graphic_y_coordinate=stat.where.y;
				pgrp->yicon=stat.where.y;
				if (refresh_time < 15) refresh_time = 15;
				print_des(pgrp,pworkdata,panel,network);
//			 mputimage(group_element[i].graphic_x_coordinate,group_element[i].graphic_y_coordinate, bitmap,XOR_PUT);
//			 display_pointer();
			 }
			}
		 }
//	 hide_pointer();
//	 mputimage(group_element[i].graphic_x_coordinate,group_element[i].graphic_y_coordinate, bitmap,XOR_PUT);
//	 print_des(&group_element[i]);
//	 move_mouse(group_element[i].graphic_x_coordinate,group_element[i].graphic_y_coordinate);
//	 display_pointer();
		 if (refresh_time < 15) refresh_time = 15;
		 print_des(pgrp,pworkdata,panel,network);
		 index_cur=index_max;
		}
		else
		 DisplayMessage(160, 150, 500, 210, NULL, " Unauthorized access! ", NULL, Black, NULL, 1200);
	 }

			if(index_cur!=index_max && key.ch[0]==ENTER && !display)
			{
			 first = 0;
			 if(!pgrp->point_absent)
			 {
				if(pgrp->point_info.point.point_type-1!=PRG &&
				 pgrp->point_info.point.point_type-1!=AMON &&
				 pgrp->point_info.point.point_type-1!=GRP &&
				 pgrp->point_info.point.point_type-1!=WR &&
				 pgrp->point_info.point.point_type-1!=AR &&
				 pgrp->point_info.point.point_type-1!=CON &&
				 pgrp->point_info.description_label != DISPLAY_ICON_VALUE+1 )
				 key.ch[0]=INSERT;
				else
				if(retpoint)
				{
				if( check_access(pgrp->point_info.point.point_type-1, pgrp->point_info.point.number)!=R_NA )
				{
				 menu=0;
				 first=1;
				 key.ch[0]=27;  //esc
				 *retpoint=pgrp->point_info.point;
				 if( pgrp->point_info.description_label ==DISPLAY_ICON_VALUE+1 )
					esc = pgrp->point_info.point.point_type+100;
				 else
					esc = pgrp->point_info.point.point_type;
//				xcur_grp=group_element[index_cur].graphic_x_coordinate;
//				ycur_grp=group_element[index_cur].graphic_y_coordinate;
//
//				if(esc-1!=PRG)
//					dontch = 1;
//				else
//					dontch = 0;
//
				 break;
				}
			 }
				 }
		 }

	 if(index_cur!=index_max && key.ch[0]==INSERT)
	 {
//			if(index_cur!=index_max)
			 if(!pgrp->point_absent)
			 {
				cell(curcol,curline,pgrp,pworkdata,panel,network,1);
				if(modify_grp)
				 if( !local_request(panel,network) )
					if( (panel_info1.panel_type == T3000 && panel_info1.version >= 221) ||
							(panel_info1.panel_type == MINI_T3000 && panel_info1.version >= 111) )
					{
						 while( DIAL_updategrp==1);
						 i = sizeof(Str_grp_element);
						 net_call(READGROUPELEMENT_T3000+100, UPDATE_ELEMENT+(current_point<<8), (char *)pgrp, &i, panel, network,
											NETCALL_RETRY,TIMEOUT_NETCALL,NULL,NULL,NULL,0,pgrp->nr_element);
						 modify_grp=0;
						 modify_grp_new=1;
					}
			 }
	 }

	 if(index_cur==index_max && key.ch[0]==INSERT && !display)
	 {
			if(screen_access==R_READ_WRITE)
			{
//					index_max++;  				 //nu exista elementul
//					index_max_group_element = index_max;
					if(index_max < MAX_ELEM)
					{
					 if( new_elem(curcol,curline,index_max,panel,network,current_point))
					 {
						index_max++;  				 //nu exista elementul
						index_max_group_element = index_max;
						hide_pointer();
						if (refresh_time < 15) refresh_time = 15;
						print_des(&group_element[index_cur],grp_workdata+index_cur,panel,network);
//					 modify_grp = 1;
						display_pointer();
					 }
					 else
					 {
						memset(&group_element[index_cur], 0, sizeof(Str_grp_element));
						memset(&grp_workdata[index_cur], 0, sizeof(Str_grp_work_data));
					 }
					}
					else
					{
					 DisplayMessage(160, 150, 500, 210, NULL, " To many elements! ", NULL, Black, NULL, 1500);
					}
			}
			else
			 DisplayMessage(160, 150, 500, 210, NULL, " Unauthorized access! ", NULL, Black, NULL, 1200);
	 }
	}
*/
//	if (key.ch[0] == '\t') //tab
	{
					page++;
					index_cur=0;
					curline=2;
					curcol=1;
					first=0; last_mouse=0;
					refresh_time = 2000;
					pgrp = &group_element[index_cur];
					for(i=index_cur;i<index_obj;pgrp++,i++)
					{
					 if(pgrp->nr_element)
					 {
						 print_des(pgrp,grp_workdata+i,panel,network);
					 }
					}
//					refresh_time = refresh_time_init;
	}
/*
 if (key.ch[0] == PGDN	&& key.ch[1] == PGDN) //Pgdn
 {
	 esc = PGDN;
	 break ;
 }
*/
/*
 if (key.ch[0] == PGUP && key.ch[1] == PGUP) //PgUP
 {
	 esc = PGUP;
	 break ;
 }
*/
/*
 if (key.ch[0]==DOWN || key.ch[0]==UP || key.ch[0] == RIGHT || key.ch[0] == LEFT )  //dn, up
 {
//	 int x,y;
	 mouse_status(stat);
	 x=stat.where.x;
	 y=stat.where.y;
	 i=2;
	 if( bioskey_new(2)&0x02 ) i=8;
	 if (key.ch[0]==DOWN)  //dn
			 y += i;
	 if (key.ch[0]==UP)  //up
			 y -= i;
	 if (key.ch[0]==RIGHT)  //right
			 x += i;
	 if (key.ch[0]==LEFT)  //left
			 x -= i;
	 if (x<0) x=0;
	 if (x>maxx) x=maxx;
	 if (y<0) y=0;
	 if (y>maxy) y=maxy;

	 if(menu==1 && key.ch[0]==DOWN)
	 {
			menu=0;
			y = 20;
			key.ch[0]=0;
			first = 0;
	 }
	 else
		 if(menu==0 && y < 19 && !display)
		 {
			menu=1;
			y = 17;
		 }
	 move_mouse(x,y);
 }
*/
/*
 if (menu && !display)
			{
			int qt=0;
		msettextstyle(DEFAULT_FONT, 0, 1);
		grp_menu->GShowMenuBar(graphic_grp_message);
		msettextstyle(SMALL_FONT, 0, 5);
			do {

			 msettextstyle(DEFAULT_FONT, 0, 1);
			 int man=grp_menu->GReturn();
			 msettextstyle(SMALL_FONT, 0, 5);
			 switch (man) {

				 case 0:

				 {
//				 set_semaphore_dos();
				 GGrid *w = new GGrid(6,22,6+70*8,450,0,NULL,GRAPHIC_GRP,ptr_panel->GlPanel_type,ptr_panel->GlPanel,ptr_panel->GlNetwork);
//				 clear_semaphore_dos();
				 w->GWSet("Group Elements",Lightgray,Darkgray);
				 w->save = 0;   //NO_SAVE
//				 w->save = 2;   //NO_SAVE
				 w->GShowGrid();
				 nothing = 0;
				 while (!w->HandleEvent());
				 nothing = 1;
//				 set_semaphore_dos();
				 delete w;
//				 clear_semaphore_dos();
				 }
				 hide_pointer();
//				 get16( pal, 0 );

				 if(strlen(rtrim(block->picture_file)))
				 {
					struct ffblk block;
					set_semaphore_dos();
					int_disk=1;
					int done=findfirst(filename,&block,FA_ARCH);
					int_disk=0;
					clear_semaphore_dos();
					if(!done)
						tif(filename,0,0,maxx,maxy);
					else
					{
						cleardevice();
						grp_current_pal=init_pal;
					}
				 }
				 else
				 {
						cleardevice();
						grp_current_pal=init_pal;
				 }

//				 get16(pal1,0);
				 BOXCOLOR=closest_rgb( init_pal[BOXCOLOR], grp_current_pal, 16);
//				 set16( pal, 0 );
				 display_pointer();
				 page = -1;
				 first = 1;
				 key.ch[0]='\t';
				 menu=0;
				 qt=1;
				 msettextstyle(SMALL_FONT, 0, 5);
				 break;
				 case 1:
					 char b[6];
					 itoa( refresh_time_init/18, b, 10);
					 ReadWindow(200, 150, 400, 220, Lightgray, Darkgray, Blue,
							 Cyan, White, "Refresh time : ", b, 3);
					 refresh_time_init = atoi(b)*18;
					 break;
				 case  2:
				 case -2:
					 menu=0;
					 first=1;
					 qt=1;
					 key.ch[0]=27;  //esc
					 break;
				 case DOWN:
					 qt=1;
					 key.ch[0]=DOWN;
					 display_pointer();
					 break;
				 case -3:
					 qt=1;
					 menu=0;
					 first=0;
					 display_pointer();
					 break;
				 case -4:
					 qt=1;
					 break;
			 }
			} while(!qt);
grp_menu->ReleaseMBar();

			}
*/
}
/*
 if(mouse_inside(0, 0, maxx, 19 ) &&  key.ch[0]!=27 && page!=-1 && !display)
 {
		 menu=1; first=1;
		 if(key.ch[0]!=DOWN) key.ch[0]=0;
 }
*/
/*
mouse_status(stat);
if(stat.rightButton)
{
		 {
			while(!get_button_released(BRIGHT,stat));
			esc = 1;
		 }
	 break ;
}
*/
/*
if(mouse_inside(500, 1, 639, 16 ))
	 if(stat.leftButton)
	 {
		Panel_info1 panel_info;
		if(local_request(panel,network))
		{
		 if( GAlarm )
				 showpoints(M3+3, Panel_Info1.panel_type, panel, network);
		}
		else
		{
		 if( gAlarm )
				if( net_call(COMMAND_50, PANEL_INFO1_COMMAND, (char*)&panel_info, 0, panel, network) == RS232_SUCCESS )
				{
					showpoints(M3+3, panel_info.panel_type, panel, network);
				}
		}
	 }
*/
/*
if(stat.leftButton)
{
 while(!get_button_released(BLEFT,stat));
 pgrp = group_element;
 pworkdata = grp_workdata;
 for(i=0;i<index_max_group_element;pgrp++,pworkdata++,i++)
 {
	int l = print_des(pgrp,pworkdata,panel,network,1);
	if( (pgrp->text_present && (stat.where.x >= pgrp->graphic_x_coordinate && stat.where.x <= pgrp->graphic_x_coordinate+l*8 &&
			 stat.where.y >= pgrp->graphic_y_coordinate && stat.where.y <= pgrp->graphic_y_coordinate+lat_char_n)) ||       // lung_char_n  am fixat la 8
			 (pgrp->icon_present && (stat.where.x >= pgrp->xicon && stat.where.x <= pgrp->xicon+pworkdata->icon->ptr_icon->width &&
				stat.where.y >= pgrp->yicon && stat.where.y <= pgrp->yicon+pworkdata->icon->ptr_icon->height)) )
	if( (pgrp->text_present && mouse_inside(pgrp->graphic_x_coordinate,pgrp->graphic_y_coordinate,
								 pgrp->graphic_x_coordinate+l*textwidth(" "),pgrp->graphic_y_coordinate+8)) ||
		 (pgrp->icon_present && mouse_inside(pgrp->xicon,pgrp->yicon,
								 pgrp->xicon+pworkdata->icon->ptr_icon->width,pgrp->yicon+pworkdata->icon->ptr_icon->height)) )
	{
	 first=1;
	 key.ch[0] = ENTER;
	 break;
	}
 }
}
*/
/*
	if(!refresh_time)
	{
			 cont = 1;
			 if(modify_grp)
			 {
					if(!display)
					{
					 if( local_request(panel,network) )
						error_communication=send_grp(1,index_obj, current_point, group_element, curline, curcol);
					 else
					 {
						if( DIAL_updategrp==1)
						 resume(DIAL);
						while(DIAL_updategrp==1);
						error_communication=send_grp(0,index_obj, current_point, group_element, curline, curcol,panel,network);
						modify_grp_new=1;
					 }
					}
			 }
			 if( local_request(panel,network) )
			 {
				 if(local_panel_old == local_panel)
					update_value_grp_elem(ptr_panel->control_group_elements[current_point].ptrgrp,ptr_panel->control_group_elements[current_point].nr_elements);
			 }
			 else
			 {
				if( !DIAL_updategrp )
				{
					current_point_GRP = current_point;
					panel_GRP = panel;
					network_GRP = network;
					DIAL_updategrp=1;
					DIAL_action=1;
					resume(DIAL);
					cont = 0;
				}
				else
				{
				 if( DIAL_updategrp!=1 )
				 {
					if( DIAL_updategrp==2 ) //OK
					{
					 if( !modify_grp_new )
					 {
						Str_grp_element *buf;
						buf = group_element_temp;
//						memset(group_element,0,MAX_ELEM*sizeof(Str_grp_element));
						index_max=update_length/sizeof(Str_grp_element);
						index_max_group_element=index_max;
//						pgrp = group_element;
						for(i=0; i<index_max;i++)
						{
						 group_element[i]=buf[i];
						}
						cont = 1;
					 }
					 else
					 {
						modify_grp_new = 0;
						cont = 0;
					 }
					}
					else                    //Error
					 cont = 2;
					DIAL_updategrp=0;
				 }
				 else
					cont = 0;
				}
			 }
			 if(cont==1)
			 {
				pgrp = group_element;
				for(i=0;i<index_obj;pgrp++,i++)
				{
					if(pgrp->nr_element)
					{
						 print_des(pgrp,grp_workdata+i,panel,network);
					}
				}
			 }
			 if(cont)
			 {
				if( refresh_time_init )	refresh_time = refresh_time_init;
				else
				{
				 if(rtime==4)
					 rtime = 5;
				 else
					 rtime = 4;
				 refresh_time = rtime;
				}
			 }
	}
*/
/*
	if( !DIAL_updategrp )
	{
	 if( timestart+timesec1970 > lastreadopenscreen+15 )
	 {
				 x=1;
				 lastreadopenscreen = timestart+timesec1970;

				 if( local_request(panel,network) )
					openscreen = localopenscreen;
				 else
				 {
					if( (panel_info1.panel_type == T3000 && panel_info1.version >= 221) ||
					 (panel_info1.panel_type == MINI_T3000 && panel_info1.version >= 111) )
					{
					 i = sizeof(Point_Net);
					 if(net_call(COMMAND_50, OPENSCREEN_COMMAND, (char *)&openscreen, &i, panel, network, BACnetConfirmedRequestPDU|NETCALL_NOTTIMEOUT)!=SUCCESS)
						x=0;
					}
				 }
				 if( x && (openscreen.point_type &&	!(openscreen==current_opened_grp)) )
					if( retpoint )
					{
					 if( check_access(openscreen.point_type-1, openscreen.number)!=R_NA )
					 {
						 menu=0;
						 first=1;
						 key.ch[0]=27;  //esc
						 *retpoint=openscreen;
						 esc = openscreen.point_type;
						 break;
					 }
					}
		}
		print_alarm_word();
   }
*/
	}

/*
if( DIAL_updategrp==1)
		 resume(DIAL);
if(!display)
{
	 delete grp_menu;

	if(!error_communication)
//	 if(local_panel)
//	 if((local_panel &&  panel==Station_NUM) || (!local_panel &&  panel==Station_NUM && panel!=station_num))
	 if(modify_grp)
	 {
		if( local_request(panel,network) )
		{
		 if(local_panel_old == local_panel)
			send_grp(1,index_obj, current_point, group_element, curline, curcol);
		}
		else
		{
		 while(DIAL_updategrp==1);
		 send_grp(0,index_obj, current_point, group_element, curline, curcol,panel,network);
		}
	 }

}
*/
/*
if(!error_communication)
{
 pgrp = group_element;
 pworkdata = grp_workdata;
 for(i=0;i<index_obj;pgrp++,pworkdata++,i++)
 {
	if(pworkdata->ptr_save_bkgnd)
	{
//		mfarfree( (HANDLE)FP_SEG(group_element[i].ptr_save_bkgnd));
		mfarfree( pworkdata->ptr_save_bkgnd );
		pworkdata->ptr_save_bkgnd = NULL;
	}
	if( pgrp->icon_present && pworkdata->icon )
	{
		if( !pgrp->default_icon )
			if( pworkdata->icon->ptr_icon )
				delete pworkdata->icon->ptr_icon;
		delete pworkdata->icon;
		pworkdata->icon = NULL;
//		if( ((struct icon huge *)group_element[i].icon)->image_under )
//		{
//			mfarfree( (HANDLE)FP_SEG(((struct icon huge *)group_element[i].icon)->image_under));
//			((struct icon huge *)group_element[i].icon)->image_under = NULL;
//		}
	}
 }
}
*/
} //while esc
/*
mouse_status(stat);
xcur_grp = stat.where.x;
ycur_grp = stat.where.y;
if( !interface_mode )
	set16( init_pal, 0 );
msettextstyle(DEFAULT_FONT, HORIZ_DIR,1);
hide_pointer();
status_mouse = HIDEMOUSE;
grp_current_pal=NULL;
display_pointer();
return esc;
*/
}

//////////////////////////

//	 delete (char *)(group_element);
//	 if( DIAL_updategrp==1)
//		 resume(DIAL);
//	 while( DIAL_updategrp==1);
//	 DIAL_updategrp=0;
//	 mfarfree(handle_group_element);
/*
	 if(!local_request(panel,network))
	 {
		if( icon_name_table )
		{
		 for(i=0;i<MAX_ICON_NAME_TABLE;i++)
		 {
			 if( icon_name_table[i][0] )
			 {
				 i = sizeof(Icon_name_table);
				 net_call(COMMAND_50+100, ICON_NAME_TABLE_COMMAND, (char*)icon_name_table, &i, panel, network,NETCALL_RETRY);
				 break;
			 }
		 }
		 set_semaphore_dos();
		 delete (char *)(icon_name_table);
		 clear_semaphore_dos();
		}
	 }
*/
//	 display_pointer();
	 icon_name_table=NULL;
	 if(exit_time_display)
	 {
//		asm pop es;
		return 0;
	 }
//	 asm pop es;
	 return r;
}


void __fastcall TGroupForm::FormDblClick(TObject *Sender)
{
/*
 Str_grp_element   *pgrp = group_element;
 Str_grp_work_data  *pworkdata = grp_workdata;
 for(DInt i=0;i<index_max_group_element;pgrp++,pworkdata++,i++)
 {
//	DInt l = print_des(pgrp,pworkdata,panel,network,1);
	if(  pgrp->icon_present )
      if(mouseX >= pgrp->xicon && mouseX <= pgrp->xicon+pworkdata->icon.width &&
		mouseY >= pgrp->yicon && mouseY <= pgrp->yicon+pworkdata->icon.height)
	{
     if( pgrp->point_info.point.point_type-1 == PRG)
     {
      TProgramForm *ProgramForm;
      ProgramForm = new TProgramForm( Application,ptr_panel->programs[ pgrp->point_info.point.number ], pgrp->point_info.point.number);
      ProgramForm->ShowModal();
      delete ProgramForm;
      break;
     }
     if( pgrp->point_info.point.point_type-1 == AMON)
     {
      TGraphForm *GraphForm;
//      GraphForm = new TGraphForm( Application,ptr_panel->analog_mon[ pgrp->point_info.point.number ], pgrp->point_info.point.number);
      GraphForm = new TGraphForm( Application );
      GraphForm->ShowModal();
      delete GraphForm;
      break;
     }
     if(pgrp->point_info.point.point_type-1!=GRP ) continue;
	 retpoint=pgrp->point_info.point;
	 if( pgrp->point_info.description_label ==DISPLAY_ICON_VALUE+1 )
	  esc = pgrp->point_info.point.point_type+100;
     else
	  esc = pgrp->point_info.point.point_type;
     ModalResult = mrOk;
     break;
	}
 }
*/
}
//---------------------------------------------------------------------------

void __fastcall TGroupForm::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  mouseX = X;
  mouseY = Y;
  if(   Button== mbRight )
  {

 bool click=false;
 Str_grp_element   *pgrp = group_element;
 Str_grp_work_data  *pworkdata = grp_workdata;
 for(DInt i=0;i<index_max_group_element;pgrp++,pworkdata++,i++)
 {
	DInt l = print_des(pgrp,pworkdata,panel,network,1);
    click=false;
	if( pgrp->text_present )
      if( mouseX >= pgrp->graphic_x_coordinate && mouseX <= pgrp->graphic_x_coordinate+l*6 &&
	      mouseY >= pgrp->graphic_y_coordinate && mouseY <= pgrp->graphic_y_coordinate+12 )
        click=true;
	if(  pgrp->icon_present )
      if(  mouseX >= pgrp->xicon && mouseX <= pgrp->xicon+pworkdata->icon.width &&
		   mouseY >= pgrp->yicon && mouseY <= pgrp->yicon+pworkdata->icon.height )
        click=true;
    if( click==true )
	{
      TObjectForm *ObjectForm;
      ObjectForm = new TObjectForm( Application, pgrp, pworkdata);
      ObjectForm->ShowModal();
      delete ObjectForm;
      break;
	}
 }

  }
}
//---------------------------------------------------------------------------

void __fastcall TGroupForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	 delete group_element;
	 delete group_element_temp;
	 delete grp_workdata;
	 group_element_temp = NULL;
	 group_element = NULL;
     grp_workdata = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TGroupForm::FormClick(TObject *Sender)
{
 bool click;
 Str_grp_element   *pgrp = group_element;
 Str_grp_work_data  *pworkdata = grp_workdata;
 for(DInt i=0;i<index_max_group_element;pgrp++,pworkdata++,i++)
 {
	DInt l = print_des(pgrp,pworkdata,panel,network,1);
    click=false;
	if( pgrp->text_present )
      if( mouseX >= pgrp->graphic_x_coordinate && mouseX <= pgrp->graphic_x_coordinate+l*6 &&
			 mouseY >= pgrp->graphic_y_coordinate && mouseY <= pgrp->graphic_y_coordinate+12)
        click=true;
	if(  pgrp->icon_present )
      if(  mouseX >= pgrp->xicon && mouseX <= pgrp->xicon+pworkdata->icon.width &&
		   mouseY >= pgrp->yicon && mouseY <= pgrp->yicon+pworkdata->icon.height )
        click=true;

    if( click==true)
	{
     if( pgrp->point_info.point.point_type-1 == PRG)
     {
      TProgramForm *ProgramForm;
      ProgramForm = new TProgramForm( Application,ptr_panel->programs[ pgrp->point_info.point.number ], pgrp->point_info.point.number);
      ProgramForm->ShowModal();
      delete ProgramForm;
      break;
     }
     if( pgrp->point_info.point.point_type-1 == AMON)
     {
      TGraphForm *GraphForm;
//      GraphForm = new TGraphForm( Application,ptr_panel->analog_mon[ pgrp->point_info.point.number ], pgrp->point_info.point.number);
      GraphForm = new TGraphForm( Application );
      GraphForm->ShowModal();
      delete GraphForm;
      break;
     }
     if(pgrp->point_info.point.point_type-1!=GRP ) continue;
	 retpoint=pgrp->point_info.point;
	 if( pgrp->point_info.description_label ==DISPLAY_ICON_VALUE+1 )
	  esc = pgrp->point_info.point.point_type+100;
     else
	  esc = pgrp->point_info.point.point_type;
     ModalResult = mrOk;
     break;
	}
 }
/*
 bool click=false;
 Str_grp_element   *pgrp = group_element;
 Str_grp_work_data  *pworkdata = grp_workdata;
 for(DInt i=0;i<index_max_group_element;pgrp++,pworkdata++,i++)
 {
//	DInt l = print_des(pgrp,pworkdata,panel,network,1);
	if(  pgrp->icon_present )
      if( (mouseX >= pgrp->xicon && mouseX <= pgrp->xicon+pworkdata->icon.width &&
		   mouseY >= pgrp->yicon && mouseY <= pgrp->yicon+pworkdata->icon.height) ||
           (mouseX >= pgrp->graphic_x_coordinate && mouseX <= pgrp->graphic_x_coordinate+40 &&
		   mouseY >= pgrp->graphic_y_coordinate && mouseY <= pgrp->graphic_y_coordinate+10 )
         )
        click=true;

	if(  !pgrp->icon_present )
      if(mouseX >= pgrp->graphic_x_coordinate && mouseX <= pgrp->graphic_x_coordinate+40 &&
		mouseY >= pgrp->graphic_y_coordinate && mouseY <= pgrp->graphic_y_coordinate+10 )
         click=true;
    if( click==true )
	{
      TObjectForm *ObjectForm;
      ObjectForm = new TObjectForm( Application );
      ObjectForm->ShowModal();
      delete ObjectForm;
      break;
	}
 }
*/
}
//---------------------------------------------------------------------------

