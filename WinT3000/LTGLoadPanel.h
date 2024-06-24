//---------------------------------------------------------------------------
#ifndef LTGLoadPanelH
#define LTGLoadPanelH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Dialogs.hpp>
//---------------------------------------------------------------------------
class TLoadPanelFileForm : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *LoadPanelFileDlg;
private:	// User declarations
public:		// User declarations
	__fastcall TLoadPanelFileForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TLoadPanelFileForm *LoadPanelFileForm;
//---------------------------------------------------------------------------
#endif
