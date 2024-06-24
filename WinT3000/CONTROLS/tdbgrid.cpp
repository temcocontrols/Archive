//---------------------------------------------------------------------------
// Borland C++Builder
// Copyright (c) 1987, 1997 Borland International Inc.  All Rights Reserved.
//---------------------------------------------------------------------------
#if !defined (REGISTER_ALL_CONTROLS)
  #include  "tdbgrid.h"
#else
  #include "controls\tdbgrid.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init);
//---------------------------------------------------------------------------

//#pragma resource "*.res"  //IDE links .res automatically for components

namespace Tdbgrid{
void __fastcall PACKAGE Register()
{
  TComponentClass classes[1] = {__classid(TTDBGrid)};
  RegisterComponents("WinT3000", classes, 0);
}
} //namespace

//---------------------------------------------------------------------------
__fastcall TTDBGrid::TTDBGrid(TComponent *AOwner) :
  TDBGrid(AOwner)
{
}
//---------------------------------------------------------------------------
long __fastcall TTDBGrid::col()
{
 return Col;
}
//---------------------------------------------------------------------------
long __fastcall TTDBGrid::leftCol()
{
 return LeftCol;
}
//---------------------------------------------------------------------------
void __fastcall TTDBGrid::col(long c)
{
  Col = c;
}
//---------------------------------------------------------------------------
void __fastcall TTDBGrid::leftCol(long lc)
{
 LeftCol = lc;
}
//---------------------------------------------------------------------------
long __fastcall TTDBGrid::row()
{
 return Row;
}
//---------------------------------------------------------------------------
long __fastcall TTDBGrid::topRow()
{
 return TopRow;
}
//---------------------------------------------------------------------------
void __fastcall TTDBGrid::row(long r)
{
 Row = r;
}
//---------------------------------------------------------------------------
void __fastcall TTDBGrid::topRow(long tr)
{
 TopRow = tr;
}
//---------------------------------------------------------------------------
void __fastcall TTDBGrid::EditorMode(bool b)
{
  TDBGrid::EditorMode = b;
}
//---------------------------------------------------------------------------
void __fastcall TTDBGrid::ToggleEditorMode()
{
  if( TDBGrid::EditorMode )
    TDBGrid::EditorMode = false;
  else
    TDBGrid::EditorMode = true;
}
//---------------------------------------------------------------------------



