//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include "pointcl.h"
//---------------------------------------------------------------------------
#include "define.h"
#include "wint3.h"
#include "baseclas.h"
#include "aio.h"
#include "ptpdll.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b))

extern DByte *heap_buf;
extern DUlong ind_heap_buf;
extern DByte *heap_grp;
extern DUlong ind_heap_grp;
extern Panel *ptr_panel;
extern unsigned int MAX_HEAP_BUF;   //15000;
extern unsigned int MAX_HEAP_GRP;   //15000;
extern unsigned int MAX_HEAP_AMON;   //15000;
extern unsigned int MAX_HEAP_DIGM;   //15000;
extern unsigned int MAX_HEAP_ARRAY;   //15000;
extern DInt local_panel;
extern dig_range_form dig_range_array[];
extern an_range_form  in_range_array[];
extern an_range_form  out_range_array[];
extern an_range_form  var_range_array[];

char *ispoint(char *token, DInt *num_point, DByte *var_type, DByte *point_type, DInt *num_panel, DInt *num_net, DInt network=0, DByte panel=0, DInt *netpresent=0);


char gdescriptor[22];
char glabel[10];
char gpointname[14];
char *on = "On";
char *off = "Off";
char *separator=":";
char *blank=" ";

//char huge *Heap_alloc(DInt bytes)
DByte *Heap_alloc(DInt bytes)
{
 if (bytes==0)
	 return NULL;
 ind_heap_buf += bytes;
 return (&heap_buf[ind_heap_buf - bytes]);
}

//DInt Heap_dealloc(DByte huge **old)
DInt Heap_dealloc(DByte **old)
{
// DByte huge *prog,*end;
// DByte *prog,*end;
 DInt bytes_old,n;
 if (*old)
 {
 memcpy(&bytes_old,*old,2);
 bytes_old += 5;
 memcpy(&n,*old+bytes_old,2);
 bytes_old += 2+n;
 memcpy(&n,*old+bytes_old,2);
 bytes_old += 2+n;
 memcpy(&n,*old+bytes_old,2);
 bytes_old += 2+n;

// set_semaphore(&t3000_flag);
// movedata(FP_SEG(*old+bytes_old),FP_OFF(*old+bytes_old) ,FP_SEG(*old),FP_OFF(*old), (&heap_buf[ind_heap_buf]-*old)-bytes_old);
 memmove(*old,(*old)+bytes_old, (&heap_buf[ind_heap_buf]-*old)-bytes_old);
	 for (DInt i=0;i<MAX_PRGS; i++)
		if (ptr_panel->program_codes[i] > *old)
			 ptr_panel->program_codes[i] -= bytes_old;
	 ind_heap_buf -= bytes_old;
 *old=NULL;
// clear_semaphore(&t3000_flag);
 }
 return 1;
}

