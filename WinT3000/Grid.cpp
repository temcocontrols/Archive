//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
#include <vcl\sysdefs.h>
#include <vcl/dbgrids.hpp>
#include <stdlib.h>

#include "Grid.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#include "define.h"
#include "baseclas.h"
#include "grid.h"
#include "calend.h"
#include "aio.h"
#include "recdef.h"
#include "ptpdll.h"
#include "range.h"
#include "RefreshIn.h"
#include "messagebox.h"
#include "Group.h"
#include "program.h"

#pragma link "Grids"
//#pragma link "t3000reg"
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

#define MAX_NEST_GRP 10
DInt nested_grp[MAX_NEST_GRP];
extern int ind_nested_grp;

int maxgridTable = MAXGRIDTABLE;
GRIDTABLE gridTable[MAXGRIDTABLE];   // tracks the open windows

void openMessage(char *ptr,int Top,int Left,int Width,int Height);
int checkdecimal(char *buf, int dec);

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
  lockedRow        = -1;
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
   TCalendarForm *CalendarForm;
   CalendarForm = new TCalendarForm( Application, (char *)ptr_panel->ar_dates[currentRec-1]);
   if( CalendarForm->ShowModal()==mrOk )
   {
     memcpy(ptr_panel->ar_dates[currentRec-1], CalendarForm->dayOn, 46);
   }
   delete CalendarForm;
  }
  if ( pointType == GRP )
  {
   TGroupForm *GroupForm;
   Point_Net retpoint;
   DInt ind;
   ind = currentRec-1;
   while(1)
   {
    GroupForm = new TGroupForm( Application, ptr_panel->control_groups[ ind ], ind, (DByte)station_num);
    GroupForm->ShowModal();
    retpoint = GroupForm->retpoint;
    delete GroupForm;

    if(retpoint.point_type-1 != GRP )
	 if(ind_nested_grp)
	 {
		ind_nested_grp--;
        ind = nested_grp[ind_nested_grp];
		continue;
	 }
    if(retpoint.point_type-1 == GRP )
    {
	 if(ind_nested_grp < MAX_NEST_GRP-1)
	 {
		nested_grp[ind_nested_grp++]=ind; //current_opened_grp;
	 }
     ind = retpoint.number;
     continue;
    }
    break;
   }
  }
  if(pointType == PRG)
  {
    TProgramForm *ProgramForm;
    ProgramForm = new TProgramForm( Application,ptr_panel->programs[ currentRec-1 ], currentRec-1);
    ProgramForm->ShowModal();
    delete ProgramForm;
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
void __fastcall TGridForm::SendIndivPoint(DInt pointnumber)
{
 DByte     *buf;
 DUint      l1,j;
 PointT point;
 point.point_type = pointType+1;
 point.number = pointnumber-1;
 if(local_panel)
 {
 }
 else
 {
  buf = new char[3000];
  j = countlength(READINDIVIDUALPOINT_T3000, *((DInt *)&point) );
  align(0, pointType, buf, pointnumber, 1);
  t3000Request(READINDIVIDUALPOINT_T3000+100, *((DInt *)&point), buf, &j, panel, network, 0, TIMEOUT_NETCALL);
  delete buf;
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
  if( editField ) return;
  refreshTimeFlag = false;
  buf  = new DByte [10000];
  if( changeStatus )
  {
    changeStatus = 0;
	if((local_panel && accesstype!=INDIVIDUAL) || (accesstype==INDIVIDUAL && panel==Station_NUM && network==NetworkAddress))
	{
//     memmove(ptr_panel->info[point_type].address+l*ptr_panel->info[point_type].str_size, ptr_block, max_points_bank*ptr_panel->info[point_type].str_size);
//	 save_prg_flag = 1;
     memset(refresh_table, 0, 128);
	}
    else
    {
     if( (!local_panel && panel==station_num) || panel!=Station_NUM )
	  if (pointType!=STATION && pointType!=CSTATION && pointType != ARRAY1  && pointType != AMON_INPUTS && pointType != GRAPHIC_GRP )
//		 && pointType != CONNECT_COM && pointType != NET_SEL )
      {
		PointT point;
		point.point_type = pointType+1;
//		for(int i = 0; i<=w_drecord - w_urecord; i++)
		for(int i = 0; i<=recordCount; i++)
		{
		 if(refresh_table[i])
		 {
          if ( editField & currentRec==i+1 )
          {
            changeStatus = 1;
          }
          else
		  {
            lockedRow = i+1;
          	point.number = i;
			j = countlength(READINDIVIDUALPOINT_T3000, *((DInt *)&point) );
            align(0, pointType, buf, i+1, 1);
			t3000Request(READINDIVIDUALPOINT_T3000+100, *((DInt *)&point), buf, &j, panel, network, 0, TIMEOUT_NETCALL);
            refresh_table[i]=0;
            lockedRow = -1;
		  }
         }
		}
      }
    }
  }
  if( !local_panel )
  {
     result=t3000Request(pointType+1, 0, buf, &l1, panel, network);
     resetImage();
  }

  if( editField || changeStatus )
  {
   delete [] buf;
   refreshTimeFlag = true;
   return;
  }

  if( result == RS232_SUCCESS && !local_panel )
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
  if( result == RS232_SUCCESS && gridTable[objectIndex].grid )
  {
		int i,j,k;
        long recNo, colNo, leftCol, rowNo, topRow;
        recNo = Table->RecNo;
        colNo = DBGridT3000->col();
        leftCol = DBGridT3000->leftCol();
        rowNo = DBGridT3000->row();
        topRow = DBGridT3000->topRow();
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
        Table->First();
        Table->MoveBy(recNo-1);
//        DBGridT3000->row(rowNo);
//        DBGridT3000->topRow(topRow);
//        Table->Refresh();
        Table->EnableControls();
        DBGridT3000->col(colNo);
        DBGridT3000->leftCol(leftCol);
//        Visible = True;
        DBGridT3000->Height = Height - 56;
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
/*
                     if( !ptr_panel->inputs[recno].auto_manual )
 	                   readOnlyFields |= (1L<<(IN_VALUE-1));
                     else
 	                   readOnlyFields &= ~(1L<<(IN_VALUE-1));
*/
					 break;
					case IN_VALUE:
					 if(ptr_panel->inputs[recno].digital_analog)
                     {
                      gcvt((float)ptr_panel->inputs[recno].value/1000.,10,buf);
                      checkdecimal(buf, 2);
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
                            Table->Fields[IN_VALUE-1]->AsString = dig_range_array[n].dunits[control1^l];
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
                            checkdecimal(buf, 1);
                            Table->Fields[IN_CALIBR-1]->AsString = buf;
					 }
					 else
                            Table->Fields[IN_CALIBR-1]->AsString = " ";
					 break;
					case IN_FILTER:
					 if( !ptr_panel->inputs[recno].digital_analog )
	                     Table->Fields[IN_FILTER-1]->AsString = "";
                     else
                     {
						 l=ptr_panel->inputs[recno].filter;
    	                 Table->Fields[IN_FILTER-1]->AsString = filter[l];
                     }
					 break;
					case IN_DECOM:
		             l=ptr_panel->inputs[recno].decom;
					 if (l==0)
	                     Table->Fields[IN_DECOM-1]->AsString = "-";
					 else
	                     Table->Fields[IN_DECOM-1]->AsString = "Y";
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
                        checkdecimal(buf, 2);
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
                     {
						gcvt((float)(n)/10,10,buf);
                        checkdecimal(buf, 1);
                        Table->Fields[OUT_LOW-1]->AsString = buf;
                     }
					 else
                        Table->Fields[OUT_LOW-1]->AsString = " ";
					 break;
					case OUT_HIGH:
					 l = ptr_panel->outputs[recno].digital_analog;
					 n = ptr_panel->outputs[recno].s_del_high;
					 if (l)
					 {
                     	gcvt((float)(n)/10,10,buf);
                        checkdecimal(buf, 1);
                        Table->Fields[OUT_HIGH-1]->AsString = buf;
                     }   
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
/*

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
*/
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

//Updated by Paul Mao on 12/8/97 for the write back of data:
	  writeback();

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

//Updated by Paul Mao on 12/8/97 for the write back of data:
	  writeback();

    }
    else
    {
      MessageBeep( MB_ICONEXCLAMATION );
      StatusBarMessage(SBText, "It is a read-only field.");
    }
  }
}
//---------------------------------------------------------------------------
//Added By Paul Mao for GRID on 11/08/97;
//---------------------------------------------------------------------------
void __fastcall TGridForm::DeleteClick(TObject *Sender)
{
  if (DBGridT3000 -> SelectedField)
  {
  	if (!(DBGridT3000 -> SelectedField -> ReadOnly))
  	{
      DBGridT3000 -> DataSource -> Edit();
      char *tempbuf = "";
      DBGridT3000 -> SelectedField -> SetData(tempbuf);

//Updated by Paul Mao on 12/8/97 for the write back of data:
	  writeback();

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
  RefreshGrid();
  refreshTimeFlag = false;
  Table->Active = false;
  delete(gridTable[objectIndex].grid);
  gridTable[objectIndex].grid = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::OnColExit(TObject *Sender)
{
  if( editField )
    switch( pointType ) {
       case T3000_OUT:
         keyPressOut(currentRec, currentCol, 0, 1);
         break;
       case T3000_IN:
         keyPressIn(currentRec, currentCol, 0, 1);
         break;
       case VAR:
         keyPressVar(currentRec, currentCol, 0, 1);
         break;
       case CON:
         keyPressCon(currentRec, currentCol, 0, 1);
         break;
       case WR:
         keyPressWr(currentRec, currentCol, 0, 1);
         break;
       case AR:
         keyPressAr(currentRec, currentCol, 0, 1);
         break;
       case PRG:
         keyPressPrg(currentRec, currentCol, 0, 1);
         break;
       case GRP:
         keyPressGrp(currentRec, currentCol, 0, 1);
         break;
       case AY:
         keyPressAy(currentRec, currentCol, 0, 1);
         break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::OnKeyPress(TObject *Sender, char &Key)
{
   switch( pointType ) {
       case T3000_OUT:
         keyPressOut( Table->RecNo, DBGridT3000->col(), Key);
         break;
       case T3000_IN:
         keyPressIn( Table->RecNo, DBGridT3000->col(), Key);
         break;
       case VAR:
         keyPressVar( Table->RecNo, DBGridT3000->col(), Key);
         break;
       case CON:
         keyPressCon( Table->RecNo, DBGridT3000->col(), Key);
         break;
       case WR:
         keyPressWr( Table->RecNo, DBGridT3000->col(), Key);
         break;
       case AR:
         keyPressAr( Table->RecNo, DBGridT3000->col(), Key);
         break;
       case PRG:
         keyPressPrg( Table->RecNo, DBGridT3000->col(), Key);
         break;
       case GRP:
         keyPressGrp( Table->RecNo, DBGridT3000->col(), Key);
         break;
       case AY:
         keyPressAy( Table->RecNo, DBGridT3000->col(), Key);
         break;
   }
}
//---------------------------------------------------------------------------

void __fastcall TGridForm::OnColEnter(TObject *Sender)
{
  currentCol = DBGridT3000->col();
  editField    = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::checkreadonlyIn( long row )
{
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::checkreadonlyOut( long row )
{
/*
//Added by Paul Mao on 11/08/97 for "OUTPUTS":
  if( !ptr_panel->outputs[row-1].auto_manual )
  {
         DBGridT3000->Columns->Items[IN_VALUE-1]->Field->ReadOnly = true;
         readOnlyFields |= (1L<<(IN_VALUE-1));
  }
  else
  {
         DBGridT3000->Columns->Items[IN_VALUE-1]->Field->ReadOnly = false;
         readOnlyFields &= ~(1L<<(IN_VALUE-1));
  }

*/
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::checkreadonlyVar( long row )
{
}
void __fastcall TGridForm::checkreadonlyCon( long row )
{
}
void __fastcall TGridForm::checkreadonlyWr( long row )
{
}
void __fastcall TGridForm::checkreadonlyAr( long row )
{
}
void __fastcall TGridForm::checkreadonlyPrg( long row )
{
}
void __fastcall TGridForm::checkreadonlyGrp( long row )
{
}
void __fastcall TGridForm::checkreadonlyAY( long row )
{
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::dataChange(TField *Field)
{
  currentRec = Table->RecNo;
/*
   switch( pointType ) {
       case T3000_OUT:
         checkreadonlyOut( Table->RecNo );
         break;
       case T3000_IN:
         checkreadonlyIn( Table->RecNo );
         break;
       case VAR:
         checkreadonlyVar( Table->RecNo );
         break;
       case CON:
         checkreadonlyCon( Table->RecNo );
         break;
       case WR:
         checkreadonlyWr( Table->RecNo );
         break;
       case AR:
         checkreadonlyAr( Table->RecNo );
         break;
       case PRG:
         checkreadonlyPrg( Table->RecNo );
         break;
       case GRP:
         checkreadonlyGrp( Table->RecNo );
         break;
       case AY:
         checkreadonlyAY( Table->RecNo );
         break;
   }
*/
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::onUpdate()
{
  if( editField )
  {
    if( currentRec == Table->RecNo )

//Updated by Paul Mao on 11/08/97 :
//	     keyPressIn(currentRec, currentCol, 0, 1);
	switch( pointType ) {
       case T3000_OUT:
	     keyPressOut(currentRec, currentCol, 0, 1);
         break;
       case T3000_IN:
	     keyPressIn(currentRec, currentCol, 0, 1);
         break;
       case VAR:
	     keyPressVar(currentRec, currentCol, 0, 1);
         break;
       case CON:
	     keyPressCon(currentRec, currentCol, 0, 1);
         break;
       case WR:
	     keyPressWr(currentRec, currentCol, 0, 1);
         break;
       case AR:
	     keyPressAr(currentRec, currentCol, 0, 1);
         break;
       case PRG:
	     keyPressPrg(currentRec, currentCol, 0, 1);
         break;
       case GRP:
	     keyPressGrp(currentRec, currentCol, 0, 1);
         break;
       case AY:
	     keyPressAy(currentRec, currentCol, 0, 1);
         break;
    }
  }
}
//---------------------------------------------------------------------------
int  __fastcall TGridForm::checkFinishedEdit(char key, int exitfield)
{
 int exit;
 exit = 0;
// check if edited was finished
         if(exitfield==1)
         {
            if( editField==1 )
            {
              exit = 1;
            }
            else
            {
             editField = 0;
             return 0;
            }
         }
         else
         {
          if( editField==0 )
          {
           if( key==13 )
            editField = 2;
           else
            editField = 1;
          }
          else
          {
           if(key==13)
           {
             if( editField==2 )
             {
	          editField = 0;
              return 0;
             }
             else
             {
              exit = 1;
             }
           }
           else
            editField = 1;
          }
         }
// end check if edited was finished
  return exit;
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::keyPressIn(long row, long field, char key, int exitfield)
{
   DLong value;
   DByte l;
   char buf[80];
   if(key==27)
   {
     if( editField )
     {
       editField==0;
     }
     return;
   }  
   if( currentRec == lockedRow )
   {
     DBGridT3000->EditorMode(true);
     editField==0;
     return;
   }
   switch (field) {
	case IN_NUM:
         if(key==13)
              DBGridT3000->EditorMode(true);
         else
             DBGridT3000->EditorMode(false);
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
//	     StatusBarMessage(SBText, "It is a read-only field!");
		 break;
	case IN_FULL_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->inputs[row-1].description, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
          Table->Post();
         }
         else
          return;
		 break;
	case IN_AUTO_MAN:
         if(key==13)
         {
          if( !ptr_panel->inputs[row-1].range )
          {
  	       MessageBeep( MB_ICONEXCLAMATION );
           openMessage("        Undefined range!       ",Top,Left,Width,Height);
           DBGridT3000->EditorMode(true);
           break;
          }
          editField = 1;
          if( ptr_panel->inputs[row-1].auto_manual )
          {
           ptr_panel->inputs[row-1].auto_manual = 0;
          }
          else
          {
           ptr_panel->inputs[row-1].auto_manual = 1;
          }
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
				         auto_man[ptr_panel->inputs[row-1].auto_manual];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
/*
		  if( !ptr_panel->inputs[row-1].auto_manual )
          {
	          DBGridT3000->Columns->Items[IN_VALUE-1]->Field->ReadOnly = true;
              readOnlyFields |= (1L<<(IN_VALUE-1));
          }
          else
          {
	          DBGridT3000->Columns->Items[IN_VALUE-1]->Field->ReadOnly = false;
              readOnlyFields &= ~(1L<<(IN_VALUE-1));
          }
*/
          editField = 0;
          changeStatus = 1;
          Table->Post();
          SendIndivPoint(currentRec);
          if(exitfield==2)
	          DBGridT3000->EditorMode(false);
          else
	          DBGridT3000->EditorMode(true);
         }
         else
          DBGridT3000->EditorMode(false);
		 break;
	case IN_VALUE:
		 DLong *pf;
		 DInt range;

		 if( !ptr_panel->inputs[row-1].auto_manual )
         {
//Warning messages:
  	       MessageBeep( MB_ICONEXCLAMATION );
//		   MessageBox(GetFocus(), " The Value can be changed only in Manual mode! ", "", MB_OK );
           openMessage(" The Value can be changed only in Manual mode! ",Top,Left,Width,Height);
           if(key==13)
              DBGridT3000->EditorMode(true);
           else
             DBGridT3000->EditorMode(false);
           break;
         }

		 range = ptr_panel->inputs[row-1].range;
		 pf = &ptr_panel->inputs[row-1].value;

		 if( ptr_panel->inputs[row-1].digital_analog )
		 {                   // analog
          if( checkFinishedEdit(key, exitfield) )
          {
           strcpy(buf, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
		   *pf=(long)(atof(buf)*1000);
           checkdecimal(buf, 2);
           dataSource->Edit();
           DBGridT3000->Columns->Items[field-1]->Field->AsString = buf;
           editField = 0;
           changeStatus = 1;
           SendIndivPoint(currentRec);
          }
          else
           return;
		  break;
		 }
		 else
		 {
          if(key==13)
          {
           editField = 1;
		   ptr_panel->inputs[row-1].control = !ptr_panel->inputs[row-1].control;
           range -= DIG1;
		   strcpy(buf, dig_range_array[range].dunits[ptr_panel->inputs[row-1].control]);
		   *pf=ptr_panel->inputs[row-1].control?1000L:0;
           dataSource->Edit();
//           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
           DBGridT3000->Columns->Items[field-1]->Field->AsString = buf;
//           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
           changeStatus = 1;
           editField = 0;
           SendIndivPoint(currentRec);
          }
          if(exitfield==2)
	          DBGridT3000->EditorMode(false);
          else
	          DBGridT3000->EditorMode(true);
		 }
		 break;
    case IN_UNITS:
         if(key==13)
              DBGridT3000->EditorMode(true);
         else
              DBGridT3000->EditorMode(false);
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
//	     StatusBarMessage(SBText, "It is a read-only field!");
    	 break;
	case IN_RANGE:
         if(key==13)
         {
           editField = 1;
           TRangeForm *RangeForm;
           RangeForm = new TRangeForm( Application );

//Added by Paul Mao on 11/8/97:
//Initialize the "AnalogList" according to the current point type:
		   AnsiString temp;
           int n;
		   RangeForm -> AnalogList -> Clear();
           for (int i=0; i<=24; i++)
           {
             temp = "  " + AnsiString(in_range_array[i].range_text) + " " +
                    AnsiString(in_range_array[i].aunits);
	         RangeForm -> AnalogList -> Items -> Add(temp);
           }

		   if( RangeForm->ShowModal()==mrOk )
           {
           	 if(RangeForm->StandardList->ItemIndex!=-1 || RangeForm->CustomList->ItemIndex!=-1)
             {
//Invert digital input ranges display:
           	   if(RangeForm->StandardList->ItemIndex!=-1)
               {
                if(RangeForm->CheckBox->Checked)
                {
                 n = RangeForm->StandardList->ItemIndex + 1 + 11;
                }
                else
                {
                 n = RangeForm->StandardList->ItemIndex + 1;
                }
               }
               else
               {
                n = RangeForm->CustomList->ItemIndex+23;
               }
               ptr_panel->inputs[row-1].range = n+DIG1;
               ptr_panel->inputs[row-1].digital_analog = 0;
               ptr_panel->inputs[row-1].auto_manual = 0;
			   ptr_panel->inputs[row-1].filter=6;
			   ptr_panel->inputs[row-1].calibration_sign=0;
			   ptr_panel->inputs[row-1].calibration=0;
               ptr_panel->inputs[row-1].calibration_increment=0;
//Update the screen display(include two fields : "RANGE" and "UNITS"):
	           dataSource->Edit();
    	       DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
        	   DBGridT3000->Columns->Items[field-1]->Field->AsString = dig_range_array[n].range_text;
               DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
    	       DBGridT3000->Columns->Items[IN_UNITS-1]->Field->ReadOnly = false;
        	   DBGridT3000->Columns->Items[IN_UNITS-1]->Field->AsString = "";
               DBGridT3000->Columns->Items[IN_UNITS-1]->Field->ReadOnly = true;
			   ptr_panel->inputs[row-1].control = (long)(dig_range_array[n].value)%2;
		       ptr_panel->inputs[row-1].value = ptr_panel->inputs[row-1].control?1000L:0;
		       strcpy(buf, dig_range_array[n].dunits[ptr_panel->inputs[row-1].control]);
//               DBGridT3000->Columns->Items[IN_VALUE-1]->Field->ReadOnly = false;
               DBGridT3000->Columns->Items[IN_VALUE-1]->Field->AsString = buf;
//               DBGridT3000->Columns->Items[IN_VALUE-1]->Field->ReadOnly = true;
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->ReadOnly = false;
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->AsString = auto_man[0];
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->ReadOnly = true;
               DBGridT3000->Columns->Items[IN_FILTER-1]->Field->ReadOnly = false;
               DBGridT3000->Columns->Items[IN_FILTER-1]->Field->AsString = "";
               DBGridT3000->Columns->Items[IN_FILTER-1]->Field->ReadOnly = true;
               changeStatus = 1;
             }
           	 if(RangeForm -> AnalogList -> ItemIndex != -1)
             {
               n = RangeForm->AnalogList->ItemIndex;
               ptr_panel->inputs[row-1].range = n;
               ptr_panel->inputs[row-1].digital_analog = 1;
               ptr_panel->inputs[row-1].auto_manual = 0;
			   ptr_panel->inputs[row-1].filter=6;
			   ptr_panel->inputs[row-1].calibration_sign=0;
			   ptr_panel->inputs[row-1].calibration=0;
               ptr_panel->inputs[row-1].calibration_increment=0;
               changeStatus = 1;
//Update the screen display(include two fields : "RANGE" and "UNITS"):
	           dataSource->Edit();
    	       DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
        	   DBGridT3000->Columns->Items[field-1]->Field->AsString = in_range_array[n].range_text;
               DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
    	       DBGridT3000->Columns->Items[IN_UNITS-1]->Field->ReadOnly = false;
        	   DBGridT3000->Columns->Items[IN_UNITS-1]->Field->AsString = in_range_array[n].aunits;
               DBGridT3000->Columns->Items[IN_UNITS-1]->Field->ReadOnly = true;
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->ReadOnly = false;
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->AsString = auto_man[0];
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->ReadOnly = true;
			   ptr_panel->inputs[row-1].control = 0;
		       ptr_panel->inputs[row-1].value = 0;
               DBGridT3000->Columns->Items[IN_VALUE-1]->Field->AsString = "0.00";
               if(n == N0_2_32counts || n == N0_3000FPM_0_5V || n == P0_255p_min)
				  ptr_panel->inputs[row-1].calibration_increment=1;
			   if( Y3K_40_150DegC <= n && n <= A10K_60_200DegF && !(n%2) )
				  ptr_panel->inputs[row-1].calibration_increment=1;
               DBGridT3000->Columns->Items[IN_CALIBR-1]->Field->AsString = "0.0";
               DBGridT3000->Columns->Items[IN_FILTER-1]->Field->ReadOnly = false;
               DBGridT3000->Columns->Items[IN_FILTER-1]->Field->AsString = filter[6];
               DBGridT3000->Columns->Items[IN_FILTER-1]->Field->ReadOnly = true;
             }
           }
           delete RangeForm;
           editField = 0;
          if(exitfield==2)
	         DBGridT3000->EditorMode(false);
          else
             DBGridT3000->ToggleEditorMode();
//           DBGridT3000->EditorMode(true);
         }
		 break;
	case IN_CALIBR:
		 if( !ptr_panel->inputs[row-1].digital_analog )
         {
            if(key==13)
              DBGridT3000->EditorMode(true);
            else
              DBGridT3000->EditorMode(false);
            break;
         }
         if( checkFinishedEdit(key, exitfield) )
         {
           strcpy(buf, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
		   value = atof(buf);
		   if(ptr_panel->inputs[row-1].calibration_increment)
		   {
		   	 if( value < -255 ) value = -255;
		   	 if( value > 255 ) value = 255;
		   }
		   else
		   {
		   	 if (value<-25.5) value=-25.5;
		   	 if (value>25.5) value=25.5;
		   }
		   if ( value < 0 )
		   {
		   	 ptr_panel->inputs[row-1].calibration_sign = 1;
		   	 value = -value;
		   }
		   else
		   {
			 ptr_panel->inputs[row-1].calibration_sign = 0;
		   }

		   if(!ptr_panel->inputs[row-1].calibration_increment)
		    	ptr_panel->inputs[row-1].calibration = (DInt)(value*10);
		   else
				ptr_panel->inputs[row-1].calibration = (DInt)(value);

           gcvt(value,10,buf);
           dataSource->Edit();
           DBGridT3000->Columns->Items[field-1]->Field->AsString = buf;
           editField = 0;
           changeStatus = 1;
         }
		 break;
	case IN_FILTER:
		 if( !ptr_panel->inputs[row-1].digital_analog )
         {
            if(key==13)
              DBGridT3000->EditorMode(true);
            else
              DBGridT3000->EditorMode(false);
           break;
         }
         if(key==13)
         {
		   l=ptr_panel->inputs[row-1].filter;
		   l++;
		   if (l==8) l=0;
		   ptr_panel->inputs[row-1].filter=l;
           dataSource->Edit();
           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
           DBGridT3000->Columns->Items[field-1]->Field->AsString = filter[l];
           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
           editField = 0;
           changeStatus = 1;
         }
		 break;
	case IN_DECOM:
         if(key==13)
         {
		   l=ptr_panel->inputs[row-1].decom;
		   if (l==1)
		   {
		   	 strcpy(buf,"-");
		   	 l=0;
		   }
		   else
		   {
		   	 strcpy(buf,"Y");
		   	 l=1;
		   }
           ptr_panel->inputs[row-1].decom=l;
           dataSource->Edit();
           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
           DBGridT3000->Columns->Items[field-1]->Field->AsString = buf;
           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
           editField = 0;
           changeStatus = 1;
           DBGridT3000->EditorMode(true);
         }
		 break;
	case IN_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->inputs[row-1].label, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
   }
   if(changeStatus) refresh_table[row-1]=1;
}
//---------------------------------------------------------------------------
//Added by Paul Mao on 12/08/97 for output edit:
void __fastcall TGridForm::keyPressOut(long row, long field, char key, int exitfield)
{
   DLong value;
   DInt range;
   DByte l;
   int exit;
   char buf[80];
   float vf;
   exit = 0;
   if(key==27)
   {
     if( editField )
     {
       editField==0;
     }
     return;
   }

   switch (field) {
	case OUT_NUM:
         if(key==13)
              DBGridT3000->EditorMode(true);
         else
             DBGridT3000->EditorMode(false);
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
		 break;
	case OUT_FULL_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->outputs[row-1].description, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
          Table->Post();
         }
         else
          return;
		 break;
	case OUT_AUTO_MAN:
//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( !ptr_panel->outputs[row-1].range )
          {
  	       MessageBeep( MB_ICONEXCLAMATION );
           openMessage("        Undefined range!       ",Top,Left,Width,Height);
           DBGridT3000->EditorMode(true);
           break;
          }
          editField = 1;
          if( ptr_panel->outputs[row-1].auto_manual )
          {
           ptr_panel->outputs[row-1].auto_manual = 0;
          }
          else
          {
           ptr_panel->outputs[row-1].auto_manual = 1;
          }
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
				         auto_man[ptr_panel->outputs[row-1].auto_manual];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
		  if( !ptr_panel->outputs[row-1].auto_manual )
          {
	          DBGridT3000->Columns->Items[IN_VALUE-1]->Field->ReadOnly = true;
              readOnlyFields |= (1L<<(IN_VALUE-1));
          }
          else
          {
	          DBGridT3000->Columns->Items[IN_VALUE-1]->Field->ReadOnly = false;
              readOnlyFields &= ~(1L<<(IN_VALUE-1));
          }
          editField = 0;
          changeStatus = 1;
          Table->Post();
          SendIndivPoint(currentRec);
          if(exitfield==2)
	          DBGridT3000->EditorMode(false);
          else
	          DBGridT3000->EditorMode(true);
         }
         else
          DBGridT3000->EditorMode(false);
		 break;
	case OUT_VALUE:
		 DLong *pf;

		 if( !ptr_panel->outputs[row-1].auto_manual )
         {
//Warning messages:
  	       MessageBeep( MB_ICONEXCLAMATION );
//		   MessageBox(GetFocus(), " The Value can be changed only in Manual mode! ", "", MB_OK );
           openMessage(" The Value can be changed only in Manual mode! ",Top,Left,Width,Height);
           if(key==13)
              DBGridT3000->EditorMode(true);
           else
             DBGridT3000->EditorMode(false);
           break;
         }

		 range = ptr_panel->outputs[row-1].range;
		 pf = &ptr_panel->outputs[row-1].value;

		 if( ptr_panel->outputs[row-1].digital_analog )
		 {                   // analog
          if( checkFinishedEdit(key, exitfield) )
          {
           strcpy(buf, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
		   *pf=(long)(atof(buf)*1000);
           checkdecimal(buf, 2);
           dataSource->Edit();
           DBGridT3000->Columns->Items[field-1]->Field->AsString = buf;
           editField = 0;
           changeStatus = 1;
           SendIndivPoint(currentRec);
          }
          else
           return;
		  break;
		 }
		 else
		 {
          if(key==13)
          {
           editField = 1;
		   ptr_panel->outputs[row-1].control = !ptr_panel->outputs[row-1].control;
           range -= DIG1;
		   strcpy(buf, dig_range_array[range].dunits[ptr_panel->outputs[row-1].control]);
		   *pf=ptr_panel->outputs[row-1].control?1000L:0;
           dataSource->Edit();
           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
           DBGridT3000->Columns->Items[field-1]->Field->AsString = buf;
           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
           changeStatus = 1;
           editField = 0;
           SendIndivPoint(currentRec);
          }
          if(exitfield==2)
	          DBGridT3000->EditorMode(false);
          else
	          DBGridT3000->EditorMode(true);
		 }
		 break;
	case OUT_UNITS:
         if(key==13)
              DBGridT3000->EditorMode(true);
         else
              DBGridT3000->EditorMode(false);
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
    	 break;
	case OUT_RANGE:
         if(key==13)
         {
           editField = 1;
           TRangeForm *RangeForm;
           RangeForm = new TRangeForm( Application );

//Added by Paul Mao on 11/8/97:
//Initialize the "AnalogList" according to the current point type:
		   AnsiString temp;
           int n;
		   RangeForm -> AnalogList -> Clear();
           for (int i=0; i<=6; i++)
           {
             temp = "  " + AnsiString(out_range_array[i].range_text) + " " +
                    AnsiString(out_range_array[i].aunits);
	         RangeForm -> AnalogList -> Items -> Add(temp);
           }

		   if( RangeForm->ShowModal()==mrOk )
           {
           	 if(RangeForm->StandardList->ItemIndex!=-1 || RangeForm->CustomList->ItemIndex!=-1)
             {
//Invert digital input ranges display:
           	   if(RangeForm->StandardList->ItemIndex!=-1)
               {
                if(RangeForm->CheckBox->Checked)
                {
                 n = RangeForm->StandardList->ItemIndex + 1 + 11;
                }
                else
                {
                 n = RangeForm->StandardList->ItemIndex + 1;
                }
               }
               else
               {
                n = RangeForm->CustomList->ItemIndex+23;
               }
               ptr_panel->outputs[row-1].range = n+DIG1;
               ptr_panel->outputs[row-1].digital_analog = 0;
               ptr_panel->outputs[row-1].auto_manual = 0;
//Update the screen display(include two fields : "RANGE" and "UNITS"):
	           dataSource->Edit();
    	       DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
        	   DBGridT3000->Columns->Items[field-1]->Field->AsString = dig_range_array[n].range_text;
               DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
    	       DBGridT3000->Columns->Items[IN_UNITS-1]->Field->ReadOnly = false;
        	   DBGridT3000->Columns->Items[IN_UNITS-1]->Field->AsString = "";
               DBGridT3000->Columns->Items[IN_UNITS-1]->Field->ReadOnly = true;
			   ptr_panel->outputs[row-1].control = (long)(dig_range_array[n].value)%2;
		       ptr_panel->outputs[row-1].value = ptr_panel->outputs[row-1].control?1000L:0;
		       strcpy(buf, dig_range_array[n].dunits[ptr_panel->outputs[row-1].control]);
               DBGridT3000->Columns->Items[IN_VALUE-1]->Field->ReadOnly = false;
               DBGridT3000->Columns->Items[IN_VALUE-1]->Field->AsString = buf;
               DBGridT3000->Columns->Items[IN_VALUE-1]->Field->ReadOnly = true;
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->ReadOnly = false;
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->AsString = auto_man[0];
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->ReadOnly = true;
               ptr_panel->outputs[row-1].m_del_low = 0;
               ptr_panel->outputs[row-1].s_del_high = 0;
               DBGridT3000->Columns->Items[OUT_HIGH-1]->Field->ReadOnly = "";
               DBGridT3000->Columns->Items[OUT_LOW-1]->Field->ReadOnly = "";
               changeStatus = 1;
             }
           	 if(RangeForm -> AnalogList -> ItemIndex != -1)
             {
               n = RangeForm->AnalogList->ItemIndex;
               ptr_panel->outputs[row-1].range = n;
               ptr_panel->outputs[row-1].digital_analog = 1;
               ptr_panel->outputs[row-1].auto_manual = 0;
               changeStatus = 1;
//Update the screen display(include two fields : "RANGE" and "UNITS"):
	           dataSource->Edit();
    	       DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
        	   DBGridT3000->Columns->Items[field-1]->Field->AsString = out_range_array[n].range_text;
               DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
    	       DBGridT3000->Columns->Items[IN_UNITS-1]->Field->ReadOnly = false;
        	   DBGridT3000->Columns->Items[IN_UNITS-1]->Field->AsString = out_range_array[n].aunits;
               DBGridT3000->Columns->Items[IN_UNITS-1]->Field->ReadOnly = true;
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->ReadOnly = false;
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->AsString = auto_man[0];
               DBGridT3000->Columns->Items[IN_AUTO_MAN-1]->Field->ReadOnly = true;
			   ptr_panel->outputs[row-1].control = 0;
		       ptr_panel->outputs[row-1].value = 0;
			   ptr_panel->outputs[row-1].s_del_high=100;
		  	   ptr_panel->outputs[row-1].m_del_low = 0;
               DBGridT3000->Columns->Items[OUT_HIGH-1]->Field->ReadOnly = "10.0";
               DBGridT3000->Columns->Items[OUT_LOW-1]->Field->ReadOnly = "0.0";
             }
           }
           delete RangeForm;
           editField = 0;
           DBGridT3000->ToggleEditorMode();
//           DBGridT3000->EditorMode(true);
         }
		 break;
    case OUT_LOW:
	case OUT_HIGH:
		 if( !ptr_panel->outputs[row-1].digital_analog )
         {
            if(key==13)
              DBGridT3000->EditorMode(true);
            else
              DBGridT3000->EditorMode(false);
           break;
         }
         if( checkFinishedEdit(key, exitfield) )
         {
		   range = ptr_panel->outputs[row-1].range;
		   if ( (range==2 || range==4 || range==5))
		   {
             strcpy(buf, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
			 vf = atof(buf);
		     if (vf<0) vf=0;
		     if (vf>10.0) vf=10.0;
             gcvt(vf,10,buf);
             checkdecimal(buf, 1);
 			 if (field==OUT_LOW)
			  	ptr_panel->outputs[row-1].m_del_low = vf*10;
		      else
		     	ptr_panel->outputs[row-1].s_del_high = vf*10;
             dataSource->Edit();
             DBGridT3000->Columns->Items[field-1]->Field->AsString = buf;
             changeStatus = 1;
		   }
           else
           {
             dataSource->Edit();
 			 if (field==OUT_LOW)
               DBGridT3000->Columns->Items[field-1]->Field->AsString = "0.0";
             else
               DBGridT3000->Columns->Items[field-1]->Field->AsString = "10.0";
           }
           editField = 0;
           DBGridT3000->EditorMode(true);
         }
    	 break;
  	case OUT_DECOM:
         if(key==13)
         {
		   l=ptr_panel->outputs[row-1].decom;
		   if (l==1)
		   {
		   	 strcpy(buf,"-");
		   	 l=0;
		   }
		   else
		   {
		   	 strcpy(buf,"Y");
		   	 l=1;
		   }
           ptr_panel->outputs[row-1].decom=l;
           dataSource->Edit();
           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
           DBGridT3000->Columns->Items[field-1]->Field->AsString = buf;
           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
           editField = 0;
           changeStatus = 1;
           DBGridT3000->EditorMode(true);
         }
		 break;
	case OUT_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->outputs[row-1].label, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
          Table->Post();
         }
         else
          return;
		 break;
   }
   if(exit) refresh_table[row-1]=1;
}

void __fastcall TGridForm::keyPressVar(long row, long field, char key, int exitfield)
{
   DLong value;
   DByte l;
   int exit;
   char buf[80];
   exit = 0;
   if(key==27)
     if( editField )
     {
       editField==0;
       return;
     }

   switch (field) {
	case VAR_NUM:
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
	     StatusBarMessage(SBText, "It is a read-only field!");
		 break;
	case VAR_FULL_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->vars[row-1].description, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
	case VAR_AUTO_MAN:
//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( ptr_panel->vars[row-1].auto_manual )
          {
           ptr_panel->vars[row-1].auto_manual = 0;
          }
          else
          {
           ptr_panel->vars[row-1].auto_manual = 1;
          }
          changeStatus = 1;

//Update the screen display:
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
				         auto_man[ptr_panel->vars[row-1].auto_manual];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;

//If the Auto/Man field is "AUTO" then the user cannot update the "VALUE" field;
//or the "VALUE" field is update enabled.
		  if( !ptr_panel->vars[row-1].auto_manual )
          {
	          DBGridT3000->Columns->Items[VAR_VALUE-1]->Field->ReadOnly = true;
              readOnlyFields |= (1L<<(VAR_VALUE-1));
          }
          else
          {
	          DBGridT3000->Columns->Items[VAR_VALUE-1]->Field->ReadOnly = false;
              readOnlyFields &= ~(1L<<(VAR_VALUE-1));
          }
         }
		 break;
	case VAR_VALUE:
		 DLong *pf;
		 DInt range;

//		 if( readOnlyFields & (1L<<(VAR_VALUE-1)) )
		 if( !ptr_panel->vars[row-1].auto_manual )
         {
//Warning messages:
  	       MessageBeep( MB_ICONEXCLAMATION );
	       StatusBarMessage(SBText, "It is a read-only field!");
           break;
         }

		 range = ptr_panel->vars[row-1].range;
		 pf = &ptr_panel->vars[row-1].value;

		 if( ptr_panel->vars[row-1].digital_analog )
		 {
          if( checkFinishedEdit(key, exitfield) )
          {

//When the point is an analog one, read the updated data from the screen.
           strcpy(buf, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());

//Update the "VALUE" field of the current panel and set the "changeStatus" true:
//Note: "pf" is a pointer.
		   *pf=(long)(atof(buf)*1000);
           changeStatus = 1;

           checkdecimal(buf, 2);

//Update the screen display:
           dataSource->Edit();
           DBGridT3000->Columns->Items[field-1]->Field->AsString = buf;
           editField = 0;
          }
          else
           return;
		  break;
		 }
		 else
		 {
//When the point is a digital one:
          if(key==13)
          {
		   ptr_panel->vars[row-1].control = !ptr_panel->vars[row-1].control;
		   range -= DIG1;
		   memcpy(buf, dig_range_array[range].dunits[ptr_panel->vars[row-1].control],L_IN_VALUE);

//Update the "VALUE" field of the current panel and set the "changeStatus" true:
//Note: "pf" is a pointer.
		   *pf=ptr_panel->vars[row-1].control?1000L:0;
           changeStatus = 1;

//Update the screen display:
           dataSource->Edit();
           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
           DBGridT3000->Columns->Items[field-1]->Field->AsString = buf;
           DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
          }
		 }
		 break;
	case VAR_UNITS:
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
	     StatusBarMessage(SBText, "It is a read-only field!");
    	 break;
	case VAR_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->vars[row-1].label, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
   }
}
void __fastcall TGridForm::keyPressCon(long row, long field, char key, int exitfield)
{
}
void __fastcall TGridForm::keyPressWr(long row, long field, char key, int exitfield)
{
   int exit;
   char *on_off[2]={"OFF","ON"};

   exit = 0;
   if(key==27)
     if( editField )
     {
       editField==0;
       return;
     }

   switch (field) {
	case WR_NUM:
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
	     StatusBarMessage(SBText, "It is a read-only field!");
		 break;
	case WR_FULL_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->weekly_routines[row-1].description, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
	case WR_AUTO_MAN:
//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( ptr_panel->weekly_routines[row-1].auto_manual )
          {
           ptr_panel->weekly_routines[row-1].auto_manual = 0;
          }
          else
          {
           ptr_panel->weekly_routines[row-1].auto_manual = 1;
          }
          changeStatus = 1;

//Update the screen display:
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
				         auto_man[ptr_panel->weekly_routines[row-1].auto_manual];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;

		  if( !ptr_panel->weekly_routines[row-1].auto_manual )
          {
	          DBGridT3000->Columns->Items[WR_VALUE-1]->Field->ReadOnly = true;
              readOnlyFields |= (1L<<(WR_VALUE-1));
          }
          else
          {
	          DBGridT3000->Columns->Items[WR_VALUE-1]->Field->ReadOnly = false;
              readOnlyFields &= ~(1L<<(WR_VALUE-1));
          }
         }
		 break;
    case WR_VALUE:

 		 if( !ptr_panel->weekly_routines[row-1].auto_manual )
         {
//Warning messages:
  	       MessageBeep( MB_ICONEXCLAMATION );
	       StatusBarMessage(SBText, "It is a read-only field!");
           break;
         }

//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( ptr_panel->weekly_routines[row-1].value )
          {
           ptr_panel->weekly_routines[row-1].value = 0;
          }
          else
          {
           ptr_panel->weekly_routines[row-1].value = 1;
          }
          changeStatus = 1;

//Update the screen display:
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
				         on_off[ptr_panel->weekly_routines[row-1].value];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
         }
		 break;
	case WR_OVERRIDE1:
    	 break;
    case WR_OR1VALUE:
//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( ptr_panel->weekly_routines[row-1].override_1_value )
          {
           ptr_panel->weekly_routines[row-1].override_1_value = 0;
          }
          else
          {
           ptr_panel->weekly_routines[row-1].override_1_value = 1;
          }
          changeStatus = 1;

//Update the screen display:
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
  				         on_off[ptr_panel->weekly_routines[row-1].override_1_value];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
         }
		 break;
	case WR_OVERRIDE2:
    	 break;
    case WR_OR2VALUE:
//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( ptr_panel->weekly_routines[row-1].override_2_value )
          {
           ptr_panel->weekly_routines[row-1].override_2_value = 0;
          }
          else
          {
           ptr_panel->weekly_routines[row-1].override_2_value = 1;
          }
          changeStatus = 1;

//Update the screen display:
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
  				         on_off[ptr_panel->weekly_routines[row-1].override_2_value];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
         }
		 break;
	case WR_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->weekly_routines[row-1].label, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
   }
}
void __fastcall TGridForm::keyPressAr(long row, long field, char key, int exitfield)
{
   int exit;
   char *on_off[2]={"OFF","ON"};

   exit = 0;
   if(key==27)
     if( editField )
     {
       editField==0;
       return;
     }

   switch (field) {
	case AR_NUM:
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
	     StatusBarMessage(SBText, "It is a read-only field!");
		 break;
	case AR_FULL_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->annual_routines[row-1].description, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
	case AR_AUTO_MAN:
//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( ptr_panel->annual_routines[row-1].auto_manual )
          {
           ptr_panel->annual_routines[row-1].auto_manual = 0;
          }
          else
          {
           ptr_panel->annual_routines[row-1].auto_manual = 1;
          }
          changeStatus = 1;

//Update the screen display:
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
				         auto_man[ptr_panel->annual_routines[row-1].auto_manual];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;

		  if( !ptr_panel->annual_routines[row-1].auto_manual )
          {
	          DBGridT3000->Columns->Items[AR_VALUE-1]->Field->ReadOnly = true;
              readOnlyFields |= (1L<<(AR_VALUE-1));
          }
          else
          {
	          DBGridT3000->Columns->Items[AR_VALUE-1]->Field->ReadOnly = false;
              readOnlyFields &= ~(1L<<(AR_VALUE-1));
          }
         }
		 break;
    case AR_VALUE:

		 if( !ptr_panel->annual_routines[row-1].auto_manual )
         {
//Warning messages:
  	       MessageBeep( MB_ICONEXCLAMATION );
	       StatusBarMessage(SBText, "It is a read-only field!");
           break;
         }

//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( ptr_panel->annual_routines[row-1].value )
          {
           ptr_panel->annual_routines[row-1].value = 0;
          }
          else
          {
           ptr_panel->annual_routines[row-1].value = 1;
          }
          changeStatus = 1;

//Update the screen display:
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
				         on_off[ptr_panel->annual_routines[row-1].value];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
         }
		 break;
	case AR_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->annual_routines[row-1].label, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
   }
}
void __fastcall TGridForm::keyPressPrg(long row, long field, char key, int exitfield)
{
   int exit;
   char *on_off[2]={"OFF","ON"};

   exit = 0;
   if(key==27)
     if( editField )
     {
       editField==0;
       return;
     }

   switch (field) {
	case PRG_NUM:
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
	     StatusBarMessage(SBText, "It is a read-only field!");
		 break;
	case PRG_FULL_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->programs[row-1].description, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
    case PRG_STATUS:
//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( ptr_panel->programs[row-1].on_off )
          {
           ptr_panel->programs[row-1].on_off = 0;
          }
          else
          {
           ptr_panel->programs[row-1].on_off = 1;
          }
          changeStatus = 1;

//Update the screen display:
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
				         on_off[ptr_panel->programs[row-1].on_off];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
         }
		 break;
	case PRG_AUTO_MAN:
//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( ptr_panel->programs[row-1].auto_manual )
          {
           ptr_panel->programs[row-1].auto_manual = 0;
          }
          else
          {
           ptr_panel->programs[row-1].auto_manual = 1;
          }
          changeStatus = 1;

//Update the screen display:
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
				         auto_man[ptr_panel->programs[row-1].auto_manual];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
         }
		 break;
	case PRG_SIZE:
    case PRG_EXIT:
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
	     StatusBarMessage(SBText, "It is a read-only field!");
		 break;
	case PRG_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->programs[row-1].label, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
   }
}
void __fastcall TGridForm::keyPressGrp(long row, long field, char key, int exitfield)
{
   int exit;
   char *mode[2]={"Text","Graphic"};

   exit = 0;
   if(key==27)
     if( editField )
     {
       editField==0;
       return;
     }

   switch (field) {
	case GRP_NUM:
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
	     StatusBarMessage(SBText, "It is a read-only field!");
		 break;
	case GRP_FULL_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->control_groups[row-1].description, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
    case GRP_PICFILE:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->control_groups[row-1].picture_file, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
    	 break;
    case GRP_MODE:
//Update the current panel data and set the "changeStatus" true:
         if(key==13)
         {
          if( ptr_panel->control_groups[row-1].mode )
          {
           ptr_panel->control_groups[row-1].mode = 0;
          }
          else
          {
           ptr_panel->control_groups[row-1].mode = 1;
          }
          changeStatus = 1;

//Update the screen display:
          dataSource->Edit();
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = false;
          DBGridT3000->Columns->Items[field-1]->Field->AsString =
				         mode[ptr_panel->control_groups[row-1].mode];
          DBGridT3000->Columns->Items[field-1]->Field->ReadOnly = true;
         }
    	 break;
	case GRP_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->control_groups[row-1].label, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
    case GRP_UPDATE:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          char tempbuf[10];
          strcpy(tempbuf, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          ptr_panel->control_groups[row-1].update_time = (DByte)atoi(tempbuf);
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
   }
}
void __fastcall TGridForm::keyPressAy(long row, long field, char key, int exitfield)
{
   int exit;

   exit = 0;
   if(key==27)
     if( editField )
     {
       editField==0;
       return;
     }

   switch (field) {
	case AY_NUM:
//Warning messages:
	     MessageBeep( MB_ICONEXCLAMATION );
	     StatusBarMessage(SBText, "It is a read-only field!");
		 break;
	case AY_LABEL:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          strcpy(ptr_panel->arrays[row-1].label, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
    case AY_LENGTH:
         if( checkFinishedEdit(key, exitfield) )
         {       // edited was finished and the changes should be saved
          char tempbuf[10];
          strcpy(tempbuf, DBGridT3000->Columns->Items[field-1]->Field->AsString.c_str());
          ptr_panel->arrays[row-1].length = (DInt)atoi(tempbuf);
          editField = 0;
          changeStatus = 1;
         }
         else
          return;
		 break;
   }
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::OnDblClick(TObject *Sender)
{
//Updated by Paul Mao on 11/08/97 :
//  keyPressIn(currentRec, currentCol, 13);
	switch( pointType ) {
       case T3000_OUT:
	     keyPressOut(currentRec, currentCol, 13, 2);
         break;
       case T3000_IN:
	     keyPressIn(currentRec, currentCol, 13, 2);
         break;
       case VAR:
	     keyPressVar(currentRec, currentCol, 13);
         break;
       case CON:
	     keyPressCon(currentRec, currentCol, 13);
         break;
       case WR:
	     keyPressWr(currentRec, currentCol, 13);
         break;
       case AR:
	     keyPressAr(currentRec, currentCol, 13);
         break;
       case PRG:
	     keyPressPrg(currentRec, currentCol, 13);
         break;
       case GRP:
	     keyPressGrp(currentRec, currentCol, 13);
         break;
       case AY:
	     keyPressAy(currentRec, currentCol, 13);
         break;
    }
}
//---------------------------------------------------------------------------
int checkdecimal(char *buf, int dec)
{
 char buf1[80],*p,zero[6];
 memset(zero,'0',6);
 p=strchr(buf,'.');
 if(!p)
 {
   strcat(buf,".");
   zero[dec]=0;
   strcat(buf,zero);
 }
 else
 {
   p++;
   switch (strlen(p)) {
      case 0:
           zero[dec]=0;
		   strcat(buf,zero);
           break;
      case 1:
           zero[dec-1]=0;
		   strcat(buf,zero);
           break;
      case 3:
           zero[dec-2]=0;
		   strcat(buf,zero);
           break;
   }
   p[dec] = 0;
 }
}
//---------------------------------------------------------------------------
//Added By Paul Mao on 04/08/97 for GRID edit menu's subitems:
void __fastcall TGridForm::writeback()
{
  editField = 1;
  keyPressIn( Table->RecNo, DBGridT3000->col(), 0, 1);
}
//---------------------------------------------------------------------------
//Added by Paul Mao on 12/08/97 to read point label for CONTROLLERS:
int __fastcall TGridForm::getpointlabel(Point_T3000 point, char *label)
{
  DByte      *buf;
  DUint      l1;
  RS232Error result;
  result = RS232_SUCCESS;

  buf  = new DByte [10000];
  if( serial_access )
//"panel" -> "point -> panel", but still use "pat_panel" to install the data;
    result=t3000Request(point.point_type, 0, buf, &l1, point.panel + 1,network);
  if( result == RS232_SUCCESS && serial_access )
  {
   switch( point.point_type - 1 ) {
       case T3000_OUT:
         	alignOutputs(1,buf,ptr_panel->outputs,tbl_bank[pointType]);
            label = ptr_panel->outputs[point.number + 1].label;
         break;
       case T3000_IN:
	        alignInputs(1,buf,ptr_panel->inputs,tbl_bank[pointType]);
            label = ptr_panel->inputs[point.number + 1].label;
         break;
       case VAR:
	        alignVars(1,buf,ptr_panel->vars,tbl_bank[pointType]);
            label = ptr_panel->vars[point.number + 1].label;
         break;
       case CON:
//	        alignCons(1,buf,ptr_panel->controllers,tbl_bank[pointType]);
//            label = ptr_panel->controllers[point.number + 1].label;
         break;
       case WR:
	        alignWeeklyroutines(1,buf,ptr_panel->weekly_routines,tbl_bank[pointType]);
            label = ptr_panel->weekly_routines[point.number + 1].label;
         break;
       case AR:
	        alignAnnualroutines(1,buf,ptr_panel->annual_routines,tbl_bank[pointType]);
            label = ptr_panel->annual_routines[point.number + 1].label;
         break;
       case PRG:
	        alignPrograms(1,buf,ptr_panel->programs,tbl_bank[pointType]);
            label = ptr_panel->programs[point.number + 1].label;
         break;
       case GRP:
	        alignScreens(1,buf,ptr_panel->control_groups,tbl_bank[pointType]);
            label = ptr_panel->control_groups[point.number + 1].label;
         break;
       case AY:
	        alignArrays(1,buf,ptr_panel->arrays,tbl_bank[pointType]);
            label = ptr_panel->arrays[point.number + 1].label;
         break;
   }
  }
  delete [] buf;
}
//---------------------------------------------------------------------------
//Added by Paul Mao on 13/08/97 for GRID refresh menu item:
void __fastcall TGridForm::Refresh1Click(TObject *Sender)
{
  char tempbuf[10];

  if (refreshTimeConst == -1 )
  {
	 MessageBeep( MB_ICONEXCLAMATION );
	 MessageBox(GetFocus(), " Refreshing not enabled! ", "", MB_OK );
     return;
  }
  TRefreshInForm *RefreshInForm;
  RefreshInForm = new TRefreshInForm( Application );

  itoa(refreshTimeConst/1000L, tempbuf, 10);
  RefreshInForm -> RefreshIn -> Text = (AnsiString)tempbuf;

  if( RefreshInForm->ShowModal() == mrOk );
  {
    refreshTimeConst = 1000L*atoi(RefreshInForm -> RefreshIn -> Text.c_str());
  }
  delete RefreshInForm;
//  RefreshGrid();
}
//---------------------------------------------------------------------------
void __fastcall TGridForm::DBGridT3000DrawColumnCell(TObject *Sender,
	const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  switch( pointType ) {
       case T3000_OUT:
           if( !ptr_panel->outputs[Table->RecNo-1].digital_analog )
             if( DataCol == (OUT_UNITS-1) || DataCol == (OUT_LOW-1) || DataCol == (OUT_HIGH-1))
             {
				 DBGridT3000->Canvas->Brush->Color = clLtGray;
                 RECT r;
                 r.left=Rect.Left+2;
                 r.top=Rect.Top+2;
                 r.right=Rect.Right-2;
                 r.bottom=Rect.Bottom-2;
                 TRect R(r);
                 DBGridT3000->Canvas->FillRect(R);
             }
           break;
       case T3000_IN:
           if( !ptr_panel->inputs[Table->RecNo-1].digital_analog )
             if( DataCol==(OUT_UNITS-1) || DataCol==(IN_CALIBR-1) || DataCol==(IN_FILTER-1) )
             {
				 DBGridT3000->Canvas->Brush->Color = clLtGray;
                 RECT r;
                 r.left=Rect.Left+2;
                 r.top=Rect.Top+2;
                 r.right=Rect.Right-2;
                 r.bottom=Rect.Bottom-2;
                 TRect R(r);
                 DBGridT3000->Canvas->FillRect(R);
             }
           break;
  }
}
//---------------------------------------------------------------------------
void openMessage(char *ptr,int top,int left,int width,int height)
{
   TMessageBoxForm *MessageBoxForm;
   MessageBoxForm = new TMessageBoxForm(Application);
   MessageBoxForm->Message->Caption = ptr;
   MessageBoxForm->SetMessageBox(top,left,width,height);
   MessageBoxForm->ShowModal();
   delete MessageBoxForm;
}




