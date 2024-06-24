//---------------------------------------------------------------------------
#ifndef LTGSaveFileDlgH
#define LTGSaveFileDlgH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Dialogs.hpp>
//---------------------------------------------------------------------------
class TSaveFileForm : public TForm
{
__published:	// IDE-managed Components
	TSaveDialog *SaveDialog;
private:	// User declarations
public:		// User declarations
	__fastcall TSaveFileForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSaveFileForm *SaveFileForm;
//---------------------------------------------------------------------------
#endif
