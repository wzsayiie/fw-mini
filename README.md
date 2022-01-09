## introduction

this is a lightweight, cross-platform program framework
that has least dependencies on os api.

## directory structure

cross-plaform resources and c++ sources files.

* appres/mini.bundle/
* appsrc/

the typescript project based on **node.js** environment.
it will compile the sources into *appres/mini.bundle/bundle.js*.

* appts/

os-related sources files and ide projects.

* host_android/
* host_ios/
* host_osx/
* host_win32/

the build scripts that depend on the ide projects.
files with extension *bat* for windows and without extension for mac os.

* build_android
* build_android.bat
* build_ios
* build_osx
* build_win32.bat

the scripts that compile sources from *appsrc/* as a javascript host executable.

* makejs_osx
* makejs_win32.bat

the scripts that compile the parts with stable abi from *appsrc/* as a dynamic library.

* makelib_osx
* makelib_win32.bat
