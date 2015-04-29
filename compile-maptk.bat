@echo off

SETLOCAL ENABLEEXTENSIONS
SETLOCAL ENABLEDELAYEDEXPANSION
		 
echo.
echo --------- INITIALIZATION ------------

SET OVERVIEW_GEN_EXE="bin/generate-overview.exe"
SET OVERVIEW_GEN_ARGS=-p Maptk.prj nemrod_overview.mp

SET MAP_TK_EXE=%~1
rem echo %MAP_TK_EXE%
if not defined MAP_TK_EXE (
	echo MapTK.exe not specified trying with PATH
	call :TRY_PATH_DEFAULT_OR_EXIT
) else (
	if not exist "%MAP_TK_EXE%" (
		echo Specified MapTK.exe not found, trying with PATH
		call :TRY_PATH_DEFAULT_OR_EXIT
	)
)
SET MAP_TK_ARGS=

echo Using maptk: %MAP_TK_EXE%
echo ------------ END-INIT ---------------
echo.
echo ------- Generate overview map --------
echo.
%OVERVIEW_GEN_EXE% %OVERVIEW_GEN_ARGS%
echo.
echo ----- Compiling overview map to Output-MapTK ------
echo.
"%MAP_TK_EXE%" nemrod_overview.mp Output-MapTK\nemrod.img
echo.
echo ----- Compiling project file ------
echo.
"%MAP_TK_EXE%" -make MapTk.prj
echo.
echo ----- DONE ------
echo.

GOTO :eof

:TRY_PATH_DEFAULT_OR_EXIT
for /f "delims=" %%i in (' where MapTK.exe 2^> nul ') do (
	SET MAP_TK_EXE=%%i
	echo %%i
)
if not exist "!MAP_TK_EXE!" (
	echo MapTK.exe is not on the path, trying default dir.
	SET MAP_TK_EXE="C:\program files (x86)\MapTK\MapTK.exe"
	if not exist "!MAP_TK_EXE!" (
		echo CANNOT FIND MAPTK.EXE, ABORTING
		exit /B 1
	)
)
GOTO :eof

