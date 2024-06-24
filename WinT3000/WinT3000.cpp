//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("wint3.cpp", MainScreen);
USEUNIT("Date.cpp");
USEUNIT("New1.cpp");
USEUNIT("T3000.cpp");
USEUNIT("WinT3000Proc.cpp");
USEUNIT("t3000Request.cpp");
USEUNIT("align.cpp");
USEUNIT("decript.cpp");
USEDATAMODULE("DataModule.cpp", DM);
USEFORM("serialint.cpp", SerialInterfaceForm);
USELIB("Ptpdll.lib");
USEFORM("about.cpp", AboutBox);
USEFORM("hardware.cpp", HardwareForm);
USEFILE("winnotes.txt");
USEFORM("printpanel.cpp", PrintPanelForm);
USEFORM("calend.cpp", CalendarForm);
USEFORM("datetime.cpp", DateTimeForm);
USEFORM("panelstatus.cpp", PanelStatusForm);
USEFORM("confirm.cpp", ConfirmForm);
USEFORM("Selectpanel.cpp", PanelSelect);
USEFORM("preview.cpp", PrevForm);
USEFORM("report.cpp", ReportForm);
USEFORM("openfile.cpp", OpenDialogForm);
USEFORM("print.cpp", PrintForm);
USEFORM("edit.cpp", EditForm);
USEFORM("Grid.cpp", GridForm);
USEFORM("range.cpp", RangeForm);
USEFORM("RefreshIn.cpp", RefreshInForm);
USEFORM("messagebox.cpp", MessageBoxForm);
USEFORM("LTGLoginInit.cpp", LoginForm);
USEUNIT("Ltgwint3.cpp");
USEUNIT("Ltgt3000.cpp");
USEFORM("LTGUserEnsur.cpp", UserEnsureForm);
USEFORM("MH_default.cpp", MH_default_form);
USEFORM("MH_UserList.cpp", MH_UserListForm);
USEFORM("LTGComPortList.cpp", ComPortListForm);
USEFORM("openfile95.cpp", OpenDialog95Form);
USEFORM("savefile95.cpp", SaveDialog95Form);
USERES("Wint3000.res");
USEUNIT("pointcl.cpp");
USEPACKAGE("VCLX35.bpi");
USEPACKAGE("VCL35.bpi");
USEPACKAGE("VCLDB35.bpi");
USEPACKAGE("QRPT35.bpi");
USEPACKAGE("WinT3000ComponentPackage.bpi");
USEFORM("Group.cpp", GroupForm);
USEFORM("program.cpp", ProgramForm);
USEFORM("monitor.cpp", GraphForm);
USEFORM("object.cpp", ObjectForm);
//---------------------------------------------------------------------------
extern int InitializeSystem();   // In LT3000.cpp
extern void Init();
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try                                                                     
	{
		Application->Initialize();
		Application->CreateForm(__classid(TMainScreen), &MainScreen);
        Application->CreateForm(__classid(TDM), &DM);
        Application->CreateForm(__classid(TSerialInterfaceForm), &SerialInterfaceForm);
        Application->CreateForm(__classid(TPrevForm), &PrevForm);
        Application->CreateForm(__classid(TReportForm), &ReportForm);
        Application->CreateForm(__classid(TPanelSelect), &PanelSelect);
        Application->CreateForm(__classid(TUserEnsureForm), &UserEnsureForm);
        Application->CreateForm(__classid(TMH_default_form), &MH_default_form);
        Application->CreateForm(__classid(TMH_UserListForm), &MH_UserListForm);
        Application->CreateForm(__classid(TComPortListForm), &ComPortListForm);
        Application->CreateForm(__classid(TGroupForm), &GroupForm);
        Application->CreateForm(__classid(TProgramForm), &ProgramForm);
        Application->CreateForm(__classid(TGraphForm), &GraphForm);
        Application->CreateForm(__classid(TObjectForm), &ObjectForm);
        Init();
		if(InitializeSystem())
		{
		 Application->Run();
        }
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
