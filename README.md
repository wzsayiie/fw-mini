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
* host_linux/
* host_osx/
* host_win32/

the build scripts that depend on the ide projects.

* build_android
* build_android.bat
* build_ios
* build_linux
* build_osx
* build_win32.bat

the scripts that compile *appsrc/* and
the corresponding os-related sources as a dynamic library.

* makelib_linux
* makelib_osx
* makelib_win32.bat
