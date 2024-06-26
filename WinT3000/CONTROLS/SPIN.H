//---------------------------------------------------------------------------
// Borland C++Builder
// Copyright (c) 1987, 1997 Borland International Inc.  All Rights Reserved.
//---------------------------------------------------------------------------
#if !defined(SpinHPP)
#define SpinHPP
//----------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Menus.hpp>
#include <Graphics.hpp>
#include <Forms.hpp>
#include <SysUtils.hpp>
#include <Messages.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Classes.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <DBCtrls.hpp>    //for InitRepeatPause and RepeatPause

//-- type declarations -------------------------------------------------------
namespace Spin{
class TSpinButton;
class TTimerSpeedButton;
enum Spin_3 { tbFocusRect, tbAllowTimer };

typedef Set<Spin_3, tbFocusRect, tbAllowTimer>  TTimeBtnState;

class TTimerSpeedButton : public TSpeedButton
{
private:
	TTimer *FRepeatTimer;
	TTimeBtnState FTimeBtnState;
	void __fastcall TimerExpired(TObject *Sender);

protected:
	virtual void __fastcall Paint(void);
	DYNAMIC void __fastcall MouseDown(TMouseButton Button,  TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseUp(TMouseButton Button,  TShiftState Shift, int X, int Y);

public:
	__fastcall virtual ~TTimerSpeedButton(void);
	__property TTimeBtnState TimeBtnState = {read=FTimeBtnState, write=FTimeBtnState, nodefault};
    __fastcall virtual TTimerSpeedButton(TComponent *AOwner);
};

class TSpinButton : public TWinControl
{

private:
	TTimerSpeedButton *FUpButton;
	TTimerSpeedButton *FDownButton;
	TTimerSpeedButton *FFocusedButton;
	TWinControl *FFocusControl;
	TNotifyEvent FOnUpClick;
	TNotifyEvent FOnDownClick;
	TTimerSpeedButton *__fastcall CreateButton(void);
	Graphics::TBitmap *__fastcall GetUpGlyph(void);
	Graphics::TBitmap *__fastcall GetDownGlyph(void);
	void __fastcall SetUpGlyph(Graphics::TBitmap *Value);
	void __fastcall SetDownGlyph(Graphics::TBitmap *Value);
	void __fastcall BtnClick(TObject *Sender);
	void __fastcall BtnMouseDown(TObject *Sender, TMouseButton Button,  TShiftState Shift, int X, int
		Y);
	void __fastcall SetFocusBtn(TTimerSpeedButton *Btn);
	void __fastcall AdjustSize(int &W, int &H);
	MESSAGE void __fastcall WMSize(TWMSize &Message);
	MESSAGE void __fastcall WMSetFocus(TWMSetFocus &Message);
	MESSAGE void __fastcall WMKillFocus(TWMKillFocus &Message);
	MESSAGE void __fastcall WMGetDlgCode(TWMNoParams &Message);

protected:
	virtual void __fastcall Loaded(void);
	DYNAMIC void __fastcall KeyDown(Word &Key,  TShiftState Shift);

public:
	__fastcall virtual TSpinButton(TComponent *AOwner);
	virtual void __fastcall SetBounds(int ALeft, int ATop, int AWidth, int AHeight);

__published:
	__property Align ;
	__property Ctl3D ;
	__property Graphics::TBitmap * DownGlyph = {read=GetDownGlyph, write=SetDownGlyph, nodefault};
	__property DragCursor ;
	__property DragMode ;
	__property Enabled ;
	__property TWinControl * FocusControl = {read=FFocusControl, write=FFocusControl, nodefault};
	__property ParentCtl3D ;
	__property ParentShowHint ;
	__property PopupMenu ;
	__property ShowHint ;
	__property TabOrder ;
	__property TabStop ;
	__property Graphics::TBitmap * UpGlyph = {read=GetUpGlyph, write=SetUpGlyph, nodefault};
	__property Visible ;
	__property TNotifyEvent OnDownClick = {read=FOnDownClick, write=FOnDownClick};
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDrag ;
	__property OnEnter ;
	__property OnExit ;
	__property TNotifyEvent OnUpClick = {read=FOnUpClick, write=FOnUpClick};
	__fastcall virtual ~TSpinButton(void) { }
BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(WM_SIZE, TWMSize, WMSize);
    MESSAGE_HANDLER(WM_SETFOCUS, TWMSetFocus, WMSetFocus);
    MESSAGE_HANDLER(WM_KILLFOCUS, TWMKillFocus, WMKillFocus);
    MESSAGE_HANDLER(WM_GETDLGCODE, TWMNoParams, WMGetDlgCode);
END_MESSAGE_MAP(TWinControl);
};

class TSpinEdit : public TCustomEdit
{
private:
	long FMinValue;
	long FMaxValue;
	TCanvas *FCanvas;
	long FIncrement;
	TSpinButton *FButton;
	bool FEditorEnabled;
	int __fastcall GetMinHeight(void);
	long __fastcall GetValue(void);
	long __fastcall CheckValue(long NewValue);
	void __fastcall SetValue(long NewValue);
	void __fastcall SetEditRect(void);
	MESSAGE void __fastcall WMSize(TWMSize &Message);
	MESSAGE void __fastcall CMEnter(TWMNoParams &Message);
	MESSAGE void __fastcall CMExit(TWMNoParams &Message);
	MESSAGE void __fastcall WMPaste(TWMNoParams &Message);
	MESSAGE void __fastcall WMCut(TWMNoParams &Message);

protected:
	DYNAMIC void __fastcall GetChildren(TGetChildProc Proc);
	virtual bool __fastcall IsValidChar(Char Key);
	virtual void __fastcall UpClick(TObject *Sender);
	virtual void __fastcall DownClick(TObject *Sender);
	DYNAMIC void __fastcall KeyDown(Word &Key,  TShiftState Shift);
	DYNAMIC void __fastcall KeyPress(Char &Key);
	virtual void __fastcall CreateParams(TCreateParams &Params);
	virtual void __fastcall CreateWnd(void);

public:
	__fastcall virtual TSpinEdit(TComponent *AOwner);
	__fastcall virtual ~TSpinEdit(void);
	__property TSpinButton * Button = {read=FButton, nodefault};

__published:
	__property AutoSelect ;
	__property AutoSize ;
	__property Color ;
	__property Ctl3D ;
	__property DragCursor ;
	__property DragMode ;
	__property bool EditorEnabled = {read=FEditorEnabled, write=FEditorEnabled, default=1};
	__property Enabled ;
	__property Font ;
	__property long Increment = {read=FIncrement, write=FIncrement, default=1};
	__property MaxLength ;
	__property long MaxValue = {read=FMaxValue, write=FMaxValue, nodefault};
	__property long MinValue = {read=FMinValue, write=FMinValue, nodefault};
	__property ParentColor ;
	__property ParentCtl3D ;
	__property ParentFont ;
	__property ParentShowHint ;
	__property PopupMenu ;
	__property ReadOnly ;
	__property ShowHint ;
	__property TabOrder ;
	__property TabStop ;
	__property long Value = {read=GetValue, write=SetValue, nodefault};
	__property Visible ;
	__property OnChange ;
	__property OnClick ;
	__property OnDblClick ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDrag ;
	__property OnEnter ;
	__property OnExit ;
	__property OnKeyDown ;
	__property OnKeyPress ;
	__property OnKeyUp ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(WM_SIZE, TWMSize, WMSize);
    MESSAGE_HANDLER(CM_ENTER, TWMNoParams, CMEnter);
    MESSAGE_HANDLER(CM_EXIT, TWMNoParams, CMExit);
    MESSAGE_HANDLER(WM_PASTE, TWMNoParams, WMPaste);
    MESSAGE_HANDLER(WM_CUT, TWMNoParams, WMCut);
END_MESSAGE_MAP(TCustomEdit);
};

//-- var, const, procedure ---------------------------------------------------
//-- template instantiations -------------------------------------------------
template class TTimeBtnState ;
//-- end unit ----------------------------------------------------------------
} //end namespace Spin

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Spin;
#endif

#endif	// Spin
