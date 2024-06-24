//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
#include <stdlib.h>

#include "decript.h"
//---------------------------------------------------------------------------
//--------------------------------------------------------//
//                                                        //
//   File:    IMAGSTORE.CPP                               //
//                                                        //
//   Desc:    Memory management classes for storing and   //
//            manipulating bitmapped images.              //
//                                                        //
//--------------------------------------------------------//

#include <dos.h>

#include "t3000def.h"
//#include "fxengine.h"
//#include "tifcodec.hpp"
//#include "imgviewr.hpp"
#include "aio.h"
//#include "imgstore.hpp"
//#include "baseclas.h"
//#include "parser.h"
//#include "mtkernel.h"

#define OKAY 1
#define NOMEM 0
#define NOUSE 2

extern Panel *ptr_panel;
extern signed char point_des[AY+1];

//char buffer[4096];
char *pixunder1;
//DInt ccc;
//................Conventional Memory

#include "xms.h"

XmsDes::XmsDes( DInt nbytes)
{
				 memset( buffer, 0, 4096 );
				 ptr = new DByte [nbytes];
				 if( ptr != 0 )
				 {
					status = OKAY;
					netlength = 0;
					memset(des_index_table,0,32*sizeof(Des_index_table));
					memset(ptr,0,nbytes);
				 }
				 else
					status = NOMEM;
}

XmsDes::~XmsDes( )
{
	if( status == OKAY )
	 {
        delete ptr;
	 }
}

DInt XmsDes::put( DByte panel, DByte point_type, DInt newlength )
{
   DLong srcoffset, dstoffset, nbytes;
   DInt i;
	if( status == OKAY )
		{
				DInt point_length = des_index_table[panel-1].des_length[point_type];
				DInt panel_length = des_index_table[panel-1].panel_length;

				if(newlength%2)
					buffer[newlength++] = 0;

				srcoffset = 0;
				for(i=0; i<panel-1; i++)
					srcoffset += des_index_table[i].panel_length;
				for(i=0; i<=point_type; i++)
					srcoffset += des_index_table[panel-1].des_length[i];

				dstoffset = srcoffset + newlength-point_length;
				nbytes = netlength-srcoffset;

				memmove(ptr+dstoffset, ptr+srcoffset, nbytes);

				nbytes = newlength;
				dstoffset = srcoffset - des_index_table[panel-1].des_length[point_type];
				memmove(ptr+dstoffset, buffer, nbytes);

				des_index_table[panel-1].panel_length += newlength-point_length;
				des_index_table[panel-1].des_length[point_type] = newlength;
				netlength += newlength-point_length;

		}
}

DByte *XmsDes::get( DByte panel, DByte point_type )
{
	DLong srcoffset, dstoffset;
	DInt  nbytes,i;
	if( status == OKAY )
		{
				nbytes = des_index_table[panel-1].des_length[point_type];
				if( nbytes )
				{
				 srcoffset = 0;
				 for(i=0; i<panel-1; i++)
					srcoffset += des_index_table[i].panel_length;
				 for(i=0; i<point_type; i++)
					srcoffset += des_index_table[panel-1].des_length[i];

				 memmove(buffer, ptr+srcoffset, nbytes);
				 return (char *)buffer;
				}
		}
	return 0;
}

DByte *XmsDes::look_label(DByte panel, DByte point_type, DByte num)
{
 DInt n;
 DInt i;
 if( status == OKAY )
 {
  if(get(panel, point_type)!=NULL)
  {
	n=0;
	i=0;
	for(DInt k=0;k<num-1;k++)
	{
	  if(point_type!=AMON && point_type!=AY)
	  {
//	 n=buffer[i++];       //descriptor
//	 i += n;
		while(buffer[i++]);
//	 if(buffer[i-1])	i++;
	  }
	  while(buffer[i++]);
//	 if(buffer[i-1])	i++;
	}
	if(point_type!=AMON && point_type!=AY)
	{
//	 n=buffer[i++];
//	 i += n;
	 while(buffer[i++]);      //descriptor
//	 if(buffer[i-1])	i++;
	}
//	n=buffer[i++];
//	if(n)
	if(buffer[i])
	 return &buffer[i];
	else
	 return 0;
/* for(DInt i=0;i<32;i++)
 {
	for(j=0;j<DAY;j++)
	{
	 get(i+1, j);
	 for(k=0;k<des_index_table[i].des_length[j]
*/
  }
 }
 return 0;
}

