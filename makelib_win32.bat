@echo off

cd %~dp0

set TMPDIR="MSCL"
set OUTDIR="..\BUILD"
set OUTEXE="..\BUILD\mini.dll"
set INCDIR="..\appsrc"
set CPPSRC="..\appsrc\m*.cpp" "..\host_win32\mini\*.cpp"
set WINLIB="gdi32.lib" "gdiplus.lib" "ole32.lib" "shell32.lib" "user32.lib"

mkdir %TMPDIR%
pushd %TMPDIR%

if not exist %OUTDIR% (md  %OUTDIR%)
if exist     %OUTEXE% (del %OUTEXE%)

cl /nologo /EHsc /LD /Fe%OUTEXE% /I%INCDIR% %CPPSRC% %WINLIB%

popd
rmdir /s /q %TMPDIR%
