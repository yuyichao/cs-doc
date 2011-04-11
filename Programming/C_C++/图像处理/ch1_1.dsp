# Microsoft Developer Studio Project File - Name="ch1_1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ch1_1 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ch1_1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ch1_1.mak" CFG="ch1_1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ch1_1 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ch1_1 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ch1_1 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ch1_1 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ch1_1 - Win32 Release"
# Name "ch1_1 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cDlgMorphClose.cpp
# End Source File
# Begin Source File

SOURCE=.\cDlgMorphDilation.cpp
# End Source File
# Begin Source File

SOURCE=.\cDlgMorphErosion.cpp
# End Source File
# Begin Source File

SOURCE=.\cDlgMorphOpen.cpp
# End Source File
# Begin Source File

SOURCE=.\ch1_1.cpp
# End Source File
# Begin Source File

SOURCE=.\ch1_1.rc
# End Source File
# Begin Source File

SOURCE=.\ch1_1Doc.cpp
# End Source File
# Begin Source File

SOURCE=.\ch1_1View.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\detect.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBAPI.CPP
# End Source File
# Begin Source File

SOURCE=.\DlgCodeGIF.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgColor.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGeoRota.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGeoTran.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGeoZoom.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHuffman.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIntensity.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLinerPara.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMidFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPointStre.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPointThre.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPointWin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShannon.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSharpThre.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSmooth.cpp
# End Source File
# Begin Source File

SOURCE=.\edgecontour.cpp
# End Source File
# Begin Source File

SOURCE=.\FreTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\geotrans.cpp
# End Source File
# Begin Source File

SOURCE=.\GIFAPI.CPP
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\morph.cpp
# End Source File
# Begin Source File

SOURCE=.\PointTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\restore.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TemplateTrans.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cDlgMorphClose.h
# End Source File
# Begin Source File

SOURCE=.\cDlgMorphDilation.h
# End Source File
# Begin Source File

SOURCE=.\cDlgMorphErosion.h
# End Source File
# Begin Source File

SOURCE=.\cDlgMorphOpen.h
# End Source File
# Begin Source File

SOURCE=.\ch1_1.h
# End Source File
# Begin Source File

SOURCE=.\ch1_1Doc.h
# End Source File
# Begin Source File

SOURCE=.\ch1_1View.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ColorTable.h
# End Source File
# Begin Source File

SOURCE=.\detect.h
# End Source File
# Begin Source File

SOURCE=.\DIBAPI.H
# End Source File
# Begin Source File

SOURCE=.\DlgCodeGIF.h
# End Source File
# Begin Source File

SOURCE=.\DlgColor.h
# End Source File
# Begin Source File

SOURCE=.\DlgGeoRota.h
# End Source File
# Begin Source File

SOURCE=.\DlgGeoTran.h
# End Source File
# Begin Source File

SOURCE=.\DlgGeoZoom.h
# End Source File
# Begin Source File

SOURCE=.\DlgHuffman.h
# End Source File
# Begin Source File

SOURCE=.\DlgIntensity.h
# End Source File
# Begin Source File

SOURCE=.\DlgLinerPara.h
# End Source File
# Begin Source File

SOURCE=.\DlgMidFilter.h
# End Source File
# Begin Source File

SOURCE=.\DlgPointStre.h
# End Source File
# Begin Source File

SOURCE=.\DlgPointThre.h
# End Source File
# Begin Source File

SOURCE=.\DlgPointWin.h
# End Source File
# Begin Source File

SOURCE=.\DlgShannon.h
# End Source File
# Begin Source File

SOURCE=.\DlgSharpThre.h
# End Source File
# Begin Source File

SOURCE=.\DlgSmooth.h
# End Source File
# Begin Source File

SOURCE=.\edgecontour.h
# End Source File
# Begin Source File

SOURCE=.\FreTrans.h
# End Source File
# Begin Source File

SOURCE=.\geotrans.h
# End Source File
# Begin Source File

SOURCE=.\GIFAPI.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\morph.h
# End Source File
# Begin Source File

SOURCE=.\PointTrans.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\restore.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TemplateTrans.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ch1_1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ch1_1.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ch1_1Doc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
