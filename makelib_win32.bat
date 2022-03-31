@echo off

setlocal

cd %~dp0

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
    appsrc\*.cpp ^
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
    call :build "x64" "BUILD\windows_x64" "mini"
    call :build "x86" "BUILD\windows_x86" "mini"
goto :eof

:build
    set plat=%~1
    set ddir=%~2
    set name=%~3

    set file=%ddir%\%name%
    set objs=%ddir%\objs\

    call :newdir  %objs%
    call :delfile %file%.dll
    call :delfile %file%.lib
    call :delfile %file%.exp

    ::build machine should be x64 platform.
    if "%plat%"=="x64" ( call vcvarsall x64     )
    if "%plat%"=="x86" ( call vcvarsall x64_x86 )

    cl  /Fe%file% ^
        /fd%file% ^
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