//DInt Heap_move(DByte huge *old, DByte huge *newp, DInt ind)
DInt Heap_move(DByte *old, DByte *newp, DInt ind)
{
// DByte huge *prog,*end;
 DByte *prog,*end,*timetableold,*timetablenew, *p, *q, *s, r[100];
 DInt n,i,bytes_old,ntimeold,ntimenew;
// set_semaphore(&t3000_flag);
	ptr_panel->program_codes[ind] =  newp;
	if(old)
	{
	 memcpy(&bytes_old, old,2);
	 bytes_old += 5;
	 memcpy(&n,old+bytes_old,2);
	 bytes_old += 2+n;
	 memcpy(&n,old+bytes_old,2);
	 ntimeold = n;
	 timetableold = old + bytes_old + 2;
	 bytes_old += 2+n;

	 memcpy(&n,old+bytes_old,2);
	 bytes_old += 2+n;

	 prog=newp;
	 memcpy(&n,newp,2);
	 n += 5;
	 memcpy(&i,newp+n,2);
	 n += 2+i;
	 memcpy(&i,newp+n,2);
	 ntimenew = i;
	 timetablenew = newp + n + 2;
	 n += 2+i;
	 memcpy(&i,newp+n,2);
	 n += 2+i;
	 end=prog+n;

 p = timetableold + ntimeold;
 while( timetableold < p )
 {
	q = (DByte *)memchr(timetableold, 0xFF, ntimeold);
	if(q) q++;
	n = q - timetableold;
	memcpy(r, timetableold, n);
	r[n] = 0;
	s = timetablenew;
	while(s < timetablenew + ntimenew)
	{
	 if(!memcmp(s, r, n))
	 {
		memcpy( s + n , q, 5);
		break;
	 }
	 s = (DByte *)memchr(s, 0xFF, ntimenew);
	 if(s) s += 6;
	 else break;
	}
	timetableold = q + 5;
 }

/*
 while((long)FP_SEG(end)*16+(long)FP_OFF(end)>(long)FP_SEG(prog)*16+(long)FP_OFF(prog))   //1
 {
//	if((prog=(DByte huge *)memchr(prog,TIME_ON,((long)FP_SEG(end)*16+(long)FP_OFF(end)) - ((long)FP_SEG(prog)*16+(long)FP_OFF(prog))))!=NULL)
	if((prog=(DByte *)memchr(prog,TIME_ON,((long)FP_SEG(end)*16+(long)FP_OFF(end)) - ((long)FP_SEG(prog)*16+(long)FP_OFF(prog))))!=NULL)
		{
		 for(i=0;i<ind_line_array;i++)
			if (!memcmp(prog-2,&line_array[i][1],2))
				 {
					memcpy(prog+1,old+line_array[i][0],4);
					break;
				 }
		 prog++;
		continue;
		}
	 else
		 break;
 }
 prog=newp;
 while((long)FP_SEG(end)*16+(long)FP_OFF(end)>(long)FP_SEG(prog)*16+(long)FP_OFF(prog))   //1
 {
//	if((prog=(DByte huge *)memchr(prog,TIME_OFF,((long)FP_SEG(end)*16+(long)FP_OFF(end)) - ((long)FP_SEG(prog)*16+(long)FP_OFF(prog))))!=NULL)
	if((prog=(DByte *)memchr(prog,TIME_OFF,((long)FP_SEG(end)*16+(long)FP_OFF(end)) - ((long)FP_SEG(prog)*16+(long)FP_OFF(prog))))!=NULL)
		{
		 for(i=0;i<ind_line_array;i++)
			if (!memcmp(prog-2,&line_array[i][1],2))
				 {
					memcpy(prog+1,old+line_array[i][0],4);
					break;
				 }
		prog++;
		continue;
		}
	 else
		 break;
 }
*/
	 DInt j=(&heap_buf[ind_heap_buf]-old);
//	 movedata(FP_SEG(old+bytes_old),FP_OFF(old+bytes_old) ,FP_SEG(old),FP_OFF(old), j-bytes_old);
	 memmove(old,old+bytes_old, j-bytes_old);
	 for (DInt i=0;i<MAX_PRGS; i++)
		if (ptr_panel->program_codes[i] > old)
			 ptr_panel->program_codes[i] -= bytes_old;
	 ind_heap_buf -= bytes_old;
	}
// clear_semaphore(&t3000_flag);
}

DByte *Heap_grp_alloc(DInt bytes)
{
 if (bytes==0)
	 return NULL;
 if( ind_heap_grp+bytes >= MAX_HEAP_GRP )
	 return NULL;
 ind_heap_grp += bytes;
 return (&heap_grp[ind_heap_grp - bytes]);
}

DInt Heap_grp_dealloc(DByte **old, DInt bytes_old)
{
 if (*old)
 {
 DByte *p,*q;
 DInt l;
 p=*old+bytes_old;
 l=(&heap_grp[ind_heap_grp]-*old)-bytes_old;
// movedata(FP_SEG(p),FP_OFF(p) ,FP_SEG(*old),FP_OFF(*old), l);
 memmove(*old,p, l);

 Control_group_elements *cgrp;
	 cgrp = ptr_panel->control_group_elements;
	 for (DInt i=0;i<MAX_GRPS; cgrp++, i++)
		if (cgrp->ptrgrp > *old)
			 cgrp->ptrgrp -= bytes_old/sizeof(Str_grp_element);
	 ind_heap_grp -= bytes_old;
 *old=NULL;
 }
}

DInt Heap_grp_resizedown(DByte **old, DInt width, DInt bytes)
{
 if (*old)
 {
	DByte *p,*q;
	DInt l;
	p=*old+width;
//	movedata(FP_SEG(p),FP_OFF(p) ,FP_SEG(*old),FP_OFF(*old), bytes);
	memmove(*old,p, bytes);

	Control_group_elements *cgrp;
	 cgrp = ptr_panel->control_group_elements;
	 for (DInt i=0;i<MAX_GRPS; cgrp++, i++)
		if (cgrp->ptrgrp > *old)
			 cgrp->ptrgrp -= width/sizeof(Str_grp_element);
	 ind_heap_grp -= width;
 }
}

