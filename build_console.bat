@echo off

cd %~dp0

set TMPDIR="MSCL"
set OUTDIR="..\BUILD"
set OUTEXE="..\BUILD\mini.exe"
set INCDIR="..\appsrc"
set CPPSRC="..\appsrc\*.cpp" "..\host_console\*.cpp"

mkdir %TMPDIR%
pushd %TMPDIR%

if not exist %OUTDIR% (md  %OUTDIR%)
if exist     %OUTEXE% (del %OUTEXE%)

cl /nologo /Fe%OUTEXE% /I%INCDIR% %CPPSRC%

popd
rmdir /s /q %TMPDIR%
