//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include <stdlib.h>

#include "calend.h"
//---------------------------------------------------------------------------
#pragma link "Grids"
#pragma link "tcalendar"
#pragma resource "*.dfm"
TCalendarForm *CalendarForm;
extern char *month_text[12];

bool IsLeapYear(int AYear)
{
  return  ((AYear %   4) == 0) &&
         (((AYear % 100) != 0) ||
          ((AYear % 400) == 0));
};

int DaysPerMonth(int AYear, int AMonth)
{
  int result;
  const int DaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  result = DaysInMonth[AMonth-1];
  if ((AMonth == 2) && IsLeapYear(AYear))
    ++result; // leap-year Feb is special

  return result;
};

//---------------------------------------------------------------------------
__fastcall TCalendarForm::TCalendarForm(TComponent* Owner, char *ptr)
	: TForm(Owner)
{
  char buf[10];
  Word  AYear, AMonth, ADay;
  DecodeDate(Date(),AYear, AMonth, ADay);
  Month->Text=month_text[AMonth-1];
  Month->ItemIndex = AMonth-1;
  itoa(AYear,buf,10);
  Year->Text = buf;
  memcpy(dayOn, ptr, 46);
  Calendar1->SetDayOn(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TCalendarForm::MonthChange(TObject *Sender)
{
  Calendar1->setMonth(Month->ItemIndex+1);
}
//---------------------------------------------------------------------------
void __fastcall TCalendarForm::YearChange(TObject *Sender)
{
  Calendar1->setYear(Year->Text.ToInt());
}
//---------------------------------------------------------------------------
void __fastcall TCalendarForm::Calendar1DblClick(TObject *Sender)
{
 TDateTime d;
 int yearday=0;
 d = Calendar1->CalendarDate;
 Word AYear, AMonth, ADay;
 DecodeDate(d,AYear, AMonth, ADay);

 for(int i=1; i<AMonth; i++)
 {
    yearday += DaysPerMonth((int)AYear, i);
 }
 yearday += ADay-1;     // 0->355

 if ( dayOn[yearday/8] & (1<<(yearday%8)) )
 {
   dayOn[yearday/8] &= ~(1<<(yearday%8));
 }
 else
 {
   dayOn[yearday/8] |= (1<<(yearday%8));
 }
 Calendar1->SetDayOn(dayOn);
}
//---------------------------------------------------------------------------




