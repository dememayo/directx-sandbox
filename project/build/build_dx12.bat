@echo off

cd /d %~dp0..

set BIN_DIR=bin

if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"
del /s /q "%BIN_DIR%\*.*" 2>nul

call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat"

echo Compile shaders
dxc -Tvs_6_0 -EVSMain shader\tutorial.hlsl -Fo bin\tutorial_VSMain.cso
dxc -Tps_6_0 -EPSMain shader\tutorial.hlsl -Fo bin\tutorial_PSMain.cso

REM === PCH ===
cl /c @build/common.rsp @build/def_dx12.rsp /Yc"pch.h" /Fo"bin/pch.obj" pch/pch.cpp

REM === Compile ===
cl /c @build/common.rsp @build/def_dx12.rsp /Yu"pch.h" /Fo"bin\\" source/*.cpp

REM === Link ===
link @"build/link_dx12.rsp" bin\*.obj /OUT:"bin/sandbox.exe"