//---------------------------------------------------------------------------
#ifndef confirmH
#define confirmH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
//---------------------------------------------------------------------------
class TConfirmForm : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TLabel *Label1;
private:	// User declarations
public:		// User declarations
	__fastcall TConfirmForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TConfirmForm *ConfirmForm;
//---------------------------------------------------------------------------
#endif
