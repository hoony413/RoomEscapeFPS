#include "Commandlet/TableExportCommandlet.h"

#if !WITH_EDITOR
int32 UTableExportCommandlet::Main(FString const& params) { return -1; }
#else

#include "PackageHelperFunctions.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "EditorFramework/AssetImportData.h"
#include "UObject/SavePackage.h"

int32 UTableExportCommandlet::Main(FString const& params)
{
	FString csvPath, outPath;
	FParse::Value(*params, TEXT("CsvPath="), csvPath);
	FParse::Value(*params, TEXT("OutPath="), outPath);

	if (csvPath.IsEmpty() || outPath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Usage: -run=TableExportCommandlet -CsvPath=\"...\" -OutPath=\"...\""))
		FPlatformProcess::Sleep(3.0f);
		return -1;
	}

	IFileManager& fileManager = IFileManager::Get();
	TArray<FString> csvFiles;
	FString const csvPattern(FPaths::Combine(csvPath, TEXT("*.csv")));
	fileManager.FindFiles(csvFiles, *csvPattern, true, false);
	if (csvFiles.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Csv folder is empty"));
		FPlatformProcess::Sleep(3.0f);
		return -1;
	}

	for (auto const& csvFilename : csvFiles)
	{
		FString const baseFileName(FPaths::GetBaseFilename(csvFilename));
		FString const rowTypeName(FString::Printf(TEXT("/Script/RoomEscapeFPS.Table%sRow"), *baseFileName));
		auto* const rowType = FindObject<UScriptStruct>(nullptr, *rowTypeName);
		if (not rowType)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not find row type UStruct: %s"), *rowTypeName);
			continue;
		}

		FString csvData;
		if (not FFileHelper::LoadFileToString(csvData, *FPaths::Combine(csvPath, csvFilename)))
		{
			UE_LOG(LogTemp, Error, TEXT("Could not read CSV file: %s"), *baseFileName);
			continue;
		}

		TArray<FString> newCsvLines;
		csvData.ParseIntoArrayLines(newCsvLines);
		newCsvLines.RemoveAt(1);
		csvData = FString::Join(newCsvLines, TEXT("\r\n"));

		FString const packagePath(FPaths::Combine(outPath, *baseFileName));
		FString const assetPath(FString::Printf(TEXT("%s.%s"), *packagePath, *baseFileName));
		UPackage* package = CreatePackage(*packagePath);
		package->FullyLoad();

		UDataTable* dataTable{};
		if (auto* existingDataTable = LoadObject<UDataTable>(nullptr, *assetPath))
		{
			auto const oldCsvString = existingDataTable->GetTableAsCSV();

			// float형 문제로 테이블 익스포트 후 값 비교.
			TArray<FString> problems = existingDataTable->CreateTableFromCSVString(csvData);
			if (not problems.IsEmpty())
			{
				for (auto const& problem : problems)
				{
					UE_LOG(LogTemp, Warning, TEXT("Problem importing data table %s: %s"), *baseFileName, *problem);
				}
			}

			if (oldCsvString.Equals(existingDataTable->GetTableAsCSV()))
			{
				// 결과값이 동일하면 skip
				UE_LOG(LogTemp, Display, TEXT("Skip export datatable because same data: %s"), *baseFileName);
				continue;
			}

			dataTable = existingDataTable;
		}
		else
		{
			// 없으면 새로 저장
			dataTable = NewObject<UDataTable>(package, FName(*baseFileName), RF_Public | RF_Standalone);
			dataTable->RowStruct = rowType;
			TArray<FString> problems = dataTable->CreateTableFromCSVString(csvData);
			if (not problems.IsEmpty())
			{
				for (auto const& problem : problems)
				{
					UE_LOG(LogTemp, Warning, TEXT("Problem importing data table %s: %s"), *baseFileName, *problem);
				}
			}
		}

		if (not dataTable)
		{
			UE_LOG(LogTemp, Error, TEXT("DataTable is nullptr: %s"), *baseFileName);
			return -1;
		}

		UE_LOG(LogTemp, Display, TEXT("Export overwrite/new one: %s"), *baseFileName);
		dataTable->PostInitProperties();
		dataTable->AssetImportData->Update(FPaths::Combine(csvPath, csvFilename));
		if (dataTable->MarkPackageDirty())
		{
			FAssetRegistryModule::AssetCreated(dataTable);
			FString const packageFileName(FPackageName::LongPackageNameToFilename(package->GetName(), FPackageName::GetAssetPackageExtension()));

			if (SavePackageHelper(package, packageFileName))
			{
				UE_LOG(LogTemp, Log, TEXT("Successfully created DataTable: %s"), *outPath);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to save DataTable: %s"), *outPath);
			}
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Check Log 3s(close soon)"));
	FPlatformProcess::Sleep(3.0f);
	return 0;
}

#endif // WITH_EDITOR
