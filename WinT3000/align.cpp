//***************************************************************************
//
// This file include the conversion from 4 bytes aligment to 1 byte aligment
//
//
//***************************************************************************


//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "align.h"
#include "baseclas.h"
//---------------------------------------------------------------------------
#include "define.h"
#include "aio.h"

extern Panel *ptr_panel;

void alignPanelInfo(int t, DByte *p_info1, Panel_info1 *panel_info1)
{
 if(t)
 {
  memcpy(&panel_info1->panel_type, p_info1, sizeof(DByte));
  p_info1 += sizeof(DByte);
  memcpy(&panel_info1->active_panels, p_info1, sizeof(DUlong));
  p_info1 += sizeof(DUlong);
  memcpy(&panel_info1->des_length, p_info1, sizeof(DUint));
  p_info1 += sizeof(DUint);
  memcpy(&panel_info1->version, p_info1, sizeof(DUint));
  p_info1 += sizeof(DInt);
  memcpy(&panel_info1->panel_number, p_info1, sizeof(DByte));
  p_info1 += sizeof(DByte);
  memcpy(panel_info1->panel_name, p_info1, NAME_SIZE*sizeof(DByte));
  p_info1 += NAME_SIZE*sizeof(DByte);
  memcpy(&panel_info1->network, p_info1, sizeof(DUint));
  p_info1 += sizeof(DInt);
  memcpy(panel_info1->network_name, p_info1,NAME_SIZE*sizeof(DByte));
 }
}

int alignInputs(int t, DByte *buf, Str_in_point *inputs, int n)
{
 int i,l;
 l=0;
 for(i=0; i<n; i++)
 {
  if(t)
  {
   memcpy(inputs[i].description, buf, 21);
   buf += 21;
   memcpy(inputs[i].label, buf, 9);
   buf += 9;
   memcpy(&inputs[i].value, buf, sizeof(DLong));
   buf += sizeof(DLong);
   memcpy(((DByte *)&inputs[i].value)+4, buf, 2);
   buf += 2;
   memcpy(&inputs[i].calibration, buf, 1);
   buf += 1;
   memcpy(&inputs[i].range, buf, 1);
   if(i<n-1)
	 buf += 1;
  }
  else
  {
   memcpy(buf, inputs[i].description, 21);
   l   += 21;
   buf += 21;
   memcpy(buf, inputs[i].label, 9);
   l   += 9;
   buf += 9;
   memcpy(buf, &inputs[i].value, sizeof(DLong));
   l   += sizeof(DLong);
   buf += sizeof(DLong);
   memcpy(buf, ((DByte *)&inputs[i].value)+4, 2);
   l   += 2;
   buf += 2;
   memcpy(buf, &inputs[i].calibration, 1);
   l   += 1;
   buf += 1;
   memcpy(buf, &inputs[i].range, 1);
   l   += 1;
   if(i<n-1)
	 buf += 1;
  }
 }
 return l;
}

int alignOutputs(int t, DByte *buf, Str_out_point *outputs, int n)
{
 int i,l;
 l=0;
 for(i=0; i<n; i++)
 {
  if(t)
  {
   memcpy(outputs[i].description, buf, 21);
   buf += 21;
   memcpy(outputs[i].label, buf, 9);
   buf += 9;
   memcpy(&outputs[i].value, buf, sizeof(DLong));
   buf += sizeof(DLong);
   memcpy(((DByte *)&outputs[i].value)+4, buf, 2);
   buf += 2;
   memcpy(&outputs[i].m_del_low, buf, 1);
   buf += 1;
   memcpy(&outputs[i].s_del_high, buf, 1);
   buf += 1;
   memcpy(&outputs[i].delay_timer, buf, 2);
   if(i<n-1)
	 buf += 2;
  }
  else
  {
   memcpy(buf, outputs[i].description, 21);
   l   += 21;
   buf += 21;
   memcpy(buf, outputs[i].label, 9);
   l   += 9;
   buf += 9;
   memcpy(buf, &outputs[i].value, sizeof(DLong));
   l   += sizeof(DLong);
   buf += sizeof(DLong);
   memcpy(buf, ((DByte *)&outputs[i].value)+4, 2);
   l   += 2;
   buf += 2;
   memcpy(buf, &outputs[i].m_del_low, 1);
   l   += 1;
   buf += 1;
   memcpy(buf, &outputs[i].s_del_high, 1);
   l   += 1;
   buf += 1;
   memcpy(buf, &outputs[i].delay_timer, 2);
   l   += 2;
   if(i<n-1)
	 buf += 2;
  }
 }
 return l;
}

