//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "LTGLoginInit.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TLoginForm *LoginForm;

//---------------------------------------------------------------------------
__fastcall TLoginForm::TLoginForm(TComponent* Owner)
	: TForm(Owner)
{
  
}
//---------------------------------------------------------------------------

void __fastcall TLoginForm::OkButtonClick(TObject *Sender)
{
  extern char password[9];          // In T3000.cpp.
  extern char user_name[16];        // In T3000.cpp.

  strcpy( user_name, UserNameEdit->Text.c_str() );
  strcpy( password, PasswordEdit->Text.c_str() );

  UserNameEdit->SetFocus();
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TLoginForm::UserNameEditKeyPress(TObject *Sender, char &Key)
{
  if ( Key == '\n' || Key == '\r' )
  {
    PasswordEdit->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TLoginForm::PasswordEditKeyPress(TObject *Sender, char &Key)
{
  if ( Key == '\n' || Key == '\r' )
  {
    OkButtonClick(Sender);
  }
}
//---------------------------------------------------------------------------
void __fastcall TLoginForm::PasswordEditEnter(TObject *Sender)
{
    PasswordEdit->Text = "";
}
//---------------------------------------------------------------------------
