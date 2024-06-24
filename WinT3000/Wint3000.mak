# ---------------------------------------------------------------------------
VERSION = BCB.03
# ---------------------------------------------------------------------------
!ifndef BCB
BCB = $(MAKEDIR)\..
!endif
# ---------------------------------------------------------------------------
PROJECT = Wint3000.exe
OBJFILES = obj\Wint3000.obj obj\wint3.obj obj\Date.obj obj\New1.obj obj\T3000.obj \
  obj\WinT3000Proc.obj obj\t3000Request.obj obj\align.obj obj\decript.obj \
  obj\DataModule.obj obj\serialint.obj obj\about.obj obj\hardware.obj \
  obj\printpanel.obj obj\calend.obj obj\datetime.obj obj\panelstatus.obj \
  obj\confirm.obj obj\Selectpanel.obj obj\preview.obj obj\report.obj \
  obj\openfile.obj obj\print.obj obj\edit.obj obj\Grid.obj obj\range.obj \
  obj\RefreshIn.obj obj\messagebox.obj obj\LTGLoginInit.obj obj\Ltgwint3.obj \
  obj\Ltgt3000.obj obj\LTGUserEnsur.obj obj\MH_default.obj obj\MH_UserList.obj \
  obj\LTGComPortList.obj obj\openfile95.obj obj\savefile95.obj obj\pointcl.obj
RESFILES = Wint3000.res
RESDEPEN = $(RESFILES) wint3.dfm DataModule.dfm serialint.dfm about.dfm hardware.dfm \
  printpanel.dfm calend.dfm datetime.dfm panelstatus.dfm confirm.dfm \
  Selectpanel.dfm preview.dfm report.dfm openfile.dfm print.dfm edit.dfm Grid.dfm \
  range.dfm RefreshIn.dfm messagebox.dfm LTGLoginInit.dfm LTGUserEnsur.dfm \
  MH_default.dfm MH_UserList.dfm LTGComPortList.dfm openfile95.dfm savefile95.dfm
LIBFILES = Ptpdll.lib
LIBRARIES = vcldbx35.lib vcldb35.lib vclx35.lib vcl35.lib
SPARELIBS = vcl35.lib vclx35.lib vcldb35.lib vcldbx35.lib
PACKAGES = VCLX35.bpi VCL35.bpi VCLDB35.bpi VCLDBX35.bpi bcbsmp35.bpi dclocx35.bpi \
  QRPT35.bpi TEEUI35.bpi TEEDB35.bpi TEE35.bpi ibsmp35.bpi NMFAST35.bpi \
  INETDB35.bpi INET35.bpi
PATHASM = .;
PATHCPP = .;
PATHPAS = .;
PATHRC = .;
DEBUGLIBPATH = $(BCB)\lib\debug
RELEASELIBPATH = $(BCB)\lib\release
DEFFILE =
# ---------------------------------------------------------------------------
CFLAG1 = -Od -w -Ve -r- -k -y -v -vi- -c -b- -w-par -w-inl -Vx
CFLAG2 = -I.;$(BCB)\include;$(BCB)\include\vcl
CFLAG3 = -4
PFLAGS = -AWinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE \
  -N2c:\wint3000\obj -N0c:\wint3000\obj \
  -U.;$(BCB)\lib\obj;$(BCB)\lib;$(DEBUGLIBPATH) \
  -I.;$(BCB)\include;$(BCB)\include\vcl -W -$Y -$W -$O- -v -JPHNV -M
RFLAGS = -i.;$(BCB)\include;$(BCB)\include\vcl
AFLAGS = /i. /i$(BCB)\include /i$(BCB)\include\vcl /mx /w2 /zi
LFLAGS = -L.;$(BCB)\lib\obj;$(BCB)\lib;$(DEBUGLIBPATH) -aa -Tpe -x -v
IFLAGS =
LINKER = ilink32
# ---------------------------------------------------------------------------
ALLOBJ = c0w32.obj sysinit.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) $(LIBRARIES) import32.lib cp32mt.lib
# ---------------------------------------------------------------------------
.autodepend

!ifdef IDEOPTIONS

[Version Info]
IncludeVerInfo=0
AutoIncBuild=0
MajorVer=1
MinorVer=0
Release=0
Build=0
Debug=0
PreRelease=0
Special=0
Private=0
DLL=0
Locale=1033
CodePage=1252

[HistoryLists\hlIncludePath]
Count=1
Item0=.;$(BCB)\include;$(BCB)\include\vcl

[HistoryLists\hlLibraryPath]
Count=1
Item0=.;$(BCB)\lib\obj;$(BCB)\lib

[HistoryLists\hlUnitAliases]
Count=1
Item0=WinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE

[HistoryLists\hlIntOutputDir]
Count=1
Item0=c:\wint3000\obj

[Debugging]
DebugSourceDirs=

[Parameters]
RunParams=
HostApplication=

!endif

$(PROJECT): $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES)
!

.pas.hpp:
    $(BCB)\BIN\dcc32 $(PFLAGS) { $** }

.pas.obj:
    $(BCB)\BIN\dcc32 $(PFLAGS) { $** }

.cpp.obj:
    $(BCB)\BIN\bcc32 $(CFLAG1) $(CFLAG2) -o$* $* 

.c.obj:
    $(BCB)\BIN\bcc32 $(CFLAG1) $(CFLAG2) -o$* $**

.rc.res:
    $(BCB)\BIN\brcc32 $(RFLAGS) $<
#-----------------------------------------------------------------------------
