@echo off
echo ============================ XlsxToCsv Generator ============================

set XLSX_TO_CSV=D:\RoomEscapeFPS\Tools\XlsxToCsv\XlsxToCsv.py
set SRC_DIRECTORY_PATH=D:\RoomEscapeFPS\ExcelTable
set DEST_DIRECTORY_PATH=D:\RoomEscapeFPS\Content\Csv

%XLSX_TO_CSV% %SRC_DIRECTORY_PATH% %DEST_DIRECTORY_PATH%

echo.
echo Process XlsxToCsv complete.
echo.

echo ============================ Enum Generator ============================

set ENUM_GEN_SCRIPT=D:\RoomEscapeFPS\Tools\EnumGenerator\enum_generator.py
set YAML_PATH=D:\RoomEscapeFPS\Yaml\enum.yaml

%ENUM_GEN_SCRIPT% %YAML_PATH%

echo.
echo Process EnumGen complete.
echo.


echo ============================ Struct Generator ============================

set STRUCT_GEN_SCRIPT=D:\RoomEscapeFPS\Tools\StructGenerator\struct_generator.py
set CSV_DIRECTORY_PATH=D:\RoomEscapeFPS\Content\Csv

%STRUCT_GEN_SCRIPT% %CSV_DIRECTORY_PATH%

echo.
echo Process StructGen complete.
echo.

echo ============================ UDataTable Exporter ============================

set ENGINE_PATH=D:\UnrealEngine\Engine\Binaries\Win64\UnrealEditor-Win64-DebugGame.exe
set PROJECT_ROOT=D:\RoomEscapeFPS
set CSV_DATA_PATH=%PROJECT_ROOT%\Content\Csv
set UASSET_SAVE_DIR=/Game/Tables/

%ENGINE_PATH% "%PROJECT_ROOT%\RoomEscapeFPS.uproject" -run=TableExportCommandlet -CsvPath="%CSV_DATA_PATH%" -OutPath="%UASSET_SAVE_DIR%" -NoSplash -Unattended -Log -StdOut -NullRHI -NoSound

echo.
echo Process UDataTable AssetExport complete.
echo.

echo ============================ TableSubsystem Generator ============================

set TABLE_SUBSYSTEM_GEN_SCRIPT=D:\RoomEscapeFPS\Tools\TableSubsystemGenerator\TableSubsystemGenerator.py

%TABLE_SUBSYSTEM_GEN_SCRIPT%

echo.
echo Process TableSubSystemGen complete.
pause
