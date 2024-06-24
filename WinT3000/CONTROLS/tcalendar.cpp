//---------------------------------------------------------------------------
// Borland C++Builder
// Copyright (c) 1987, 1997 Borland International Inc.  All Rights Reserved.
//---------------------------------------------------------------------------
#if !defined (REGISTER_ALL_CONTROLS)
  #include  "tcalendar.h"
#else
  #include "controls\tcalendar.h"
#endif
#pragma resource "*.res"  

//---------------------------------------------------------------------------
#pragma package(smart_init);
//---------------------------------------------------------------------------

//#pragma resource "*.res"  //IDE links .res automatically for components

namespace Tcalendar{
void __fastcall PACKAGE Register()
{
  TComponentClass classes[1] = {__classid(TTCalendar)};
  RegisterComponents("WinT3000", classes, 0);
}
} //namespace

__fastcall TTCalendar::TTCalendar(TComponent *AOwner) :
	TCustomGrid(AOwner)
{
	FUseCurrentDate = true;
	FixedCols       = 0;
	FixedRows       = 1;
	ColCount        = 7;
	RowCount        = 7;
	StartOfWeek     = 1;
  ScrollBars      = ssNone;
  FDate           = Date();
  (Options >> goRangeSelect) << goDrawFocusSelected;
  UpdateCalendar();
  memset(dayOn, 0, 46);
}

void __fastcall TTCalendar::Change()
{
	if(FOnChange)
		FOnChange(this);
}

void __fastcall TTCalendar::Click()
{
  AnsiString  TheCellText;

	TheCellText = CellText[Col][Row];
	if(!TheCellText.IsEmpty())
    Day = TheCellText.ToInt();
}


/* Don't have any leap year code available.  Seemed like a straigthforward
   translation, but should be checked.
*/
bool __fastcall TTCalendar::IsLeapYear(int AYear)
{
  return  ((AYear %   4) == 0) &&
         (((AYear % 100) != 0) ||
          ((AYear % 400) == 0));
};


int __fastcall TTCalendar::DaysPerMonth(int AYear, int AMonth)
{
  int result;
  const int DaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  result = DaysInMonth[AMonth-1];
  if ((AMonth == 2) && IsLeapYear(AYear))
    ++result; // leap-year Feb is special

  return result;
};

int __fastcall TTCalendar::DaysThisMonth()
{
  return DaysPerMonth(Year, Month);
};


void __fastcall TTCalendar::DrawCell(int ACol, int ARow, const TRect &ARect, TGridDrawState /*AState*/ )
{
  const AnsiString  TheText = CellText[ACol][ARow];
//  CellColor(ACol,ARow,ARect);
  Word  ADay;
  int yearday=0;
  TColor color;

  color = Canvas->Brush->Color;
  if(!TheText.IsEmpty())
  {
    if (ARow)
    {
     ADay = TheText.ToInt();
     for(int i=1; i<Month; i++)
     {
      yearday += DaysPerMonth(Year, i);
     }
     yearday += ADay-1;     // 0->355

     if ( dayOn[yearday/8] & (1<<(yearday%8)) )
     {
      Canvas->Brush->Color = clRed;
      Canvas->FillRect( ARect );
     }
    }
  }
  Canvas->TextRect(
    ARect,
    ARect.Left + (ARect.Right - ARect.Left - Canvas->TextWidth(TheText)) / 2,
    ARect.Top + (ARect.Bottom - ARect.Top - Canvas->TextHeight(TheText)) / 2,
    TheText);
  Canvas->Brush->Color = color;
};


AnsiString __fastcall TTCalendar::GetCellText(int ACol, int ARow)
{
  int DayNum;
  AnsiString  result;

  if (!ARow)
    // day names at tops of columns
    //
    result = ShortDayNames[(StartOfWeek + ACol) % 7];
  else
  {
    DayNum = FMonthOffset + ACol + (ARow - 1) * 7;
    if ((DayNum < 1) || (DayNum > DaysThisMonth()))
      result = "";
    else
      result = IntToStr(DayNum);
  }

  return result;
};

bool __fastcall TTCalendar::SelectCell(int ACol, int ARow)
{
  bool  result;

  if ((!FUpdating && FReadOnly) || !CellText[ACol][ARow].c_str())
    result = false;
  else
   result = TCustomGrid::SelectCell(ACol, ARow);

  return result;
};

void __fastcall TTCalendar::SetCalendarDate(TDateTime Value)
{
  FDate = Value;
  UpdateCalendar();
  Change();
};

bool __fastcall TTCalendar::StoreCalendarDate()
{
  return !FUseCurrentDate;
};

int __fastcall  TTCalendar::GetDateElement(int Index)
{
  Word  AYear, AMonth, ADay;
  int result;

  DecodeDate(FDate, AYear, AMonth, ADay);
  switch(Index)
  {
    case 1:
      result = AYear;break;

    case 2:
      result = AMonth;break;

    case 3:
      result = ADay;break;

    default:
      result = -1;
  };

  return result;
};

