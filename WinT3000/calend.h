//---------------------------------------------------------------------------
#ifndef calendH
#define calendH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include "Grids.hpp"
#include "tcalendar.h"
//---------------------------------------------------------------------------
class TCalendarForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *Calendar;
	TComboBox *Month;
	TEdit *Year;
	TUpDown *UpDown1;
	TLabel *Label1;
	TLabel *Label2;
	TButton *Button1;
	TButton *Button2;
	TTCalendar *Calendar1;
//    TTCalendar *TCalendar1;
	void __fastcall MonthChange(TObject *Sender);
	void __fastcall YearChange(TObject *Sender);
	void __fastcall Calendar1DblClick(TObject *Sender);
    
    
    
    
private:	// User declarations
public:		// User declarations
    char    dayOn[46];
	__fastcall TCalendarForm(TComponent* Owner, char *ptr);
};
//---------------------------------------------------------------------------
extern TCalendarForm *CalendarForm;
//---------------------------------------------------------------------------
#endif
