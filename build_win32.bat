@echo off

setlocal

cd %~dp0\host_win32

set _out=mini

:main
    call :build "x64" "x64\Release" "..\BUILD\windows_x64"
    call :build "x86" "Release"     "..\BUILD\windows_x86"
goto :eof

:build
    set plat=%~1
    set sdir=%~2
    set ddir=%~3

    call :newdir  %ddir%
    call :delfile %ddir%\%_out%.exe

    msbuild ^
        /nologo ^
        /p:configuration="release" ^
        /p:platform="%plat%" ^
        /t:rebuild ^
        mini_win32.sln

    copy %sdir%\%_out%.exe %ddir%

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

endlocal
