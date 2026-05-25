# Microsoft Developer Studio Generated NMAKE File, Based on YCCARD.dsp
!IF $(CFG)" == "
CFG=yoccard - Win32 Debug
!MESSAGE No configuration specified. Defaulting to yoccard - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "yoccard - Win32 Release" && "$(CFG)" != "yoccard - Win32 Debug"
!MESSAGE 指定的配置 "$(CFG)" 无效.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "YCCARD.mak" CFG="yoccard - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "yoccard - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "yoccard - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF $(OS)" == "Windows_NT
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "yoccard - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# 开始自定义宏
OutDir=.\Release
# 结束自定义宏

ALL : "$(OUTDIR)\YCCARD.dll" "$(OUTDIR)\YCCARD.bsc"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\YCCARD.OBJ"
	-@erase "$(INTDIR)\YCCARD.res"
	-@erase "$(INTDIR)\YCCARD.SBR"
	-@erase "$(OUTDIR)\YCCARD.bsc"
	-@erase "$(OUTDIR)\YCCARD.dll"
	-@erase "$(OUTDIR)\YCCARD.exp"
	-@erase "$(OUTDIR)\YCCARD.ilk"
	-@erase "$(OUTDIR)\YCCARD.lib"
	-@erase "$(OUTDIR)\YCCARD.map"
	-@erase "$(OUTDIR)\YCCARD.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "YOCCARD_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\YCCARD.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\YCCARD.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\YCCARD.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\YCCARD.SBR"

"$(OUTDIR)\YCCARD.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\YCCARD.pdb" /map:"$(INTDIR)\YCCARD.map" /debug /machine:I386 /def:".\DLL.DEF" /out:"$(OUTDIR)\YCCARD.dll" /implib:"$(OUTDIR)\YCCARD.lib" 
DEF_FILE= \
	".\DLL.DEF"
LINK32_OBJS= \
	"$(INTDIR)\YCCARD.OBJ" \
	"$(INTDIR)\YCCARD.res"

"$(OUTDIR)\YCCARD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "yoccard - Win32 Debug"

OUTDIR=.\Release
INTDIR=.\Release
# 开始自定义宏
OutDir=.\Release
# 结束自定义宏

ALL : "$(OUTDIR)\YCCARD.dll" "$(OUTDIR)\YCCARD.bsc"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\YCCARD.OBJ"
	-@erase "$(INTDIR)\YCCARD.res"
	-@erase "$(INTDIR)\YCCARD.SBR"
	-@erase "$(OUTDIR)\YCCARD.bsc"
	-@erase "$(OUTDIR)\YCCARD.dll"
	-@erase "$(OUTDIR)\YCCARD.exp"
	-@erase "$(OUTDIR)\YCCARD.ilk"
	-@erase "$(OUTDIR)\YCCARD.lib"
	-@erase "$(OUTDIR)\YCCARD.map"
	-@erase "$(OUTDIR)\YCCARD.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "YOCCARD_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\YCCARD.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\YCCARD.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\YCCARD.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\YCCARD.SBR"

"$(OUTDIR)\YCCARD.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\YCCARD.pdb" /map:"$(INTDIR)\YCCARD.map" /debug /machine:I386 /def:".\DLL.DEF" /out:"$(OUTDIR)\YCCARD.dll" /implib:"$(OUTDIR)\YCCARD.lib" /pdbtype:sept 
DEF_FILE= \
	".\DLL.DEF"
LINK32_OBJS= \
	"$(INTDIR)\YCCARD.OBJ" \
	"$(INTDIR)\YCCARD.res"

"$(OUTDIR)\YCCARD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("YCCARD.dep")
!INCLUDE "YCCARD.dep"
!ELSE 
!MESSAGE Warning: cannot find "YCCARD.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "yoccard - Win32 Release" || "$(CFG)" == "yoccard - Win32 Debug"
SOURCE="F:\7.发卡器系列\YC-M3\Code\USB_Lib\YCCARD\YCCARD.CPP"

"$(INTDIR)\YCCARD.OBJ"	"$(INTDIR)\YCCARD.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\YCCARD.rc

"$(INTDIR)\YCCARD.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

