@echo off

::sometimes, android studio can't work normally for variety of reasons,
::use this script to clean the project.

pushd %~dp0

            set target=.gradle
call :rmp & set target=.idea
call :rmp & set target=local.properties
call :rmp & set target=host_android.iml

call :rmp & set target=app\.cxx
call :rmp & set target=app\.externalNativeBuild
call :rmp & set target=app\app.iml
call :rmp & set target=app\build
call :rmp

goto end

:rmp
if exist %target%\nul (
    ::is a directory
    rmdir /q /s %target%
) else (
    if exist %target% (
        del %target%
    )
)
goto end

:end

popd
