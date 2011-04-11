# Microsoft Developer Studio Project File - Name="ds7" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ds7 - Win32 Checked
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ds7.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ds7.mak" CFG="ds7 - Win32 Checked"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ds7 - Win32 IA64 Free" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ds7 - Win32 IA64 Checked" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ds7 - Win32 Free" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ds7 - Win32 Checked" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ds7 - Win32 Free"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "objfre\i386"
# PROP BASE Intermediate_Dir "Free"
# PROP BASE Target_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "objfre\i386"
# PROP Intermediate_Dir "Free"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /YX /Gy /Gz /Oxs /Oy /D "WIN32" /c
# ADD CPP /nologo /W3 /YX /Gy /Gz /Oxs /Oy /FD /FR /GB /QI0f /Gi- /Gm- /GR- /GX- /D "WIN32=100" /c /I "$(DRIVERWORKS)\..\common\include\stl" 
# ADD CPP /I "$(TARGET_INC_PATH)" /I "$(CRT_INC_PATH)" /I "$(DDK_INC_PATH)" /I "$(WDM_INC_PATH)"
# ADD CPP /I "$(DRIVERWORKS)\include" /I "." /D "STD_CALL" /D "CONDITION_HANDLING=1" /D "NT_UP=1" /D "NT_INST=0" /D "_NT1X_=100"
# ADD CPP /D "WINNT=1" /D "_WIN32_WINNT=0x0400" /D "WIN32_LEAN_AND_MEAN=1" /D "DEVL=1" /D "FPO=1" /D "_IDWBUILD"
# ADD CPP /D "NDEBUG" /D "_DLL=1" /Zel /Zp8 /Gy -cbstring /QIfdiv- /QIf /GF /Fd".\objfre\i386\ds7.pdb"
# ADD CPP /D "_X86_=1" /D "$(CPU)=1" /GR- /GX- /D NTVERSION='WDM' /I "$(DRIVERWORKS)\source" 
# ADD CPP /I "$(BASEDIR)\inc\win98"
# ADD CPP /I "$(DRIVERWORKS)\include\dep_vxd" /I "$(DRIVERWORKS)\include\dep_wdm" /I "$(DRIVERWORKS)\include\dep_ndis"
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /I "$(TARGET_INC_PATH)" /I "$(CRT_INC_PATH)" /I "$(DDK_INC_PATH)" /I "$(WDM_INC_PATH)" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"objfre\i386/ds7.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo 
# ADD LINK32 /nologo /driver:WDM /debug:FULL /debugtype:cv /IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096,4210
# ADD LINK32 usbd.lib
# ADD LINK32 wdm.lib "$(DRIVERWORKS)\lib\$(CPU)\free\vdw_wdm.lib" /entry:"DriverEntry" /incremental:no
# ADD LINK32 /MERGE:_PAGE=PAGE /MERGE:_TEXT=.text /SECTION:INIT,d /MERGE:.rdata=.text /STACK:262144,4096 /MAP
# ADD LINK32 /nodefaultlib /out:.\objfre\i386\ds7.sys /FULLBUILD /RELEASE /FORCE:MULTIPLE /OPT:REF 
# ADD LINK32 /libpath:$(TARGET_LIB_PATH)
# ADD LINK32 /align:0x20 /base:0x10000
# ADD LINK32 /version:5.00 /osversion:5.00
# ADD LINK32 /subsystem:native,1.10
# Begin Special Build Tool
PostBuild_Desc=Generating SoftICE Symbol file ds7.nms
PostBuild_Cmds=$(DRIVERWORKS)\bin\nmsym /translate:source,package,always .\objfre\i386\ds7.sys
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ds7 - Win32 Checked"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "objchk\i386"
# PROP BASE Intermediate_Dir "Checked"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "objchk\i386"
# PROP Intermediate_Dir "Checked"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gz /Zi /Od /D "WIN32" /YX /FD /c
# ADD CPP /nologo /W3 /Gz /Zi /Od /Gi- /Gm- /GR- /GX- /D "WIN32=100" /D RDRDBG /D SRVDBG /GB /QI0f /YX /FR /FD /c /I "$(DRIVERWORKS)\..\common\include\stl"
# ADD CPP /I "$(TARGET_INC_PATH)" /I "$(CRT_INC_PATH)" /I "$(DDK_INC_PATH)" /I "$(WDM_INC_PATH)"
# ADD CPP /I "$(DRIVERWORKS)\include" /I "." /D "STD_CALL" /D "CONDITION_HANDLING=1" /D "NT_UP=1" /D "NT_INST=0" /D "_NT1X_=100"
# ADD CPP /D "WINNT=1" /D "_WIN32_WINNT=0x0400" /D "WIN32_LEAN_AND_MEAN=1" /D "DBG=1" /D "DEVL=1" /D "FPO=0"
# ADD CPP /D "NDEBUG" /D "_DLL=1" /Zel /Zp8 /Gy -cbstring /QIfdiv- /QIf /GF /Zi /Oi /Oy- 
# ADD CPP /D "_X86_=1" /D "$(CPU)=1" /D NTVERSION='WDM' /I "$(DRIVERWORKS)\source" 
# ADD CPP /I "$(BASEDIR)\inc\win98" /Fd".\objchk\i386\ds7.pdb"
# ADD CPP /I "$(DRIVERWORKS)\include\dep_vxd" /I "$(DRIVERWORKS)\include\dep_wdm" /I "$(DRIVERWORKS)\include\dep_ndis"
# ADD BASE MTL /nologo /mktyplib203 /o NUL /win32
# ADD MTL /nologo /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 
# ADD RSC /l 0x409 /I "$(TARGET_INC_PATH)" /I "$(CRT_INC_PATH)" /I "$(DDK_INC_PATH)" /I "$(WDM_INC_PATH)" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"objchk\i386/ds7.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo
# ADD LINK32 /nologo /driver:WDM /debug:FULL /debugtype:cv /IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096,4210
# ADD LINK32 usbd.lib
# ADD LINK32 wdm.lib "$(DRIVERWORKS)\lib\$(CPU)\checked\vdw_wdm.lib" /entry:"DriverEntry" /incremental:no
# ADD LINK32 /MERGE:_PAGE=PAGE /MERGE:_TEXT=.text /SECTION:INIT,d /MERGE:.rdata=.text /STACK:262144,4096 /MAP
# ADD LINK32 /nodefaultlib /out:.\objchk\i386\ds7.sys /FULLBUILD /RELEASE /FORCE:MULTIPLE /OPT:REF 
# ADD LINK32 /libpath:$(TARGET_LIB_PATH);$(BASEDIR)\lib\i386\free
# ADD LINK32 /align:0x20 /base:0x10000
# ADD LINK32 /version:5.00 /osversion:5.00
# ADD LINK32 /subsystem:native,1.10
# Begin Special Build Tool
PostBuild_Desc=Generating SoftICE Symbol file ds7.nms
PostBuild_Cmds=$(DRIVERWORKS)\bin\nmsym /translate:source,package,always .\objchk\i386\ds7.sys
# End Special Build Tool

