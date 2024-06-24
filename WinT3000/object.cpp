//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "object.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "aio.h"


TObjectForm *ObjectForm;

extern int pointtotext(char *buf,Point_Net *point);
extern char *ispoint(char *token, DInt *num_point, DByte *var_type, DByte *point_type, DInt *num_panel, DInt *num_net, DInt network=0, DByte panel=0, DInt *netpresent=0);
extern void longtoa(DLong f, DInt length, DInt ndec, char *buf);
extern char *rtrim(char *text);

extern DInt networkaddress;    // sec de la inceputul programului
extern Panel *ptr_panel;
extern DInt  station_num;

//---------------------------------------------------------------------------
__fastcall TObjectForm::TObjectForm(TComponent* Owner, Str_grp_element *ppgrp, Str_grp_work_data *ppworkdata)
    : TForm(Owner)
{
  pgrp = *ppgrp;
  pworkdata = *ppworkdata;
  char buf[15];
  char *label; //=islabel(point_name,&num_point,&var_type,&point_type,&num_panel);
//  char *des;
  Point_Net p;
  DInt num_net,/*last_cur,*/num_panel,num_point;
  DByte var_type,point_type;
  DLong value;
  buf[0]=0;
  p = ppgrp->point_info.point;
  pointtotext(buf, &p);
  Label1->Caption = buf;

  label=ispoint(rtrim(buf),&num_point,&var_type, &point_type, &num_panel, &num_net, networkaddress, station_num);
  Label3->Caption = label;

  value = 22300;
  buf[0]=0;
  switch(pgrp.point_info.point.point_type-1){
   case T3000_OUT:
         strcpy(buf, ptr_panel->outputs[pgrp.point_info.point.number].description);
         value = ptr_panel->outputs[pgrp.point_info.point.number].value;
         break;
   case T3000_IN:
         strcpy(buf, ptr_panel->inputs[pgrp.point_info.point.number].description);
         value = ptr_panel->inputs[pgrp.point_info.point.number].value;
         break;
   case VAR:
         strcpy(buf, ptr_panel->vars[pgrp.point_info.point.number].description);
         value = ptr_panel->vars[pgrp.point_info.point.number].value;
         break;
   case WR:
   case PRG:
         strcpy(buf, ptr_panel->programs[pgrp.point_info.point.number].description);
         break;
   case GRP:
         strcpy(buf, ptr_panel->control_groups[pgrp.point_info.point.number].description);
         break;
   case AMON:
         break;
  }
   Label2->Caption = buf;
   longtoa(value, 7, 2,  buf);
   Value->Text = buf;
}
//---------------------------------------------------------------------------
