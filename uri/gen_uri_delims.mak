# Microsoft Developer Studio Generated NMAKE File, Based on gen_uri_delims.dsp
!IF "$(CFG)" == ""
CFG=gen_uri_delims - Win32 Debug
!MESSAGE No configuration specified. Defaulting to gen_uri_delims - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "gen_uri_delims - Win32 Release" && "$(CFG)" !=\
 "gen_uri_delims - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gen_uri_delims.mak" CFG="gen_uri_delims - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gen_uri_delims - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "gen_uri_delims - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gen_uri_delims - Win32 Release"

OUTDIR=.
INTDIR=.\Release
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\gen_uri_delims.exe"

!ELSE 

ALL : "$(OUTDIR)\gen_uri_delims.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\gen_uri_delims.idb"
	-@erase "$(INTDIR)\gen_uri_delims.obj"
	-@erase "$(OUTDIR)\gen_uri_delims.exe"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\gen_uri_delims" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\gen_uri_delims.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\Release\gen_uri_delims.pdb" /machine:I386\
 /out:"$(OUTDIR)\gen_uri_delims.exe" 
LINK32_OBJS= \
	"$(INTDIR)\gen_uri_delims.obj"

"$(OUTDIR)\gen_uri_delims.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "gen_uri_delims - Win32 Debug"

OUTDIR=.
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\gen_uri_delims.exe"

!ELSE 

ALL : "$(OUTDIR)\gen_uri_delims.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\gen_uri_delims.idb"
	-@erase "$(INTDIR)\gen_uri_delims.obj"
	-@erase "$(OUTDIR)\Debug\gen_uri_delims.pdb"
	-@erase "$(OUTDIR)\gen_uri_delims.exe"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\gen_uri_delims" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\gen_uri_delims.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\Debug\gen_uri_delims.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\gen_uri_delims.exe" 
LINK32_OBJS= \
	"$(INTDIR)\gen_uri_delims.obj"

"$(OUTDIR)\gen_uri_delims.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "gen_uri_delims - Win32 Release" || "$(CFG)" ==\
 "gen_uri_delims - Win32 Debug"
SOURCE=.\gen_uri_delims.c

"$(INTDIR)\gen_uri_delims.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

