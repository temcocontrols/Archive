//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
#include <vcl\sysdefs.h>
#include <vcl/dbgrids.hpp>

#include "Grid1.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#include "define.h"
#include "baseclas.h"
#include "grid.h"
#include "calend.h"
#include "aio.h"
#include "recdef.h"
#include "ptpdll.h"

#pragma link "Grids"
#pragma resource "*.dfm"
TGridForm *GridForm;

extern void closeGrid(void);
extern char *intervaltotext( DLong seconds , DInt minutes , DInt hours, char *c );
extern DInt pointtotext(DByte *buf,Point_T3000 *point);
extern DInt pointtotext(DByte *buf,Point_Net *point);
extern int alignInputs(int t, DByte *buf, Str_in_point *inputs, int n);
extern int alignOutputs(int t, DByte *buf, Str_out_point *inputs, int n);
extern int alignVars(int t, DByte *buf, Str_variable_point *vars, int n);
extern int alignCons(int t, DByte *buf, Str_controller_point *cons, int n);
extern int alignWeeklyroutines(int t, DByte *buf, Str_weekly_routine_point *wr, int n);
extern int alignAnnualroutines(int t, DByte *buf, Str_annual_routine_point *annual, int n);
extern int alignArrays(int t, DByte *buf, Str_array_point *ay, int n);
extern int alignScreens(int t, DByte *buf, Control_group_point *grps, int n);
extern int alignPrograms(int t, DByte *buf, Str_program_point *prgs, int n);
extern void resetImage(void);
extern DUint countlength(DInt command, DInt arg);
extern void align(DInt type, DInt point_type, DByte *buf, DInt start, DInt nr);

extern DInt  station_num;
extern DInt local_panel;
extern unsigned DByte tbl_bank[MAX_TBL_BANK];
extern Panel *ptr_panel;
extern dig_range_form dig_range_array[];
extern an_range_form in_range_array[];
extern an_range_form out_range_array[];
extern an_range_form var_range_array[];
extern Byte serial_access;

//Added by Paul Mao on 7/8/97:
extern void StatusBarMessage(int i, AnsiString text);

char *auto_man[2]={"Auto","Manual"};
char *filter[]={"1","2","4","8","16","32","64","128"};
char *textstext="Text";
char *graphictext="Graphic";

int maxgridTable = MAXGRIDTABLE;
GRIDTABLE gridTable[MAXGRIDTABLE];   // tracks the open windows

