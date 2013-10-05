# Microsoft Developer Studio Project File - Name="FxjFunc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FxjFunc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FxjFunc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FxjFunc.mak" CFG="FxjFunc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FxjFunc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FxjFunc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FxjFunc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FXJFUNC_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FXJFUNC_EXPORTS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"./dll/me.dll"

!ELSEIF  "$(CFG)" == "FxjFunc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FXJFUNC_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp1 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FXJFUNC_EXPORTS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"./dll/me.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FxjFunc - Win32 Release"
# Name "FxjFunc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChanlunCore.cpp
# End Source File
# Begin Source File

SOURCE=.\ChanlunTools.cpp
# End Source File
# Begin Source File

SOURCE=.\ChanlunVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\FxjFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChanlunCore.h
# End Source File
# Begin Source File

SOURCE=.\ChanlunTools.h
# End Source File
# Begin Source File

SOURCE=.\ChanlunVersion.h
# End Source File
# Begin Source File

SOURCE=.\FxjFunc.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lua\lapi.c
# End Source File
# Begin Source File

SOURCE=.\lua\lapi.h
# End Source File
# Begin Source File

SOURCE=.\lua\lauxlib.c
# End Source File
# Begin Source File

SOURCE=.\lua\lauxlib.h
# End Source File
# Begin Source File

SOURCE=.\lua\lbaselib.c
# End Source File
# Begin Source File

SOURCE=.\lua\lbitlib.c
# End Source File
# Begin Source File

SOURCE=.\lua\lcode.c
# End Source File
# Begin Source File

SOURCE=.\lua\lcode.h
# End Source File
# Begin Source File

SOURCE=.\lua\lcorolib.c
# End Source File
# Begin Source File

SOURCE=.\lua\lctype.c
# End Source File
# Begin Source File

SOURCE=.\lua\lctype.h
# End Source File
# Begin Source File

SOURCE=.\lua\ldblib.c
# End Source File
# Begin Source File

SOURCE=.\lua\ldebug.c
# End Source File
# Begin Source File

SOURCE=.\lua\ldebug.h
# End Source File
# Begin Source File

SOURCE=.\lua\ldo.c
# End Source File
# Begin Source File

SOURCE=.\lua\ldo.h
# End Source File
# Begin Source File

SOURCE=.\lua\ldump.c
# End Source File
# Begin Source File

SOURCE=.\lua\lfunc.c
# End Source File
# Begin Source File

SOURCE=.\lua\lfunc.h
# End Source File
# Begin Source File

SOURCE=.\lua\lgc.c
# End Source File
# Begin Source File

SOURCE=.\lua\lgc.h
# End Source File
# Begin Source File

SOURCE=.\lua\linit.c
# End Source File
# Begin Source File

SOURCE=.\lua\liolib.c
# End Source File
# Begin Source File

SOURCE=.\lua\llex.c
# End Source File
# Begin Source File

SOURCE=.\lua\llex.h
# End Source File
# Begin Source File

SOURCE=.\lua\llimits.h
# End Source File
# Begin Source File

SOURCE=.\lua\lmathlib.c
# End Source File
# Begin Source File

SOURCE=.\lua\lmem.c
# End Source File
# Begin Source File

SOURCE=.\lua\lmem.h
# End Source File
# Begin Source File

SOURCE=.\lua\loadlib.c
# End Source File
# Begin Source File

SOURCE=.\lua\lobject.c
# End Source File
# Begin Source File

SOURCE=.\lua\lobject.h
# End Source File
# Begin Source File

SOURCE=.\lua\lopcodes.c
# End Source File
# Begin Source File

SOURCE=.\lua\lopcodes.h
# End Source File
# Begin Source File

SOURCE=.\lua\loslib.c
# End Source File
# Begin Source File

SOURCE=.\lua\lparser.c
# End Source File
# Begin Source File

SOURCE=.\lua\lparser.h
# End Source File
# Begin Source File

SOURCE=.\lua\lstate.c
# End Source File
# Begin Source File

SOURCE=.\lua\lstate.h
# End Source File
# Begin Source File

SOURCE=.\lua\lstring.c
# End Source File
# Begin Source File

SOURCE=.\lua\lstring.h
# End Source File
# Begin Source File

SOURCE=.\lua\lstrlib.c
# End Source File
# Begin Source File

SOURCE=.\lua\ltable.c
# End Source File
# Begin Source File

SOURCE=.\lua\ltable.h
# End Source File
# Begin Source File

SOURCE=.\lua\ltablib.c
# End Source File
# Begin Source File

SOURCE=.\lua\ltm.c
# End Source File
# Begin Source File

SOURCE=.\lua\ltm.h
# End Source File
# Begin Source File

SOURCE=.\lua\lua.h
# End Source File
# Begin Source File

SOURCE=.\lua\lua_tinker.cpp
# End Source File
# Begin Source File

SOURCE=.\lua\lua_tinker.h
# End Source File
# Begin Source File

SOURCE=.\lua\luaconf.h
# End Source File
# Begin Source File

SOURCE=.\lua\lualib.h
# End Source File
# Begin Source File

SOURCE=.\lua\lundump.c
# End Source File
# Begin Source File

SOURCE=.\lua\lundump.h
# End Source File
# Begin Source File

SOURCE=.\lua\lvm.c
# End Source File
# Begin Source File

SOURCE=.\lua\lvm.h
# End Source File
# Begin Source File

SOURCE=.\lua\lzio.c
# End Source File
# Begin Source File

SOURCE=.\lua\lzio.h
# End Source File
# End Group
# Begin Group "me"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\misc.cpp
# End Source File
# Begin Source File

SOURCE=.\misc.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Chan.asp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