DByte *XmsDes::islabel(DByte *token,DByte *num_point,DByte *var_type,DByte *point_type,DByte *num_panel)
{
 DInt n,l,m;
 if( status == OKAY )
  for(DInt i=0;i<32;i++)
	for(DInt j=0;j<AY;j++)
	{
//	 DInt m=point_des[j];
//	 if(m>=0)
	 m=j;
	 {
		get(i+1, m);
		n=0;
		l=0;
		for(DInt k=0;k<des_index_table[i].des_length[m];)
		{
			l++;
			if(m!=AMON && m!=AY)
			{
//				n=buffer[k++];  // descriptor
//				k += n;
					while(buffer[k++]);
//					if(buffer[k-1])	k++;
			}
//			n=buffer[k++];
//			if(n)
			if(buffer[k])
			{
				if(!strcmp(token,&buffer[k]))
				 {
					char point[12],x[5];
					*num_point=l;
					*point_type=j;
					*num_panel=i+1;
					 *var_type = LABEL_VAR;
					 itoa(*num_panel,point,10);
					 strcat(point,"-");
					 strcat(point,ptr_panel->info[j].name);
					 strcat(point,itoa(l,x,10));
					 strcpy(token,point);
					 return(&buffer[k]);
				 }
			}
			while(buffer[k++]);    //step over label
		}
	 }
	}
 return 0;
}

DByte *XmsDes::look_descriptor(DByte panel, DByte point_type, DByte num)
{
 DInt n;
 DInt i;
 if( status == OKAY )
 {
  if(get(panel, point_type)!=NULL)
  {
	n=0;
	i=0;
	for(DInt k=0;k<num-1;k++)
	{
	  if(point_type!=AMON && point_type!=AY)
	  {
		while(buffer[i++]);
	  }
	  while(buffer[i++]);
	}
//	if(point_type!=DAMON && point_type!=DAY)
//	{
//	 while(buffer[i++]);      //descriptor
//	}
	if(buffer[i])
	 return &buffer[i];
	else
	 return 0;
  }
 }
 return 0;
}


/*
void XmsDes::cpy( Byte *dest, DInt row )
{
	if( status == imgstoreOKAY )
		if( (row >= 0) && (row < rowcnt) )
			memcpy( dest, get(row), rowbytes );
}

void XmsImgStore::cpy( Byte *dest, DInt row,
											 DInt col, DInt cnt )
{
	if( status == imgstoreOKAY )
			if( (row >= 0) && (row < rowcnt) )
			memcpy( dest, get(row)+col, cnt );
}

*/

//................XMS Memory