#pragma warn -sig
void __fastcall TTCalendar::SetDateElement(int Index, int Value)
{
  Word  AYear, AMonth, ADay;
  bool  Update  = false;

  if (Value > 0)
  {
    DecodeDate(FDate, AYear, AMonth, ADay);
    switch(Index)
    {
      case 1:
        if (AYear != Value)
        {
          AYear   = Value;
          Update  = true;
        }
        break;

      case 2:
        if ((Value <= 12) && (Value != AMonth))
        {
          AMonth  = Value;
          Update  = true;
        }
        break;

      case 3:
        if ((Value <= DaysThisMonth()) && (Value != ADay))
        {
          ADay    = Value;
          Update  = true;
        }
        break;
    }
    if (Update)
    {
      FDate = EncodeDate(AYear, AMonth, ADay);
      FUseCurrentDate = false;
      UpdateCalendar();
      Change();
    }
  };
};

void __fastcall TTCalendar::SetStartOfWeek(TDayOfWeek Value)
{
  if( (Value > 7) || (Value < 1) )
  {
  	FStartOfWeek=1; // bad value sets to 1 (Sunday)
  }
  else
  {
  	if (Value != FStartOfWeek)
  	{
    	FStartOfWeek = Value;
    	UpdateCalendar();
  	}
  }
};

void __fastcall TTCalendar::SetUseCurrentDate(bool Value)
{
  if (Value != FUseCurrentDate)
  {
    FUseCurrentDate = Value;
    if (Value)
    {
      FDate = Date(); // use the current date, then
      UpdateCalendar();
    };
  };
};

// Given a value of 1 or -1, moves to Next or Prev month accordingly.
//
void __fastcall TTCalendar::ChangeMonth(int Delta)
{
  Word  AYear, AMonth, ADay;
  TDateTime NewDate;
  int CurDay;

  DecodeDate(FDate, AYear, AMonth, ADay);
  CurDay = ADay;
  if (Delta > 0)
    ADay = DaysPerMonth(AYear, AMonth);
  else
    ADay = 1;

  NewDate = EncodeDate(AYear, AMonth, ADay);
  NewDate = NewDate + Delta;
  DecodeDate(NewDate, AYear, AMonth, ADay);
  if (DaysPerMonth(AYear, AMonth) > CurDay)
    ADay = CurDay;
  else
    ADay = DaysPerMonth(AYear, AMonth);

  CalendarDate = EncodeDate(AYear, AMonth, ADay);
};
#pragma warn .sig

void __fastcall TTCalendar::PrevMonth()
{
  ChangeMonth(-1);
}

void __fastcall TTCalendar::NextMonth()
{
  ChangeMonth(1);
}

void __fastcall TTCalendar::NextYear()
{
  if (IsLeapYear(Year) && (Month == 2) && (Day == 29))
    Day = 28;
  ++Year;
}

void __fastcall TTCalendar::PrevYear()
{
  if (IsLeapYear(Year) && (Month == 2) && (Day == 29))
    Day = 28;
  --Year;
}

void __fastcall TTCalendar::UpdateCalendar()
{
  Word  AYear, AMonth, ADay;
  TDateTime FirstDate;

  FUpdating = True;
  DecodeDate(FDate, AYear, AMonth, ADay);
  FirstDate = EncodeDate(AYear, AMonth, 1);

  // Day of week for 1st of month.
  //
  FMonthOffset = 2 - ((DayOfWeek(FirstDate) - StartOfWeek + 7) % 7);
  if (FMonthOffset == 2)
    FMonthOffset = -5;

  MoveColRow((ADay - FMonthOffset) % 7,
             (ADay - FMonthOffset) / 7 + 1,
             False, False);
    Invalidate();

  FUpdating = False;
};

void __fastcall TTCalendar::WMSize(TWMSize &Message)
{
  int GridLines;

  GridLines = 6 * GridLineWidth;
  DefaultColWidth   = (Message.Width - GridLines) / 7;
  DefaultRowHeight  = (Message.Height - GridLines) / 7;
};

void __fastcall TTCalendar::SetDayOn(char *ptr)
{
  memcpy(dayOn, ptr, 46);
}

void __fastcall TTCalendar::setYear(int AYear)
{
  Year = AYear;
  CalendarDate = EncodeDate(AYear, Month, Day);
}

void __fastcall TTCalendar::setMonth(int MMonth)
{
  Word  AYear, AMonth, ADay;

  DecodeDate(Date(), AYear, AMonth, ADay);
  Month = MMonth;
  if (AMonth == MMonth)
    Day = ADay;
  else
    Day = 1;
  CalendarDate = EncodeDate(Year, Month, Day);
}
