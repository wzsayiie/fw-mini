@echo off

cd %~dp0

::configuration begin.
set _IncDir=appsrc
set _CxxSrc=appsrc\m*.cpp host_win32\mini\*.cpp
set _SysLib=gdi32.lib gdiplus.lib ole32.lib shell32.lib user32.lib
set _CxxFlg=/nologo /std:c++14 /Ox /EHsc /LD

set _OutDir=BUILD
set _OutDll=mini.dll
set _OutLib=mini.lib
set _OutExp=mini.exp

set _ObjDir=CL_TMP_OBJ
::configuration end.

if not exist %_OutDir% (md %_OutDir%)
if not exist %_ObjDir% (md %_ObjDir%)

pushd %_OutDir%
    if exist %_OutDll% (del %_OutDll%)
    if exist %_OutLib% (del %_OutLib%)
    if exist %_OutExp% (del %_OutExp%)
popd

cl /Fe"%_OutDir%\\%_OutDll%" /Fo"%_ObjDir%\\" /I%_IncDir% %_CxxFlg% %_CxxSrc% %_SysLib%

rmdir /s /q %_ObjDir%
