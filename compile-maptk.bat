@echo off

SETLOCAL ENABLEEXTENSIONS
SETLOCAL ENABLEDELAYEDEXPANSION
		 
echo.
echo --------- INITIALIZATION ------------

SET OVERVIEW_GEN_EXE="bin/generate-overview.exe"

SET TRANS_OVERVIEW_GEN_ARGS=-c -p Maptk_NemrodTrans.prj nemrod_trans_overview.mp
SET COMPLETE_OVERVIEW_GEN_ARGS=-c -p Maptk_NemrodComplete.prj nemrod_complete_overview.mp

SET MAP_TK_EXE=
call :TRY_PATH_DEFAULT_OR_EXIT_MAPTK

SET CANDLE_EXE=
SET LIGHT_EXE=
call :TRY_PATH_DEFAULT_OR_EXIT_WIX
SET CANDLE_ARGS=installer.wxs
SET LIGHT_ARGS=-ext WixUIExtension -spdb installer.wixobj

echo Using maptk: %MAP_TK_EXE%
echo Using candle: %CANDLE_EXE%
echo Using light: %LIGHT_EXE%
echo ------------ END-INIT ---------------
echo.
echo ---- BUILDING NEMROD TRANSPARENT -----
echo ------- Generate overview map --------
echo.
%OVERVIEW_GEN_EXE% %TRANS_OVERVIEW_GEN_ARGS%
echo.
echo ----- Compiling overview map to Output-MapTK ------
echo.
%MAP_TK_EXE% nemrod_trans_overview.mp Output-MapTK\NemrodTransparent\overview.img
echo.
echo ----- Compiling project file ------
echo.
@echo on
%MAP_TK_EXE% MapTk_NemrodTrans.prj
echo ------ BUILDING NEMROD COMPLETE ------
echo ------- Generate overview map --------
echo.
%OVERVIEW_GEN_EXE% %COMPLETE_OVERVIEW_GEN_ARGS%
echo.
echo ----- Compiling overview map to Output-MapTK ------
echo.
%MAP_TK_EXE% nemrod_complete_overview.mp Output-MapTK\NemrodComplete\overview.img
echo.
echo ----- Compiling project file ------
echo.
%MAP_TK_EXE% MapTk_NemrodComplete.prj

echo.
echo ----- Assembling installer ------
echo.
pushd Installer\Wix\
%CANDLE_EXE% %CANDLE_ARGS%
%LIGHT_EXE% %LIGHT_ARGS%
popd

echo.
echo ----- DONE ------
echo.

GOTO :eof

:TRY_PATH_DEFAULT_OR_EXIT_MAPTK
for /f "delims=" %%i in (' where MapTK.exe 2^> nul ') do SET MAP_TK_EXE="%%i"
if not exist "!MAP_TK_EXE!" (
	echo MapTK.exe is not on the path, trying default dir.
	SET MAP_TK_EXE="C:\program files (x86)\MapTK\MapTK.exe"
	if not exist "!MAP_TK_EXE!" (
rem test for x86 systems
		SET MAP_TK_EXE="C:\program files\MapTK\MapTK.exe"
		if not exist "!MAP_TK_EXE!" (
			echo CANNOT FIND MAPTK.EXE, ABORTING
			goto :errord
		)
	)
)
GOTO :eof

:TRY_PATH_DEFAULT_OR_EXIT_WIX
for /f "delims=" %%i in (' where candle.exe 2^> nul ') do SET CANDLE_EXE="%%i"
if not exist "!CANDLE_EXE!" (
	echo Candle.exe is not on the path, trying default dir.
	SET CANDLE_EXE="C:\Program Files (x86)\WiX Toolset v3.9\bin\candle.exe"
	if not exist "!CANDLE_EXE!" (
		SET CANDLE_EXE="C:\Program Files (x86)\WiX Toolset v3.10\bin\candle.exe"
		if not exist "!CANDLE_EXE!" (
rem test for x86 systems
			SET CANDLE_EXE="C:\program files\WiX Toolset v3.9\bin\candle.exe"
			if not exist "!CANDLE_EXE!" (
				SET CANDLE_EXE="C:\program files\WiX Toolset v3.10\bin\candle.exe"
				if not exist "!CANDLE_EXE!" (
					echo CANNOT FIND CANDLE.EXE, ABORTING
					goto :errord
				)
			)
		)
	)
)

for /f "delims=" %%i in (' where light.exe 2^> nul ') do SET LIGHT_EXE="%%i"
if not exist "!LIGHT_EXE!" (
	echo Light.exe is not on the path, trying default dir.
	SET LIGHT_EXE="C:\Program Files (x86)\WiX Toolset v3.9\bin\light.exe"
	if not exist "!LIGHT_EXE!" (
		SET LIGHT_EXE="C:\Program Files (x86)\WiX Toolset v3.10\bin\light.exe"
		if not exist "!LIGHT_EXE!" (
rem test for x86 systems
			SET LIGHT_EXE="C:\program files\WiX Toolset v3.9\bin\light.exe"
			if not exist "!LIGHT_EXE!" (
				SET LIGHT_EXE="C:\program files\WiX Toolset v3.10\bin\light.exe"
				if not exist "!LIGHT_EXE!" (
					echo CANNOT FIND LIGHT.EXE, ABORTING
					goto :errord
				)
			)
		)
	)
)
GOTO :eof

:errord
exit /b 1
GOTO :eof