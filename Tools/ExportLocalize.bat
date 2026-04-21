@echo off

set PROJECT_ROOT=D:\RoomEscapeFPS
set XLSX_TO_CSV=%PROJECT_ROOT%\Tools\XlsxToCsv\XlsxToCsv.py
set SRC_DIRECTORY_PATH=%PROJECT_ROOT%\ExcelTable\Localize
set DEST_DIRECTORY_PATH=%PROJECT_ROOT%\Content\Csv\Localize

set ENGINE_PATH=D:\UnrealEngine\Engine\Binaries\Win64\UnrealEditor-Win64-DebugGame.exe
set ST_OUT=/Game/Localization/StringTables/

echo ============================ XlsxToCsv (Localize) ============================

rem XlsxToCsv.py requires dest folder to exist before execution.
if not exist "%SRC_DIRECTORY_PATH%" (
    echo Error: Source directory not found: %SRC_DIRECTORY_PATH%
    echo Please create ExcelTable\Localize\ and place localization xlsx files there.
    goto :fail
)
if not exist "%DEST_DIRECTORY_PATH%" mkdir "%DEST_DIRECTORY_PATH%"

%XLSX_TO_CSV% %SRC_DIRECTORY_PATH% %DEST_DIRECTORY_PATH%
if errorlevel 1 goto :fail

echo.
echo Process XlsxToCsv (Localize) complete.
echo.

echo ============================ LocStringTable Commandlet ============================

%ENGINE_PATH% "%PROJECT_ROOT%\RoomEscapeFPS.uproject" ^
  -run=LocStringTableCommandlet ^
  -CsvPath="%DEST_DIRECTORY_PATH%" ^
  -StringTableOutPath="%ST_OUT%" ^
  -NativeCulture=ko ^
  -Cultures=ko+en ^
  -NoSplash -Unattended -Log -StdOut -NullRHI -NoSound

if errorlevel 1 goto :fail

echo.
echo Process LocStringTable export complete.
echo.
echo [ExportLocalize] Done.
pause
exit /b 0

:fail
echo.
echo [ExportLocalize] FAILED.
pause
exit /b 1
