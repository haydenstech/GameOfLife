@echo off

set ROOT_DIR=%~dp0
set ROOT_DIR=%ROOT_DIR:~0,-1%

call %ROOT_DIR%\Binaries\Win64\GameOfLifeHeadless-Win64-Debug.exe %*