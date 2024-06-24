//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include <windows.h>
#include "pool.h"
#include "netb_bac.h"
//---------------------------------------------------------------------------
void POOL::init_pool(DByte *p, DInt l)
{
 Header_pool *h;
 length = l;
 first = last = access = 0;
 buf=p;
 h = (Header_pool *)buf;
 h->status = 0;
 h->length=l-sizeof(Header_pool);
 h->next=0;
 h->last=0xFFFF;
}

POOL::POOL(DByte *p, DInt l)
{
 init_pool(p, l);
}

POOL::POOL(void)
{
}

DByte *POOL::put(DByte *p, DInt l)
{
 DInt i,j;
 Header_pool *h;
 h = (Header_pool *)(buf+last);
 if(h->status) return 0;
 if( h->length < l+sizeof(Header_pool) ) return 0;
 j = h->length;
 h->length = l;
 h->status = 1;
 i = last + sizeof(Header_pool) + l;
 if( i >= length )
 {
  h->next = i-length;
  memcpy( ((DByte *)h)+sizeof(Header_pool), p, length-(last + sizeof(Header_pool)) );
  memcpy( buf, p+length-(last + sizeof(Header_pool)), i-length );
 }
 else
 {
  h->next = i;
  memcpy( ((DByte *)h)+sizeof(Header_pool), p, l );
  if( i+sizeof(Header_pool) >= length )
  {
	h->length += length - i;
	l = h->length;
	h->next = 0;
  }
 }
 i = last;
 last = h->next;
 h = (Header_pool *)(buf+last);
 h->status = 0;
 h->length = j - l - sizeof(Header_pool);
 h->next   = 0;
 h->last   = i;
}

DByte *POOL::next(void)
{
 if( ((Header_pool *)(buf+first))->status )
	return ( buf+first+sizeof(Header_pool) );
 else
	return 0;
}

void POOL::release(void)
{
 Header_pool *h;
 DInt i;
 h = (Header_pool *)(buf+last);
 if(h->status)
 {
  h->next = first;
  i = last;
  first = ((Header_pool *)(buf+first))->next;
  last = h->next;
  h = (Header_pool *)(buf+last);
  h->status=0;
  h->next = 0;
  h->last = i;
 }
 else
 {
  h->length += ((Header_pool *)(buf+first))->length + sizeof(Header_pool);
  first = ((Header_pool *)(buf+first))->next;
 }
}

DUint POOL::alloc(DInt l)
{
 DUint i,j,offset,next;
 Header_pool *h;
 if (access) return 0;
 h = (Header_pool *)buf;
 offset = 0;
 while(1)
 {
	if( h->status || (!h->status && h->length < l+sizeof(Header_pool)+1) )
	{
	  if(h->next)
	  {
		 offset = h->next;
		 h = (Header_pool *)(buf+(h->next));
	  }
	  else
	  {
		 offset = 0;
		 break;
	  }	 
	}
	else
	{
//	  p = (DByte *)h;
	  next = h->next;
	  j = h->length;
	  h->length = l;
	  h->status = 1;
	  i = offset + sizeof(Header_pool) + l;
	  h->next = i;
	  h = (Header_pool *)(buf+i);
	  h->status = 0;
	  h->length = j - l - sizeof(Header_pool);
	  h->next   = next;
	  h->last   = offset;
	  if(h->next)
	  {
		h = (Header_pool *)(buf+next);
		h->last   = i;
	  }
	  offset += sizeof(Header_pool);
	  break;
	}
 }
 return offset;
}

void POOL::free(DInt offset)
{
 Header_pool *h,*h1;
 if(offset==0xFFFF) return;
 access = 1;
 offset -= sizeof(Header_pool);
 h = (Header_pool *)(buf+offset);
 h->status = 0;
/*
 h = (Header_pool *)buf;
 while(h->next)
 {
  h = (Header_pool *)(buf+(h->next));
 }
 while(h->last!=0xFFFF)
 {
  if( !h->status )
  {
	h1 = (Header_pool *)(buf+(h->last));
	if( !(h1->status ) )
	{
	 h1->length += h->length + sizeof(Header_pool);
	 h1->next = h->next;
	 if(h->next)
	 {
	  h1 = (Header_pool *)(buf+(h->next));
	  h1->last = h->last;
	 }
	}
  }
  h = (Header_pool *)(buf+(h->last));
 }
*/

 if(h->next)
 {
  h1 = (Header_pool *)(buf+(h->next));
  if( !(h1->status ) )
  {
	h->length += h1->length + sizeof(Header_pool);
	h->next = h1->next;
	if(h->next)
	{
	 h1 = (Header_pool *)(buf+(h->next));
	 h1->last = offset;
	}
  }
 }
 if(h->last!=0xFFFF)
 {
  h1 = (Header_pool *)(buf+(h->last));
  if( !(h1->status ) )
  {
	h1->length += h->length + sizeof(Header_pool);
	h1->next = h->next;
	if(h->next)
	{
	 h1 = (Header_pool *)(buf+(h->next));
	 h1->last = h->last;
	}
  }
 }

 access = 0;
}