int alignVars(int t, DByte *buf, Str_variable_point *vars, int n)
{
 int i,l;
 l = 0;
 for(i=0; i<n; i++)
 {
  if(t)
  {
   memcpy(vars[i].description, buf, 21);
   buf += 21;
   memcpy(vars[i].label, buf, 9);
   buf += 9;
   memcpy(&vars[i].value, buf, sizeof(DLong));
   buf += sizeof(DLong);
   memcpy(((DByte *)&vars[i].value)+4, buf, 2);
   if(i<n-1)
	 buf += 2;
  }
  else
  {
   memcpy(buf, vars[i].description, 21);
   l   += 21;
   buf += 21;
   memcpy(buf, vars[i].label, 9);
   l   += 9;
   buf += 9;
   memcpy(buf, &vars[i].value, sizeof(DLong));
   l   += sizeof(DLong);
   buf += sizeof(DLong);
   memcpy(buf, ((DByte *)&vars[i].value)+4, 2);
   l   += 2;
   if(i<n-1)
	 buf += 2;
  }
 }
 return l;
}

int alignCons(int t, DByte *buf, Str_controller_point *controllers, int n)
{
 int i,l;
 l = 0;
 for(i=0; i<n; i++)
 {
  if(t)
  {
   memcpy(&controllers[i].input, buf, sizeof(Point_T3000));
   buf += sizeof(Point_T3000);
   memcpy(&controllers[i].input_value, buf, sizeof(DLong));
   buf += sizeof(DLong);
   memcpy(&controllers[i].value, buf, sizeof(DLong));
   buf += sizeof(DLong);
   memcpy(&controllers[i].setpoint, buf, sizeof(Point_T3000));
   buf += sizeof(Point_T3000);
   memcpy(&controllers[i].setpoint_value, buf, sizeof(DLong));
   buf += sizeof(DLong);
   memcpy(&controllers[i].units, buf, 1);
   buf += 1;
   memcpy(((DByte *)&controllers[i].units)+1, buf, 2);
   buf += 2;
   memcpy(&controllers[i].reset, buf, 1);
   buf += 1;
   memcpy(&controllers[i].bias, buf, 1);
   buf += 1;
   memcpy(&controllers[i].rate, buf, 1);
   if(i<n-1)
	 buf += 1;
  }
  else
  {
   memcpy(buf, &controllers[i].input, sizeof(Point_T3000));
   l   += sizeof(Point_T3000);
   buf += sizeof(Point_T3000);
   memcpy(buf, &controllers[i].input_value, sizeof(DLong));
   l   += sizeof(DLong);
   buf += sizeof(DLong);
   memcpy(buf, &controllers[i].value, sizeof(DLong));
   l   += sizeof(DLong);
   buf += sizeof(DLong);
   memcpy(buf, &controllers[i].setpoint, sizeof(Point_T3000));
   l   += sizeof(Point_T3000);
   buf += sizeof(Point_T3000);
   memcpy(buf, &controllers[i].setpoint_value, sizeof(DLong));
   l   += sizeof(DLong);
   buf += sizeof(DLong);
   memcpy(buf, &controllers[i].units, 1);
   l   += 1;
   buf += 1;
   memcpy(buf, ((DByte *)&controllers[i].units)+1, 2);
   l   += 2;
   buf += 2;
   memcpy(buf, &controllers[i].reset, 1);
   l   += 1;
   buf += 1;
   memcpy(buf, &controllers[i].bias, 1);
   l   += 1;
   buf += 1;
   memcpy(buf, &controllers[i].rate, 1);
   l   += 1;
   if(i<n-1)
	 buf += 1;
  }
 }
 return l;
}

