@echo off

set ROOT_DIR=%~dp0
set ROOT_DIR=%ROOT_DIR:~0,-1%

@REM Change UE_4 dir to your local ue4 Dir!
set UE4_DIR=C:\dev\UnrealEngine\4.27\UnrealEngine
set BUILD_BAT=%UE4_DIR%\Engine\Build\BatchFiles\Build.bat

%BUILD_BAT% -project=%ROOT_DIR%\GameOfLife.uproject GameOfLifeHeadless Win64 Debug -waitmutex -NoHotReload