DInt Heap_grp_resizeup(DByte **old, DInt bytes)
{
 if (bytes==0)
	 return 0;
 if( ind_heap_grp+bytes >= MAX_HEAP_GRP )
	 return 0;
 Control_group_elements *cgrp;
 cgrp = ptr_panel->control_group_elements;
 for (DInt i=0;i<MAX_GRPS; cgrp++, i++)
		if (cgrp->ptrgrp >= *old)
			 cgrp->ptrgrp += bytes/sizeof(Str_grp_element);
 ind_heap_grp += bytes;
 return 1;
}

int pointtotext(char *buf,Point_T3000 *point)
{
 char x[4];
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

int pointtotext(char *buf,Point_Net *point)
{
 char x[4];
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

char * intervaltotext( DLong seconds , DInt minutes , DInt hours, char *c )
{ /*  char * text;          /* return this string */
	char *separator = c ;
	char itoabuffer[12];       /* temporary sotrage for itoa */
	char textbuffer[12] ;
	textbuffer[0]=0;
	if(seconds<0)
	{
	 seconds=-seconds;
	 strcat(textbuffer, "-" ) ;        /* add the '-' */
	}
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

char *intervaltotext(char *textbuf, DLong seconds , DUint minutes , DUint hours, char *c)
{
	char buf[12], *textbuffer;
	char *separator = c ;
	textbuffer = buf;
	if( seconds < 0 )
	{
		seconds = -seconds;
		strcpy(textbuffer++, "-" ) ;        /* add the '-' */
	}
	if(*c!='-')
	{
	 hours += seconds/3600;
	 minutes += (unsigned)(seconds%3600)/60;
	 seconds = (unsigned)(seconds%3600)%60;
	}
	if( hours < 10 ) {
			strcpy(textbuffer++, "0" ) ;        /* add the leading zero 0#:##:## */
	}
	itoa(hours,textbuffer,10) ;
	textbuffer += strlen(textbuffer);
	strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/

	if( minutes < 10 ) {
			strcpy(textbuffer++, "0" ) ;        /* add the leading zero ##:0#:## */
	}
	itoa(minutes,textbuffer,10) ;
	textbuffer += strlen(textbuffer);
	strcpy(textbuffer++, separator ) ;        /* add the ":" separator*/
	if( seconds < 10 ) {
			strcpy(textbuffer++, "0" ) ;        /* add the leading zero ##:##:0# */
	}
	itoa(seconds,textbuffer,10)  ;

	if(textbuf) strcpy(textbuf, buf);
	return( buf ) ;
}

void ftoa(float f, DInt length, DInt ndec, char *buf)
{
 DInt i,n;
 char xxxtmp[20],*p,c;
 for(i=0;i<length-ndec;i++)
			buf[i]=' ';
 buf[length-ndec-1]='.';
 for(i=length-ndec;i<length;i++)
			buf[i]='0';
 if(f>1000000.00 || f<-1000000.00 || (f>-1.e-6 && f<1.e-6) )
	 gcvt(f, 5, xxxtmp);
 else
	 gcvt(f, length, xxxtmp);
 if ((p=strchr(xxxtmp, 'e'))!=NULL  ||  (p=strchr(xxxtmp, 'E'))!=NULL)
		strcpy(buf+length-strlen(xxxtmp),xxxtmp);
 else
 {
	c='.';
	if ((p=strchr(xxxtmp, c))!=NULL)
		{
		 memcpy(&buf[length-ndec],p+1,min(ndec,(int)strlen(p+1)));
		}
	else
		p=&xxxtmp[strlen(xxxtmp)];
	n=min(p-&xxxtmp[0],length-ndec-1);
	memcpy(&buf[length-ndec-1]-n,p-n,n);
 }
	buf[length]=0;
}

void longtoa(DLong f, DInt length, DInt ndec, char *buf)
{
	DInt n,i;
	char xxxtmp[20],*p,c, q[6];
	if(f>1000000000L || f<-1000000000L)
	{
	 ftoa((float)f/1000.,  length, ndec, buf);
	 return;
	}
	memset(buf, ' ', length-ndec-1);
	buf[length-ndec-1]='.';
	memset(&buf[length-ndec], '0', ndec);

	ltoa(f/1000, xxxtmp, 10);
	if( f<0 && f>-1000 )
	{
	 strcpy(xxxtmp,"-0");
	}
	n=strlen(xxxtmp);
	strcat(xxxtmp,".");
	p=&xxxtmp[n+1];
	f %= 1000;
	if(f<0) f = -f;
    q[0]=0;
	itoa(f, q, 10);
	if(f<100)
	{
	 memmove(&q[1], q, strlen(q)+1 );
	 q[0] = '0';
	}
	if(f<10)
	{
	 memmove(&q[1], q, strlen(q)+1 );
	 q[0] = '0';
	}
	memcpy(p, q, ndec);

	memcpy(&buf[length-ndec], p, min(ndec,(int)strlen(p)));
	n=min(n,length-ndec-1);
	memcpy(&buf[length-ndec-1]-n,p-n-1,n);
	buf[length]=0;
}

DInt GetRange_Out(DInt range)
{
	 DInt r;
	 r =range;
     return r;
}

DInt GetRange_In(DInt range)
{
 DInt r;
 if( ( table5 <= range && range <= DIG1 ) || (	range > custom_digital8 ) )
			{ r = 0 ; }
 else
			{r = range;	}
 return r;
}

DInt GetRange_Var(DInt range)
{
 int r;
/* translate the range to the appropriate range array text */
 r = range;
 return r;
}

DInt get_point_info(Point_info *point_info, char **des, char **label, char **ontext, char **offtext, char pri, DInt network)
{
 char buf[160], buf1[12],*q;
 Point_Net p;
 DInt num_point,num_net,num_panel,r,t=0;
 DByte point_type,var_type;
 union  {
		Str_out_point *p_out;
		Str_in_point *p_in;
		Str_variable_point *p_var;
		Str_controller_point *p_con;
		Str_weekly_routine_point *p_wr;
		Str_annual_routine_point *p_ar;
		Str_program_point *p_prg;
		Control_group_point *p_grp;
		Str_monitor_point *p_amon;
		Point_info *pinfo;
		} ptr;

 gdescriptor[0]=glabel[0]=0;
// memcpy(&p, &point_info->point, sizeof(Point_Net));
 p = point_info->point;
 if (!p.point_type) return ERROR_COMM;

 point_type = p.point_type-1;
 num_point = p.number;

 r = ptr_panel->info[point_type].str_size;
 memset(buf, 0, r);

 if( (p.panel+1==Station_NUM && (local_panel || (!local_panel&&( (pri==1)||(pri==3&&(p.network==0xFFFF)) ))  ))
//		|| local_request(p.panel+1)
      )//pri=0 remote priority
 {
  switch (point_type) {
	 case T3000_OUT:
					 if(num_point < ptr_panel->table_bank[point_type])
						 memcpy(buf, &ptr_panel->outputs[num_point], sizeof( Str_out_point ));
					 break;
	 case T3000_IN:
					 if(num_point < ptr_panel->table_bank[point_type])
						 memcpy(buf, &ptr_panel->inputs[num_point], sizeof( Str_in_point ));
					 break;
	 case VAR:
					 if(num_point < ptr_panel->table_bank[point_type])
						 memcpy(buf, &ptr_panel->vars[num_point], sizeof( Str_variable_point ));
					 break;
	 case CON:
					 if(num_point < ptr_panel->table_bank[point_type])
						 memcpy(buf, &ptr_panel->controllers[num_point], sizeof( Str_controller_point ));
					 break;
	 case WR:
					 if(num_point < ptr_panel->table_bank[point_type])
						 memcpy(buf, &ptr_panel->weekly_routines[num_point], sizeof( Str_weekly_routine_point ));
					 break;
	 case AR:
					 if(num_point < ptr_panel->table_bank[point_type])
						 memcpy(buf, &ptr_panel->annual_routines[num_point], sizeof( Str_annual_routine_point ));
					 break;
	 case PRG:
					 if(num_point < ptr_panel->table_bank[point_type])
						 memcpy(buf, &ptr_panel->programs[num_point], sizeof( Str_program_point ));
					 break;
	 case AMON:
					 if(num_point < ptr_panel->table_bank[point_type])
						 memcpy(buf, &ptr_panel->analog_mon[num_point],sizeof( Str_monitor_point ));
					 break;
	 case GRP:
					 if(num_point < ptr_panel->table_bank[point_type])
						 memcpy(buf, &ptr_panel->control_groups[num_point], sizeof( Control_group_point ));
/*
					 if(num_point < ptr_panel->table_bank[point_type])
						 memcpy(buf, ptr_panel->info[point_type].address+num_point*r, r);
					 break;
*/
	 }
	 t=1;
	 point_info->point_value=0;
	 point_info->auto_manual=0;
	 point_info->digital_analog=1;
	 point_info->security=0;
	 point_info->decomisioned=0;
	}
/*
	else
	{
	 if(pri==2)
	 {
	  if( p.network == 0xFFFF ) p.network = network;
		if( get_indiv_point(p, buf) != SUCCESS )
			 return ERROR_COMM;
		t=1;
		point_info->point_value=0;
		point_info->auto_manual=0;
		point_info->digital_analog=1;
		point_info->security=0;
		point_info->decomisioned=0;
	 }
	 else
	 {
//	  if( p.network == 0xFFFF ) p.network = network;
		point_info->point_value=0;
		point_info->auto_manual=0;
		point_info->digital_analog=1;
		point_info->security=0;
		point_info->decomisioned=0;
		r=0;
		for(int i=0;i<MAXREMOTEPOINTS82;i++)
		{
		if( !remote_points_list[i].info.point.zero() )
		{
		 if( remote_points_list[i].info.point==p )
		 {
		  memcpy(buf, &remote_points_list[i].info, sizeof(Point_info));

		  ptr.pinfo = (Point_info *)buf;
		  point_info->point_value=ptr.pinfo->point_value;
			point_info->auto_manual=ptr.pinfo->auto_manual;
		  point_info->digital_analog=ptr.pinfo->digital_analog;
		  point_info->security=ptr.pinfo->security;
		  point_info->decomisioned=ptr.pinfo->decomisioned;
		  r=ptr.pinfo->units;
		  point_info->units=r;
		  if(!point_info->digital_analog)
		  {
				if(r < 0) r = 0;
				num_point=dig_range_array[r].value % 2;
				if(ontext)
				 *ontext= &(dig_range_array[r].dunits[!num_point][0]);
				if(offtext)
				 *offtext= &(dig_range_array[r].dunits[num_point][0]);
		  }
		  break;
		 }
		}
	  }
	  pointtotext(buf1,&p);
	  q=ispoint(rtrim(buf1),&num_point,&var_type, &point_type, &num_panel, &num_net, networkaddress, Station_NUM);
	  strcpy(glabel,q);
		if(Des)
	  {
		if(point_type!=CON && point_type!=TBL)
			q = Des->look_descriptor(num_panel,point_type,num_point);
		if(!q) q = glabel;
		strcpy(gdescriptor,q);
	  }
	 } //if pri==2
	}
*/
 ptr.p_out = (Str_out_point *)buf;
 pointtotext(gpointname,&p);
 switch (point_type) {
	 case T3000_OUT:
					 {
					  if(t)
					  {
						point_info->point_value=ptr.p_out->value;
						point_info->auto_manual=ptr.p_out->auto_manual;
						point_info->digital_analog=ptr.p_out->digital_analog;
						point_info->security=ptr.p_out->access_level;
						point_info->decomisioned=ptr.p_out->decom;
						r = GetRange_Out(ptr.p_out->range);
						if(!ptr.p_out->digital_analog)
						{
							r -= DIG1;
							if(r < 0) r = 0;
							if(ontext)
							 *ontext= &(dig_range_array[r].dunits[1][0]);
							if(offtext)
							 *offtext= &(dig_range_array[r].dunits[0][0]);
						}
						point_info->units = r;
						strcpy(gdescriptor,ptr.p_out->description);
						strcpy(glabel,ptr.p_out->label);
					  }
					 }
					break;
	 case T3000_IN:
					 {
					  if(t)
						{
						point_info->point_value   =ptr.p_in->value;
						point_info->auto_manual   =ptr.p_in->auto_manual;
						point_info->digital_analog=ptr.p_in->digital_analog;
						point_info->decomisioned  =ptr.p_in->decom;
						r = GetRange_In(ptr.p_in->range);
						if(!ptr.p_in->digital_analog)
						{
							r -= DIG1;
							if(r < 0) r = 0;
							if(ontext)
							 *ontext= &(dig_range_array[r].dunits[1][0]);
							if(offtext)
							 *offtext= &(dig_range_array[r].dunits[0][0]);
						}
						point_info->units=r;
						strcpy(gdescriptor,ptr.p_in->description);
						strcpy(glabel,ptr.p_in->label);
					  }
						break;
					 }
	 case VAR:
					 {
						if(t)
						{
						point_info->point_value=ptr.p_var->value;
						point_info->auto_manual=ptr.p_var->auto_manual;
						point_info->digital_analog=ptr.p_var->digital_analog;
						r = GetRange_Var(ptr.p_var->range);
						if(!ptr.p_var->digital_analog)
						{
							r -= DIG1;
							if(r < 0) r = 0;
							if(ontext)
							 *ontext= &(dig_range_array[r].dunits[1][0]);
							if(offtext)
							 *offtext= &(dig_range_array[r].dunits[0][0]);
						}
						point_info->units=r;
						strcpy(gdescriptor,ptr.p_var->description);
						strcpy(glabel,ptr.p_var->label);
					  }
					 }
					break;

	 case CON:
					{
						if(t)
						{
						 point_info->point_value = ptr.p_con->value;
						 point_info->auto_manual = ptr.p_con->auto_manual;
						 point_info->digital_analog=1;
						 point_info->units=procent;
						}
					}
					break;
	 case WR:
					 {
					  if(t)
					  {
						 point_info->point_value=ptr.p_wr->value;
						 point_info->auto_manual=ptr.p_wr->auto_manual;
						 point_info->digital_analog=0;
						 point_info->units=1;
						 strcpy(gdescriptor,ptr.p_wr->description);
						 strcpy(glabel,ptr.p_wr->label);
						}
						if(ontext)
							 *ontext= on;
						if(offtext)
							 *offtext= off;
						break;
					 }
					break;
	 case AR:
					 {
						if(t)
						{
						 point_info->point_value=ptr.p_ar->value;
						 point_info->auto_manual=ptr.p_ar->auto_manual;
						 point_info->digital_analog=0;
						 point_info->units=1;
						 strcpy(gdescriptor,ptr.p_ar->description);
						 strcpy(glabel,ptr.p_ar->label);
						}
						if(ontext)
							 *ontext= on;
						if(offtext)
							 *offtext= off;
						break;
					 }
					break;
	 case PRG:
					 {
						if(t)
						{
						 point_info->point_value=ptr.p_prg->on_off;
						 point_info->auto_manual=ptr.p_prg->auto_manual;
						 point_info->digital_analog=0;
						 point_info->units=1;
						 strcpy(gdescriptor,ptr.p_prg->description);
						 strcpy(glabel,ptr.p_prg->label);
						}
						if(ontext)
							 *ontext= on;
						if(offtext)
							 *offtext= off;
					 }
						break;
	 case AMON:
					 {
						if(t)
						{
						 point_info->point_value=ptr.p_amon->status;
						 point_info->digital_analog=0;
						 point_info->units=1;
						 gdescriptor[0]=0;
						 strcpy(glabel,ptr.p_amon->label);
						}
						if(ontext)
							 *ontext= on;
						if(offtext)
							 *offtext= off;
					 }
					break;
	 case GRP:
					 {
						if(t)
						{
						 point_info->digital_analog=0;
						 point_info->units=1;
						 strcpy(gdescriptor,ptr.p_grp->description);
						 strcpy(glabel,ptr.p_grp->label);
						}
					 }
						break;
	 }
	 if(des)
		 *des = gdescriptor;
	 if(label)
		 *label = glabel;
	return SUCCESS;
}

char *rtrim(char *text)
{
	DInt n,i;
	n=strlen(text);
	for (i=n-1;i>=0;i--)
			if (text[i]!=' ')
					break;
	text[i+1]='\0';
	return text;
}

char *ltrim(char *text)
{
	DInt n,i;
	n=(DInt)strlen(text);
	for (i=0;i<n;i++)
			if (text[i]!=' ')
					break;
	strcpy(text,text+i);
	return text;
}

char *islabel(char *token, DInt *num_point, DByte *var_type, DByte *point_type, DInt *num_panel)
{
 char *p;
 DInt i,j;

/*
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
*/
 for(i=T3000_OUT;i<=AY;i++)
//		for(j=0;j<ptr_panel->info[i].max_points;j++)
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
						 strcat(token,"-");    //lin);
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
/*
 if(local_panel)
 {
	if(Des)
	{
		p=Des->islabel(token,num_point,var_type,point_type,num_panel);
	}
 }
*/
 return(p);
}


char *look_label(DInt panel, DInt point_type, DInt num, DInt network)
{
 char *p=NULL;
// if(local_panel)
// if( local_request(panel, network) )
	 if(panel == Station_NUM)
			{
				switch (point_type) {
						case T3000_OUT:
									p = ptr_panel->outputs[num-1].label;
									break;
						case T3000_IN:
									p = ptr_panel->inputs[num-1].label;
									break;
						case VAR:
									p = ptr_panel->vars[num-1].label;
									break;
						case CON:
									return 0;
//									p = ptr_panel->controllers[num].label;
//									break;
						case WR:
									p = ptr_panel->weekly_routines[num-1].label;
									break;
						case AR:
									p = ptr_panel->annual_routines[num-1].label;
									break;
						case PRG:
									p = ptr_panel->programs[num-1].label;
									break;
						case AMON:
									p = ptr_panel->analog_mon[num-1].label;
									break;
						case AY:
									p = ptr_panel->arrays[num-1].label;
									break;
						default:
									break;
						}
						if(p)
						{
						 p=rtrim(p);
						 if (strlen(p) == 0)
							return 0;
						 else
							return p;
						}
						else
							return 0;

			}
	p=0;
/*
	if(point_type!=CON && point_type!=TBL)
	{
	 if(Des)
		p = Des->look_label(panel,point_type,num);
	}
*/
	return p;
}


char *ispoint(char *token,DInt *num_point, DByte *var_type, DByte *point_type, DInt *num_panel, DInt *num_net, DInt network, DByte panel, DInt *netpresent)
{
DInt j,k,l/*,i*/;
char pc[11],*p,*q,buf[21];
			 if(netpresent) *netpresent = 0;
			 *num_net = network;
			 if(strlen(token)==0) return 0;
			 if ((q=strchr(token,'.'))!=NULL)
			 {
						memcpy(pc,token,min( (int)(q-token),10));
						pc[min((int)(q-token),10)]=0;
						q++;
						*num_net = atoi(pc);
						token = q;
						if(netpresent)
							*netpresent = *num_net;
			 }
			 if ((q=strchr(token,'-'))!=NULL)
			 {
						memcpy(pc,token,min( (int)(q-token),10));
						pc[min((int)(q-token),10)]=0;
						q++;
						*num_panel = (DInt)atoi(pc);
						if ( *num_panel>32 || *num_panel==0 )
										 q=token;
			 }
			 else
			 {
					for(l=0;l<3 && token[l]!=0;l++)
						if (token[l]<'0' || token[l]>'9')
								 break;
					q=token+l;
					memcpy(pc,token,l);
					pc[l]=0;
					*num_panel = (DInt)atoi(pc);
					if (*num_panel==0)
					  *num_panel = panel;
					if ( *num_panel>32 && strlen(pc) )
										 return(islabel(token,num_point,var_type,point_type,num_panel));
			 }
			 if ((p=strpbrk(q, "123456789"))!=NULL)
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
/*
												memcpy(pmes,"error line : ",13);
												pmes += 13;
												itoa(lline,pmes,10);
												pmes += strlen(pmes);
												*pmes++ = 0x0d;
//												fprintf(pmes,"error line %d\n",line);
												error=TRUE;return 0;
*/
									}
					else  if ((strlen(p)==1) && (*p=='0')) {
/*
																memcpy(pmes,"error line : ",13);
																pmes += 13;
																itoa(lline,pmes,10);
																pmes += strlen(pmes);
																*pmes++ = 0x0d;
																fprintf(pmes,"error line %d\n",line);
    															error=TRUE;return 0;
*/
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
														*num_point=(DInt)atoi(p);
														*point_type = k;
														if(*num_panel<10 || *num_point<100)
															strcat(buf,"-");
														strcat(buf,pc);
														strcat(buf,p);
														strcpy(token,buf);
														if((p=look_label(*num_panel,k,*num_point,*num_net)) != NULL)
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
// return(islabel(token,num_point,var_type,point_type,&ptr_panel->GlPanel));
			 return(islabel(token,num_point,var_type,point_type,num_panel));
}