int alignWeeklyroutines(int t, DByte *buf, Str_weekly_routine_point *weekly_routines, int n)
{
 int i,l;
 l = 0;
 for(i=0; i<n; i++)
 {
  if(t)
  {
   memcpy(weekly_routines[i].description, buf, 21);
   buf += 21;
   memcpy(weekly_routines[i].label, buf, 9);
   buf += 9;
   memcpy(((DByte *)&weekly_routines[i].label)+9, buf, 2);
   buf += 2;
   memcpy(&weekly_routines[i].override_1, buf, sizeof(Point_T3000));
   buf += sizeof(Point_T3000);
   memcpy(&weekly_routines[i].override_2, buf, sizeof(Point_T3000));
   if(i<n-1)
     buf += sizeof(Point_T3000);
  }
  else
  {
   memcpy(buf, weekly_routines[i].description, 21);
   l   += 21;
   buf += 21;
   memcpy(buf, weekly_routines[i].label, 9);
   l   += 9;
   buf += 9;
   memcpy(buf, ((DByte *)&weekly_routines[i].label)+9, 2);
   l   += 2;
   buf += 2;
   memcpy(buf, &weekly_routines[i].override_1, sizeof(Point_T3000));
   l   += sizeof(Point_T3000);
   buf += sizeof(Point_T3000);
   memcpy(buf, &weekly_routines[i].override_2, sizeof(Point_T3000));
   l   += sizeof(Point_T3000);
   if(i<n-1)
     buf += sizeof(Point_T3000);
  }
 }
 return l;
}

int alignAnnualroutines(int t, DByte *buf, Str_annual_routine_point *annual, int n)
{
 int i,l;
 l = 0;
 for(i=0; i<n; i++)
 {
  if(t)
  {
   memcpy(annual[i].description, buf, 21);
   buf += 21;
   memcpy(annual[i].label, buf, 9);
   buf += 9;
   memcpy(((DByte *)&annual[i].label)+9, buf, 2);
   if(i<n-1)
	 buf += 2;
  }
  else
  {
   memcpy(buf, annual[i].description, 21);
   l   += 21;
   buf += 21;
   memcpy(buf, annual[i].label, 9);
   l   += 9;
   buf += 9;
   memcpy(buf, ((DByte *)&annual[i].label)+9, 2);
   l   += 2;
   if(i<n-1)
	 buf += 2;
  }
 }
 return l;
}

int alignPrograms(int t, DByte *buf, Str_program_point *prgs, int n)
{
 int i,l;
 l = 0;
 for(i=0; i<n; i++)
 {
  if(t)
  {
   memcpy(prgs[i].description, buf, 21);
   buf += 21;
   memcpy(prgs[i].label, buf, 9);
   buf += 9;
   memcpy(&prgs[i].bytes, buf, sizeof(DUint));
   buf += sizeof(DUint);
   memcpy(((DByte *)&prgs[i].bytes)+sizeof(DUint), buf, 3);
   if(i<n-1)
	 buf += 3;
  }
  else
  {
   memcpy(buf, prgs[i].description, 21);
   l   += 21;
   buf += 21;
   memcpy(buf, prgs[i].label, 9);
   l   += 9;
   buf += 9;
   memcpy(buf, &prgs[i].bytes, sizeof(DUint));
   l   += sizeof(DUint);
   buf += sizeof(DUint);
   memcpy(buf, ((DByte *)&prgs[i].bytes)+sizeof(DUint), 3);
   l   += 3;
   if(i<n-1)
	 buf += 3;
  }
 }
 return l;
}

int alignScreens(int t, DByte *buf, Control_group_point *grps, int n)
{
 int i,l;
 l = 0;
 for(i=0; i<n; i++)
 {
  if(t)
  {
   memcpy(grps[i].description, buf, 21);
   buf += 21;
   memcpy(grps[i].label, buf, 9);
   buf += 9;
   memcpy(grps[i].picture_file, buf, 11);
   buf += 11;
   memcpy(&grps[i].update_time, buf, 1);
   buf += 1;
   memcpy(((DByte *)&grps[i].update_time)+1, buf, 2);
   buf += 2;
   memcpy(&grps[i].ycur_grp, buf,  sizeof(DInt));
   if(i<n-1)
	 buf += sizeof(DInt);
  }
  else
  {
   memcpy(buf, grps[i].description, 21);
   l   += 21;
   buf += 21;
   memcpy(buf, grps[i].label, 9);
   l   += 9;
   buf += 9;
   memcpy(buf, grps[i].picture_file, 11);
   l   += 11;
   buf += 11;
   memcpy(buf, &grps[i].update_time, 1);
   l   += 1;
   buf += 1;
   memcpy(buf, ((DByte *)&grps[i].update_time)+1, 2);
   l   += 2;
   buf += 2;
   memcpy(buf, &grps[i].ycur_grp,  sizeof(DInt));
   l   += sizeof(DInt);
   if(i<n-1)
	 buf += sizeof(DInt);

  }
 }
 return l;
}