/*
XmsMem::XmsMem(  )
{
	 if( XMS_installed() )
		{
		  memset(table,0,4*8);
		  status = NOUSE;
		  nitems = 0;
		 }
		else
		{
			status = NOMEM;
		}
}
XmsMem::XmsMem( unsigned Long nbytes )
{
	 if( XMS_installed() )
	{
			// total xms bytes required
			// convert to KB
			nbytes += 1023;
			nbytes /= 1024;
			DInt n = (DInt) nbytes;

				 // and attempt to allocate
//				 handle = XMS_alloc( n );
				 if( handle != 0 )
				 {
				  memset(table,0,4*8);
				  status = OKAY;
				  nitems = 0;
				 }
				else
				{
					status = NOMEM;
				}
		}
		else
			status = NOMEM;
}

XmsMem::~XmsMem( )
{
	if( status == OKAY )
	 {
//	  ;
		XMS_free( handle );
	 }
}

void XmsMem::put( char *data , unsigned Long l)
{
				c2x.nbytes = l;
				c2x.srchandle = 0;
//				c2x.srcoffset = (Long)(((Long)FP_SEG(data))*16+(Long)FP_OFF(data));
				c2x.srcoffset = (Long)data;
				c2x.dsthandle = handle;
				c2x.dstoffset = 0;
				if(nitems)
					c2x.dstoffset = table[nitems-1].offset+table[nitems-1].length;
			table[nitems].length = l;
			table[nitems].offset = c2x.dstoffset;
			nitems++;
         disable();
			XMS_move( &c2x );
			enable();

}

void XmsMem::get( DInt item , char *buffer)
{
	if( item < nitems )
		{
			x2c.nbytes = table[item].length;
			x2c.srchandle = handle;
			x2c.srcoffset = table[item].offset;
			x2c.dsthandle = 0;
			x2c.dstoffset = (Long)buffer;
			disable();
			XMS_move( &x2c );
				 enable();
		}
}

void XmsMem::Save( DInt sx, DInt sy, DInt jx, DInt jy)
{
			DInt v;
			unsigned Long big= ((unsigned Long)(jx-sx)*(unsigned Long)(jy-sy)*4+7)/8;
			unsigned Long big1=big;
			big1 += 1023;
			big1 /= 1024;
			DInt n = (DInt) big1;
			if(n<26)
				v = 1;
			else
				v = 4;

			handle = XMS_alloc( n+4 );

			if(handle)
			{
			 status = OKAY;
			if(v==1)
			{
//			pixunder1=(char far *)farmalloc((unsigned Long)big);
//			if(pixunder1==NULL) exit(0);
			mgetimage(sx, sy, jx, jy,pixunder1);
			put(pixunder1,big);
//			farfree(pixunder1);
			}
			else
			{
			DInt big1 = imagesize(sx, sy, jx, sy+(jy-sy)/4);
//			pixunder1=(char far *)farmalloc((unsigned Long)big1);
			mgetimage(sx, sy, jx, sy+(jy-sy)/4,pixunder1);
			put(pixunder1,big1);
//			mfarfree(pixunder1);
			big1 = imagesize(sx, sy+(jy-sy)/4, jx, sy+(jy-sy)/2);
//			pixunder1=(char far *)farmalloc((unsigned Long)big1);
			mgetimage(sx, sy+(jy-sy)/4,  jx,   sy+(jy-sy)/2,pixunder1);
			put(pixunder1,big1);
//			mfarfree(pixunder1);
			big1 = imagesize(sx, sy+(jy-sy)/2,jx, sy+3*(jy-sy)/4);
//			pixunder1=(char far *)farmalloc((unsigned Long)big1);
			mgetimage(sx, sy+(jy-sy)/2, jx,  sy+3*(jy-sy)/4,pixunder1);
			put(pixunder1,big1);
//			mfarfree(pixunder1);
			big1 = imagesize(sx, sy+3*(jy-sy)/4, jx,jy);
//			pixunder1=(char far *)farmalloc((unsigned Long)big1);
			mgetimage(sx,	 sy+3*(jy-sy)/4, 	 jx,  	 jy,pixunder1);
			put(pixunder1,big1);
//			farfree(pixunder1);
			}
			}
			else
			{
			 status=NOMEM;
			}
}

void XmsMem::Restore(DInt ltopx, DInt ltopy, DInt rbottomx, DInt rbottomy)
{
//			pixunder1=(char far *)farmalloc((unsigned Long)table[0].length+1000);
			get( 0 , pixunder1);
			mputimage(ltopx,ltopy,pixunder1,COPY_PUT);
//			mfarfree(pixunder1);
			if(nitems!=1)
			{
//			pixunder1=(char far *)farmalloc((unsigned Long)table[1].length);
			get( 1 , pixunder1);
			mputimage((DInt)ltopx,(DInt)(ltopy+(rbottomy-ltopy)/4),pixunder1,COPY_PUT);
//			mfarfree(pixunder1);
//			pixunder1=(char far *)farmalloc((unsigned Long)table[2].length);
			get( 2 , pixunder1);
			mputimage((DInt)ltopx,(DInt)(ltopy+(rbottomy-ltopy)/2),pixunder1,COPY_PUT);
//			mfarfree(pixunder1);
//			pixunder1=(char far *)farmalloc((unsigned Long)table[3].length);
			get( 3 , pixunder1);
			mputimage((DInt)ltopx,(DInt)ltopy+3*(rbottomy-ltopy)/4,pixunder1,COPY_PUT);
//			mfarfree(pixunder1);
			}
//			mfarfree(pixunder1);

	if( status == OKAY )
	 {
//		XMS_free( handle );
		nitems = 0;
//		status = NOUSE;
		memset(table,0,4*8);
	 }

}
*/