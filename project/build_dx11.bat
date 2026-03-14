@echo off

set BIN_DIR=%~dp0bin

if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"
del /s /q "%BIN_DIR%\*.*" 2>nul

call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat"

cl.exe %*