!ENDIF 

!ELSEIF  "$(CFG)" == "ds7 - Win32 IA64 Free"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "objfre\ia64"
# PROP BASE Intermediate_Dir "obj\ia64\free"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "objfre\ia64"
# PROP Intermediate_Dir "obj\ia64\free"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W3 /Zi /O1 /Ob2 /I "$(DRIVERWORKS)\..\common\include\stl" /I "$(DRIVERWORKS)\include" /I "$(DRIVERWORKS)\source" 
# ADD CPP /nologo /Gz /W3 /WX /Zi /Gy /I "$(DRIVERWORKS)\..\common\include\stl" /I "$(DRIVERWORKS)\include" /I ".." /I "$(DRIVERWORKS)\source" 
# ADD CPP /I "$(TARGET_INC_PATH)" /I "$(CRT_INC_PATH)" /I "$(DDK_INC_PATH)" /I "$(WDM_INC_PATH)" /I "$(BASEDIR)\src\usb\inc" 
# ADD CPP /D _IA64_=1 /D "_WIN64" /D "IA64" /D "NO_HW_DETECT" /D "_MSC_EXTENSIONS" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 
# ADD CPP /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x501 /D WINVER=0x501 /D _WIN32_IE=0x560 /D WIN32_LEAN_AND_MEAN=1 /D _MERCED_A0_=1 /D DBG=0 
# ADD CPP /D DEVL=1 /D "NDEBUG" /D NTVERSION='WDM' /Oxs /Wp64 /Zel /GF /QIA64_fr32 -cbstring /GF /Fd".\objfre\ia64\ds7.pdb" /c
# ADD CPP /I "$(DRIVERWORKS)\include\dep_vxd" /I "$(DRIVERWORKS)\include\dep_wdm" /I "$(DRIVERWORKS)\include\dep_ndis"
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(DRIVERWORKS)\include" /d "NDEBUG" /d WIN32_LEAN_AND_MEAN=1
# ADD RSC /l 0x409 /i "$(DRIVERWORKS)\include" /d "NDEBUG" /d WIN32_LEAN_AND_MEAN=1
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wdm.lib hal.lib $(DRIVERWORKS)\lib\i386\free\vdw_wdm.lib usbd.lib /nologo /base:"0x10000" /version:5.0 /stack:0x40000,0x1000 /entry:"DriverEntry@8" /subsystem:windows /machine:I386 /nodefaultlib /out:".\objfre\i386\ds7.sys" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096,4210 -MERGE:.rdata=.text  -driver -align:0x20 -osversion:5.00 -subsystem:native,1.10 -driver:WDM -debug:MINIMAL
# SUBTRACT BASE LINK32 /pdb:none /map /debug
# ADD LINK32 wdm.lib "$(DRIVERWORKS)\lib\$(CPU)\free\vdw_wdm.lib" usbd.lib /nologo /base:"0x10000" /version:4.0 /entry:"DriverEntry" /machine:IX86 /nodefaultlib /out:".\objfre\ia64\ds7.sys" /libpath:"$(TARGET_LIB_PATH)" /machine:IA64 /driver /debug:MINIMAL /IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096,4210 /MERGE:_PAGE=PAGE /MERGE:_TEXT=.text /SECTION:INIT,d /MERGE:.rdata=.text /FULLBUILD /RELEASE /FORCE:MULTIPLE /OPT:REF /align:0x20 /osversion:4.00 /subsystem:native
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ds7 - Win32 IA64 Checked"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "objchk\i386"
# PROP BASE Intermediate_Dir "obj\ia64\checked"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "objchk\i386"
# PROP Intermediate_Dir "obj\ia64\checked"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP
# ADD CPP /nologo /Gz /W3 /WX /Zi /Oi /Gy /I "$(DRIVERWORKS)\..\common\include\stl" /I "$(DRIVERWORKS)\include" /I ".." /I "$(DRIVERWORKS)\source" /D _IA64_=1 /D "_WIN64" /D "IA64" /D "NO_HW_DETECT" /D "_MSC_EXTENSIONS" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x501 /D WINVER=0x501 /D _WIN32_IE=0x560 /D WIN32_LEAN_AND_MEAN=1 /D _MERCED_A0_=1 /D DBG=1 /D DEVL=1 /D "NDEBUG" /D NTVERSION='WDM' /YX /Wp64 /Zel /GF /QIA64_fr32 -cbstring /GF /Fd".\objchk\ia64\ds7.pdb" /c
# ADD CPP /I "$(TARGET_INC_PATH)" /I "$(CRT_INC_PATH)" /I "$(DDK_INC_PATH)" /I "$(WDM_INC_PATH)" /I "$(BASEDIR)\src\usb\inc" 
# ADD CPP /I "$(DRIVERWORKS)\include\dep_vxd" /I "$(DRIVERWORKS)\include\dep_wdm" /I "$(DRIVERWORKS)\include\dep_ndis"
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(DRIVERWORKS)\include" /d "NDEBUG" /d WIN32_LEAN_AND_MEAN=1
# ADD RSC /l 0x409 /i "$(DRIVERWORKS)\include" /d "NDEBUG" /d WIN32_LEAN_AND_MEAN=1
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wdm.lib hal.lib $(DRIVERWORKS)\lib\$(CPU)\checked\vdw_wdm.lib usbd.lib /nologo /base:"0x10000" /version:5.0 /stack:0x40000,0x1000 /entry:"DriverEntry@8" /subsystem:windows /machine:I386 /nodefaultlib /out:".\objchk\i386\ds7.sys" /libpath:"$(BASEDIR)\libfre\i386" /libpath:"$(BASEDIR)\lib\i386\free" /libpath:"$(BASEDIR)\lib\i386" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096,4210 -MERGE:.rdata=.text -driver -align:0x20 -osversion:5.00 -subsystem:native,1.10 -driver:WDM -debug:MINIMAL
# SUBTRACT BASE LINK32 /pdb:none /map /debug
# ADD LINK32 wdm.lib  "$(DRIVERWORKS)\lib\$(CPU)\checked\vdw_wdm.lib" usbd.lib /nologo /base:"0x10000" /version:4.0 /entry:"DriverEntry" /debug /machine:IX86 /nodefaultlib /out:".\objchk\ia64\ds7.sys" /libpath:"$(TARGET_LIB_PATH)" /machine:IA64 /driver /debug:FULL /IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096,4210 /MERGE:_PAGE=PAGE /MERGE:_TEXT=.text /SECTION:INIT,d /MERGE:.rdata=.text /FULLBUILD /RELEASE /FORCE:MULTIPLE /OPT:REF /align:0x20 /osversion:4.00 /subsystem:native
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ds7 - Win32 Free"
# Name "ds7 - Win32 Checked"
# Name "ds7 - Win32 IA64 Free"
# Name "ds7 - Win32 IA64 Checked"
# Begin Group "Source Files"

# PROP Default_Filter ".c;.cpp"

# Begin Source File

SOURCE=.\Ds7.cpp
# End Source File
# Begin Source File

SOURCE=.\Ds7Device.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"

# Begin Source File

SOURCE=.\Ds7.h
# End Source File
# Begin Source File

SOURCE=.\function.h
# End Source File
# Begin Source File

SOURCE=.\Ds7Device.h
# End Source File
# Begin Source File

SOURCE=..\ds7ioctl.h
# End Source File
# Begin Source File

SOURCE=..\Ds7DeviceInterface.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ".rc;.mc;.mof"

# Begin Source File

SOURCE=.\Ds7.rc
# End Source File


# End Group
# Begin Source File
SOURCE=..\readme.txt
# End Source File

# Begin Source File
SOURCE=.\ds7.inf
# End Source File

# End Target
# End Project
