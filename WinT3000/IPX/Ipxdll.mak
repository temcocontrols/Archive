# ---------------------------------------------------------------------------
VERSION = BCB.01
# ---------------------------------------------------------------------------
!ifndef BCB
BCB = $(MAKEDIR)\..
!endif
# ---------------------------------------------------------------------------
PROJECT = Ipxdll.dll
OBJFILES = Ipxdll.obj data.obj connect.obj Ipx.obj Net_asm.obj Net_bac.obj \
   \WinT3000\PTPDLL\Pool.obj
RESFILES = ipxdll.res
RESDEPEN = $(RESFILES)
LIBFILES = Ptpdll.lib
DEFFILE =
# ---------------------------------------------------------------------------
CFLAG1 = -WD -Od -w -k -r- -y -v -vi- -c -a4 -b- -w-par -w-inl -Vx -Ve -x
CFLAG2 = -DNET_BAC_COMM; IPXDLL_DLL \
   -I$(BCB)\include;$(BCB)\include\vcl;c:\wint3000;c:\wint3000\ptpdll 
PFLAGS = -AWinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE \
   -DNET_BAC_COMM; IPXDLL_DLL -Uc:\wint3000\ptpdll;$(BCB)\lib\obj;$(BCB)\lib \
   -I$(BCB)\include;$(BCB)\include\vcl;c:\wint3000;c:\wint3000\ptpdll -v -$Y -$W \
   -$O- -JPHNV -M  
RFLAGS = -DNET_BAC_COMM; IPXDLL_DLL \
   -i$(BCB)\include;$(BCB)\include\vcl;c:\wint3000;c:\wint3000\ptpdll 
LFLAGS = -Lc:\wint3000\ptpdll;$(BCB)\lib\obj;$(BCB)\lib -aa -Tpd -x -v -V4.0 
IFLAGS = 
LINKER = ilink32
# ---------------------------------------------------------------------------
ALLOBJ = c0d32.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) vcl.lib import32.lib cp32mt.lib 
# ---------------------------------------------------------------------------
.autodepend

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