//---------------------------------------------------------------------------
__fastcall TGridForm::TGridForm(TComponent* Owner, DUint ptype, int recs, int oindex)
	: TForm(Owner)
{
  Height           = 380;
  pointType        = ptype;
  objectIndex      = oindex;
  recordCount      = recs;
  refreshTime      = -1;
  refreshTimeConst = -1;
  refreshTimeFlag = false;
  menuType         = 1;
  emptyTable       = true;
  readOnlyFields   = 0;

  DM->grid         = this;
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::Setup(DUint ppanel, DInt pnetwork,
                                 TDataSource *ds, TTable *table, char *t,
                                 int width, int height,
                                 DLong ronly, int refresh, int menu, int empty)
{
  panel          = ppanel;
  network        = pnetwork;
  dataSource     = ds;
  Table          = table;
  Caption         = t;
  if ( width != -1 )
	  Width           = width;
  if ( height != -1 )
      Height          = height;
  DBGridT3000->Width = width-24;
//  Bevel1->Width = width-22;
  readOnlyFields  = ronly;
  if( refresh >= 0 )
  {
      refreshTime      = refresh;
      refreshTimeConst = refresh;
   	  refreshTimeFlag  = true;
  }
  else
  {
      refreshTime      = -1;
      refreshTimeConst = -1;
  	  refreshTimeFlag = false;
  }
  menuType        = menu;
  if( menuType == 1 )
  {
   Menu = MainMenu1;
   DBGridT3000->PopupMenu = PopupMenu1;
  }
  if( menuType == 2 )
  {
   Menu = MainMenu2;
   DBGridT3000->PopupMenu = PopupMenu2;
  }
  if( menuType == 3 )
  {
   Menu = MainMenu3;
   DBGridT3000->PopupMenu = PopupMenu3;
  }
  emptyTable      = empty;
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::OnActivate(TObject *Sender)
{
  DM->grid  = this;
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  refreshTimeFlag = false;
  Table->Active   = false;
  delete(gridTable[objectIndex].grid);
  gridTable[objectIndex].grid = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TGridForm::ShowObject2Click(TObject *Sender)
{
  if ( pointType == AR )
  {
   CalendarForm = new TCalendarForm( Application );
   CalendarForm->ShowModal();
  }
}
//---------------------------------------------------------------------------
DUint __fastcall TGridForm::getPointType(void)
{
  return pointType;
}
//---------------------------------------------------------------------------
DInt getpoint(Point_T3000 point, char *dest, int read)
{
				DByte num_point,var_type, point_type, num_panel;
                DInt  num_net;
				DByte *p, buf[MAX_ALPHA+1];
				if(read)
				{
/*
						GetAlpha(field, MAX_ALPHA, buf);
						pgrid->GDGets(buf,pgrid->w_record,pgrid->t_fields[field-1],GetFldSize(field),Black,White);
						if( (p=ispoint(rtrim(buf),&num_point,&var_type, &point_type, &num_panel, &num_net, network, panel)) || !strlen(rtrim(buf)) )
						{
						if(strlen(rtrim(buf)))
						{
						 strcpy(buf,p);
						 point.panel = num_panel-1;point.number = num_point-1;point.point_type = point_type+1;
						}
						else
						 point.panel = point.number = point.point_type = 0;

						ralign(lalign(buf,L_AMON_INPUT),L_AMON_INPUT);
						PutAlpha(field, buf);
						movedata(FP_SEG(&point), FP_OFF(&point),
										FP_SEG( dest ), FP_OFF( dest ), sizeof(point));
						}
*/
				}
				else
				{
/*
					  if (!pointtotext(buf,&point))
					  {
						p=ispoint(rtrim(buf),&num_point,&var_type, &point_type, &num_panel, &num_net, network, panel);
						strcpy(buf,p);
						ralign(lalign(buf,L_AMON_INPUT),L_AMON_INPUT);
					  }
					  else
					  {
						movedata(FP_SEG(snothing), FP_OFF(snothing),FP_SEG(buf), FP_OFF(buf), 8);
					  }
					  movedata(FP_SEG(buf), FP_OFF(buf),FP_SEG(dest), FP_OFF(dest), 8);
*/
				}
}

DInt getpoint(Point_Net point, char *dest, int read)
{
  DByte num_point,var_type, point_type, num_panel;
  DInt num_net;
  DByte *p, buf[MAX_ALPHA+1];
  if(read)
  {
  /*
  					 GetAlpha(field, MAX_ALPHA, buf);
					 pgrid->GDGets(buf,pgrid->w_record,pgrid->t_fields[field-1],GetFldSize(field),Black,White);
					 if( (p=ispoint(rtrim(buf),&num_point,&var_type, &point_type, &num_panel, &num_net, network, panel)) || !strlen(rtrim(buf)) )
					 {
						if(strlen(rtrim(buf)))
						{
						 strcpy(buf,p);
						 point.panel = num_panel-1;point.number = num_point-1;point.point_type = point_type+1;
						 point.network = 0xFFFF;          //network;
						}
						else
						 point.panel = point.number = point.point_type = point.network = 0;

						ralign(lalign(buf,L_AMON_INPUT),L_AMON_INPUT);
						PutAlpha(field, buf);
						movedata(FP_SEG(&point), FP_OFF(&point),
									FP_SEG( dest ), FP_OFF( dest ), sizeof(point));
					 }
  */
  }
  else
  {
/*
					  if (!pointtotext(buf,&point))
					  {
						p=ispoint(rtrim(buf),&num_point,&var_type, &point_type, &num_panel, &num_net, network, panel);
						strcpy(buf,p);
						ralign(lalign(buf,L_AMON_INPUT),L_AMON_INPUT);
					  }
					  else
					  {
						movedata(FP_SEG(snothing), FP_OFF(snothing),FP_SEG(buf), FP_OFF(buf), 8);
					  }
					  movedata(FP_SEG(buf), FP_OFF(buf),FP_SEG(dest), FP_OFF(dest), 8);
*/
  }
}

//---------------------------------------------------------------------------
void __fastcall TGridForm::RefreshGrid(void)
{
 RS232Error result;
 DByte      *buf;
 DUint      l1,j;
 result = RS232_SUCCESS;
 if( refreshTimeFlag )
 {
  refreshTimeFlag = false;
  if( changeStatus )
  {
	if((local_panel && accesstype!=INDIVIDUAL) || (accesstype==INDIVIDUAL && panel==Station_NUM && network==NetworkAddress))
	{
//     memmove(ptr_panel->info[point_type].address+l*ptr_panel->info[point_type].str_size, ptr_block, max_points_bank*ptr_panel->info[point_type].str_size);
//	 save_prg_flag = 1;
	}
    else
    {
     if( (!local_panel && panel==station_num) || panel!=Station_NUM )
	  if (pointType!=STATION && pointType!=CSTATION && pointType != ARRAY1  && pointType != AMON_INPUTS && pointType != GRAPHIC_GRP )
//		 && pointType != CONNECT_COM && pointType != NET_SEL )
      {
		PointT point;
//		point.panel = Gpanel-1;
		point.point_type = pointType+1;
//		for(int i = 0; i<=w_drecord - w_urecord; i++)
		for(int i = 0; i<=recordCount; i++)
		{
		 if(refresh_table[i])
		 {
			point.number = i;
			j = countlength(READINDIVIDUALPOINT_T3000, *((DInt *)&point) );
			buf = (DByte *)&ptr_panel->info[pointType];
            align(0, pointType, buf, i+1, 1);
			t3000Request(READINDIVIDUALPOINT_T3000+100, *((DInt *)&point), buf, &j, panel, network, 0, TIMEOUT_NETCALL);
		 }
		}
	   memset(refresh_table, 0, 24);
      }
    }
  }

  buf  = new DByte [10000];
  if( serial_access )
    result=t3000Request(pointType+1, 0, buf, &l1, panel, network);
  if( result == RS232_SUCCESS && serial_access )
  {
   switch( pointType ) {
       case T3000_OUT:
         	alignOutputs(1,buf,ptr_panel->outputs,tbl_bank[pointType]);
         break;
       case T3000_IN:
	         alignInputs(1,buf,ptr_panel->inputs,tbl_bank[pointType]);
         break;
       case VAR:
	         alignVars(1,buf,ptr_panel->vars,tbl_bank[pointType]);
         break;
       case CON:
	         alignCons(1,buf,ptr_panel->controllers,tbl_bank[pointType]);
         break;
       case WR:
	         alignWeeklyroutines(1,buf,ptr_panel->weekly_routines,tbl_bank[pointType]);
         break;
       case AR:
	         alignAnnualroutines(1,buf,ptr_panel->annual_routines,tbl_bank[pointType]);
         break;
       case PRG:
	         alignPrograms(1,buf,ptr_panel->programs,tbl_bank[pointType]);
         break;
       case GRP:
	         alignScreens(1,buf,ptr_panel->control_groups,tbl_bank[pointType]);
         break;
       case AY:
	         alignArrays(1,buf,ptr_panel->arrays,tbl_bank[pointType]);
         break;
   }
  }
  delete [] buf;
  resetImage();
  if( result == RS232_SUCCESS && gridTable[objectIndex].grid )
  {
		int i,j,k;
        long recNo, colNo, leftCol;
        recNo = Table->RecNo;
//        colNo = col();
//        leftCol = leftCol();
        Table->Active = false;
        Table->DisableControls();
        Table->Active = true;
        Table->First();
        for(i=0;i<fieldCount;i++)
        {
          Table->Fields[i]->ReadOnly = false;
        }
		k=1;
		for (j = 0; j < recordCount; j++)
		{
            if( k>1 )
              Table->Next();
            Table->Edit();
            k++;
            updateRecTable(j);
        }
        DLong temp;
 	    temp = 1 ;
        for(i=0;i<fieldCount;i++)
        {
 	     if( readOnlyFields & (temp<<i) )
 		    Table->Fields[i]->ReadOnly = true;
        }
//        Table->First();
        Table->Refresh();
        Table->EnableControls();
        Table->First();
        Table->MoveBy(recNo-1);
//        DBGridT3000->Col = colNo;
//        DBGridT3000->LeftCol = leftCol;
//        Visible = True;
        refreshTime      = refreshTimeConst;
  }
  refreshTimeFlag = true;
 }
}

//---------------------------------------------------------------------------
void __fastcall TGridForm::updateRecTable(int recno)
{
            int i;
	        char *buf;
		    DInt n, l, control1;
            buf = new char [30];
			for (i = 1; i <= fieldCount; i++)
			{
			   switch(pointType)
			   {
				 case T3000_IN:
                 {
				   switch (i) {
					case IN_NUM:
                     itoa(recno+1,buf,10);
                     Table->Fields[IN_NUM-1]->AsString = buf;
					 break;
					case IN_FULL_LABEL:
                     Table->Fields[IN_FULL_LABEL-1]->AsString = ptr_panel->inputs[recno].description;
  					 break;
					case IN_AUTO_MAN:
                     Table->Fields[IN_AUTO_MAN-1]->AsString = auto_man[ptr_panel->inputs[recno].auto_manual];
					 break;
					case IN_VALUE:
					 if(ptr_panel->inputs[recno].digital_analog)
                     {
                      gcvt((float)ptr_panel->inputs[recno].value/1000.,10,buf);
                      Table->Fields[IN_VALUE-1]->AsString = buf;
                     }
					 else
					 {
						 n=ptr_panel->inputs[recno].range;
						 if( OFF_ON <= n && n <= custom_digital8 )
						 {
							n -= DIG1;
							control1=ptr_panel->inputs[recno].control;
							l =	dig_range_array[n].value % 2;
                            Table->Fields[IN_VALUE-1]->AsString = &dig_range_array[n].dunits[control1^l][0];
						 }
                         else
                         {
                            Table->Fields[IN_VALUE-1]->AsString = " ";
                         }
					 }
					 break;
					case IN_UNITS:
					 n = ptr_panel->inputs[recno].range;
					 if( n < DIG1 )
					 {
                       Table->Fields[IN_UNITS-1]->AsString = in_range_array[n].aunits;
					 }
					 else
					 {
                       Table->Fields[IN_UNITS-1]->AsString = " ";
					 }
					 break;
					case IN_RANGE:
					 n = ptr_panel->inputs[recno].range;
					 if( n > DIG1 ) //digital
					 {
						n -= DIG1;
                        Table->Fields[IN_RANGE-1]->AsString = dig_range_array[n].range_text;
					 }
					 else           //analog
                        Table->Fields[IN_RANGE-1]->AsString = in_range_array[n].range_text;
					 break;
					case IN_CALIBR:
					 l = ptr_panel->inputs[recno].digital_analog;
					 n = ptr_panel->inputs[recno].calibration;
					 if (l==ANALOG)
					 {
							if( ptr_panel->inputs[recno].calibration_sign )
								n = -n;
							if(ptr_panel->inputs[recno].calibration_increment)
							{
                                gcvt(n,10,buf);
							}
							else
                            {
                               gcvt((float)n/10.,10,buf);
                            }
                            Table->Fields[IN_CALIBR-1]->AsString = buf;
					 }
					 else
                            Table->Fields[IN_CALIBR-1]->AsString = " ";
					 break;
					case IN_FILTER:
					 l=ptr_panel->inputs[recno].filter;
                     Table->Fields[IN_FILTER-1]->AsString = filter[l];
					 break;
					case IN_DECOM:
                     Table->Fields[IN_DECOM-1]->AsString = "-";
					 break;
					case IN_LABEL:
                     Table->Fields[IN_LABEL-1]->AsString = ptr_panel->inputs[recno].label;
					 break;
				   }
				   break;
                 }
				 case T3000_OUT:
				 {
                   switch (i) {
					case OUT_NUM:
                     itoa(recno+1,buf,10);
                     Table->Fields[OUT_NUM-1]->AsString = buf;
					 break;
					case OUT_FULL_LABEL:
                     Table->Fields[OUT_FULL_LABEL-1]->AsString = ptr_panel->outputs[recno].description;
					 break;
					case OUT_AUTO_MAN:
                     Table->Fields[OUT_AUTO_MAN-1]->AsString = auto_man[ptr_panel->outputs[recno].auto_manual];
					 break;
					case OUT_VALUE:
					 if(ptr_panel->outputs[recno].digital_analog)
                     {
						gcvt((float)ptr_panel->outputs[recno].value/1000.,10,buf);
                        Table->Fields[OUT_VALUE-1]->AsString = buf;
                     }
					 else
					 {
						 n = ptr_panel->outputs[recno].range;
						 if( OFF_ON <= n && n <= custom_digital8 )
						 {
							n -= DIG1;
							control1=ptr_panel->outputs[recno].control;
							l =	dig_range_array[n].value % 2;
                            Table->Fields[OUT_VALUE-1]->AsString = dig_range_array[n].dunits[control1^l];
						 }
                         else
                            Table->Fields[OUT_VALUE-1]->AsString = " ";
					 }
					 break;
					case OUT_UNITS:
					 n = ptr_panel->outputs[recno].range;
					 if( n < DIG1 )
					 {
                        Table->Fields[OUT_UNITS-1]->AsString = out_range_array[n].aunits;
					 }
					 else
					 {
                        Table->Fields[OUT_UNITS-1]->AsString = " ";
					 }
					 break;
					case OUT_RANGE:
					 n = ptr_panel->outputs[recno].range;
					 if( n > DIG1 ) //digital
					 {
						n -= DIG1;
                        Table->Fields[OUT_RANGE-1]->AsString = dig_range_array[n].range_text;
					 }
					 else           //analog
                        Table->Fields[OUT_RANGE-1]->AsString = out_range_array[n].range_text;
					 break;
					case OUT_LOW:
					 l = ptr_panel->outputs[recno].digital_analog;
					 n = ptr_panel->outputs[recno].m_del_low;
					 if (l)
                        Table->Fields[OUT_LOW-1]->AsString = gcvt((float)(n)/10,10,buf);
					 else
                        Table->Fields[OUT_LOW-1]->AsString = " ";
					 break;
					case OUT_HIGH:
					 l = ptr_panel->outputs[recno].digital_analog;
					 n = ptr_panel->outputs[recno].s_del_high;
					 if (l)
                        Table->Fields[OUT_HIGH-1]->AsString = gcvt((float)(n)/10,10,buf);
					 else
                        Table->Fields[OUT_HIGH-1]->AsString = " ";
					 break;
					case OUT_DECOM:
                     Table->Fields[OUT_DECOM-1]->AsString = "-";
					 break;
					case OUT_LABEL:
                     Table->Fields[OUT_LABEL-1]->AsString = ptr_panel->outputs[recno].label;
					 break;
				   }
				   break;
				 }
                 case VAR:
				 {
                 	switch (i) {
					case VAR_NUM:
                     itoa(recno+1,buf,10);
                     Table->Fields[VAR_NUM-1]->AsString = buf;
					 break;
					case VAR_FULL_LABEL:
                     Table->Fields[VAR_FULL_LABEL-1]->AsString = ptr_panel->vars[recno].description;
					 break;
					case VAR_AUTO_MAN:
                     Table->Fields[VAR_AUTO_MAN-1]->AsString = auto_man[ptr_panel->vars[recno].auto_manual];
					 break;
					case VAR_VALUE:
					 n = ptr_panel->vars[recno].range;
					 if(ptr_panel->vars[recno].digital_analog)
					 {
							 if(n == time_unit)
							 {
								intervaltotext( ptr_panel->vars[recno].value/1000L , 0 , 0, buf);
				                Table->Fields[VAR_VALUE-1]->AsString = buf;
							 }
							 else
							 {
                             	gcvt((float)ptr_panel->vars[recno].value/1000.,10,buf);
				                Table->Fields[VAR_VALUE-1]->AsString = buf;
                             }
					 }
					 else
					 {
							DInt inv = 0;
							if( ON_OFF <= n && n <= HIGH_LOW ) inv++;
							if( OFF_ON <=  n && n <= custom_digital8 )
							{
								n -= DIG1;
								control1=ptr_panel->vars[recno].control;
			                    Table->Fields[VAR_VALUE-1]->AsString = &dig_range_array[n].dunits[inv^control1][0];
							}
                            else
			                    Table->Fields[VAR_VALUE-1]->AsString = " ";
					 }
					 break;
					case VAR_UNITS:
					 n = ptr_panel->vars[recno].range;
					 if(ptr_panel->vars[recno].digital_analog== ANALOG)
					 {
	                     Table->Fields[VAR_UNITS-1]->AsString = var_range_array[n].aunits;
					 }
					 else
					 {
						n -= DIG1;
	                    Table->Fields[VAR_UNITS-1]->AsString = dig_range_array[n].range_text;
					 }
					 break;
					case VAR_LABEL:
                     Table->Fields[VAR_LABEL-1]->AsString = ptr_panel->vars[recno].label;
					 break;
					}
					break;
				 }
                 case CON:
				 {
                 	switch (i) {
					case CON_NUM:
                     itoa(recno+1,buf,10);
                     Table->Fields[CON_NUM-1]->AsString = buf;
					 break;
					case CON_INPUT:
					 getpoint(ptr_panel->controllers[recno].input, buf, 0);
					 break;
					case CON_INUNITS:
					case CON_UNITS:
					 break;
					case CON_SETPOINT:
					 break;
					case CON_AUTO_MAN:
						 break;
					case CON_OUTPUT:
						 break;
					case CON_SETPOINTV:
						 break;
					case CON_ACTION:
						 break;
					case CON_PROP:
						 break;
					case CON_RESET:
						 break;
					case CON_RATE:
						 break;
					case CON_BIAS:
						 break;
				   }
				   break;
                 }
                 case WR:
                 {
					switch (i) {
					case WR_NUM:
                     itoa(recno+1,buf,10);
                     Table->Fields[WR_NUM-1]->AsString = buf;
					 break;
					case WR_FULL_LABEL:
                     Table->Fields[WR_FULL_LABEL-1]->AsString = ptr_panel->weekly_routines[recno].description;
					 break;
					case WR_AUTO_MAN:
                     Table->Fields[WR_AUTO_MAN-1]->AsString = auto_man[ptr_panel->weekly_routines[recno].auto_manual];
					 break;
					case WR_VALUE:
					 n = ptr_panel->weekly_routines[recno].value;
					 if( n )
					 {
	                    Table->Fields[WR_VALUE-1]->AsString = "ON";
					 }
                     else
	                    Table->Fields[WR_VALUE-1]->AsString = "OFF";
					 break;
					case WR_OVERRIDE1:
					 break;
					case WR_OR1VALUE:
					 n = ptr_panel->weekly_routines[recno].override_1_value;
					 if( n )
					 {
	                    Table->Fields[WR_OR1VALUE-1]->AsString = "ON";
					 }
                     else
	                    Table->Fields[WR_OR1VALUE-1]->AsString = "OFF";
					 break;
					case WR_OVERRIDE2:
					 break;
					case WR_OR2VALUE:
					 n = ptr_panel->weekly_routines[recno].override_2_value;
					 if( n )
					 {
	                    Table->Fields[WR_OR2VALUE-1]->AsString = "ON";
					 }
                     else
	                    Table->Fields[WR_OR2VALUE-1]->AsString = "OFF";
					 break;
					case WR_LABEL:
                     Table->Fields[AR_LABEL-1]->AsString = ptr_panel->weekly_routines[recno].label;
					 break;
					}
					break;
				 }
                 case AR:
                 {
					switch (i) {
					case AR_NUM:
                     itoa(recno+1,buf,10);
                     Table->Fields[AR_NUM-1]->AsString = buf;
					 break;
					case AR_FULL_LABEL:
                     Table->Fields[AR_FULL_LABEL-1]->AsString = ptr_panel->annual_routines[recno].description;
					 break;
					case AR_AUTO_MAN:
                     Table->Fields[AR_AUTO_MAN-1]->AsString = auto_man[ptr_panel->annual_routines[recno].auto_manual];
					 break;
					case AR_VALUE:
					 n = ptr_panel->annual_routines[recno].value;
					 if( n )
					 {
	                    Table->Fields[AR_VALUE-1]->AsString = "ON";
					 }
                     else
	                    Table->Fields[AR_VALUE-1]->AsString = "OFF";
					 break;
					case AR_LABEL:
                     Table->Fields[AR_LABEL-1]->AsString = ptr_panel->annual_routines[recno].label;
					 break;
					}
					break;
				 }
                 case GRP:
				 {
                   switch (i) {
					case GRP_NUM:
                     itoa(recno+1,buf,10);
                     Table->Fields[GRP_NUM-1]->AsString = buf;
					 break;
					case GRP_FULL_LABEL:
                     Table->Fields[GRP_FULL_LABEL-1]->AsString = ptr_panel->control_groups[recno].description;
					 break;
					case GRP_LABEL:
                     Table->Fields[GRP_LABEL-1]->AsString = ptr_panel->control_groups[recno].label;
					 break;
					case GRP_PICFILE:
                     Table->Fields[GRP_PICFILE-1]->AsString = ptr_panel->control_groups[recno].picture_file;
					 break;
					case GRP_MODE:
					 if (ptr_panel->control_groups[recno].mode==0)
					 {
	                    Table->Fields[GRP_MODE-1]->AsString = "Text";
					 }
                     else
	                    Table->Fields[GRP_MODE-1]->AsString = "Graphic";
					 break;
					case GRP_UPDATE:
                        itoa(ptr_panel->control_groups[recno].update_time,buf,10);
	                    Table->Fields[GRP_UPDATE-1]->AsString = buf;
						break;
					}
					break;
				 }
                 case PRG:
				 {
                   switch (i) {
					case PRG_NUM:
                     itoa(recno+1,buf,10);
                     Table->Fields[GRP_NUM-1]->AsString = buf;
					 break;
					case PRG_FULL_LABEL:
                     Table->Fields[GRP_FULL_LABEL-1]->AsString = ptr_panel->programs[recno].description;
					 break;
					case PRG_STATUS:
					 n = ptr_panel->programs[recno].on_off;
					 if( n )
					 {
	                    Table->Fields[PRG_STATUS-1]->AsString = "ON";
					 }
                     else
	                    Table->Fields[PRG_STATUS-1]->AsString = "OFF";
					 break;
					case PRG_AUTO_MAN:
                     Table->Fields[AR_AUTO_MAN-1]->AsString = auto_man[ptr_panel->programs[recno].auto_manual];
					 break;
					case PRG_SIZE:
                        itoa(ptr_panel->programs[recno].bytes,buf,10);
	                    Table->Fields[PRG_SIZE-1]->AsString = buf;
						break;
		            case PRG_EXIT:
					 if (ptr_panel->programs[recno].errcode)
  	                    Table->Fields[PRG_STATUS-1]->AsString = "TIMEOUT";
                     else
  	                    Table->Fields[PRG_STATUS-1]->AsString = "Normal";
                     break;
					case PRG_LABEL:
                     Table->Fields[PRG_LABEL-1]->AsString = ptr_panel->programs[recno].label;
					 break;
                    }
				 }
                 case SYSTEM_LIST:
				 {
/*
                 	switch (i) {
					case DIAL_NUM:
                     itoa(recno+1,buf,10);
                     Table->Fields[DIAL_NUM-1]->AsString = buf;
					 break;
					case DIAL_SYSTEM_NAME:
                     Table->Fields[DIAL_SYSTEM_NAME-1]->AsString = ptr_panel->system_list[recno].system_name;
					 break;
					case DIAL_PHONE_NUMBER:
                     Table->Fields[DIAL_PHONE_NUMBER-1]->AsString = ptr_panel->system_list[recno].phone_number;
					 break;
					case DIAL_DIRECTORY:
                     Table->Fields[DIAL_PHONE_NUMBER-1]->AsString = ptr_panel->system_list[recno].directory;
					 break;
					}
*/
					break;
                 }
                 case NETSTAT:
				 {
                 	switch (i) {
					case 1:
                     itoa(recno+1,buf,10);
                     Table->Fields[0]->AsString = buf;
					 break;
/*
					case DIAL_SYSTEM_NAME:
                     Table->Fields[DIAL_SYSTEM_NAME-1]->AsString = ptr_panel->system_list[recno].system_name;
					 break;
					case DIAL_PHONE_NUMBER:
                     Table->Fields[DIAL_PHONE_NUMBER-1]->AsString = ptr_panel->system_list[recno].phone_number;
					 break;
					case DIAL_DIRECTORY:
                     Table->Fields[DIAL_PHONE_NUMBER-1]->AsString = ptr_panel->system_list[recno].directory;
					 break;
*/
					}
					break;
                 }

			   }
			}
            delete [] buf;
}

//---------------------------------------------------------------------------
void __fastcall TGridForm::setUpData(void)
{
//        TTable      *table;
		DInt n, l, control1;
        int recCountTable, i,j,k;

        DBGridT3000->DataSource = dataSource;

        Table->Active = false;
        Table->DisableControls();
        Table->Active = true;
        if (emptyTable)
	        Table->EmptyTable();
        Table->First();
        fieldCount  = Table->FieldCount;
        recCountTable = Table->RecordCount;

        for(i=0;i<fieldCount;i++)
        {
          Table->Fields[i]->ReadOnly = false;
        }
		if (fieldCount)
   		{
		  k=1;
		  for (j = 0; j < recordCount; j++)
		  {
			if(k > recCountTable)
			{
             Table->Append();
            }
            else
            {
              if( k>1 )
               Table->Next();
              Table->Edit();
            }
            k++;
            updateRecTable(j);
		  }

          DLong temp;
 	      temp = 1 ;
          for(i=0;i<fieldCount;i++)
          {
 	       if( readOnlyFields & (temp<<i) )
 		    Table->Fields[i]->ReadOnly = true;
          }
          DBGridT3000->Width = Width - 24;
//          Bevel1->Width = Width - 22;
          DBGridT3000->Height = Height - 56;

	      Table->First();
          Table->Refresh();
          Table->EnableControls();
          Visible = True;

        }
}
//---------------------------------------------------------------------------
void ProcEvRefreshGrid(DUint point_type)
{
}
//---------------------------------------------------------------------------
//Added By Paul Mao for GRID on 04/08/97:
void __fastcall TGridForm::CutClick(TObject *Sender)
{
/*
    AnsiString test = "test";
    AnsiString test2;
    test2 = test;
    char test3[100];
    strcpy(test3, test.c_str());
	strcpy(Clip->buf1, test.c_str());
	Clip -> buf = test;
    Clip -> buf = DBGridT3000 -> SelectedField -> AsString;
    ClipBuf = DBGridT3000 -> SelectedField -> AsString;
*/
  if (DBGridT3000 -> SelectedField)
  {
  	if (!(DBGridT3000 -> SelectedField -> ReadOnly))
  	{
      DBGridT3000 -> SelectedField -> GetData(ClipBuffer);
      ClipBufferEmpty = 0;
      DBGridT3000 -> DataSource -> Edit();
      char *tempbuf = "";
      DBGridT3000 -> SelectedField -> SetData(tempbuf);
    }
    else
    {
      MessageBeep( MB_ICONEXCLAMATION );
      StatusBarMessage(SBText, "It is a read-only field. Try copy instead!");
    }
  }
  else
  {
    MessageBeep( MB_ICONEXCLAMATION );
    StatusBarMessage(SBText, "There is no selected field!");
  }
/*
  We delete the following statements for the reason that :
  Let the former Cut/Copy action still work.
----------------------
  else
  {
    ClipBufferEmpty = 1;
  }
----------------------
*/
}
//---------------------------------------------------------------------------
/*
//Added by Paul Mao for "edit by list selection"(GRID) on 4/8/97:
void __fastcall TGridForm::editSetup(DInt point_type)
{
  DBGridT3000 -> DataSource = dataSource;
  DBGridT3000 -> Columns -> RebuildColumns();

  switch( point_type ) {
    case T3000_OUT:
        DBGridT3000 -> Columns -> Items[2] -> ButtonStyle  = cbsAuto;
        DBGridT3000 -> Columns -> Items[2] -> PickList -> Clear();
        DBGridT3000 -> Columns -> Items[2] -> PickList -> Clear();
        DBGridT3000 -> Columns -> Items[2] -> DropDownRows = 2;
        DBGridT3000 -> Columns -> Items[2] -> PickList -> CommaText = "Auto Manual";
//        DBGridT3000 -> Columns -> Items[2] -> PickList -> Add("Auto");
//        DBGridT3000 -> Columns -> Items[2] -> PickList -> Add("Manual");
    	break;
    case T3000_IN:
        DBGridT3000 -> Columns -> Items[2] -> ButtonStyle  = cbsAuto;
        DBGridT3000 -> Columns -> Items[2] -> PickList -> Clear();
        DBGridT3000 -> Columns -> Items[2] -> PickList -> CommaText = "Auto Manual";
//        DBGridT3000 -> Columns -> Items[2] -> PickList -> Add("Auto");
//        DBGridT3000 -> Columns -> Items[2] -> PickList -> Add("Manual");
    	break;
  }
}
*/
//---------------------------------------------------------------------------

//Added by Paul Mao for "edit by list selection"(GRID) on 4/8/97:
void __fastcall TGridForm::editSetup(int num, IndexedPickList *IndexedPickLists)
{
  DBGridT3000 -> DataSource = dataSource;
  DBGridT3000 -> Columns -> RebuildColumns();

  for (int i=0; i<num; i++)
  {
    DBGridT3000 -> Columns -> Items[IndexedPickLists[i].Index] -> ButtonStyle  = cbsAuto;
    DBGridT3000 -> Columns -> Items[IndexedPickLists[i].Index] -> PickList -> Clear();
    if (IndexedPickLists[i].Rows != 0)
    	DBGridT3000 -> Columns -> Items[IndexedPickLists[i].Index] -> DropDownRows = IndexedPickLists[i].Rows;
    DBGridT3000 -> Columns -> Items[IndexedPickLists[i].Index] -> PickList -> CommaText = IndexedPickLists[i].PickListCommaText;
  }
}
//---------------------------------------------------------------------------
//Added By Paul Mao for GRID on 04/08/97:
void __fastcall TGridForm::CopyClick(TObject *Sender)
{
  if (DBGridT3000 -> SelectedField)
  {
    DBGridT3000 -> SelectedField -> GetData(ClipBuffer);
    ClipBufferEmpty = 0;
  }
}
//---------------------------------------------------------------------------
//Added By Paul Mao for GRID on 04/08/97:
void __fastcall TGridForm::PasteClick(TObject *Sender)
{
  if (ClipBufferEmpty)
  {
    MessageBeep( MB_ICONEXCLAMATION );
    StatusBarMessage(SBText, "Clipboard is empty!");
  }
  else
  {
    if (!(DBGridT3000 -> SelectedField -> ReadOnly))
    {
      DBGridT3000 -> DataSource -> Edit();
      DBGridT3000 -> SelectedField -> SetData(ClipBuffer);
    }
    else
    {
      MessageBeep( MB_ICONEXCLAMATION );
      StatusBarMessage(SBText, "It is a read-only field.");
    }
  }
}
//---------------------------------------------------------------------------
//Added By Paul Mao for GRID on 28/07/97;
void __fastcall TGridForm::ColorClick(TObject *Sender)
{
  if(ColorDialog -> Execute())
  {
  	DBGridT3000 -> Color = ColorDialog -> Color;
  }
}
//---------------------------------------------------------------------------
//Added By Paul Mao for GRID on 28/07/97;
void __fastcall TGridForm::FontClick(TObject *Sender)
{
  if(FontDialog -> Execute())
  {
  	DBGridT3000 -> Font = FontDialog -> Font;
  }
}
//---------------------------------------------------------------------------
//Added By Paul Mao for GRID on 31/07/97:
void __fastcall TGridForm::CloseClick(TObject *Sender)
{
  refreshTimeFlag = false;
  Table->Active = false;
  delete(gridTable[objectIndex].grid);
  gridTable[objectIndex].grid = NULL;
//  refreshTimeFlag = false;
//  gridTable[objectIndex].grid = NULL;
//  Table->Active = false;
//  Close();
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::OnColExit(TObject *Sender)
{
  if( editField )
  {
   changeStatus = 1;
   editField    = 0;
  }
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::OnKeyPress(TObject *Sender, char &Key)
{
 if(Key!=13)
	  editField=1;
}
//---------------------------------------------------------------------------

void __fastcall TGridForm::OnColEnter(TObject *Sender)
{
  editField    = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::dataChange(void)
{
  if( editField )
  {
   changeStatus = 1;
   editField    = 0;
  }
}

