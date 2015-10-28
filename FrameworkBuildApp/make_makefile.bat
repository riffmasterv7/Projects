@echo off
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

:: set MAMEDEV_MINGW_DIR=D:\Tools\MameDevMinGW\mingw64-w32\bin
set MAMEDEV_MINGW_DIR=D:\Tools\MinGW\bin
set CC1=%MAMEDEV_MINGW_DIR%\gcc.exe
set MAKE1=%MAMEDEV_MINGW_DIR%\mingw32-make.exe
set CFG1=Release

%MAKE1% CC=%CC1% CFG=%CFG1%
pause

