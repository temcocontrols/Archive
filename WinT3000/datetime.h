//---------------------------------------------------------------------------
#ifndef datetimeH
#define datetimeH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\ComCtrls.hpp>
#include "Grids.hpp"
//#include "t3000reg.h"
#include "tcalendar.h"
//---------------------------------------------------------------------------
class TDateTimeForm : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TPanel *Panel1;
	TGroupBox *GroupBox3;
	TEdit *Edit3;
	TUpDown *UpDown3;
	TGroupBox *GroupBox2;
	TLabel *Label2;
	TEdit *Edit2;
	TComboBox *ComboBox2;
	TUpDown *UpDown2;
	TLabel *Label1;
private:	// User declarations
public:		// User declarations
	__fastcall TDateTimeForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TDateTimeForm *DateTimeForm;
//---------------------------------------------------------------------------
#endif
