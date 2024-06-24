//----------------------------------------------------------------------------
//Borland C++Builder
//Copyright (c) 1987, 1997 Borland International Inc. All Rights Reserved.
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#ifndef editH
#define editH
//----------------------------------------------------------------------------
#include <Outline.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Messages.hpp>
#include <Windows.hpp>
#include <System.hpp>
//----------------------------------------------------------------------------
class TEditForm : public TForm
{
__published:
	TPanel *SpeedBar;
	TSpeedButton *OpenButton;
	TSpeedButton *SaveButton;
	TSpeedButton *PrintButton;
	TSpeedButton *CutButton;
	TSpeedButton *CopyButton;
	TSpeedButton *PasteButton;
	TBevel *Bevel1;
	TComboBox *FontName;
	TEdit *FontSize;
	TUpDown *UpDown1;
	TStatusBar *StatusBar;
	TMainMenu *MainMenu;
	TMenuItem *ProgramMenu;
	TMenuItem *Send;
	TMenuItem *Open;
	TMenuItem *Save;
	TMenuItem *N1;
	TMenuItem *Print;
	TMenuItem *N4;
	TMenuItem *FileExit;
	TMenuItem *EditMenu;
	TMenuItem *EditUndo;
	TMenuItem *N2;
	TMenuItem *EditCut;
	TMenuItem *EditCopy;
	TMenuItem *EditPaste;
	TMenuItem *N5;
	TMenuItem *EditFont;
	TMenuItem *HelpMenu;
	TMenuItem *HelpContents;
	TMenuItem *HelpSearch;
	TMenuItem *HelpHowTo;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TPrintDialog *PrintDialog;
	TFontDialog *FontDialog;
	TRichEdit *RichEdit;
	TMenuItem *Clear;
	TMenuItem *N6;
	TMenuItem *Find;
	TMenuItem *FindNext;
	TMenuItem *Replace;
	TMenuItem *N3;
	TSpeedButton *SendButton;
	TSpeedButton *ClearButton;
	TLabel *Label1;
	TLabel *Label2;
	TRichEdit *RichEdit1;
	TRichEdit *RichEdit2;
	void __fastcall SelectionChange(TObject *Sender);
	void __fastcall AlignClick(TObject *Sender);
	void __fastcall PrintClick(TObject *Sender);
	void __fastcall FileExitClick(TObject *Sender);
	void __fastcall EditUndoClick(TObject *Sender);
	void __fastcall EditCutClick(TObject *Sender);
	void __fastcall EditCopyClick(TObject *Sender);
	void __fastcall EditPasteClick(TObject *Sender);
	void __fastcall HelpContentsClick(TObject *Sender);
	void __fastcall HelpSearchClick(TObject *Sender);
	void __fastcall HelpHowToClick(TObject *Sender);
	void __fastcall HelpAboutClick(TObject *Sender);
	void __fastcall RulerResize(TObject *Sender);
	void __fastcall SelectFont(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall SendClick(TObject *Sender);
	void __fastcall OpenClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall FileSaveAsClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FontSizeChange(TObject *Sender);
	void __fastcall FontNameChange(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall FormActivate(TObject *Sender);
private:
	AnsiString FFileName;
	bool FUpdating;
	int FDragOfs;
	bool FDragging;
	TTextAttributes *__fastcall CurrText(void);
	void __fastcall GetFontNames(void);
	void __fastcall SetFileName(const AnsiString FileName);
	void __fastcall CheckFileSave(void);
	void __fastcall SetupRuler(void);
	void __fastcall SetEditRect(void);
	void __fastcall ShowHint(TObject *Sender);

public:
    virtual __fastcall TEditForm(TComponent *Owner);
};
//----------------------------------------------------------------------------
extern TEditForm *EditForm;
//----------------------------------------------------------------------------
#endif
