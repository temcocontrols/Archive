//---------------------------------------------------------------------------
// Borland C++Builder
// Copyright (c) 1987, 1997 Borland International Inc.  All Rights Reserved.
//---------------------------------------------------------------------------
#ifndef TTDBGrid_HPP
#define TTDBGrid_HPP

#include <Menus.hpp>
#include <SysUtils.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <Graphics.hpp>
#include <Forms.hpp>
#include <Windows.hpp>
#include <Messages.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <System.hpp>
#include <dbgrids.hpp>


class PACKAGE TTDBGrid : public TDBGrid
{
 public:
   __fastcall virtual TTDBGrid(TComponent *AOwner);
   long __fastcall col();
   long __fastcall leftCol();
   void __fastcall col(long c);
   void __fastcall leftCol(long lc);
   long __fastcall row();
   long __fastcall topRow();
   void __fastcall row(long r);
   void __fastcall topRow(long tr);
   void __fastcall EditorMode(bool);
   void __fastcall ToggleEditorMode();

 __published:

    __property OnMouseDown;
    __property OnClick;
    __property TopRow;

};

#endif  // Calendar_HPP
