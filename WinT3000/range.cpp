//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "range.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"

//Added by Paul Mao on 12/8/97:
#include "t3000def.h"
extern dig_range_form dig_range_array[];

TRangeForm *RangeForm;
//---------------------------------------------------------------------------
__fastcall TRangeForm::TRangeForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
//Added by Paul Mao on 12/8/97:
void __fastcall TRangeForm::StandardClick(TObject *Sender)
{
  if(StandardList -> ItemIndex != -1)
  {
    CustomList -> ItemIndex = -1;
    AnalogList -> ItemIndex = -1;
  }
}
//---------------------------------------------------------------------------
void __fastcall TRangeForm::CustomClick(TObject *Sender)
{
  if(CustomList -> ItemIndex != -1)
  {
    StandardList -> ItemIndex = -1;
    AnalogList -> ItemIndex = -1;
  }
}
//---------------------------------------------------------------------------
void __fastcall TRangeForm::AnalogClick(TObject *Sender)
{
  if(AnalogList -> ItemIndex != -1)
  {
    CustomList -> ItemIndex = -1;
    StandardList -> ItemIndex = -1;
  }
}
//---------------------------------------------------------------------------
void __fastcall TRangeForm::InvertClick(TObject *Sender)
{
  AnsiString temp;
  if (!(CheckBox -> Checked))
  {
    StandardList -> Clear();
    for (int i=1; i<=11; i++)
    {
      temp = "         " + AnsiString(dig_range_array[i].range_text);
      StandardList -> Items -> Add(temp);
    }
  }
  else
  {
    StandardList -> Clear();
    for (int i=12; i<=22; i++)
    {
      temp = "         " + AnsiString(dig_range_array[i].range_text);
      StandardList -> Items -> Add(temp);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TRangeForm::CustomListDblClick(TObject *Sender)
{
 CustomClick(Sender);
 ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TRangeForm::StandardListDblClick(TObject *Sender)
{
 StandardClick(Sender);
 ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TRangeForm::AnalogListDblClick(TObject *Sender)
{
 AnalogClick(Sender);
 ModalResult = mrOk;
}
//---------------------------------------------------------------------------
