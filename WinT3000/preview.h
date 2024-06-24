//----------------------------------------------------------------------------
//Borland C++Builder
//Copyright (c) 1987, 1997 Borland International Inc. All Rights Reserved.
//----------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifndef previewH
#define previewH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\QuickRpt.hpp>
#include <vcl\ComCtrls.hpp>
#include <qrprntr.hpp>
//---------------------------------------------------------------------------
class TPrevForm : public TForm
{
__published:    // IDE-managed Components 
    TPanel *Panel1;
    TLabel *Label1;
    TLabel *Label3;
    TButton *Button1;
    TButton *Button2;
    TQRPreview *QRPreview1;
	TUpDown *UpDown1;
	TEdit *Edit1;
	TUpDown *UpDown2;
	TEdit *Edit2;
    
    void __fastcall FormShow(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
	void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
	void __fastcall UpDown2Click(TObject *Sender, TUDBtnType Button);
	
    
    
    
    void __fastcall Edit2KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall Edit1KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
private:        // User declarations
public:         // User declarations
    virtual __fastcall TPrevForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TPrevForm *PrevForm;
//---------------------------------------------------------------------------
#endif
