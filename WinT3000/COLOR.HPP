//--------------------------------------------------------//
//                                                        //
//   File:    COLOR.HPP                                   //
//                                                        //
//   Desc:    Classes for implementation of various       //
//            color models and their conversion.          //
//                                                        //
//--------------------------------------------------------//

#ifndef _COLOR_HPP_
#define _COLOR_HPP_

#include "t3000def.h"
struct rgb;
struct cmy;
struct cmyk;

//...............an RGB class

struct rgb
{
   DByte red;
   DByte grn;
   DByte blu;

	rgb( )
	{
		red = grn = blu = 0;
	}
	rgb( DByte r, DByte g, DByte b )
	{
		red = (DByte)r;
		grn = (DByte)g;
		blu = (DByte)b;
	}
	rgb( rgb& x )
	{
		red = x.red;
		grn = x.grn;
		blu = x.blu;
	}

	rgb( cmy& x );
   rgb( cmyk& x );
   DByte graylevel( void );
	void setrgb( DByte r, DByte g, DByte b );

	rgb& operator = ( rgb& x )
	{
      red = x.red;
		grn = x.grn;
		blu = x.blu;
		return *this;
	}
	rgb operator << ( DByte i )
	{
//	  rgb x( red<<i, grn<<i, blu<<i );
	  rgb x;
      return x;
   }
	rgb& operator <<= ( DByte i )
   {
      red <<= (DByte)i;
      grn <<= (DByte)i;
      blu <<= (DByte)i;
      return *this;
   }
   rgb operator >> ( DByte i )
   {
//      rgb x( red>>(DByte)i, grn>>(DByte)i, blu>>(DByte)i );
      rgb x;
      return x;
	}
	rgb& operator >>= ( DByte i )
	{
		red >>= (DByte)i;
		grn >>= (DByte)i;
		blu >>= (DByte)i;
		return *this;
	}

};

//...............a CMY class

struct cmy
{
   DByte cyn;
   DByte mag;
   DByte yel;

   cmy( )
   {
      cyn = mag = yel = 0;
   }
   cmy( DInt c, DInt m, DInt y )
   {
      cyn = (DByte)c;
      mag = (DByte)m;
      yel = (DByte)y;
   }
   cmy( cmy& x )
   {
      cyn = x.cyn;
      mag = x.mag;
      yel = x.yel;
   }

   cmy( rgb& x );
   cmy( cmyk& x );
   DByte graylevel( void );

   cmy& operator = ( cmy& x )
   {
      cyn = x.cyn;
      mag = x.mag;
      yel = x.yel;
      return *this;
   }
   cmy operator << ( DByte i )
   {
//      cmy x( cyn<<(DByte)i, mag<<(DByte)i, yel<<(DByte)i );
      cmy x;
       return x;
   }
   cmy& operator <<= ( DByte i )
   {
      cyn <<= (DByte)i;
      mag <<= (DByte)i;
      yel <<= (DByte)i;
      return *this;
   }
   cmy operator >> ( DByte i )
   {
//      cmy x( cyn>>(DByte)i, mag>>(DByte)i, yel>>(DByte)i );
      cmy x;
      return x;
   }
   cmy& operator >>= ( DByte i )
   {
      cyn >>= (DByte)i;
      mag >>= (DByte)i;
      yel >>= (DByte)i;
      return *this;
   }

};

//...............a CMYK class

struct cmyk
{
/*
   DByte cyn;
   DByte mag;
   DByte yel;
   DByte blk;

   cmyk( )
   {
      cyn = mag = yel = blk = 0;
   }
   cmyk( DInt c, DInt y, DInt m, DInt k )
   {
      cyn = (DByte)c;
      mag = (DByte)m;
      yel = (DByte)y;
      blk = (DByte)k;
   }
   cmyk( cmyk& x )
   {
      cyn = x.cyn;
      mag = x.mag;
      yel = x.yel;
      blk = x.blk;
   }

   cmyk( rgb& x );
   cmyk( cmy& x );
   DByte graylevel( void );

   cmyk& operator = ( cmyk& x )
   {
      cyn = x.cyn;
      mag = x.mag;
      yel = x.yel;
      blk = x.blk;
      return *this;
   }
   cmyk operator << ( DByte i )
   {
      cmyk x( cyn<<(DByte)i, mag<<(DByte)i, yel<<(DByte)i, blk<<(DByte)i );
      return x;
   }
   cmyk& operator <<= ( DByte i )
   {
      cyn <<= (DByte)i;
      mag <<= (DByte)i;
      yel <<= (DByte)i;
      blk <<= (DByte)i;
      return *this;
   }
   cmyk operator >> ( DByte i )
   {
      cmyk x( cyn>>(DByte)i, mag>>(DByte)i, yel>>(DByte)i, blk>>(DByte)i );
      return x;
   }
   cmyk& operator >>= ( DByte i )
   {
      cyn >>= (DByte)i;
      mag >>= (DByte)i;
      yel >>= (DByte)i;
      blk >>= i;
      return *this;
   }
*/
};

//...............an RGB palette class

struct RgbPalette
{
   rgb *colors;
   DInt  ncolors;

   RgbPalette( )
   {
      colors = 0;
      ncolors = 0;
   }
   RgbPalette( rgb *clrs, DInt nclrs )
   {
      colors = clrs;
      ncolors = nclrs;
   }
  ~RgbPalette( )
   {
   }
};

#endif
