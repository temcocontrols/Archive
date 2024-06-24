# ---------------------------------------------------------------------------
VERSION = BCB.01
# ---------------------------------------------------------------------------
!ifndef BCB
BCB = $(MAKEDIR)\..
!endif
# ---------------------------------------------------------------------------
PROJECT = ipxdll.dll
OBJFILES = ipxdll.obj data.obj connect.obj Ipx.obj Net_asm.obj Net_bac.obj \
   pool.obj
RESFILES = ipxdll.res
RESDEPEN = $(RESFILES)
LIBFILES = Ptpdll.lib
DEFFILE =
# ---------------------------------------------------------------------------
CFLAG1 = -WD -O2 -w -k- -r -vi -c -a4 -b- -w-par -w-inl -Vx -Ve -x
CFLAG2 = -I$(BCB)\projects;$(BCB)\include;$(BCB)\include\vcl;c:\wint3000 \
   -DNET_BAC_COMM 
PFLAGS = -U$(BCB)\projects;$(BCB)\lib\obj;$(BCB)\lib \
   -I$(BCB)\projects;$(BCB)\include;$(BCB)\include\vcl;c:\wint3000 \
   -AWinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE \
   -DNET_BAC_COMM -$L- -$D- -JPHNV -M  
RFLAGS = -i$(BCB)\projects;$(BCB)\include;$(BCB)\include\vcl;c:\wint3000 \
   -DNET_BAC_COMM 
LFLAGS = -L$(BCB)\projects;$(BCB)\lib\obj;$(BCB)\lib -aa -Tpd -x -V4.0 
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
