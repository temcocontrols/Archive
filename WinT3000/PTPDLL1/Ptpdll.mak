# ---------------------------------------------------------------------------
VERSION = BCB.03
# ---------------------------------------------------------------------------
!ifndef BCB
BCB = $(MAKEDIR)\..
!endif
# ---------------------------------------------------------------------------
PROJECT = Ptpdll.dll
OBJFILES = Ptpdll.obj ptp.obj mstp.obj data.obj connect.obj comset.obj \
   router.obj pool.obj
RESFILES = ptpdll.res
RESDEPEN = $(RESFILES)
LIBFILES = 
LIBRARIES = vcldbx35.lib vcldb35.lib vclx35.lib vcl35.lib
SPARELIBS = vcl35.lib vclx35.lib vcldb35.lib vcldbx35.lib
PACKAGES = VCLX35.bpi VCL35.bpi VCLDB35.bpi VCLDBX35.bpi bcbsmp35.bpi dclocx35.bpi \
  QRPT35.bpi TEEUI35.bpi TEEDB35.bpi TEE35.bpi ibsmp35.bpi NMFAST35.bpi \
  INETDB35.bpi INET35.bpi WinT3000ComponentPackage.bpi
PATHASM = .;
PATHCPP = .;
PATHPAS = .;
PATHRC = .;
DEBUGLIBPATH = $(BCB)\lib\debug
RELEASELIBPATH = $(BCB)\lib\release
DEFFILE =
# ---------------------------------------------------------------------------
CFLAG1 = -WD -O2 -w -Ve -k- -vi -c -b- -w-par -w-inl -Vx
CFLAG2 = -DPTPDLL_DLL -I$(BCB)\include;$(BCB)\include\vcl;.
CFLAG3 = -4
PFLAGS = -AWinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE \
  -DPTPDLL_DLL -U$(BCB)\lib\obj;$(BCB)\lib;$(RELEASELIBPATH) \
  -I$(BCB)\include;$(BCB)\include\vcl;. -$L- -$D- -JPHNV -M
RFLAGS = -DPTPDLL_DLL -i$(BCB)\include;$(BCB)\include\vcl;.
AFLAGS = /i$(BCB)\include /i$(BCB)\include\vcl /i. /dPTPDLL_DLL /mx /w2 /zn
LFLAGS = -L$(BCB)\lib\obj;$(BCB)\lib;$(RELEASELIBPATH) -aa -Tpd -x -Gi
IFLAGS =
LINKER = ilink32
# ---------------------------------------------------------------------------
ALLOBJ = c0d32.obj sysinit.obj $(OBJFILES)
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

[Version Info Keys]
FileVersion=1.0.0.0

[HistoryLists\hlIncludePath]
Count=2
Item0=$(BCB)\include;$(BCB)\include\vcl;.
Item1=$(BCB)\include;$(BCB)\include\vcl;..\;

[HistoryLists\hlLibraryPath]
Count=1
Item0=$(BCB)\lib\obj;$(BCB)\lib

[HistoryLists\hlConditionals]
Count=1
Item0=PTPDLL_DLL

[HistoryLists\hlUnitAliases]
Count=1
Item0=WinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE

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
