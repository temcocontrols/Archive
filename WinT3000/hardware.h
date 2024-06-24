//---------------------------------------------------------------------------
#ifndef hardwareH
#define hardwareH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
//---------------------------------------------------------------------------
class THardwareForm : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall THardwareForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern THardwareForm *HardwareForm;
//---------------------------------------------------------------------------
#endif