int alignArrays(int t, DByte *buf, Str_array_point *ay, int n)
{
 int i,l;
 l = 0;
 for(i=0; i<n; i++)
 {
  if(t)
  {
   memcpy(ay[i].label, buf, 9);
   buf += 9;
   memcpy(&ay[i].length, buf, sizeof(DInt));
   if(i<n-1)
	 buf += sizeof(DInt);
  }
  else
  {
   memcpy(buf, ay[i].label, 9);
   buf += 9;
   memcpy(buf, &ay[i].length, sizeof(DInt));
   if(i<n-1)
	 buf += sizeof(DInt);
  }
 }
 return l;
}

int alignScreenElements(int t, DByte *buf, Str_grp_element *ptrgrp, int n)
{
 int i,l;
 l = 0;
 for(i=0; i<n; i++)
 {
  if(t)
  {
   memcpy(&ptrgrp[i].point_info.point, buf, 3);
   buf += 3;
   memcpy(&ptrgrp[i].point_info.point.network, buf, 2);
   buf += 2;
   memcpy(&ptrgrp[i].point_info.point_value, buf, 4);
   buf += 4;
   memcpy(((DByte *)&ptrgrp[i].point_info.point_value)+4, buf, 2);
   buf += 2;

   memcpy( (Point_info *)(&ptrgrp[i].point_info)+1, buf, 2);
   buf += 2;
   memcpy( &ptrgrp[i].high_limit, buf, 4);
   buf += 4;
   memcpy( &ptrgrp[i].low_limit, buf, 4);
   buf += 4;
   memcpy( (DByte *)&ptrgrp[i].low_limit+4, buf, 6);
   buf += 6;
   memcpy( &ptrgrp[i].bkgnd_icon, buf, 1);
   buf += 1;
   memcpy( (DByte *)&ptrgrp[i].bkgnd_icon+1, buf, 2);
   buf += 2;
   memcpy( (DByte *)&ptrgrp[i].bkgnd_icon+4, buf, 2);
   if(i<n-1)
	 buf += 2;
  }
  else
  {
/*
   memcpy(buf, grps[i].description, 21);
   l   += 21;
   buf += 21;
   memcpy(buf, grps[i].label, 9);
   l   += 9;
   buf += 9;
   memcpy(buf, grps[i].picture_file, 11);
   l   += 11;
   buf += 11;
   memcpy(buf, &grps[i].update_time, 1);
   l   += 1;
   buf += 1;
   memcpy(buf, ((DByte *)&grps[i].update_time)+1, 2);
   l   += 2;
   buf += 2;
   memcpy(buf, &grps[i].ycur_grp,  sizeof(DInt));
   l   += sizeof(DInt);
   if(i<n-1)
	 buf += sizeof(DInt);
*/
  }
 }
 return l;
}


// type = 1 load from panel
//      = 0 save to panel
// start = 1-n

void align(DInt type, DInt point_type, DByte *buf, DInt start, DInt nr)
{
   switch( point_type ) {
       case T3000_OUT:
         alignOutputs(type,buf,&ptr_panel->outputs[start-1],nr);
         break;
       case T3000_IN:
         alignInputs(type,buf,&ptr_panel->inputs[start-1],nr);
         break;
       case VAR:
         alignVars(type,buf,&ptr_panel->vars[start-1],nr);
         break;
       case CON:
         alignCons(type,buf,&ptr_panel->controllers[start-1],nr);
         break;
       case WR:
         alignWeeklyroutines(type,buf,&ptr_panel->weekly_routines[start-1],nr);
         break;
       case AR:
         alignAnnualroutines(type,buf,&ptr_panel->annual_routines[start-1],nr);
         break;
       case PRG:
         alignPrograms(type,buf,&ptr_panel->programs[start-1],nr);
         break;
       case GRP:
         alignScreens(type,buf,&ptr_panel->control_groups[start-1],nr);
         break;
       case AY:
         alignArrays(type,buf,&ptr_panel->arrays[start-1],nr);
         break;
       case READGROUPELEMENTS_T3000:
         alignScreenElements(type,buf,(Str_grp_element *)ptr_panel->control_group_elements[start-1].ptrgrp,nr);
         break;
   }
}

