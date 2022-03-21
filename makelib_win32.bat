@echo off

setlocal

cd %~dp0

::configuration begin.
set _IncDir=appsrc
set _CxxSrc=appsrc\*.cpp host_win32\mini\*.cpp
set _SysLib=chakrart.lib gdi32.lib gdiplus.lib ole32.lib shell32.lib user32.lib
set _CxxFlg=/nologo /std:c++17 /EHsc /Ox /MD /LD

set _OutDir=BUILD
set _OutNam=mini

set _ObjDir=TMP
::configuration end.

set _OutFil=%_OutDir%\%_OutNam%

if not exist %_OutDir% (md %_OutDir%)
if not exist %_ObjDir% (md %_ObjDir%)

if exist %_OutFil%.dll (
    del  %_OutFil%.dll
)

cl /Fe%_OutFil% /Fo%_ObjDir%\ /I%_IncDir% %_CxxFlg% %_CxxSrc% %_SysLib%

rmdir /s /q %_ObjDir%

del %_OutFil%.exp
del %_OutFil%.lib

endlocal
