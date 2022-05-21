@echo off

setlocal

cd %~dp0

set _out=mini

set _flg= ^
    /nologo ^
    /std:c++17 ^
    /EHsc ^
    /MD ^
    /LD

set _inc= ^
    /I"appsrc" ^
    /I"host_win32\mini_win32" ^
    /I"host\mini_pc"

set _src= ^
    appsrc\c*.cpp ^
    appsrc\d*.cpp ^
    appsrc\m*.cpp ^
    appsrc\r*.cpp ^
    host_win32\mini_win32\*.cpp ^
    host\mini_pc\*.cpp

set _lib= ^
    chakrart.lib ^
    gdi32.lib ^
    gdiplus.lib ^
    ole32.lib ^
    shell32.lib ^
    user32.lib

:main
    call :build "x64" "BUILD\windows_x64"
    call :build "x86" "BUILD\windows_x86"
goto :eof

:build
    set plat=%~1
    set ddir=%~2

    set file=%ddir%\%_out%
    set objs=%ddir%\objs\

    call :newdir  %objs%
    call :delfile %file%.dll
    call :delfile %file%.lib
    call :delfile %file%.exp

    ::build machine should be x64 platform.
    if "%plat%"=="x64" ( call vcvarsall x64     )
    if "%plat%"=="x86" ( call vcvarsall x64_x86 )

    cl  /Fe%file% ^
        /Fo%objs% ^
        %_flg% ^
        %_inc% ^
        %_src% ^
        %_lib%

    call :deldir %objs%

goto :eof

:newdir
    if not exist %~1 (
        md %~1
    )
goto :eof

:delfile
    if exist %~1 (
        del %~1
    )
goto :eof

:deldir
    if exist %~1 (
        rd /s/q %~1
    )
goto :eof

endlocal
